#include "Manager/AnimationManager.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

Animation::Animation(int num_frames,
					 GLuint animation_frames,
					 GLfloat frame_duration,
					 float offset_x,
					 std::vector<glm::vec2> tex_vtx) :
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

GLuint Animation::GetAnimationFramesHandle() {

	return animation_frames_;
}

float Animation::GetOffsetX() {

	return offset_x_;
}

std::vector<glm::vec2>* Animation::GetTexVtx() {

	return &tex_vtx_;
}

AnimationSet::AnimationSet(GLuint animation_frames,
						   std::vector<std::pair<std::string, int>>* animation_names_frames) :
	animation_frames_{ animation_frames },
	animation_names_frames_{ animation_names_frames } {

}

void AnimationSet::UnloadAnimationSet() {

	glDeleteTextures(1, &animation_frames_);
}


void AnimationManager::Init() {

	M_DEBUG->WriteDebugMessage("Animation Manager Init\n");
	texture_manager_ = &*CORE->GetManager<TextureManager>();
}

void AnimationManager::AnimationBatchLoad(std::string level_name) {

	rapidjson::Document animations_to_load;
	std::string path = "Resources/AssetsLoading/" + level_name + "_animation.json";

	DeSerializeJSON(path, animations_to_load);

	const rapidjson::Value& files_arr = animations_to_load;
	DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

	//handle sorting of information into the map
	for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

		std::string path_name{ file_it->value.GetString() };

		rapidjson::Document animation_set_data;
		DeSerializeJSON(path_name, animation_set_data);

		//Only one element in this array
		const rapidjson::Value& animation_set_arr = animation_set_data;
		DEBUG_ASSERT(animation_set_arr.IsObject(), "Level JSON does not exist in proper format");


		const rapidjson::Value& animation_set_param = *animation_set_arr.MemberBegin()->value.Begin();

		rapidjson::Value::ConstMemberIterator animation_set_param_it = animation_set_param.MemberBegin();

		std::string texture_pathname = std::string{ "Resources/Sprites/" } + (animation_set_param_it++)->value.GetString() +
											"/" + animation_set_arr.MemberBegin()->name.GetString();

		std::string animation_set_name = (animation_set_param_it++)->value.GetString();
		int columns = std::stoi((animation_set_param_it++)->value.GetString());
		int rows = std::stoi((animation_set_param_it++)->value.GetString());

		std::vector<std::pair<std::string, int>> animation_names_frames;
		std::vector<GLfloat> frame_durations;

		for (; animation_set_param_it != animation_set_param.MemberEnd(); ++animation_set_param_it) {

			std::stringstream stream;
			stream << animation_set_param_it->value.GetString();

			std::string animation_name;
			int number_of_frames;
			float frame_duration;

			stream >> animation_name >> number_of_frames >> frame_duration;

			animation_names_frames.push_back({animation_name, number_of_frames});
			frame_durations.push_back(frame_duration);
		}

		CreateAnimation(texture_pathname.c_str(), animation_set_name,
						columns, rows,
						&animation_names_frames, frame_durations);
	}
}

void AnimationManager::CreateAnimation(const char* filename,
									   std::string animation_set_name,
									   int columns,
									   int rows,
									   std::vector<std::pair<std::string, int>>* animation_names_frames,
									   std::vector<GLfloat> frame_durations) {
	
	GLuint image_handle = texture_manager_->LoadImageFile(filename);
	float offset_x = 1.0f / columns;

	for (int i = 0; i < rows; ++i) {

		if ((*animation_names_frames)[i].first == "Blank") {

			continue;
		}

		std::vector<glm::vec2> temp{ { 0.0f, 1 - ((i+1) * 1.0f / rows) },
									 { offset_x, 1 - ((i+1) * 1.0f / rows) },
									 { 0.0f, 1 - (i * 1.0f / rows) },
									 { offset_x, 1 - (i * 1.0f / rows) } };

		int num_frames = (*animation_names_frames)[i].second;
		animations_[(*animation_names_frames)[i].first] = Animation{ num_frames, image_handle, frame_durations[i], offset_x,  temp };
	}

	animation_sets_[animation_set_name] = AnimationSet{ image_handle, animation_names_frames };
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

std::map<std::string, Animation>& AnimationManager::GetAnimationMap() {

	return animations_;
}
