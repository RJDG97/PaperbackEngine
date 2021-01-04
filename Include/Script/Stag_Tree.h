#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Manager/BehaviourTree.h"

class Stag_Tree : public Behaviour
{
	bool isalive_;

public:

	Stag_Tree() {
		isalive_ = true;
	}

	class CheckAlive : public Behaviour::Node
	{
	public:
		bool run() override
		{
			//if (!isalive_)
			//{
			//	return false;
			//}
			return true;
		}
	};

	bool run() const {
		return run();
	}

	class SayHi :public Behaviour::Node
	{
	public:
		bool run() override
		{
			std::cout << "Hi" << std::endl;
			return false;
		}
	};
};

#endif