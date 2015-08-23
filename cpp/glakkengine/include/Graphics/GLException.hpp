/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GLEXCEPTION_HPP
#define KKE_GLEXCEPTION_HPP

#include <GL/glew.h>
#include <exception>
#include <string>

namespace kke
{
	class GLException : public std::exception
	{
	public:
        static bool GetGLError(GLenum& error);
        
    public:
		GLException(GLenum error, const std::string& src, const std::string& reason = "{Unknown reason}") noexcept;
		virtual ~GLException() noexcept;
		const char* what() const noexcept override;
		
    private:
        static std::string errorToString(GLenum err) noexcept;
		
    private:        
		std::string msg;
	};
    
    
}

#endif // KKE_GLEXCEPTION_HPP
