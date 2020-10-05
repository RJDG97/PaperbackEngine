#include "Manager/TextureManager.h"
#include "Systems/Debug.h"
#include <FreeImage.h>
#include <iostream>

void TextureManager::Init() {

    //Initialize FreeImage
    FreeImage_Initialise();
    std::cout << "FreeImage Version " << FreeImage_GetVersion() << std::endl;

    M_DEBUG->WriteDebugMessage("Texture Manager Init\n");
}

void TextureManager::TempFunctionForTesting() {

    //create misc textures
    LoadMiscTextures();

    //load textures
    LoadTexture("Resources\\Sprites\\tiles.png", 3, 7, environment_tiles_, 32, 32);

    LoadTexture("Resources\\Sprites\\MC_Walk.png", 8, 1, player_walk_, 60, 128);

    LoadTexture("Resources\\Sprites\\Background.png", 1, 1, background_, 1524, 854);
}

void TextureManager::CreateQuadTexture(std::string texture_name, unsigned char red,
                                       unsigned char green, unsigned char blue, unsigned char alpha) {

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

void TextureManager::LoadMiscTextures() {

    CreateQuadTexture("BlackQuad", 0, 0, 0, 255);
    CreateQuadTexture("WhiteQuad", 255, 255, 255, 255);
}

bool TextureManager::LoadTexture(const char* filename,
                                 size_t columns, size_t rows,
                                 std::vector<std::string> texture_names,
                                 size_t tile_width, size_t tile_height) {

    std::cout << "Tileset is being loaded." << std::endl;

    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP* dib(0);
    //OpenGL's image ID to map to
    GLuint texobj_hdl;

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename, 0);
    
    //if format is cannot be deduced, try to guess the format from file extension
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename);
    }

    //if format is still unkown, return failure
    if (fif == FIF_UNKNOWN) {

        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {

        dib = FreeImage_Load(fif, filename, PNG_DEFAULT);

        //convert all images to 32 bits for reading
        if (FreeImage_GetBPP(dib) != 32) {

            dib = FreeImage_ConvertTo32Bits(FreeImage_Load(fif, filename, PNG_DEFAULT));
        }
    }

    //if the image failed to load, return failure
    if (!dib) {

        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dib);
    //get the image width and height
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    if ((bits == 0) || (width == 0) || (height == 0)) {

        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    BYTE* pixels = new BYTE[tile_width * tile_height * 4];

    for (int currentTile = 0; currentTile < columns * rows; ++currentTile) {

        if (texture_names[currentTile] == "Empty") {

            continue;
        }

        //if this texture ID is in use, unload the current texture
        UnloadTexture(texture_names[currentTile]);

        size_t startY = (rows - 1 - currentTile / columns) * tile_height;
        size_t startX = (currentTile % columns) * tile_width;
        int i = 0;

        for (size_t y = startY; y < startY + tile_height; ++y) {

            for (size_t x = startX; x < startX + tile_width; ++x, ++i) {

                size_t j = y * tile_width * columns + x;
                pixels[i * 4 + 0] = bits[j * 4 + 2];
                pixels[i * 4 + 1] = bits[j * 4 + 1];
                pixels[i * 4 + 2] = bits[j * 4 + 0];
                pixels[i * 4 + 3] = bits[j * 4 + 3];
            }
        }

        glGenTextures(1, &texobj_hdl);
        glBindTexture(GL_TEXTURE_2D, texobj_hdl);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(tile_width), static_cast<GLsizei>(tile_height),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        textures_[texture_names[currentTile]] = texobj_hdl;
    }

    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    delete[]pixels;

    //return success
    std::cout << "Tileset successfully loaded!" << std::endl;
    return true;
}

bool TextureManager::UnloadTexture(std::string texture_name) {

    //if this texture ID is in use, unload the current texture
    if (textures_.find(texture_name) != textures_.end()) {

        glDeleteTextures(1, &(textures_[texture_name]));
        textures_.erase(texture_name);
        std::cout << "Texture " << texture_name << "is unloaded" << std::endl;
        return true;
    }

    return false;
}

void TextureManager::UnloadTexture(std::map<std::string, Texture>::iterator texture_iterator) {

    glDeleteTextures(1, &(*texture_iterator).second);
}

void TextureManager::UnloadAllTextures() {

    for (auto i = textures_.begin(); i != textures_.end(); ++i)
    {
        UnloadTexture(i);
    }
}

Texture* TextureManager::GetTexture(std::string texture_name) {

    return &textures_[texture_name];
}

TextureManager::~TextureManager() {

    UnloadAllTextures();
}