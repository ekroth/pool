/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "Thread/GameObj_Work.hpp"
#include "Interface/INoncopyable.hpp"
#include "GameObject.hpp"
#include <queue>
#include <vector>
#include <map>
#include <boost/thread.hpp>

namespace kke
{
    struct ThreadWorker
    {
        // Access pool queues, mutexes and conditionals
        friend class ThreadPool;

        ThreadWorker() { }
        // Thread operation
        void operator()();
    };

    class ThreadPool : kke::Noncopyable
    {
        // Thread worker
        friend class ThreadWorker;

    public:
        // Wait for all work from a specific
        // owner to be done.
        void WaitOwner(void* owner);
        // Wait for all work to be done.
        void WaitAll();
        // Check if specific owner is finished
        bool FinishedOwner(void* owner);
        // Check if all owners are done
        bool FinishedAll();
        // Add work.
        void AddWork(GameObj_Work work, void* owner, GameObject* object, void* parameter = 0);

        void AddWorkLoad(void* owner, GameObject* object, void* parameter = 0);
        void AddWorkUnload(void* owner, GameObject* object);
        void AddWorkUpdate(void* owner, GameObject* object, void* parameter);


	static void Initialize();
        static void Terminate();

        // Set additional threads. Function
        // will have to wait for current threads
        // to finish. Threads will then be removed
        // and new threads will be added. Costly operation.
        void SetThreads(uint threads);
    private:
        ThreadPool() { }
        
        static ThreadPool* Instance();
        // Singleton static instance pointer
        static ThreadPool* instance;
        // If pointer has been allocated
        static bool isInitialized;
        void StopThreads();
        void AddWorkQueue(const GameObj_Data& data);
        // If threads should be stopped
        bool stopThreads;
        // Work to be processed
        std::queue<GameObj_Data> workQueue;
        // Threads
        std::vector<boost::thread*> threads;
        std::vector<ThreadWorker> workers;
        // Owner statistics
        std::map<void*, uint> ownerWork;

        // Mutexes and conditional variables
        // When queue is changed
        boost::mutex queueMutex;
        boost::condition_variable queueCondition;
        // When owner stats are changed
        boost::mutex ownerMutex;
        boost::condition_variable ownerCondition;
    };
}

#endif
