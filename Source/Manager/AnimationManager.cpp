#include "Manager/AnimationManager.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

Animation::Animation(int num_frames, GLuint animation_frames, GLfloat frame_duration, float offset_x, std::vector<glm::vec2> tex_vtx) :
	num_frames_ { num_frames },
	animation_frames_ { animation_frames },
	frame_duration_ { frame_duration },
	offset_x_ { offset_x },
	tex_vtx_ { tex_vtx } {

}

void Animation::ChangeFrameDuration(GLfloat frame_duration) {
	
	frame_duration_ = frame_duration;
}

float Animation::GetFrameDuration() {
	
	return frame_duration_;
}

int Animation::GetNumFrames() {

	return num_frames_;
}

GLuint* Animation::GetAnimationFramesHandle() {

	return &animation_frames_;
}

float Animation::GetOffsetX() {

	return offset_x_;
}

std::vector<glm::vec2>* Animation::GetTexVtx() {

	return &tex_vtx_;
}

AnimationSet::AnimationSet(GLuint animation_frames, std::vector<std::pair<std::string, int>>* animation_names) :
	animation_frames_{ animation_frames },
	animation_names_{ animation_names } {

}

void AnimationSet::UnloadAnimationSet() {

	glDeleteTextures(1, &animation_frames_);
}


void AnimationManager::Init() {
	
	M_DEBUG->WriteDebugMessage("Animation Manager Init\n");
	texture_manager_ = CORE->GetManager<TextureManager>();
}

void AnimationManager::TempFunctionForTesting() {
	
	CreateAnimation("Resources\\Sprites\\MC_States.png", &player_animations_, 0.07f);
}

void AnimationManager::CreateAnimation(const char* filename,
									   std::vector<std::pair<std::string, int>>* animation_name,
									   GLfloat frame_duration) {
	
	GLuint image_handle = texture_manager_->LoadImageFile(filename);
	float offset_x = 1.0f / (*animation_name)[0].second;

	for (int i = 1; i < animation_name->size(); ++i) {

		std::vector<glm::vec2> temp{ { 0.0f, 1 - (i * 1.0f / (animation_name->size() - 1)) },
									 { offset_x, 1 - (i * 1.0f / (animation_name->size() - 1)) },
									 { 0.0f, 1 - ((i - 1) * 1.0f / (animation_name->size() - 1)) },
									 { offset_x, 1 - ((i - 1) * 1.0f / (animation_name->size() - 1)) } };

		int num_frames = (*animation_name)[i].second;
		animations_[(*animation_name)[i].first] = Animation{ num_frames, image_handle, frame_duration, offset_x,  temp };
	}

	animation_sets_[(*animation_name)[0].first] = AnimationSet{ image_handle, animation_name };
}

bool AnimationManager::UnloadAnimationSet(std::vector<std::pair<std::string, int>>* animation_name) {
	
	auto it = animation_sets_.find((*animation_name)[0].first);

	if (it != animation_sets_.end()) {

		for (int i = 1; i < animation_name->size(); ++i) {

			animations_.erase((*animation_name)[i].first);
		}

		it->second.UnloadAnimationSet();
		animation_sets_.erase(it);
		return true;
	}

	return false;
}

void AnimationManager::UnloadAllAnimationSets()
{
	for (auto it = animation_sets_.begin(); it != animation_sets_.end(); ++it) {

		it->second.UnloadAnimationSet();
	}

	animations_.clear();
	animation_sets_.clear();
}

Animation AnimationManager::GetAnimation(std::string animation_name) {
	
	return animations_[animation_name];
}

void AnimationManager::ChangeAnimationFrameDuration(std::string animation_name, GLfloat new_frame_duration) {
	
	animations_[animation_name].ChangeFrameDuration(new_frame_duration);
}