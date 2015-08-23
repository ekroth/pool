/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/GfxCache.hpp"
#include "IO/LogManager.hpp"

using namespace kke;

#define GFX_CACHE_DISABLED false

#define GFXCACHE_SET_1PARAM_SRC(name, func, type1) \
    void GfxCache::Set##name(type1 var1) \
    { \
        if (GFX_CACHE_DISABLED || cached##name##_var1 != var1) \
        { \
            cached##name##_var1 = var1; \
            func(var1); \
        } \
    }
#define GFXCACHE_VAR_1PARAM_INIT_SRC(name, value1) cached##name##_var1(value1)

#define GFXCACHE_SET_2PARAM_SRC(name, func, type1, type2) \
    void GfxCache::Set##name(type1 var1, type2 var2) \
    { \
        if (GFX_CACHE_DISABLED || cached##name##_var1 != var1 || cached##name##_var2 != var2) \
        { \
            cached##name##_var1 = var1; \
            cached##name##_var2 = var2; \
            func(var1, var2); \
        } \
    }
#define GFXCACHE_VAR_2PARAM_INIT_SRC(name, value1, value2) cached##name##_var1(value1), cached##name##_var2(value2)

#define GFXCACHE_SET_3PARAM_SRC(name, func, type1, type2, type3) \
    void GfxCache::Set##name(type1 var1, type2 var2, type3 var3) \
    { \
        if (GFX_CACHE_DISABLED || cached##name##_var1 != var1 || cached##name##_var2 != var2 || cached##name##_var3 != var3) \
        { \
            cached##name##_var1 = var1; \
            cached##name##_var2 = var2; \
            cached##name##_var3 = var3; \
            func(var1, var2, var3); \
        } \
    }
#define GFXCACHE_VAR_3PARAM_INIT_SRC(name, value1, value2, value3) cached##name##_var1(value1), cached##name##_var2(value2), cached##name##_var3(value3)

#define GFXCACHE_SET_4PARAM_SRC(name, func, type1, type2, type3, type4) \
    void GfxCache::Set##name(type1 var1, type2 var2, type3 var3, type4 var4) \
    { \
        if (GFX_CACHE_DISABLED || cached##name##_var1 != var1 || cached##name##_var2 != var2 || cached##name##_var3 != var3 || cached##name##_var4 != var4) \
        { \
            cached##name##_var1 = var1; \
            cached##name##_var2 = var2; \
            cached##name##_var3 = var3; \
            cached##name##_var4 = var4; \
            func(var1, var2, var3, var4); \
        } \
    }
#define GFXCACHE_VAR_4PARAM_INIT_SRC(name, value1, value2, value3, value4) cached##name##_var1(value1), cached##name##_var2(value2), cached##name##_var3(value3), cached##name##_var4(value4)

#define GFXCACHE_SET_TARGET_4PARAM_SRC(targetType, varType, name, func, indexTranslator) \
    void GfxCache::Set##name(targetType target, varType var1) \
    { \
        if (GFX_CACHE_DISABLED || cached##name[indexTranslator(target)] != var1) \
        { \
            cached##name[indexTranslator(target)] = var1; \
            func(target, var1); \
        } \
    } 

#define GFXCACHE_SET_TARGET_1PARAM_SRC(targetType, varType, name, func) \
    void GfxCache::Set##name(targetType target, varType var1) \
    { \
        if (GFX_CACHE_DISABLED || GFXCACHE_VAR_TARGET_1PARAM_NAME(name)[target] != var1) \
        { \
            GFXCACHE_VAR_TARGET_1PARAM_NAME(name)[target] = var1; \
            func(target, var1); \
        } \
    }

GfxCache::GfxCache() :
    GFXCACHE_VAR_1PARAM_INIT_SRC(Program, 0),
    GFXCACHE_VAR_1PARAM_INIT_SRC(VAO, 0),
    GFXCACHE_VAR_1PARAM_INIT_SRC(ActiveTex, 0)
{
}

GfxCache::~GfxCache()
{

}

// Specific
void GfxCache::SetEnable (GLenum cap, bool var)
{
	auto& boolMap = GFXCACHE_VAR_TARGET_1PARAM_NAME(Enable);
    auto it = boolMap.find(cap);
	
	if (var)
	{
		if (GFX_CACHE_DISABLED || it == boolMap.end() || !it->second)
		{
			glEnable(cap);
			boolMap[cap] = true;
		}
	}
	else
	{
		if (GFX_CACHE_DISABLED || it == boolMap.end() || it->second)
		{
			glDisable(cap);
			boolMap[cap] = true;
		}
	}
}

// Specific
void GfxCache::SetTexture(GLenum target, GLuint tex)
{
	auto& units = GFXCACHE_VAR_TARGET_1PARAM_NAME(TexUnit);
	auto& unit = units[GFXCACHE_VAR_NAME_1PARAM(ActiveTex)];
	
	if (GFX_CACHE_DISABLED || unit[target] != tex)
	{
		glBindTexture(target, tex);
		unit[target] = tex;
	}
}

// Specific
void GfxCache::SetActiveTex(GLenum tex)
{
	if (GFX_CACHE_DISABLED || GFXCACHE_VAR_NAME_1PARAM(ActiveTex) != tex)
	{
		glActiveTexture(tex);
		GFXCACHE_VAR_NAME_1PARAM(ActiveTex) = tex;
	}
}

// Specific
void GfxCache::SetVAO(GLuint vao)
{
    if (GFX_CACHE_DISABLED || GFXCACHE_VAR_NAME_1PARAM(VAO) != vao)
    {
        glBindVertexArray(vao);
        GFXCACHE_VAR_NAME_1PARAM(VAO) = vao;
        
        // Reset all Buffers handles cache. TODO: CACHE AND SAVE WHICH ELEMENTS_BUFFER THAT IS SED...
        for (auto& b : GFXCACHE_VAR_TARGET_1PARAM_NAME(Buffer))
            b.second = 0;
    }
}

// Specific
void GfxCache::SetFBO(GLenum target, GLuint fbo)
{
    if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER)
    {
        if (GFX_CACHE_DISABLED || GFXCACHE_VAR_TARGET_1PARAM_NAME(FBO)[0] != fbo)
        {
            glBindFramebuffer(target, fbo);
            GFXCACHE_VAR_TARGET_1PARAM_NAME(FBO)[0] = fbo;
        }
    }
    
    if (target == GL_READ_FRAMEBUFFER || target == GL_FRAMEBUFFER)
    {
        if (GFX_CACHE_DISABLED || GFXCACHE_VAR_TARGET_1PARAM_NAME(FBO)[1] != fbo)
        {
            glBindFramebuffer(target, fbo);
            GFXCACHE_VAR_TARGET_1PARAM_NAME(FBO)[1] = fbo;
        }
    }
}

// Auto
GFXCACHE_SET_1PARAM_SRC(Program, glUseProgram, GLuint);
GFXCACHE_SET_TARGET_1PARAM_SRC(GLenum, GLuint, RBO, glBindRenderbuffer);
GFXCACHE_SET_TARGET_1PARAM_SRC(GLenum, GLuint, Buffer, glBindBuffer);