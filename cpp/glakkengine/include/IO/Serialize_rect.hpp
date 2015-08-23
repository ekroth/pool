/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SERIALIZE_RECT_HPP
#define KKE_SERIALIZE_RECT_HPP

#include <boost/archive/xml_oarchive.hpp>

#include "Math/Rect.hpp"

using boost::serialization::make_nvp;

namespace boost 
{
	namespace serialization 
	{
		template<class Archive>
		void serialize(Archive& ar, kke::Recti& rect, const unsigned int version)
		{
			ar & make_nvp("X", rect.X);
			ar & make_nvp("Y", rect.Y);
			ar & make_nvp("W", rect.Width);
			ar & make_nvp("H", rect.Height);
		}
		
		template<class Archive>
		void serialize(Archive& ar, kke::Rectf& rect, const unsigned int version)
		{
			ar & make_nvp("X", rect.X);
			ar & make_nvp("Y", rect.Y);
			ar & make_nvp("W", rect.Width);
			ar & make_nvp("H", rect.Height);
		}
	}
}

#endif // KKE_SERIALIZE_RECT_HPP