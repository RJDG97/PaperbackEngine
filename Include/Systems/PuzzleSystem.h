/**********************************************************************************
*\file         PuzzleSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Puzzle System
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PUZZLE_SYSTEM_H_
#define _PUZZLE_SYSTEM_H_

#include "Systems/GraphicsSystem.h"
#include "Manager/ComponentManager.h"

class PuzzleSystem : public ISystem
{
public:
	using PuzzleMap = CMap<Puzzle>;
	using PuzzleMapIt = PuzzleMap::MapTypeIt;

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Used to initialise a system if required;
	*/
	/******************************************************************************/
	void Init();

	/******************************************************************************/
	/*!
	  \fn Update()

	  \brief Used to run logic that is required every game loop.
	*/
	/******************************************************************************/
	void Update(float frametime);

	/******************************************************************************/
	/*!
	  \fn UpdatePuzzleEntities()

	  \brief Used update the stats of both the "Puzzle" and "PuzzlePiece"
	*/
	/******************************************************************************/
	void UpdatePuzzleEntities(AABB* aabb1, AABB* aabb2);

	/******************************************************************************/
	/*!
	  \fn GetName()

	  \brief Returns the name of the system
	*/
	/******************************************************************************/
	std::string GetName() { return "PuzzleSystem"; }

	/******************************************************************************/
	/*!
	  \fn SendMessageD()

	  \brief Handles incoming messages and sorts based on message id
	*/
	/******************************************************************************/
	void SendMessageD(Message* m);


private:
	ComponentManager* component_manager_;
	GraphicsSystem* graphics_system_;
	PuzzleMap* puzzle_arr_;
};


#endif