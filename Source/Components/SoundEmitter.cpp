#include "Components/SoundEmitter.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"

// MAT140 basic dot product (Trivial computation)
// Imagine slicewise
float SoundEmitter::GetMinDist(const Vector2D& pos) {

	float min_dist{999999999.999999999f};

	for (SoundLine& sound_line : sound_lines_) {

		float sq_length = Vector2DSquareDistance(sound_line.start_, sound_line.end_);

		if (sq_length < 0.001f && sq_length > -0.001f)
			return abs( Vector2DDistance(sound_line.start_, pos - sound_line.start_) );
		
		//use parametric line eqn, find t for closest point on the segment p + t * v
		float time = Vector2DDotProduct(pos - sound_line.start_, sound_line.end_ - sound_line.start_) / sq_length;

		if (time > 1.0f)
			time = 1.0f;
		else if (time < 0.0f)
			time = 0.0f;

		//with time, get closest point
		Vector2D closest_point = sound_line.start_ + time * (sound_line.end_ - sound_line.start_);

		//now get distance from point to player pos
		float dist = abs(Vector2DDistance(closest_point, pos));

		if (dist < min_dist)
			min_dist = dist;
	}

	return min_dist;
}

SoundEmitter::SoundEmitter() :
	sound_name_{},
	num_sound_lines_{},
	sound_lines_{}
{	
}


SoundEmitter::~SoundEmitter() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<SoundEmitter>(Component::GetOwner()->GetID());
}


void SoundEmitter::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<SoundEmitter>(Component::GetOwner()->GetID(), this);
}


std::string SoundEmitter::GetSoundName() const {
	
	return sound_name_;
}


void SoundEmitter::SetSoundName(const std::string& name) {
	
	sound_name_ = name;
}

std::vector<SoundLine>& SoundEmitter::GetSoundLines() {

	return sound_lines_;
}

void SoundEmitter::AddSoundLine(const Vector2D& start, const Vector2D& end) {
	
	sound_lines_.push_back( { {start.x, start.y}, {end.x, end.y} } );
	++num_sound_lines_;
}

void SoundEmitter::RemoveBackSoundLine() {
	
	sound_lines_.pop_back();
}

void SoundEmitter::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("SoundEmitter");

	writer->EndObject();
}


void SoundEmitter::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("SoundEmitter");

	writer->Key("sound name");
	writer->String(sound_name_.c_str());

	writer->Key("number of sound lines");
	writer->String(std::to_string(num_sound_lines_).c_str());

	for (auto& line : sound_lines_) {
		
		writer->Key("start");
		writer->String((std::to_string(line.start_.x) + " " + std::to_string(line.start_.y)).c_str());

		writer->Key("end");
		writer->String((std::to_string(line.end_.x) + " " + std::to_string(line.end_.y)).c_str());
	}

	writer->EndObject();
}


void SoundEmitter::DeSerialize(std::stringstream& data) {

	(void)data;
}


void SoundEmitter::DeSerializeClone(std::stringstream& data) {
	
	data >> sound_name_ >> num_sound_lines_;

	for (size_t i = 0; i < num_sound_lines_; ++i) {
	
		SoundLine line{};
		data >> line.start_.x >> line.start_.y;
		data >> line.end_.x >> line.end_.y;

		sound_lines_.push_back(line);
	}
}


std::shared_ptr<Component> SoundEmitter::Clone() {
	
	return std::make_shared<SoundEmitter>();
}