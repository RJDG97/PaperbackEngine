/**********************************************************************************
*\file         SoundEmitter.h
*\brief        Contains declaration of functions and variables used for
*			   the SoundEmitter Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _SOUND_EMITTER_H_
#define _SOUND_EMITTER_H_

#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include "MathLib/Vector2D.h"
#include <sstream>

// Defines a single line that emits sound 
struct SoundLine {

	Vector2D start_, end_;
};

class SoundEmitter : public Component {

	friend class SoundSystem;

	std::string sound_name_;
	size_t num_sound_lines_;
	std::vector<SoundLine> sound_lines_;
	SoundLine new_line_;

public:


/******************************************************************************/
/*!
  \fn Emitter()

  \brief Constructor for Emitter
*/
/******************************************************************************/
	SoundEmitter();

/******************************************************************************/
/*!
  \fn ~Emitter()

  \brief Destructor for Emitter
*/
/******************************************************************************/
	~SoundEmitter();

/******************************************************************************/
/*!
  \fn Init()

  \brief
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn GetMinDist()

  \brief Returns the minimum distance to one of the sound lines
*/
/******************************************************************************/
	float GetMinDist(const Vector2D& pos);

/******************************************************************************/
/*!
  \fn GetSoundName()

  \brief Returns the name of the sound file
*/
/******************************************************************************/
	std::string GetSoundName() const;

/******************************************************************************/
/*!
  \fn SetSoundName()

  \brief Returns the name of the sound file
*/
/******************************************************************************/
	void SetSoundName(const std::string& name);

/******************************************************************************/
/*!
  \fn AddSoundLine()

  \brief Adds a sound line
*/
/******************************************************************************/
	void AddSoundLine(const Vector2D& start, const Vector2D& end);

/******************************************************************************/
/*!
  \fn RemoveBackSoundLine()

  \brief Removes the last sound line in the vector
*/
/******************************************************************************/
	void RemoveBackSoundLine();

/******************************************************************************/
/*!
  \fn GetSoundLines()

  \brief Returns reference to vector of sound lines
*/
/******************************************************************************/
	std::vector<SoundLine>& GetSoundLines();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn DeSerializeClone()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
  \fn SetSoundLine(std::vector<SoundLine> updated_vec)

  \brief Set the Vector of SoundLines
*/
/******************************************************************************/	
	void SetSoundLine(std::vector<SoundLine> updated_vec);

/******************************************************************************/
/*!
  \fn GetNewSoundLine()

  \brief Get an Empty Sound Line 
*/
/******************************************************************************/
	SoundLine GetNewSoundLine();

/******************************************************************************/
/*!
  \fn SetNewSoundLine(SoundLine newline)

  \brief Set the Empty Sound Line 
*/
/******************************************************************************/
	void SetNewSoundLine(SoundLine newline);

/******************************************************************************/
/*!
  \fn GetNumberLines()

  \brief Get the current number of SoundLines
*/
/******************************************************************************/
	size_t GetNumberLines();

/******************************************************************************/
/*!
  \fn SetNumberLines(size_t num_of_lines)

  \brief Set the current number of SoundLines
*/
/******************************************************************************/
	void SetNumberLines(size_t num_of_lines);
};

#endif