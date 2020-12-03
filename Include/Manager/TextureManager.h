/**********************************************************************************
*\file         TextureManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Texture Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include "Manager/IManager.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "prettywriter.h"

class Texture {

	size_t width_;
	size_t height_;
	GLuint tileset_handle_;
	std::vector<glm::vec2> tex_vtx_;

public:

/******************************************************************************/
/*!
	\fn Texture()

	\brief Default constructor for Texture
*/
/******************************************************************************/
	Texture() = default;

/******************************************************************************/
/*!
	\fn Texture(size_t width, size_t height, GLuint tileset_handle, std::vector<glm::vec2> tex_vtx)

	\brief Constructor for Texture that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
	Texture(size_t width, size_t height, GLuint tileset_handle, std::vector<glm::vec2> tex_vtx);

/******************************************************************************/
/*!
	\fn GetWidth()

	\brief Gets width of texture.
*/
/******************************************************************************/
	size_t GetWidth();

/******************************************************************************/
/*!
	\fn GetHeight()

	\brief Gets height of texture.
*/
/******************************************************************************/
	size_t GetHeight();

/******************************************************************************/
/*!
	\fn GetTilesetHandle()

	\brief Gets the handle of the tileset texture
*/
/******************************************************************************/
	GLuint GetTilesetHandle();

/******************************************************************************/
/*!
	\fn GetTexVtx()

	\brief Gets the vertices of the tile within each tileset texture
*/
/******************************************************************************/
	std::vector<glm::vec2>* GetTexVtx();

/******************************************************************************/
/*!
	\fn SetTexVtx(std::vector<glm::vec2> new_vertex)

	\brief Sets the vertices of the tile within each tileset texture
*/
/******************************************************************************/
	void SetTexVtx(std::vector<glm::vec2> new_vertex);
};

class Tileset {

	GLuint tileset_handle_;
	std::vector<std::string> tile_names_;

public:

/******************************************************************************/
/*!
	\fn Tileset()

	\brief Default constructor for Tileset
*/
/******************************************************************************/
	Tileset() = default;

/******************************************************************************/
/*!
	\fn Tileset(GLuint tileset_handle, std::vector<std::string>* tileset_name)

	\brief Constructor for Tileset that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
	Tileset(GLuint tileset_handle, std::vector<std::string> tileset_name);

/******************************************************************************/
/*!
	\fn GetTileNames()

	\brief Gets the name of all the tiles that is contained in the tileset
*/
/******************************************************************************/
	std::vector<std::string> GetTileNames();

/******************************************************************************/
/*!
	\fn UnloadTileset()

	\brief Unloads the tileset's texture
*/
/******************************************************************************/
	void UnloadTileset();
};

class TextureManager : public IManager {

	std::map<std::string, Tileset> tilesets_;
	std::map<std::string, Texture> textures_;

public:

/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the Texture Manager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn TextureBatchLoad(std::string level_name)

	\brief Loads textures that will be used for the level
*/
/******************************************************************************/
	void TextureBatchLoad(std::string level_name);

/******************************************************************************/
/*!
	\fn DeSerializeJSON()

	\brief Loads the content of a JSON file into a doc
*/
/******************************************************************************/
	void DeSerializeJSON(const std::string& filename, rapidjson::Document& doc);

/******************************************************************************/
/*!
	\fn CreateQuadTexture(std::string texture_name,
						  unsigned char red,
						  unsigned char green,
						  unsigned char blue,
						  unsigned char alpha)

	\brief Creates a quad texture that is just a single color
*/
/******************************************************************************/
	void CreateQuadTexture(std::string texture_name, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

/******************************************************************************/
/*!
	\fn LoadMiscTextures()

	\brief Loads all the quad textures used, will make use of
		   serialization at a later time
*/
/******************************************************************************/
	void LoadMiscTextures();

/******************************************************************************/
/*!
	\fn LoadImageFile(const char* filename)

	\brief Loads an image file and returns the width, height and handle to the texture file
*/
/******************************************************************************/
	std::vector<GLuint> LoadImageFile(const char* filename);

/******************************************************************************/
/*!
	\fn CreateTileset(const char* filename, size_t columns, size_t rows,
					  std::string tileset_name)

	\brief Creates a tileset using the path given and stores all the tiles
		   in the Texture Manager's map
*/
/******************************************************************************/
	void CreateTileset(const char* filename, size_t columns, size_t rows,
					   std::string tileset_name);

/******************************************************************************/
/*!
	\fn UnloadTileset(std::string texture_name)

	\brief Unloads the tileset with the given name, and removes it from
		   the Texture Manager's map
*/
/******************************************************************************/
	bool UnloadTileset(std::string texture_name);

/******************************************************************************/
/*!
	\fn UnloadAllTilesets()

	\brief Unloads all tilesets, and removes them from the Texture
		   Manager's map
*/
/******************************************************************************/
	void UnloadAllTilesets();

/******************************************************************************/
/*!
	\fn  GetTexture(std::string texture_name)

	\brief Gets texture with given name from the Texture Manager's map
*/
/******************************************************************************/
	Texture* GetTexture(std::string texture_name);

/******************************************************************************/
/*!
	\fn  GetTextureMap()

	\brief Gets the Texture Manager map
*/
/******************************************************************************/
	std::map<std::string, Texture>& GetTextureMap();

/******************************************************************************/
/*!
	\fn  GetTextureMap()

	\brief Gets the Texture Manager map
*/
/******************************************************************************/
	Tileset* GetTileset(std::string tileset_name);

/******************************************************************************/
/*!
	\fn TextureManager()

	\brief Destructor for Texture Manager
*/
/******************************************************************************/
	~TextureManager();
};

#endif