#ifndef _COMMON_NODES_H_
#define _COMMON_NODES_H_

#include "Manager/BehaviourTree.h"
#include "Systems/Physics.h"
#include "Engine/Core.h"

class Common : public Behaviour
{
public:
	/******************************************************************************/
	/*!
	  \class CheckAlive

	  \brief Checks whether AI is alive, if not, run respawn
	*/
	/******************************************************************************/
	class CheckAlive :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		Time_Channel respawn_timer_;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckAlive(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		CheckAlive(EntityID id);
		  /******************************************************************************/
		  /*!
			\fn run()

			\brief Checks whether AI is alive, if not, run respawn
		  */
		  /******************************************************************************/
		  bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class CheckSentry

	  \brief CheckSentry Node, Checks whether AI has more than 1 destinations
	*/
	/******************************************************************************/
	class CheckSentry :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckSentry(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		CheckSentry(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether AI has more than 1 destination
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class SentryReturn
	
	  \brief Sentry Return Node, Checks if AI is at sentry position
	*/
	/******************************************************************************/
	class SentryReturn :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		/******************************************************************************/
		/*!
		  \fn SentryReturn(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		SentryReturn(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether AI is at sentry position
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class CheckWaypoint
	
	  \brief CheckWaypoint Node, Contains nodes for the waypoint system
	*/
	/******************************************************************************/
	class CheckWaypoint :public Selector
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckWaypoint(EntityID id)

		  \brief Constructor, setting multiple children relating to the waypoint system
		*/
		/******************************************************************************/
		CheckWaypoint(EntityID id) : id_(id) {
			addChild(new AtWaypoint(id_));
			addChild(new ChangeWaypoint(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class AtWaypoint

	  \brief AtWaypoint Node, Checks if AI is at the waypoint
	*/
	/******************************************************************************/
	class AtWaypoint :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		/******************************************************************************/
		/*!
		  \fn AtWaypoint(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		AtWaypoint(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether AI is at the waypoint
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class ChangeWaypoint

	  \brief ChangeWaypoint Node, Change to the next waypoint
	*/
	/******************************************************************************/
	class ChangeWaypoint :public Node
	{
		EntityID id_;
		AI* ai_;
		ComponentManager* component_mgr;
	public:
		/******************************************************************************/
		/*!
		  \fn ChangeWaypoint(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		ChangeWaypoint(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Change to the next waypoint
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class CheckPath

	  \brief Check Path Node, check if pathfinding algorithm can find a path
	*/
	/******************************************************************************/
	class CheckPath :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		AMap* map_;
		Vector2D SetDes;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckPath(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		CheckPath(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Check if pathfinding algorithm can find a path
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class Move
	
	  \brief Move Node, Move towards destination
	*/
	/******************************************************************************/
	class Move :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		ForcesManager* forces_;
		float Speed_;
	public:
		/******************************************************************************/
		/*!
		  \fn Move(EntityID id, float spd)
	
		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		Move(EntityID id, float spd);
		/******************************************************************************/
		/*!
		  \fn run()
	
		  \brief Move towards destination
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class PlayerWithinDistance

	  \brief PlayerWithinDistance Node, checks whether the player is within
		close proximity
	*/
	/******************************************************************************/
	class PlayerWithinDistance :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		float detectdistance_;
	public:
		/******************************************************************************/
		/*!
		  \fn PlayerWithinDistance(EntityID id, float dist)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		PlayerWithinDistance(EntityID id, float dist);
		/******************************************************************************/
		/*!
		  \fn PlayerInit()

		  \brief Sets nodes private variables relating to the player
		*/
		/******************************************************************************/
		void PlayerInit();
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether the player is within close proximity
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class PlayerWithinVision

	  \brief PlayerWithinVision Node, checks whether the player is within
		cone vision
	*/
	/******************************************************************************/
	class PlayerWithinVision : public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		float detectdistance_ = 0.0f;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckSentry(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		PlayerWithinVision(EntityID id, float dist);
		/******************************************************************************/
		/*!
		  \fn PlayerInit()

		  \brief Sets nodes private variables relating to the player
		*/
		/******************************************************************************/
		void PlayerInit();
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether the player is within cone vision
		*/
		/******************************************************************************/
		bool run() override;
	};


};

#endif
