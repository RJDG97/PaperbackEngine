#pragma once
#ifndef _A_MAP_H_
#define _A_MAP_H_

#include <map>
#include <vector>
#include "MathLib/Vector2D.h"
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


	void AMapInitialization(std::map<EntityID, Entity*> entity_map);
	void SetAMapSize(Vector2D& size);
	void SetNodeNeighbours();
	void InsertEntityNodes(const Vector2D& pos);
	void ClearMap();
};

#endif