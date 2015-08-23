/* Copyright (c) 2010-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RESOURCETYPE_HPP
#define KKE_RESOURCETYPE_HPP

#include <string>
#include <vector>

#define RESOURCETYPE_CATEGORY(x) public: \
	static const std::string& Category() { static const std::string category = #x; return category; } \
	const std::string& GetCategory() const { return x::Category(); } 

namespace kke
{
	class ResourceType
	{       
    public:
		// Extract multiple sources from one string. [ Source <separator> source <separator> .. ]
		static std::vector<std::string> ExtractSources(const std::string& sources, const char separator = '|');
        static std::string MergeSources(std::vector<std::string> sources, char separator = '|');
		
	public:
		ResourceType();
		virtual ~ResourceType();

		bool operator==(const ResourceType& resource) const;
		bool operator!=(const ResourceType& resource) const;

		// Get Source
		const std::string& GetSource() const;
		
		// Get Category
		virtual const std::string& GetCategory() const = 0;
		virtual bool Load(const std::string& source) = 0;
		virtual bool Unload() = 0;
		virtual bool Reload();
		virtual ResourceType* Copy() const;

	protected:
		std::string source;
	};
}

#endif // KKE_RESOURCETYPE_HPP
