/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Thread/ThreadPool.hpp"
#include <cstdio>


using kke::ThreadPool;
using kke::ThreadWorker;
using kke::GameObject;
using kke::GameObj_Data;
using kke::Logger;
using boost::thread;
using boost::mutex;
using boost::condition_variable;

typedef std::queue<GameObj_Data> gameObjQueue;
typedef std::vector<boost::thread*> threadVec;
typedef std::vector<boost::thread*>::iterator threadVecIt;

void ThreadWorker::operator()()
{
    ThreadPool* pool = ThreadPool::Instance();
    // Main loop of a thread
    while (!pool->stopThreads)
    {
        GameObj_Data workData;

        {
            mutex::scoped_lock lock(pool->queueMutex);
            while (pool->workQueue.empty() && !pool->stopThreads)
            {
                // Wait for notification of added data
                pool->queueCondition.wait(lock);

                /* NOTE TO SELF
                lock get locked, wait unlocks the lock. When
                notified, wait will try to unlock the lock,
                which is only possible if it is unlocked.
                */
            }

            if (pool->stopThreads)
            {
                return;
            }

            // Data added
            workData = pool->workQueue.front();
            pool->workQueue.pop();

            // Lock unlocked
        }

        // Run work
        switch (workData.Work)
        {
        case GameObj_Load:

            if (workData.Parameter == nullptr)
            {
                // Empty load
                workData.Object->LoadContent();
            }
            else
            {
                // Cast to ResourceManager
                workData.Object->LoadContent(*((kke::ResourceManager*)workData.Parameter));
            }

            break;

        case GameObj_Unload:
            workData.Object->UnloadContent();
            break;

        case GameObj_Update:
            workData.Object->Update(*((float*)workData.Parameter));
            break;
        }

        mutex::scoped_lock lock(pool->ownerMutex);

        // Set amount of work by owner (one more done)
        pool->ownerWork[workData.Owner]--;


        if (pool->ownerWork[workData.Owner] == nullptr)
        {
            pool->ownerWork.erase(workData.Owner);
            // Notify that a new owner is done
            pool->ownerCondition.notify_one();
        }

    }
}

/* THREADPOOL */

ThreadPool* ThreadPool::instance = nullptr;
bool ThreadPool::isInitialized = false;

void ThreadPool::WaitOwner(void* owner)
{
    mutex::scoped_lock lock(ownerMutex);

    while (ownerWork.find(owner) != ownerWork.end())
    {
        // While the owner work is still not done
        ownerCondition.wait(lock);
    }

    // Owner done!
}

void ThreadPool::WaitAll()
{
    mutex::scoped_lock lock(ownerMutex);

    while (!workQueue.empty())
    {
        // Notify new owner done, is queue empty?
        ownerCondition.wait(lock);
    }
}

bool ThreadPool::FinishedOwner(void* owner)
{
    bool done = false;

    // Lock owner mutex and check if owner exists in
    // map. If exists, then it's not done
    mutex::scoped_lock lock(ownerMutex);
    done = (ownerWork.find(owner) == ownerWork.end());
    lock.unlock();

    return done;
}

bool ThreadPool::FinishedAll()
{
    bool done = false;

    // Lock owner queue mutex and see if it's empty
    mutex::scoped_lock lock(queueMutex);
    done = workQueue.empty();
    lock.unlock();

    return done;
}

void ThreadPool::AddWork(GameObj_Work work, void* owner, GameObject* object, void* parameter)
{
    // Create data
    GameObj_Data data;
    data.Work = work;
    data.Owner = owner;
    data.Object = object;
    data.Parameter = parameter;

    // Add work to queue
    AddWorkQueue(data);
}

ThreadPool* ThreadPool::Instance()
{
    // Return static instance
    return instance;
}

// Static
void ThreadPool::Initialize()
{
    if (!isInitialized)
    {
	instance = new ThreadPool();
	isInitialized = true;
	Logger::Log("ThreadPool initialized.", "Thread");
    }
}

// Static
void ThreadPool::Terminate()
{
    if (isInitialized)
    {
		delete instance;
		instance = nullptr;
		isInitialized = false;
		Logger::Log("ThreadPool terminated.", "Thread");
    }
}

void ThreadPool::SetThreads(uint threads)
{
    if (this->threads.size() == threads)
    {
        return;
    }

    if (this->threads.size() > 0)
    {
        StopThreads();
    }

    // Add threads

    for (uint i = 0; i < threads; i++)
    {
        this->workers.push_back(ThreadWorker());
        this->threads.push_back(new boost::thread(workers[i]));
    }
}

void ThreadPool::StopThreads()
{
    // Tell threads to stop processing
    stopThreads = true;

    // Tell all threads to continue
    queueCondition.notify_all();

    for (threadVecIt it = this->threads.begin(); it != this->threads.end()
    {
        // Wait for all threads to finish
        (*it)->join();
        delete (*it);
    }


    // Remove all existing threads
    this->threads.clear();
    workers.clear();
    stopThreads = false;
}

void ThreadPool::AddWorkQueue(const GameObj_Data& data)
{
    // Lock owner
    mutex::scoped_lock lockown(ownerMutex);
    // Lock the queue
    mutex::scoped_lock lock(queueMutex);
    // Add data
    workQueue.push(data);
    // Add amount of work by owner
    ownerWork[data.Owner]++;
    lock.unlock();

    // Let one thread in
    queueCondition.notify_one();
}
