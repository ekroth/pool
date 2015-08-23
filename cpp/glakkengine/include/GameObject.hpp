/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef GAMECOMPONENT_HPP
#define GAMECOMPONENT_HPP

namespace kke
{
	class IRenderer;
    class ResourceManager;

    class GameObject
    {
    public:
		explicit GameObject() { }
		virtual ~GameObject() { }
		
        virtual void LoadContent() {  }
        virtual void UnloadContent() { }

        virtual void Update(float elapsed) { }
        virtual void Draw(kke::IRenderer& renderer) { }
    };
}

#endif
