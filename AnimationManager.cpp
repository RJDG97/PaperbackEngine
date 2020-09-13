#include "AnimationManager.h"
#include "GraphicsSystem.h"

AnimationManager::AnimationManager()
	: texture_manager{ GraphicsSystem::Instance().GetTextureManager() }
{

}

void AnimationManager::Init()
{
	texture_manager = GraphicsSystem::Instance().GetTextureManager();
}

void AnimationManager::AddAnimation(AnimationName name, size_t num_frames, size_t texID)
{
	std::vector<Texture*> temp;

	for (int i = 0; i < num_frames; ++i)
	{
		temp.push_back(texture_manager.GetTexture(texID + i));
	}

	animations[name] = temp;
}