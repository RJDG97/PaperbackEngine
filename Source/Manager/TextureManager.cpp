#include "Manager/TextureManager.h"
#include "Systems/Debug.h"
#include <FreeImage.h>
#include <iostream>

TextureManager* TEXTUREMANAGER;

void TextureManager::Init()
{
    //Initialize FreeImage
    FreeImage_Initialise();
    std::cout << "FreeImage Version " << FreeImage_GetVersion() << std::endl;

    M_DEBUG->WriteDebugMessage("Texture Manager Init\n");
}

void TextureManager::TempFunctionForTesting()
{
    //create misc textures
    LoadMiscTextures();

    //load textures
    LoadTexture("Resources\\Sprites\\tiles.png", 3, 7, environment_tiles_, 32);
}

void TextureManager::CreateQuadTexture(TextureName texture_name, unsigned char red,
                                       unsigned char green, unsigned char blue, unsigned char alpha)
{
    GLuint texobj_hdl;
    BYTE* pixels = new BYTE[4];

    pixels[0] = blue;
    pixels[1] = green;
    pixels[2] = red;
    pixels[3] = alpha;

    glGenTextures(1, &texobj_hdl);
    glBindTexture(GL_TEXTURE_2D, texobj_hdl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1,
        0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    textures_[texture_name] = texobj_hdl;
    delete[] pixels;
}

void TextureManager::LoadMiscTextures()
{
    CreateQuadTexture(TextureName::BlackQuad, 0, 0, 0, 255);
    CreateQuadTexture(TextureName::WhiteQuad, 255, 255, 255, 255);
}

bool TextureManager::LoadTexture(const char* filename, size_t columns, size_t rows, std::vector<TextureName> texture_names, size_t tile_size)
{
    std::cout << "Tileset is being loaded." << std::endl;

    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP* dib(0);
    //OpenGL's image ID to map to
    GLuint texobj_hdl;

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename, 0);
    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename);
    //if still unkown, return failure
    if (fif == FIF_UNKNOWN)
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, filename, PNG_DEFAULT);

        if (FreeImage_GetBPP(dib) != 32)
        {
            dib = FreeImage_ConvertTo32Bits(FreeImage_Load(fif, filename, PNG_DEFAULT));
        }
    }

    //if the image failed to load, return failure
    if (!dib)
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dib);
    //get the image width and height
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    if ((bits == 0) || (width == 0) || (height == 0))
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    BYTE* pixels = new BYTE[tile_size * tile_size * 4];

    for (int currentTile = 0; currentTile < columns * rows; ++currentTile)
    {
        if (texture_names[currentTile] == Empty)
        {
            continue;
        }

        //if this texture ID is in use, unload the current texture
        UnloadTexture(texture_names[currentTile]);

        const int startY = (rows - 1 - currentTile / columns) * tile_size;
        const int startX = (currentTile % columns) * tile_size;
        int i = 0;

        for (int y = startY; y < startY + tile_size; ++y)
        {
            for (int x = startX; x < startX + tile_size; ++x, ++i)
            {
                int j = y * tile_size * columns + x;
                pixels[i * 4 + 0] = bits[j * 4 + 2];
                pixels[i * 4 + 1] = bits[j * 4 + 1];
                pixels[i * 4 + 2] = bits[j * 4 + 0];
                pixels[i * 4 + 3] = bits[j * 4 + 3];
            }
        }

        glGenTextures(1, &texobj_hdl);
        glBindTexture(GL_TEXTURE_2D, texobj_hdl);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_size, tile_size,
            0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        textures_[texture_names[currentTile]] = texobj_hdl;
    }

    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    delete []pixels;

    //return success
    std::cout << "Tileset successfully loaded!" << std::endl;
    return true;
}

bool TextureManager::UnloadTexture(GLint tex_id)
{
    //if this texture ID is in use, unload the current texture
    if (textures_.find(tex_id) != textures_.end())
    {
        glDeleteTextures(1, &(textures_[tex_id]));
        textures_.erase(tex_id);
        std::cout << "Texture " << tex_id << "is unloaded" << std::endl;
        return true;
    }

    return false;
}

void TextureManager::UnloadAllTextures()
{
    glDeleteTextures(textures_.size(), &(textures_[0]));
}

Texture* TextureManager::GetTexture(GLint tex_id)
{
    return &textures_[tex_id];
}

TextureManager::TextureManager()
{
    TEXTUREMANAGER = this;
}

TextureManager::~TextureManager()
{
    UnloadAllTextures();
}
