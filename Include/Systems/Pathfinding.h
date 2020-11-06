#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include <iostream>
#include <vector>
#include <list> 
#include "ISystem.h"

class Pathfinding : public ISystem {

public:
	struct Node
	{
		bool obstacle_ = false;
		bool visited_ = false;
		float globaldes_;
		float localdes_;
		int x, y;

		std::vector<Node*> neighbour_;
		Node* parent_;
	};

private:

	Node* nodes_{ nullptr };
	Node* nstart_{ nullptr };
	Node* nend_{ nullptr };

	int mapwidth_ = 16;
	int mapheight_ = 16;

public:

/******************************************************************************/
/*!
  \fn Astar()

  \brief Uses the Astar algorithm
*/
/******************************************************************************/
	void AStar();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes the node map
*/
/******************************************************************************/
	virtual void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Performs update of AStar 
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn Draw()

  \brief Used to draw the Nodes and path for A star
*/
/******************************************************************************/
	virtual void Draw() override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Pathfinding"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn ~Collision()

  \brief Default implementation destructor
*/
/******************************************************************************/
	virtual ~Pathfinding() = default;

};

#endif
