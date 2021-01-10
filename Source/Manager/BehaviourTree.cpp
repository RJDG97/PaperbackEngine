#include "Manager/BehaviourTree.h"
#include <iostream>

const std::vector<Behaviour::Node*>& Behaviour::CompositeNode::getChildren() const {
	return children;
}

void Behaviour::CompositeNode::addChild(Node* child) {
	children.emplace_back(child);
}

Behaviour::CompositeNode::~CompositeNode(){
	for (Node* child : getChildren()) {
		if (child)
			delete child;
	}
}

bool Behaviour::Selector::run(){
	for (Node* child : getChildren()) {
		if (child->run())
			return true;
	}
	return false;
}

bool Behaviour::Sequence::run() {
	for (Node* child : getChildren()) {
		if (!child->run())
			return false;
	}
	return true;
}

void Behaviour::Root::setChild(Node* newchild) {
	child = newchild;
}

bool Behaviour::Root::run() {
	return child->run();
}