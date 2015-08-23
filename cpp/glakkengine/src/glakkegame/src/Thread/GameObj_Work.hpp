/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef GAMEOBJ_WORK_HPP
#define GAMEOBJ_WORK_HPP

namespace kke
{
    class GameObject;

    enum GameObj_Work
    {
        GameObj_Load,
        GameObj_Unload,
        GameObj_Update
    };

    struct GameObj_Data
    {
        GameObject* Object;
        void* Parameter;
        GameObj_Work Work;
        void* Owner;
    };
}

#endif
