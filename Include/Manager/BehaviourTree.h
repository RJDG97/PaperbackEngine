#ifndef _BEHAVIOUR_TREE_H_
#define _BEHAVIOUR_TREE_H_

#include <vector>
#include "Manager/EntityManager.h"
#include "Manager/AMap.h"
#include <iostream>

class Behaviour
{
public:
	class Node {
	public:
		virtual bool run() = 0;
		virtual ~Node() = default;
	};

	class CompositeNode : public Node {
		std::vector<Node*> children;
	public:
		const std::vector<Node*>& getChildren() const;
		void addChild(Node* child);
		~CompositeNode();
	};

	class Selector : public CompositeNode {
	public:
		bool run() override;
	};

	class Sequence : public CompositeNode {
	public:
		bool run() override;
	};

	class Root : public Node
	{
		Node* child;
	public:
		void setChild(Node* newchild);
		bool run() override;
		virtual void CollisionResponse(EntityID obj) = 0;
		~Root() { delete child; }
	};

	class Inverter :public Node
	{
		Node* child;
	public:
		void setChild(Node* newchild);
		bool run() override;
		~Inverter() { delete child; }

	};
};

#endif