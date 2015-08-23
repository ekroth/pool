/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVEL_HPP
#define KKE_LEVEL_HPP

#include "Graphics/IRenderer.hpp"

#include <Interface/Noncopyable.hpp>
#include <vector>
#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace kke
{
	class LevelItem;
	class GameTime;
	class Layer;
	
	typedef std::vector<LevelItem*> LevelItems;
	class Level : public INoncopyable
	{
	public:
		explicit Level(const std::string& name = std::string());
		virtual ~Level();
		
		LevelItems& Items();
		
		bool IsLoaded() const;
		
		void SetName(const std::string& name);
		const std::string& GetName() const;
		
		virtual void* Copy() const;
		
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(GameTime& gameTime);
		virtual void Draw(IRenderer& renderer);
		
	private:
		std::string name;
		LevelItems items;
		bool isLoaded;
		
		friend class boost::serialization::access;
		template<class Archive> 
		void serialize(Archive& ar, const unsigned int version)
		{
			ar.register_type(static_cast<LevelItem*>(0));
			ar.register_type(static_cast<Layer*>(0));
			ar & BOOST_SERIALIZATION_NVP(name);
			ar & BOOST_SERIALIZATION_NVP(items);
		}
	};
}

#endif // KKE_LEVEL_HPP