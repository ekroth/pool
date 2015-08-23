/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTURE_HPP
#define KKE_LAYERTEXTURE_HPP

#include "Level/LayerItem.hpp"
#include "Math/Rect.hpp"
#include "Resource.hpp"

#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
// #include <boost/serialization/export.hpp>

#include "IO/Serialize_glm.hpp"
#include "IO/Serialize_rect.hpp"

namespace kke
{
	class Layer;
	class Texture2D;
	
	class LayerTexture : public LayerItem
	{
	public:
		explicit LayerTexture(const std::string& name = std::string(), Layer* layer = nullptr);
		
		void SetPosition(const glm::vec3& position);
		const glm::vec3 &GetPosition() const;
		
		void SetOrigin(const glm::vec2& origin);
		const glm::vec2 &GetOrigin() const;
		
		void SetScale(const glm::vec2& scale);
		const glm::vec2 &GetScale() const;
		
		void SetRotation(float rotation);
		float GetRotation() const;
		
		void SetColor(int index, const glm::vec4& color);
		const glm::vec4& GetColor(int index) const;
		
		void SetSource(const std::string& source);
		const std::string& GetSource() const;
		
		void SetTexcoords(const Rectf& texcoords);
		const Rectf& GetTexcoords() const;
		
		const Texture2D& GetTexture() const;
		bool SourceLoaded() const;
		
		void LoadContent();
		void UnloadContent();
		void Update(GameTime& gameTime);
		void Draw(IRenderer& renderer);
		
	private:
		Resource<Texture2D> *texture;
		glm::vec3 position;
		glm::vec2 origin;
		glm::vec2 scale;
		float rotation;
		glm::vec4 color[4];
		std::string source;
		Rectf texcoords;
		
		friend class boost::serialization::access;
		template<class Archive> 
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LayerItem);
			ar & BOOST_SERIALIZATION_NVP(position);
			ar & BOOST_SERIALIZATION_NVP(origin);
			ar & BOOST_SERIALIZATION_NVP(scale);
			ar & BOOST_SERIALIZATION_NVP(rotation);
			ar & BOOST_SERIALIZATION_NVP(color);
			ar & BOOST_SERIALIZATION_NVP(source);
			ar & BOOST_SERIALIZATION_NVP(texcoords);
		}
	};
}

#endif // KKE_LAYERTEXTURE_HPP
