#pragma once
#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_
#include "Manager/TextureManager.h"
#include "Manager/IManager.h"
#include <list>

class Animation {

	int num_frames_;
	GLuint animation_frames_;
	GLfloat frame_duration_;
	float offset_x_;
	std::vector<glm::vec2> tex_vtx_;

public:

	Animation() = default;
	Animation(int num_frames_, GLuint animation_frames, GLfloat frame_duration, float offset_x, std::vector<glm::vec2> tex_vtx);
	
	void ChangeFrameDuration(GLfloat frame_duration);
	float GetFrameDuration();
	GLuint* GetAnimationFramesHandle();
	std::vector<glm::vec2>* GetTexVtx();
	
	void SetToNextFrame();
	void SetToFirstFrame();
	bool IsLastFrame();
};

class AnimationSet {

	GLuint animation_frames_;
	std::vector<std::pair<std::string, int>>* animation_names_;

public:

	AnimationSet() = default;
	AnimationSet(GLuint animation_frames, std::vector<std::pair<std::string, int>>* animation_names);
	void UnloadAnimationSet();

};

class AnimationManager : public IManager {

	std::map<std::string, Animation> animations_;
	std::map<std::string, AnimationSet> animation_sets_;
	
	TextureManager* texture_manager_;

	std::vector<std::pair<std::string, int>> player_animations_{

		{"Player_Animations", 8}, //first element stores name of AnimationSet and the number of columns the file has
		{"Player_Walk", 8},
		{"Player_Idle", 8}
	};

public:

	void Init() override;
	void TempFunctionForTesting();
	void CreateAnimation(const char* filename, std::vector<std::pair<std::string, int>>* texture_name, GLfloat frame_duration);
	bool UnloadAnimationSet(std::vector<std::pair<std::string, int>>* animation_name);
	void UnloadAllAnimationSets();

	Animation GetAnimation(std::string animation_name);

	void ChangeAnimationFrameDuration(std::string animation_name, GLfloat new_frame_duration);
};

#endif