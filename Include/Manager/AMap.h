/**********************************************************************************
*\file         AMap.h
*\brief        Contains declaration of functions and variables used for
*			   the AMap System
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _A_MAP_H_
#define _A_MAP_H_

#include <map>
#include <vector>
#include <list>
#include "MathLib/Vector2D.h"
#include "MathLib/MathHelper.h"
#include "Entity/Entity.h"
#include "Manager/IManager.h"

class AMap : public IManager
{
	struct node
	{
		Vector2D nodepos_;
		float F, G, H;

		bool start_;
		bool des_;
		bool obstacle_;
		bool visited_;
		bool path_;
		node* parent_;
		std::vector<node*> neighbour_;
	};

public:

	using AMapTypeX = std::vector<node>;
	using AMapTypeY = std::vector<AMapTypeX>;
	using AMapTypeItX = AMapTypeX::iterator;
	using AMapTypeItY = AMapTypeY::iterator;
	
	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief AMap Init
	*/
	/******************************************************************************/
	void Init() override;

	/******************************************************************************/
	/*!
	  \fn InitAMap(std::map<EntityID, Entity*> entity_map)

	  \brief Init AMap 
	*/
	/******************************************************************************/
	void InitAMap(std::map<EntityID, Entity*> entity_map);

	/******************************************************************************/
	/*!
	  \fn UpdateAMap(std::pair<EntityID, Entity*> entity)

	  \brief Update AMap on runtime
	*/
	/******************************************************************************/
	void UpdateAMap(std::pair<EntityID, Entity*> entity);

	/******************************************************************************/
	/*!
	  \fn GetNodeMap()

	  \brief Return Node Map
	*/
	/******************************************************************************/
	AMapTypeY GetNodeMap();

	/******************************************************************************/
	/*!
	  \fn DrawMap()

	  \brief Draw AMap in the console
	*/
	/******************************************************************************/
	void DrawMap();
	
	/******************************************************************************/
	/*!
	  \fn InitializeNodes()

	  \brief Initialize nodes
	*/
	/******************************************************************************/
	void InitializeNodes();

	/******************************************************************************/
	/*!
	  \fn inlist()

	  \brief Return if in list
	*/
	/******************************************************************************/
	bool inlist(std::list<node> L, node N);

	/******************************************************************************/
	/*!
	  \fn Pathing(std::vector<Vector2D>& path, Vector2D start, Vector2D des)

	  \brief Finds viable paths from start to destination
	*/
	/******************************************************************************/
	void Pathing(std::vector<Vector2D>& path, Vector2D start, Vector2D des);

	/******************************************************************************/
	/*!
	  \fn GetTopRight()

	  \brief Return top right
	*/
	/******************************************************************************/
	Vector2D GetTopRight();

	/******************************************************************************/
	/*!
	  \fn GetBottomLeft()

	  \brief Return bottom left
	*/
	/******************************************************************************/
	Vector2D GetBottomLeft();

private:
	
	Vector2D top_right_;
	Vector2D bottom_left_;
	AMapTypeY node_map_;

	/******************************************************************************/
	/*!
	  \fn AMapInitialization()

	  \brief Initialize AMap
	*/
	/******************************************************************************/
	void AMapInitialization(std::map<EntityID, Entity*> entity_map);

	/******************************************************************************/
	/*!
	  \fn SetAMapSize()

	  \brief Resize AMap depending on furthest entities in game
	*/
	/******************************************************************************/
	void SetAMapSize(Vector2D& size);

	/******************************************************************************/
	/*!
	  \fn SetNodeNeighbours()

	  \brief Set neighbours of a node
	*/
	/******************************************************************************/
	void SetNodeNeighbours();

	/******************************************************************************/
	/*!
	  \fn InsertEntityNodes()

	  \brief Insert an entity in a "node"
	*/
	/******************************************************************************/
	void InsertEntityNodes(const Vector2D& pos, const Vector2D& scale);

	/******************************************************************************/
	/*!
	  \fn ClearMap()

	  \brief Clear the AMap
	*/
	/******************************************************************************/
	void ClearMap();
};

#endif