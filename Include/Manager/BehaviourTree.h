#ifndef _BEHAVIOUR_TREE_H_
#define _BEHAVIOUR_TREE_H_

#include <vector>
#include "Manager/EntityManager.h"

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
	};

	class Selector : public CompositeNode {
	public:
		bool run() override;
	};

	class Sequence : public CompositeNode {
	public:
		bool run() override;
	};

	class DecoratorNode : public Node {
		Node* child;
	public:
		void setChild(Node* newchild);
		bool run() override;
	};

	class Root : public Node
	{
		Node* child;
	public:
		void setChild(Node* newchild);
		bool run() override;
	};
};

#endif