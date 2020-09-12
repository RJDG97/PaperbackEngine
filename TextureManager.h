#pragma once
#include <GL/glew.h>
#include <map>
#include <vector>

enum TextureNames
{
	Empty,
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

class TextureManager
{
public:

	void Init();
	bool LoadTexture(const char* filename, size_t columns, size_t rows, std::vector<TextureNames> tile_names, size_t tile_size);
	void BindTexture(size_t texID);

	std::vector<TextureNames> environment_tiles
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

	std::map<size_t, GLuint> textures;
};