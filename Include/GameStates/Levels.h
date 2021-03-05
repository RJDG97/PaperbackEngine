/**********************************************************************************
*\file         Levels.h
*\brief        Contains declaration of functions and variables used for
*			   the Level and Levels Struct
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _LEVELS_H_
#define _LEVELS_H_

#include "Components/IComponent.h"
#include "prettywriter.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

struct Level {
	std::string name_;
	std::string path_;
	std::string optional_next_; // used to set next "play"
	using EntityPaths = std::unordered_map<std::string, std::string>;
	using EntityPathsIt = EntityPaths::iterator;
	EntityPaths entity_paths_;

	/******************************************************************************/
	/*!
	  \fn Level()

	  \brief Constructor for a level
	*/
	/******************************************************************************/
	Level();

	/******************************************************************************/
	/*!
	  \fn Level()

	  \brief Constructor for a level
	*/
	/******************************************************************************/
	Level(const std::string name, const std::string path_name, const std::string next = "");

	/******************************************************************************/
	/*!
	  \fn DeSerialize()

	  \brief Read the data of a level
	*/
	/******************************************************************************/
	void DeSerialize();

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Saves existing game data into a json at "filepath"
	*/
	/******************************************************************************/
	void Serialize(const std::string filepath);

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Saves existing game data into current level
	*/
	/******************************************************************************/
	void Serialize();

	/******************************************************************************/
	/*!
	  \fn AddNewEntityPath()

	  \brief Adds a new entity path to save to
	*/
	/******************************************************************************/
	void AddNewEntityPath(const std::string& name, const std::string& path);

	/******************************************************************************/
	/*!
	  \fn DeleteEntityPath()

	  \brief Deletes an existing entity path
	*/
	/******************************************************************************/
	void DeleteEntityPath(const std::string& name);
};

struct Levels {

	std::string path_;
	Level* current_state_;
	Level splash_, menu_, credits_, editor_, win_, lose_, pause_;
	using Plays = std::vector<Level>;
	using PlaysIt = Plays::iterator;
	Plays plays_;
	size_t current_play_index_;


	/******************************************************************************/
	/*!
	  \fn Levels()

	  \brief Constructor for levels
	*/
	/******************************************************************************/
	Levels();

	/******************************************************************************/
	/*!
	  \fn DeSerialize()

	  \brief Reads the data of a single level in a game
	*/
	/******************************************************************************/
	void DeSerialize(const std::string filepath);

	/******************************************************************************/
	/*!
	  \fn DeSerializeLevels()

	  \brief Read the data of a game (All levels)
	*/
	/******************************************************************************/
	void DeSerializeLevels();

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Saves existing path data into a json at "filepath"
	*/
	/******************************************************************************/
	void Serialize(std::string filename);

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Saves existing game data
	*/
	/******************************************************************************/
	void Serialize();

	/******************************************************************************/
	/*!
	  \fn SerializeLevels()

	  \brief Save all levels in the game
	*/
	/******************************************************************************/
	void SerializeLevels();

	/******************************************************************************/
	/*!
	  \fn GetPlayLevel()

	  \brief Get a pointer to a play level, accepts index
	*/
	/******************************************************************************/
	Level* GetPlayLevel(size_t index = 0);

	/******************************************************************************/
	/*!
	  \fn GetPlayLevel()

	  \brief Get a pointer to a play level, accepts level name
	*/
	/******************************************************************************/
	Level* GetPlayLevel(const std::string name);

	/******************************************************************************/
	/*!
	  \fn GetLastPlayLevel()

	  \brief Get a pointer to the last played play level
	*/
	/******************************************************************************/
	Level* GetLastPlayLevel();

	/******************************************************************************/
	/*!
	  \fn GetNextPlayableLevel()

	  \brief Get a pointer to the next playable level
	*/
	/******************************************************************************/
	Level* GetNextPlayableLevel();

	/******************************************************************************/
	/*!
	  \fn ResetPlayLevels()

	  \brief Resets progression to initial state for playable levels
	*/
	/******************************************************************************/
	void ResetPlayLevels();

	/******************************************************************************/
	/*!
	  \fn AddNewPlayLevelEntry()

	  \brief Add a new play level
	*/
	/******************************************************************************/
	void AddNewPlayLevelEntry();

	/******************************************************************************/
	/*!
	  \fn DeletePlayLevelEntry()

	  \brief Delete an existing play level
	*/
	/******************************************************************************/
	void DeletePlayLevelEntry();
}; 

#endif