/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "ResourceFactory.hpp"

using namespace kke;

ResourceFactory::ResourceFactory()
{

}

ResourceFactory::~ResourceFactory()
{
    for (auto p : resources)
    {        
        for (auto r : p.second)
        {
            LOGD() << "Force zero users..";
            r.second.Users = 0;
            noUsersCallback(r.second.Resource);
        }
    }
}

void ResourceFactory::ReloadAll()
{
    
}

void ResourceFactory::ReloadCategory(const std::string& category)
{
    
}

void ResourceFactory::noUsersCallback(ResourceType* res)
{
    auto& resMap = resources[res->GetCategory()];
    auto it = resMap.find(res->GetSource());
    
    if (it != resMap.end())
    {
        if (it->second.Users == 0 || (it->second.Users > 0 && --it->second.Users == 0))
        {
            LOGD() << "No users, " << res->GetCategory() << ": " << res->GetSource();
            
            resMap.erase(it);
            if (resMap.empty())
                resources.erase(res->GetCategory());
            
            it->second.Resource->Unload();
            delete res;
        }
    }
    else
    {
        LOGE() << "Resource doesn't exist in ResourceFactory: " << res->GetSource();
    }
}
