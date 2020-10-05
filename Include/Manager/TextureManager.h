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

	GLuint* tileset_handle_;
	std::vector<glm::vec2> tex_vtx_;

public:

	Texture() = default;
	Texture(GLuint* tileset_handle, std::vector<glm::vec2> tex_vtx);

	GLuint* GetTilesetHandle();
	std::vector<glm::vec2> GetTexVtx();
	void SetTexVtx(std::vector<glm::vec2> new_vertex);
};

class Tileset {

	GLuint tileset_handle_;
	std::vector<std::string>* tile_names_;

public:

	Tileset() = default;
	Tileset(GLuint tileset_handle, std::vector<std::string>* tileset_name);

	GLuint* GetTilesetHandle();
	std::vector<std::string>* GetTileNames();
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

	std::vector<std::string> player_walk_ {

		"PlayerTiles",
		"Player_Walk0",
		"Player_Walk1",
		"Player_Walk2",
		"Player_Walk3",
		"Player_Walk4",
		"Player_Walk5",
		"Player_Walk6",
		"Player_Walk7"
	};				 

public:

	void Init() override;
	void TempFunctionForTesting();

	//Used for misc tilesets
	void CreateQuadTexture(std::string texture_name, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	void LoadMiscTextures();

	//Used for all other tilesets
	bool LoadTileset(const char* filename, size_t columns, size_t rows, std::vector<std::string> texture_names, size_t tile_width, size_t tile_height);

	//Cleanup
	bool UnloadTileset(std::string texture_name);
	void UnloadAllTilesets();

	//Getter
	Texture* GetTexture(std::string texture_name);

	~TextureManager();
};

#endif