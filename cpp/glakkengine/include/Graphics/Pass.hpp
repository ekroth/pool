/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_PASS_HPP
#define KKE_PASS_HPP

#include "Graphics/Texture2D.hpp"

#include <unordered_map>
#include <glm/glm.hpp>

#define uniformf(x) std::unordered_map<std::string, glm::vec##x> uniform##x##f
#define g_uniformf(x) \
    const std::unordered_map<std::string, glm::vec##x>& GetUniform##x##f() const { return uniform##x##f; } \
    std::unordered_map<std::string, glm::vec##x>& GetUnifrom##x##f() { return uniform##x##f; }

namespace kke
{    
    /**
     * @brief A pass of a technique, contains parameters to a technique.
     **/
    class Pass
    {
    public:
        typedef std::unordered_map<GLenum, Resource<Texture2D>> TexUnitsMap;
        
    public:
        const TexUnitsMap& GetTextureUnits() const { return texUnits; }
        TexUnitsMap& GetTextureUnits() { return texUnits; }
        
        const std::unordered_map<std::string, float>& GetUniform1f() const { return uniform1f; }
        std::unordered_map<std::string, float> GetUniform1f() { return uniform1f; }
        
        g_uniformf(2)
        g_uniformf(3)
        g_uniformf(4)
        
    private:
        std::unordered_map<std::string, float> uniform1f;
        uniformf(2);
        uniformf(3);
        uniformf(4);
        TexUnitsMap texUnits;
    };
}

#endif // KKE_PASS_HPP