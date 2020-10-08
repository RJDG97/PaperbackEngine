#pragma once
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include "Manager/IManager.h"

class Texture {

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
	\fn Texture(GLuint tileset_handle, std::vector<glm::vec2> tex_vtx)

	\brief Constructor for Texture that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
	Texture(GLuint tileset_handle, std::vector<glm::vec2> tex_vtx);

/******************************************************************************/
/*!
	\fn GetTilesetHandle()

	\brief Gets the handle of the tileset texture
*/
/******************************************************************************/
	GLuint* GetTilesetHandle();

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
	std::vector<std::string>* tile_names_;

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
	Tileset(GLuint tileset_handle, std::vector<std::string>* tileset_name);

/******************************************************************************/
/*!
	\fn GetTileNames()

	\brief Gets the name of all the tiles that is contained in the tileset
*/
/******************************************************************************/
	std::vector<std::string>* GetTileNames();

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

	//Serialize for names in the future
	std::vector<std::string> misc_tiles_ {

		"MiscTiles",
		"BlackQuad",
		"WhiteQuad"
	};

	std::vector<std::string> environment_tiles_	{

		"EnvironmentTiles",
		"Carpet_TopLeft",
		"Carpet_TopMid",
		"Carpet_TopRight",
		"Carpet_Left",
		"Carpet_Mid",
		"Carpet_Right",
		"Carpet_BtmLeft",
		"Carpet_BtmMid",
		"Carpet_BtmRight",
		"Carpet_Brick_BtmLeft",
		"Carpet_Brick_BtmMid",
		"Carpet_Brick_BtmRight",
		"Brick_Left",
		"Brick_Mid",
		"Brick_Right",
		"Empty",
		"Plank",
		"Stone",
		"Empty",
		"Empty",
		"Rock"
	};			 

	std::vector<std::string> background_test_{
		"BackgroundTest",
		"Background",
	};

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
	\fn Init()

	\brief Loads textures that will be used for the level, will make use of
		   serialization at a later time
*/
/******************************************************************************/
	void TempTextureBatchLoad();

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

	\brief Loads an image file and returns the handle to the texture file
*/
/******************************************************************************/
	GLuint LoadImageFile(const char* filename);

/******************************************************************************/
/*!
	\fn CreateTileset(const char* filename,
					  size_t columns, size_t rows,
					  std::vector<std::string> texture_names)

	\brief Creates a tileset using the path given and stores all the tiles
		   in the Texture Manager's map
*/
/******************************************************************************/
	void CreateTileset(const char* filename, size_t columns, size_t rows, std::vector<std::string> texture_names);

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
	\fn UnloadAllTilesets()

	\brief Gets texture with given name from the Texture Manager's map
*/
/******************************************************************************/
	Texture* GetTexture(std::string texture_name);

/******************************************************************************/
/*!
	\fn TextureManager()

	\brief Destructor for Texture Manager
*/
/******************************************************************************/
	~TextureManager();
};

#endif