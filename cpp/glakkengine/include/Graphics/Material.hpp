/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_MATERIAL_HPP
#define KKE_MATERIAL_HPP

#include "Graphics/Technique.hpp"
#include "Interface/INoncopyable.hpp"
#include "ResourceType.hpp"

namespace kke
{
    /**
     * @brief Contains textures and techniques to render these. 
    Note that only one technique is used per material.
     **/
    class Material : public ResourceType, public INoncopyable
    {
    public:
        typedef std::vector<Technique> TechniqueVec;
        
    public:
        const TechniqueVec& GetTechniques() const { return techniques; }
        TechniqueVec& GetTechniques() { return techniques; }
        
    private:
        TechniqueVec techniques;
        
    };
}

#endif // KKE_MATERIAL_HPP