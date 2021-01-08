/**********************************************************************************
*\file         FontManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Font Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/FontManager.h"
#include "Systems/Debug.h"

Character::Character(unsigned int textureID,
                     glm::ivec2 size,
                     glm::ivec2 bearing,
                     unsigned int advance) :
    textureID_ {textureID},
    size_ {size},
    bearing_ {bearing},
    advance_ {advance}
{

}

unsigned int Character::GetTexID()
{
    return textureID_;
}

glm::ivec2 Character::GetSize()
{
    return size_;
}

glm::ivec2 Character::GetBearing()
{
    return bearing_;
}

unsigned int Character::GetAdvance()
{
    return advance_;
}


Font::Font(std::map<char, Character> characters) :
    characters_{characters}
{


}

std::map<char, Character>* Font::GetCharacters()
{
    return &characters_;
}

FontManager::~FontManager()
{

    FT_Done_FreeType(ft);
}

void FontManager::Init() {

    if (FT_Init_FreeType(&ft)) {

        DEBUG_ASSERT(!(1), "Could not init FreeType Library!");
    }

}

Font* FontManager::GetFont(std::string name)
{
    return &(fonts_.find(name)->second);
}

void FontManager::FontBatchLoad(std::string level_name)
{
    rapidjson::Document fonts_to_load;
    std::string path = "Resources/AssetsLoading/" + level_name + "_font.json";

    DeSerializeJSON(path, fonts_to_load);

    const rapidjson::Value& fonts_arr = fonts_to_load;
    DEBUG_ASSERT(fonts_arr.IsObject(), "Level JSON does not exist in proper format");

    for (rapidjson::Value::ConstMemberIterator font_it = fonts_arr.MemberBegin(); font_it != fonts_arr.MemberEnd(); ++font_it) {

        LoadFont(font_it->value.GetString());
    }
}

void FontManager::LoadFont(std::string font_name)
{
    auto it = fonts_.find(font_name);

    if (it != fonts_.end()) {

        return;
    }

    std::string pathname = "Resources/Font/" + font_name + ".ttf";

    FT_Face face;
    if (FT_New_Face(ft, pathname.c_str(), 0, &face)) {

        DEBUG_ASSERT(!(1), "Failed to load font!");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {

        DEBUG_ASSERT(!(1), "Failed to load Glyph!");
    }

    std::map<char, Character> temp;

    for (unsigned char c = 0; c < 128; c++) {

        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {

            DEBUG_ASSERT(!(1), "Failed to load Glyph!");
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = { texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                static_cast<unsigned int>(face->glyph->advance.x)
                              };

        temp.insert(std::pair<char, Character>(c, character));
    }

    fonts_[font_name] = Font(temp);
    FT_Done_Face(face);
}

void FontManager::DeSerializeJSON(const std::string& filename, rapidjson::Document& doc) {

    std::ifstream input_file(filename.c_str());
    DEBUG_ASSERT(input_file.is_open(), "File does not exist");

    // Read each line separated by a '\n' into a stringstream
    std::stringstream json_doc_buffer;
    std::string input;

    while (std::getline(input_file, input)) {

        json_doc_buffer << input << "\n";
    }

    // Close the file (.json) after
    input_file.close();

    // Parse the stringstream into document (DOM) format
    doc.Parse(json_doc_buffer.str().c_str());
}