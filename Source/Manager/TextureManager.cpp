#include "Manager/TextureManager.h"
#include "Systems/Debug.h"
#include <FreeImage.h>
#include <iostream>

Texture::Texture(GLuint tileset_handle, std::vector<glm::vec2> tex_vtx) :
    tileset_handle_{ tileset_handle },
    tex_vtx_ { tex_vtx } {

}

GLuint Texture::GetTilesetHandle() {

    return tileset_handle_;
}

std::vector<glm::vec2>* Texture::GetTexVtx() {

    return &tex_vtx_;
}

void Texture::SetTexVtx(std::vector<glm::vec2> new_vertex) {

	tex_vtx_ = new_vertex;
}

Tileset::Tileset(GLuint tileset_handle, std::vector<std::string>* tileset_name) :
    tileset_handle_ { tileset_handle },
    tile_names_ { tileset_name } {
    
}

std::vector<std::string>* Tileset::GetTileNames() {

    return tile_names_;
}

void Tileset::UnloadTileset() {

    glDeleteTextures(1, &tileset_handle_);
}

void TextureManager::Init() {

    //Initialize FreeImage
    FreeImage_Initialise();
    std::cout << "FreeImage Version " << FreeImage_GetVersion() << std::endl;

    M_DEBUG->WriteDebugMessage("Texture Manager Init\n");
}

void TextureManager::TextureBatchLoad(std::string level_name) {

    rapidjson::Document textures_to_load;
    std::string path = "Resources/AssetsLoading/" + level_name + "_texture.json";

    DeSerializeJSON(path, textures_to_load);

    const rapidjson::Value& files_arr = textures_to_load;
    DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

    //handle sorting of information into the map
    for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

        std::string path_name{ file_it->value.GetString() };

        rapidjson::Document texture_data;
        DeSerializeJSON(path_name, texture_data);
        
        //Only one element in this array
        const rapidjson::Value& texture_arr = texture_data;
        DEBUG_ASSERT(texture_arr.IsObject(), "Level JSON does not exist in proper format");
        std::string texture_pathname = std::string{"Resources/Sprites/"} + texture_arr.MemberBegin()->name.GetString();

        const rapidjson::Value& texture_param = *texture_arr.MemberBegin()->value.Begin();
        
        rapidjson::Value::ConstMemberIterator texture_param_it = texture_param.MemberBegin();
        
        std::string tileset_name = (texture_param_it++)->value.GetString();
        int columns = std::stoi((texture_param_it++)->value.GetString());
        int rows = std::stoi((texture_param_it++)->value.GetString());

        std::vector<std::string> tile_names;

        for (; texture_param_it != texture_param.MemberEnd(); ++texture_param_it)
        {
            tile_names.push_back(texture_param_it->value.GetString());
        }

        CreateTileset(texture_pathname.c_str(), columns, rows, tile_names, tileset_name);
    }
}

void TextureManager::DeSerializeJSON(const std::string& filename, rapidjson::Document& doc) {

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

    //haven't decided on how to store these!
    textures_[texture_name] = Texture{ texobj_hdl, { {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}} };
    delete[] pixels;
}

void TextureManager::LoadMiscTextures() {

    CreateQuadTexture("DarknessTexture", 100, 65, 80, 255);
    //CreateQuadTexture("WhiteQuad", 255, 255, 255, 255);
}

GLuint TextureManager::LoadImageFile(const char* filename) {

    std::cout << "Tileset is being loaded : " << filename << std::endl;

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
    DEBUG_ASSERT(!(fif == FIF_UNKNOWN), (std::string{"Format of image file is unknown : "} += filename).c_str());

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {

        dib = FreeImage_Load(fif, filename, PNG_DEFAULT);

        //convert all images to 32 bits for reading
        if (FreeImage_GetBPP(dib) != 32) {

            dib = FreeImage_ConvertTo32Bits(FreeImage_Load(fif, filename, PNG_DEFAULT));
        }
    }

    //if the image failed to load, return failure
    DEBUG_ASSERT(dib, (std::string{ "Failed to load image : " } += filename).c_str());

    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dib);
    //get the image width and height
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    DEBUG_ASSERT(!(bits == 0) || (width == 0) || (height == 0),
        (std::string{ "Bits and/or Width and/or Height of image file is zero : " } += filename).c_str());

    glGenTextures(1, &texobj_hdl);
    glBindTexture(GL_TEXTURE_2D, texobj_hdl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);

    //return success
    std::cout << "Tileset successfully loaded : " << filename << std::endl;
    return texobj_hdl;
}

void TextureManager::CreateTileset(const char* filename, size_t columns, size_t rows, std::vector<std::string>& texture_names, std::string tileset_name)
{
    auto it = tilesets_.find(tileset_name);

    if (it != tilesets_.end()) {

        std::cout << "Tileset already exists!" << std::endl;
        return;
    }

    GLuint tileset_handle_ = LoadImageFile(filename);
    tilesets_[tileset_name] = { tileset_handle_, &texture_names };

    glm::vec2 offset{ 1.0f / columns, 1.0f / rows };

    for (int y = 0; y < rows; ++y) {

        for (int x = 0; x < columns; ++x) {

            if (texture_names[y * columns + x] == "Empty") {

                continue;
            }

            glm::vec2 origin{ x / static_cast<float>(columns) ,
                               1 - y / static_cast<float>(rows) - offset.y };

            // + 1 skips over the name of the tilset
            textures_[texture_names[y * columns + x]] =
                Texture{ tileset_handle_,
                         { {origin.x, origin.y},
                           {origin.x + offset.x, origin.y},
                           {origin.x, origin.y + offset.y},
                           {origin.x + offset.x, origin.y + offset.y} } };
        }
    }
}

bool TextureManager::UnloadTileset(std::string tileset_name) {

    auto it = tilesets_.find(tileset_name);

    //if this texture ID is in use, unload the current texture
    if (it != tilesets_.end()) {

        it->second.UnloadTileset();
        std::vector<std::string>* tile_name = it->second.GetTileNames();

        for (int i = 0; i < tile_name->size(); ++i) {

            textures_.erase((*tile_name)[i]);
        }

        tilesets_.erase(tileset_name);

        std::cout << "Tileset " << tileset_name << "is unloaded" << std::endl;
        return true;
    }

    return false;
}

void TextureManager::UnloadAllTilesets() {

    for (auto it = tilesets_.begin(); it != tilesets_.end(); ++it) {

        it->second.UnloadTileset();
    }

    textures_.clear();
    tilesets_.clear();
}

Texture* TextureManager::GetTexture(std::string texture_name) {

    return &textures_[texture_name];
}

std::map<std::string, Texture>& TextureManager::GetTextureMap() {

    return textures_;
}

TextureManager::~TextureManager() {

    UnloadAllTilesets();
}