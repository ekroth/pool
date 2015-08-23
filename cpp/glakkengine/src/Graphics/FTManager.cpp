/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/FTManager.hpp"
#include "IO/LogManager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace kke;

FT_Library FTManager::GetLib()
{
    return library;
}

FTManager::FTManager()
{
    if (FT_Init_FreeType(&library))
    {
        LOGE() << "Error init FT_library.";
    }
}

FTManager::~FTManager()
{
    FT_Done_FreeType(library);
}