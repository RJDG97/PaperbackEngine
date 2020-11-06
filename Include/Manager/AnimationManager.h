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

/******************************************************************************/
/*!
	\fn Animation()

	\brief Default constructor for Animation
*/
/******************************************************************************/
	Animation() = default;

/******************************************************************************/
/*!
	\fn Animation(int num_frames_,
				  GLuint animation_frames,
				  GLfloat frame_duration,
				  float offset_x,
				  std::vector<glm::vec2> tex_vtx)

	\brief Constructor for Animation that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
	Animation(int num_frames_,
			  GLuint animation_frames,
			  GLfloat frame_duration,
			  float offset_x,
			  std::vector<glm::vec2> tex_vtx);
	
/******************************************************************************/
/*!
	\fn ChangeFrameDuration(GLfloat frame_duration)

	\brief Changes the duration each frame is displayed on the screen
*/
/******************************************************************************/
	void ChangeFrameDuration(GLfloat frame_duration);

/******************************************************************************/
/*!
	\fn GetNumFrames()

	\brief Gets the number of frames that the animation contains
*/
/******************************************************************************/
	int GetNumFrames();

/******************************************************************************/
/*!
	\fn GetAnimationFramesHandle()

	\brief Gets the handle of the texture used to render the animation
*/
/******************************************************************************/
	GLuint* GetAnimationFramesHandle();

/******************************************************************************/
/*!
	\fn GetFrameDuration()

	\brief Gets the duration that each frame is displayed on the screen
*/
/******************************************************************************/
	float GetFrameDuration();

/******************************************************************************/
/*!
	\fn GetOffsetX()

	\brief Gets the offset applied to all vertex to move to the next frame of
		   the animation
*/
/******************************************************************************/
	float GetOffsetX();

/******************************************************************************/
/*!
	\fn GetTexVtx()

	\brief Gets the vertex of the first animation frame
*/
/******************************************************************************/
	std::vector<glm::vec2>* GetTexVtx();
};

class AnimationSet {

	GLuint animation_frames_;
	std::vector<std::pair<std::string, int>>* animation_names_frames_;

public:

/******************************************************************************/
/*!
	\fn AnimationSet()

	\brief Default constructor of an Animation Set
*/
/******************************************************************************/
	AnimationSet() = default;

/******************************************************************************/
/*!
	\fn AnimationSet(GLuint animation_frames,
					 std::vector<std::pair<std::string, int>>* animation_names)

	\brief Constructor for AnimationSet that initializes all its data members
		   according to the respective values
*/
/******************************************************************************/
	AnimationSet(GLuint animation_frames,
				 std::vector<std::pair<std::string, int>>* animation_names);

/******************************************************************************/
/*!
	\fn UnloadAnimationSet()

	\brief Unloads the texture used for the animation set and erases it from
		   the AnimationManger's animation set container and animation container
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the Animation Manager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn TempAnimationBatchLoad()

	\brief Loads animations that will be used for the level, will make use of
		   serialization at a later time
*/
/******************************************************************************/
	void TempAnimationBatchLoad();

/******************************************************************************/
/*!
	\fn AnimationBatchLoad(std::string level_name)

	\brief Loads animations that will be used for the level
*/
/******************************************************************************/
void AnimationBatchLoad(std::string level_name);

/******************************************************************************/
/*!
	\fn CreateAnimation(const char* filename,
						std::string animation_set_name,
						int columns,
						int rows,
						std::vector<std::pair<std::string, int>>* animation_names_frames,
						std::vector<GLfloat> frame_durations)

	\brief Loads an image file, creates and stores all the animations that the
		   file contains into the Animation Manager's map.
*/
/******************************************************************************/
	void CreateAnimation(const char* filename,
						 std::string animation_set_name,
						 int columns,
						 int rows,
						 std::vector<std::pair<std::string, int>>* animation_names_frames,
						 std::vector<GLfloat> frame_durations);

/******************************************************************************/
/*!
	\fn UnloadAnimationSet(std::vector<std::pair<std::string, int>>* animation_name)

	\brief Unloads an image file, deletes and removes all the animations that the
		   file contains from the Animation Manager's map.
*/
/******************************************************************************/
	bool UnloadAnimationSet(std::vector<std::pair<std::string, int>>* animation_name);

/******************************************************************************/
/*!
	\fn UnloadAllAnimationSets()

	\brief Unloads all animation sets that are stored within the Animation
		   Manager's map
*/
/******************************************************************************/
	void UnloadAllAnimationSets();

/******************************************************************************/
/*!
	\fn GetAnimation()

	\brief Gets an animation from the Animation Manager's map
*/
/******************************************************************************/
	Animation GetAnimation(std::string animation_name);

/******************************************************************************/
/*!
	\fn ChangeAnimationFrameDuration(std::string animation_name,
									 GLfloat new_frame_duration)

	\brief Changes the duration each frame is displayed for
*/
/******************************************************************************/
	void ChangeAnimationFrameDuration(std::string animation_name, GLfloat new_frame_duration);
};

#endif