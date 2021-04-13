/**********************************************************************************
*\file         BehaviourTree.cpp
*\brief        Contains definition of functions and variables used for
*			   the Behaviour Tree
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Manager/BehaviourTree.h"

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

void Behaviour::Inverter::setChild(Node* newchild) {
	child = newchild;
}

bool Behaviour::Inverter::run() {
	return !child->run();
}