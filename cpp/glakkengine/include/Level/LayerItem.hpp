/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERITEM_HPP
#define KKE_LAYERITEM_HPP

#include "Level/LevelItem.hpp"

#include <string>
// #include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
// #include <boost/serialization/vector.hpp>
// #include <iostream>

namespace kke
{
	class Layer;
	
	class LayerItem : public LevelItem
	{
	public:
		explicit LayerItem(const std::string& name = std::string(), Layer* layer = 0);
		virtual ~LayerItem();
		
		void SetLayer(Layer* layer);
		Layer* GetLayer();
		
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(GameTime& gametime);
		virtual void Draw(IRenderer& renderer);
		
	protected:
		Layer *layer;
	private:
		
		friend class boost::serialization::access;
		template<class Archive> 
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LevelItem);
		}
	};
}

#endif // KKE_LAYEROBJECT_HPP