/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SERIALIZE_GLM_HPP
#define KKE_SERIALIZE_GLM_HPP

#include <boost/archive/xml_oarchive.hpp>

#include <glm/glm.hpp>

using boost::serialization::make_nvp;

namespace boost 
{
	namespace serialization 
	{
		template<class Archive>
		void serialize(Archive& ar, glm::vec2& g, const unsigned int version)
		{
			ar & make_nvp("x", g.x);
			ar & make_nvp("y", g.y);
		}
		
		template<class Archive>
		void serialize(Archive& ar, glm::vec3& g, const unsigned int version)
		{
			ar & make_nvp("x", g.x);
			ar & make_nvp("y", g.y);
			ar & make_nvp("z", g.z);
		}
		
		template<class Archive>
		void serialize(Archive& ar, glm::vec4& g, const unsigned int version)
		{
			ar & make_nvp("x", g.x);
			ar & make_nvp("y", g.y);
			ar & make_nvp("z", g.z);
			ar & make_nvp("w", g.w);
		}
	}
}

#endif // KKE_SERIALIZE_GLM_HPP