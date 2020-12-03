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
	using EntityPaths = std::unordered_map<std::string, std::string>;
	using EntityPathsIt = EntityPaths::iterator;
	EntityPaths entity_paths_;

	Level();
	Level(const std::string name, const std::string path_name);
	void DeSerialize();
	void Serialize(const std::string filepath);
	void Serialize();
	void AddNewEntityPath(const std::string& name, const std::string& path);
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


	Levels();
	void DeSerialize(const std::string filepath);
	void DeSerializeLevels();
	void Serialize(std::string filename);
	void Serialize();
	void SerializeLevels();
	Level* GetPlayLevel(size_t index = 0);
	Level* GetNextPlayableLevel();
	void AddNewPlayLevelEntry();
	void DeletePlayLevelEntry();
}; 

#endif