/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SHADERPART_HPP
#define KKE_SHADERPART_HPP

#include "ResourceType.hpp"
#include "Interface/INoncopyable.hpp"

#include <GL/glew.h>

namespace kke
{	
	/**
	 * @brief OpenGL shader. Handled by GLProgram.
	 **/
	class ShaderPart : public INoncopyable
	{
	public:
		explicit ShaderPart(GLenum type, bool generate = false);
		~ShaderPart();
		
		void Generate();
		bool Load(const std::string& path);
		/**
		 * @brief Source will be modified!
		 *
		 * @param source ...
		 * @return bool
		 **/
		bool LoadSource(std::string& source);
		bool Unload();
		
		void SetType(GLenum type);
		GLenum GetType() const;
		GLuint GetHandle() const;
		
		void GetLog(std::string& log);
	private:
		bool processSource(std::string& source);
		
		GLuint handle;
		GLenum type;
	};
}

#endif // KKE_SHADERPART_HPP