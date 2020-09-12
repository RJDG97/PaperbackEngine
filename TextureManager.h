#pragma once
#include <GL/glew.h>
#include <map>

class TextureManager
{
public:

	void Init();
	bool LoadTexture(const char* filename, size_t texID);
	void BindTexture(size_t texID);

	std::map<size_t, GLuint> textures;
};