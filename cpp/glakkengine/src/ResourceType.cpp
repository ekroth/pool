/* Copyright (c) 2010-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "ResourceType.hpp"
#include "IO/LogManager.hpp"

#include <algorithm>

using namespace kke;

using std::string;
using std::vector;
using std::sort;

ResourceType::ResourceType()
{
}

ResourceType::~ResourceType()
{
}

bool ResourceType::operator==(const ResourceType& resource) const
{
	// If source is the same
	return resource.source == source;
}

bool ResourceType::operator!=(const ResourceType& resource) const
{
	// If source is the same
	return resource.source != source;
}

const string& ResourceType::GetSource() const
{
	return source;
}

std::vector<std::string> ResourceType::ExtractSources(const std::string& sources, const char separator)
{
    std::vector<std::string> vec;
    size_t currPos = 0;

    while (true)
    {
        size_t pos = sources.find(separator, currPos);

        if (pos == string::npos)
        {
            // The rest in one source
            vec.push_back(sources.substr(currPos));
            break; // No more
        }

        // Add it
        vec.push_back(sources.substr(currPos, pos));

        currPos = pos + 1;
    }
    
    sort(vec.begin(), vec.end());
    
    return vec;
}

string ResourceType::MergeSources(std::vector<std::string> sources, char separator)
{
    string src;
    
    sort(sources.begin(), sources.end());
    
    for (auto it = sources.begin(); it != sources.end(); it++)
    {
        src += *it;
       
        if (it + 1 != sources.end()) // Not last
            src += '|';
    }
    
    return src;
}


bool ResourceType::Load(const std::string& source)
{
    this->source = source;
    return true;
}

bool ResourceType::Unload()
{
    this->source.clear();
    return true;
}

bool ResourceType::Reload()
{
    const std::string src = source;
    Unload();
	return Load(src);
}

ResourceType* ResourceType::Copy() const
{
	return nullptr;
}