/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GFXCACHE_HPP
#define KKE_GFXCACHE_HPP

#include "Utilities/Singleton.hpp"

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <array>
#include <unordered_map>

#define GFXCACHE_VAR_NAME_1PARAM(name) cached##name##_var1
#define GFXCACHE_SET_FUNC_NAME(name) void Set##name
#define GFXCACHE_VAR_1PARAM_HEAD(name, type1) type1 GFXCACHE_VAR_NAME_1PARAM(name); 
#define GFXCACHE_SET_1PARAM_HEAD(name, type1) void Set##name(type1 var1);

#define GFXCACHE_VAR_2PARAM_HEAD(name, type1, type2) type1 cached##name##_var1; type2 cached##name##_var2; 
#define GFXCACHE_SET_2PARAM_HEAD(name, type1, type2) void Set##name(type1 var1, type2 var2);

#define GFXCACHE_VAR_3PARAM_HEAD(name, type1, type2, type3) type1 cached##name##_var1; type2 cached##name##_var2; type3 cached##name##_var3;
#define GFXCACHE_SET_3PARAM_HEAD(name, type1, type2, type3) void Set##name(type1 var1, type2 var2, type3 var3);

#define GFXCACHE_VAR_4PARAM_HEAD(name, type1, type2, type3, type4) type1 cached##name##_var1; type2 cached##name##_var2; type3 cached##name##_var3; type4 cached##name##_var4;
#define GFXCACHE_SET_4PARAM_HEAD(name, type1, type2, type3, type4) void Set##name(type1 var1, type2 var2, type3 var3, type4 var4);
    
#define GFXCACHE_VAR_TARGET_4PARAM_HEAD(varType, name, count) std::array<varType, count> cached##name;
#define GFXCACHE_SET_TARGET_4PARAM_HEAD(targetType, varType, name) void Set##name(targetType target, varType var1);    
    
#define GFXCACHE_VAR_TARGET_1PARAM_NAME(name) cached##name
#define GFXCACHE_VAR_TARGET_1PARAM_HEAD(targetType, varType, name) std::unordered_map<targetType, varType> GFXCACHE_VAR_TARGET_1PARAM_NAME(name);
#define GFXCACHE_SET_TARGET_1PARAM_HEAD(targetType, varType, name) void Set##name(targetType target, varType var1);

namespace kke
{
	// Reduces the amount of unnecessary OpenGL state changes.
	// Only stores cached primitives, doesn't support storing Shader/Texture pointers.
    class GfxCache : public Singleton<GfxCache>
    {
		friend class Singleton<GfxCache>;
        
    public:       
        GFXCACHE_SET_1PARAM_HEAD(Program, GLuint);
        GFXCACHE_SET_1PARAM_HEAD(VAO, GLuint);
		GFXCACHE_SET_1PARAM_HEAD(ActiveTex, GLenum);
        GFXCACHE_SET_TARGET_1PARAM_HEAD(GLenum, GLuint, RBO);
        GFXCACHE_SET_TARGET_1PARAM_HEAD(GLenum, GLuint, Buffer);
        GFXCACHE_SET_TARGET_1PARAM_HEAD(GLenum, GLuint, FBO);
		GFXCACHE_SET_TARGET_1PARAM_HEAD(GLenum, GLuint, Texture);
		GFXCACHE_SET_TARGET_1PARAM_HEAD(GLenum, bool, Enable);
		
	private:
		typedef std::unordered_map<GLenum,GLuint> TextureUnit;
		
	private:
		GfxCache();
		~GfxCache();
		
	private:
        GFXCACHE_VAR_1PARAM_HEAD(Program, GLuint);
        GFXCACHE_VAR_1PARAM_HEAD(VAO, GLuint);
		GFXCACHE_VAR_1PARAM_HEAD(ActiveTex, GLenum);
        GFXCACHE_VAR_TARGET_1PARAM_HEAD(GLenum, GLuint, RBO);
        GFXCACHE_VAR_TARGET_1PARAM_HEAD(GLenum, GLuint, Buffer);
        GFXCACHE_VAR_TARGET_1PARAM_HEAD(GLenum, GLuint, FBO);
		GFXCACHE_VAR_TARGET_1PARAM_HEAD(GLenum, TextureUnit, TexUnit);
		GFXCACHE_VAR_TARGET_1PARAM_HEAD(GLenum, bool, Enable);
    };
	
	#include "Graphics/GfxCache.inl"
}

#endif // KKE_GFXCACHE_HPP