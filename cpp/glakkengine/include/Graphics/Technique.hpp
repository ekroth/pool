/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_TECHNIQUE_HPP
#define KKE_TECHNIQUE_HPP

#include "Graphics/Pass.hpp"

#include <vector>

namespace kke
{
    /**
     * @brief Contains one or more passes, used by Material. Contains one or more passes.
    A Technique is a way/technique to render a material.
     **/
    class Technique
    {
    public:
        typedef std::vector<Pass> PassVec;
        
    public:
        void Draw();
        
        const PassVec& GetPasses() const { return passes; }
        PassVec& GetPasses() { return passes; }
        
    private:
        PassVec passes;
    };
}

#endif // KKE_TECHNIQUE_HPP