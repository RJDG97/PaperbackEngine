#ifndef _STAGBEHAVIOUR_H_
#define _STAGBEHAVIOUR_H_

#include <iostream>

#include "Manager/BehaviourTree.h"

class StagBehaviour : public MonoBehaviour
{

public:

	virtual bool Run() override {
		return true;
	}

	//void Init()
	//{
	//	
	//}

	//class MoveLeft : Node {
	//	virtual bool Run() override {
	//		std::cout << "MoveLeft" << std::endl;
	//		return true;
	//	}
	//};

	//class MoveRight : Node {
	//	virtual bool Run() override {
	//		std::cout << "MoveRight" << std::endl;
	//		return true;
	//	}
	//};

};

#endif