#ifndef _BEHAVIOUR_TREE_H_
#define _BEHAVIOUR_TREE_H_

#include <vector>

class MonoBehaviour
{
public:

	class Node {
	public:
		virtual bool Run() = 0;
	};

	class CompositeNode : public Node {
		
		std::vector<Node*> children_;
	
	public:
		const std::vector<Node*>& GetChildren() const
		{
			return children_;
		}

		void AddChild(Node* child){
			children_.emplace_back(child);
		}

		void AddChildren(std::initializer_list<Node*>&& newchildren)
		{
			for (Node* child : newchildren)
			{
				AddChild(child);
			}
		}

		template <typename T>
		void AddChildren(const T& newchildren) {
			for (Node* child : newchildren)
			{
				AddChild(child);
			}
		}
	};

	// One child fails all fails
	class SelectorNode : public CompositeNode {
	public:

		virtual bool Run() override {
			for (Node* child : GetChildren()) {
				if (child->Run())
					return true;
			}
			return false;
		}
	};

	// One child succeeds all succeed
	class SequenceNode : public CompositeNode {
	public:
		virtual bool Run() override {
			for (Node* child : GetChildren()) {
				if (!child->Run())
					return false;
			}
			return true;
		}
	};

	class DecoratorNode :public Node {
		Node* child_;
	protected:
		Node* GetChild() const{
			return child_;
		}
	public:
		void SetChild(Node* newchild) {
			child_ = newchild;
		}
	};

	class Root : public DecoratorNode {
		friend class MonoBehaviour;
		virtual bool Run() override {
			return GetChild()->Run();
		}

	};

	virtual bool Run() = 0;

};

#endif