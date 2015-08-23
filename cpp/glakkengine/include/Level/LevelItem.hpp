/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELENTITY_HPP
#define KKE_LEVELENTITY_HPP

#include <Graphics/Renderer.hpp>
#include <GameTime.hpp>
#include <Debug/Logger.hpp>
#include <Interface/Noncopyable.hpp>

#include <string>
#include <boost/archive/xml_oarchive.hpp>

namespace kke
{
	class Level;
	
	class LevelItem : public INoncopyable
	{
	public:
		explicit LevelItem(const std::string& name = std::string(), Level* level = nullptr);
		virtual ~LevelItem();
		
		void SetName(const std::string& name);
		const std::string& GetName() const;
		
		void SetLevel(Level* level);
		Level* GetLevel();
		
		virtual void* Copy() const;
		
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(GameTime& gameTime);
		virtual void Draw(IRenderer& renderer);
		
	protected:
		bool isLoaded;
		std::string name;
		Level *level;
		
	private:
		
		friend class boost::serialization::access;
		template<class Archive> 
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(name);
		}
	};
}

#endif // KKE_LEVELENTITIY_HPP