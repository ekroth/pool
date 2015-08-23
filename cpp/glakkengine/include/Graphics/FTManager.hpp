/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_FTMANAGER_HPP
#define KKE_FTMANAGER_HPP

#include "Utilities/Singleton.hpp"
#include "Interface/INoncopyable.hpp"

typedef struct FT_LibraryRec_ *FT_Library;

namespace kke
{
    class FTManager : public Singleton<FTManager>
    {
        friend class Singleton<FTManager>;
        
    public:        
		FT_Library GetLib();

    private:
		 FTManager();
        ~FTManager();

        FT_Library library;
    };
}

#endif // KKE_FTMANAGER_HPP
