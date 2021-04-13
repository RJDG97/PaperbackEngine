/**********************************************************************************
*\file         FontManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Font Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <GL/glew.h>
#include "Manager/IManager.h"
#include <ft2build.h>
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "prettywriter.h"
#include FT_FREETYPE_H

class Character {

    unsigned int textureID_;  // ID handle of the glyph texture
    glm::ivec2   size_;       // Size of glyph
    glm::ivec2   bearing_;    // Offset from baseline to left/top of glyph
    unsigned int advance_;    // Offset to advance to next glyph

public:

/******************************************************************************/
/*!
    \fn Character()

    \brief Default constructor for Character
*/
/******************************************************************************/
    Character() = default;

/******************************************************************************/
/*!
    \fn Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing,
                  unsigned int advance)

    \brief Constructor for character that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
    Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);

/******************************************************************************/
/*!
    \fn GetTexID()

    \brief Get texture ID for character
*/
/******************************************************************************/
    unsigned int GetTexID();

/******************************************************************************/
/*!
    \fn GetSize()

    \brief Get size of character
*/
/******************************************************************************/
    glm::ivec2 GetSize();

/******************************************************************************/
/*!
    \fn GetBearing()

    \brief Get bearing of character
*/
/******************************************************************************/
    glm::ivec2 GetBearing();

/******************************************************************************/
/*!
    \fn GetAdvance()

    \brief Get advance of character
*/
/******************************************************************************/
    unsigned int GetAdvance();
    
};

class Font {

    std::map<char, Character> characters_;

public:

/******************************************************************************/
/*!
    \fn Font()

    \brief Default constructor for font
*/
/******************************************************************************/
    Font() = default;

/******************************************************************************/
/*!
    \fn Font(std::map<char, Character> characters)

    \brief Constructor for Font that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
    Font(std::map<char, Character> characters);

/******************************************************************************/
/*!
    \fn GetCharacters()

    \brief Gets the characters within a font
*/
/******************************************************************************/
    std::map<char, Character>* GetCharacters();
};


class FontManager : public IManager {

    FT_Library ft;
    std::map <std::string, Font> fonts_;

public:

/******************************************************************************/
/*!
    \fn ~FontManager()

    \brief Destructor for Font Manager
*/
/******************************************************************************/
    ~FontManager();

/******************************************************************************/
/*!
    \fn Init()

    \brief Initializes the Font Manager
*/
/******************************************************************************/
    void Init() override;

/******************************************************************************/
/*!
    \fn GetFont(std::string name)

    \brief Gets font from the Font Manager's container
*/
/******************************************************************************/
    Font* GetFont(std::string name);

/******************************************************************************/
/*!
    \fn LoadFont(std::string font_name)

    \brief Loads a font into the Font Manager's container
*/
/******************************************************************************/
    void LoadFont(std::string font_name);

/******************************************************************************/
/*!
    \fn FontBatchLoad(std::string level_name)

    \brief Loads all fonts used for the level
*/
/******************************************************************************/
    void FontBatchLoad(std::string level_name);

/******************************************************************************/
/*!
    \fn DeSerializeJSON()

    \brief Loads the content of a JSON file into a doc
*/
/******************************************************************************/
    void DeSerializeJSON(const std::string& filename, rapidjson::Document& doc);
};

#endif