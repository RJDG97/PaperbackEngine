#pragma once
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_
#include <GL/glew.h>
#include <map>
#include <vector>
#include "Manager/IManager.h"

typedef GLuint Texture; //idk if this should be here

enum TextureName
{
	Empty,
	
	//misc_tiles
	BlackQuad,
	WhiteQuad,

	//environment_tiles
	Carpet_TopLeft,
	Carpet_TopMid,
	Carpet_TopRight,
	Carpet_Left,
	Carpet_Mid,
	Carpet_Right,
	Carpet_BtmLeft,
	Carpet_BtmMid,
	Carpet_BtmRight,
	Carpet_Brick_BtmLeft,
	Carpet_Brick_BtmMid,
	Carpet_Brick_BtmRight,
	Brick_Left,
	Brick_Mid,
	Brick_Right,
	Plank,
	Stone,
	Rock
};

class TextureManager : public IManager
{
	std::map<size_t, Texture> textures_;

	std::vector<TextureName> misc_tiles_
	{
		BlackQuad,
		WhiteQuad
	};

	std::vector<TextureName> environment_tiles_
	{
		Carpet_TopLeft,
		Carpet_TopMid,
		Carpet_TopRight,
		Carpet_Left,
		Carpet_Mid,
		Carpet_Right,
		Carpet_BtmLeft,
		Carpet_BtmMid,
		Carpet_BtmRight,
		Carpet_Brick_BtmLeft,
		Carpet_Brick_BtmMid,
		Carpet_Brick_BtmRight,
		Brick_Left,
		Brick_Mid,
		Brick_Right,
		Empty,
		Plank,
		Stone,
		Empty,
		Empty,
		Rock
	};

public:

	void Init() override;
	void TempFunctionForTesting();

	//Used for misc textures
	void CreateQuadTexture(TextureName texture_name, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	void LoadMiscTextures();

	//Used for all other textures
	bool LoadTexture(const char* filename, size_t columns, size_t rows, std::vector<TextureName> texture_names, size_t tile_size);

	//Cleanup
	bool UnloadTexture(GLint tex_id);
	void UnloadAllTextures();

	//Getter
	Texture* GetTexture(GLint tex_id);

	TextureManager();
	~TextureManager();
};

extern TextureManager* TEXTUREMANAGER;

#endif