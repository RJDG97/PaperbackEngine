/**********************************************************************************
*\file         BehaviourTree.cpp
*\brief        Contains declerations of functions and variables used for
*			   the Behaviour Tree
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _BEHAVIOUR_TREE_H_
#define _BEHAVIOUR_TREE_H_

#include <vector>
#include "Manager/EntityManager.h"
#include "Manager/AMap.h"
#include <iostream>

/******************************************************************************/
/*!
  \class Behaviour

  \brief Class containing multiple behaviour tree node classes
*/
/******************************************************************************/
class Behaviour
{
public:
	/******************************************************************************/
	/*!
	  \class Node
	
	  \brief Interface Node for all behaviour tree nodes
	*/
	/******************************************************************************/
	class Node {
	public:
		/******************************************************************************/
		/*!
		  \fn run()
		
		  \brief Virtual run for derived classes
		*/
		/******************************************************************************/
		virtual bool run() = 0;
		/******************************************************************************/
		/*!
		  \fn ~Node()

		  \brief Virtual destructor for derived classes
		*/
		/******************************************************************************/
		virtual ~Node() = default;
	};

	/******************************************************************************/
	/*!
	  \class CompositeNode
	
	  \brief Composite Node, Node that contains multiple children
	*/
	/******************************************************************************/
	class CompositeNode : public Node {
		std::vector<Node*> children;
	public:
		/******************************************************************************/
		/*!
		  \fn getChildren()

		  \brief Returns Nodes children
		*/
		/******************************************************************************/
		const std::vector<Node*>& getChildren() const;
		/******************************************************************************/
		/*!
		  \fn addChild(Node* child)

		  \brief Adds a child node into the back of the children list
		*/
		/******************************************************************************/
		void addChild(Node* child);
		/******************************************************************************/
		/*!
		  \fn ~CompositeNode()

		  \brief Destructor for Composite nodes
		*/
		/******************************************************************************/
		~CompositeNode();
	};

	/******************************************************************************/
	/*!
	  \class Selector
	
	  \brief Selector Node, Checks children for succesful outcome, if one child is 
		succesful, returns succesful, if not checks next child, returns fail 
		if all children fail
	*/
	/******************************************************************************/
	class Selector : public CompositeNode {
	public:
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Returns successful should one child succeed
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class Sequence

	  \brief Sequence Node, Checks children for succesful outcome, if one child
		fails, returns failure, if not checks next child, returns success
		if all children succeed
	*/
	/******************************************************************************/
	class Sequence : public CompositeNode {
	public:
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Returns failure should one child fail
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class Root

	  \brief Root Node, A starting node that points to one child
	*/
	/******************************************************************************/
	class Root : public Node
	{
		Node* child;
	public:
		/******************************************************************************/
		/*!
		  \fn setChild(Node* newchild)

		  \brief Sets the child node
		*/
		/******************************************************************************/
		void setChild(Node* newchild);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Runs child node
		*/
		/******************************************************************************/
		bool run() override;
		/******************************************************************************/
		/*!
		  \fn CollisionResponse(EntityID obj)

		  \brief Virtual response upon ai collision
		*/
		/******************************************************************************/
		virtual void CollisionResponse(EntityID obj) = 0;
		/******************************************************************************/
		/*!
		  \fn ~Root()

		  \brief Deletes child
		*/
		/******************************************************************************/
		~Root() { delete child; }
	};

	/******************************************************************************/
	/*!
	  \class Inverter

	  \brief Inverter Node, Inverts output of child
	*/
	/******************************************************************************/
	class Inverter :public Node
	{
		Node* child;
	public:
		/******************************************************************************/
		/*!
		  \fn setChild(Node* newchild)

		  \brief Sets the child node
		*/
		/******************************************************************************/
		void setChild(Node* newchild);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Runs child node with inverted result
		*/
		/******************************************************************************/
		bool run() override;
		/******************************************************************************/
		/*!
		  \fn ~Inverter()

		  \brief Deletes child
		*/
		/******************************************************************************/
		~Inverter() { delete child; }

	};
};

#endif