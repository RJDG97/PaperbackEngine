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

void FontManager::Init() {

    if (FT_Init_FreeType(&ft))
    {
        DEBUG_ASSERT(!(1), "Could not init FreeType Library!");
    }

}

Font* FontManager::GetFont(std::string name)
{
    return &(fonts_.find(name)->second);
}

void FontManager::LoadFont(std::string font_name)
{
    std::string pathname = "Resources\\Font\\" + font_name + ".ttf";

    FT_Face face;
    if (FT_New_Face(ft, pathname.c_str(), 0, &face))
    {
        DEBUG_ASSERT(!(1), "Failed to load font!");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        DEBUG_ASSERT(!(1), "Failed to load Glyph!");
    }

    std::map<char, Character> temp;

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
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

    fonts_.insert({ font_name, Font(temp) });
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}