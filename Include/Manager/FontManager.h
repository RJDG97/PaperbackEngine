#pragma once
#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include <ft2build.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <GL/glew.h>
#include "Manager/IManager.h"
#include FT_FREETYPE_H

class Character {

    unsigned int textureID_;  // ID handle of the glyph texture
    glm::ivec2   size_;       // Size of glyph
    glm::ivec2   bearing_;    // Offset from baseline to left/top of glyph
    unsigned int advance_;    // Offset to advance to next glyph

public:

    Character() = default;
    Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);
};

class Font {

    std::map<char, Character> characters_;

public:

    Font() = default;
    Font(std::map<char, Character> characters);
};


class FontManager : public IManager {

    FT_Library ft;
    std::map <std::string, Font> fonts_;

public:

    void Init() override;
    void LoadFont(std::string font_name);
    Font* GetFont(std::string name);
};

#endif