#include "TextureManager.h"
#include <FreeImage.h>
#include <iostream>

void TextureManager::Init()
{
    //Initialize FreeImage
    FreeImage_Initialise();
    std::cout << "FreeImage Version " << FreeImage_GetVersion() << std::endl;

    //load textures
    LoadTexture("Resources\\Sprites\\tiles.png", 0);
}

bool TextureManager::LoadTexture(const char* filename, size_t texID)
{
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

    //get the image width and height
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dib);
    BYTE* pixels = new BYTE[width * height * 4];

    //if this somehow one of these failed (they shouldn't), return failure
    if ((bits == 0) || (width == 0) || (height == 0))
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    for (int i = 0; i < width * height; i++)
    {
        pixels[i * 4 + 0] = bits[i * 4 + 2];
        pixels[i * 4 + 1] = bits[i * 4 + 1];
        pixels[i * 4 + 2] = bits[i * 4 + 0];
        pixels[i * 4 + 3] = bits[i * 4 + 3];
    }

    //if this texture ID is in use, unload the current texture
    if (textures.find(texID) != textures.end())
    {
        glDeleteTextures(1, &(textures[texID]));
    }

    glGenTextures(1, &texobj_hdl);
    glBindTexture(GL_TEXTURE_2D, texobj_hdl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    textures[texID] = texobj_hdl;

    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);

    //return success
    return true;
}
void TextureManager::BindTexture(size_t texID)
{
}
