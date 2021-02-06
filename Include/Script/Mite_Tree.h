/**********************************************************************************
*\file         Mite_Tree.h
*\brief        Contains declaration of functions and variables used for
*			   the Mite AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _MITE_TREE_H_
#define _MITE_TREE_H_

#include "Manager/BehaviourTree.h"
#include "Systems/Physics.h"
#include "Engine/Core.h"

/******************************************************************************/
/*!
  \class Mite_Tree

  \brief Class containing behaviour nodes for Mite AI
*/
/******************************************************************************/
class Mite_Tree : public Behaviour
{
public:
	/******************************************************************************/
	/*!
	  \class MiteRoot

	  \brief MiteRoot Node, Starting node that points to one child
	*/
	/******************************************************************************/
	class MiteRoot : public Root
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn MiteRoot(EntityID id)

		  \brief Constructor, setting child
		*/
		/******************************************************************************/
		MiteRoot(EntityID id) : id_(id) {
			setChild(new MiteSequence(id));
		}
		/******************************************************************************/
		/*!
		  \fn CollisionResponse(EntityID obj)

		  \brief Mite response upon ai collision
		*/
		/******************************************************************************/
		void CollisionResponse(EntityID obj) override;
	};

	/******************************************************************************/
	/*!
	  \class MiteSequence

	  \brief Mite Sequence Node, Contains nodes for respawn and overall actions
	*/
	/******************************************************************************/
	class MiteSequence : public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn MiteSequence(EntityID id)

		  \brief Constructor, setting multiple children
		*/
		/******************************************************************************/
		MiteSequence(EntityID id) : id_(id) {
			addChild(new CheckAlive(id));
			addChild(new ActionSelector(id));
		}
	};

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
	  \class ActionSelector

	  \brief ActionSelector Node, Contains nodes for overall AI actions
	*/
	/******************************************************************************/
	class ActionSelector : public Selector
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn ActionSelector(EntityID id)

		  \brief Constructor, setting multiple children
		*/
		/******************************************************************************/
		ActionSelector(EntityID id) : id_(id) {
			addChild(new DetectSequence(id_));
			addChild(new PatrolSequence(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class PatrolSequence

	  \brief Patrol Sequence Node, Contains nodes for Patrol and overall actions
	*/
	/******************************************************************************/
	class PatrolSequence : public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn PatrolSequence(EntityID id) : id_(id)

		  \brief Constructor, setting multiple children related to Patrol actions
		*/
		/******************************************************************************/
		PatrolSequence(EntityID id) : id_(id) {
			addChild(new CheckWaypoint(id_));
			addChild(new CheckPath(id_));
			addChild(new Move(id_, 300.0f));
			addChild(new WalkAnim(id_));
		}
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
	  \class WalkAnim

	  \brief Sentry Anim Node, Runs Sentry animation (walk)
	*/
	/******************************************************************************/
	class WalkAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai;
	public:
		/******************************************************************************/
		/*!
		  \fn WalkAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		WalkAnim(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Runs animation for walking
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
	  \class DetectSequence

	  \brief Detect Sequence Node, containing nodes related to detection
	*/
	/******************************************************************************/
	class DetectSequence :public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn DetectSequence(EntityID id) : id_(id)

		  \brief Constructor, sets children nodes relating to detection
		*/
		/******************************************************************************/
		DetectSequence(EntityID id) : id_(id) {
			addChild(new DetectPlayer(id_));
			addChild(new AttackSelector(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class DetectPlayer

	  \brief Detect Player Node, containing nodes related to detection
	*/
	/******************************************************************************/
	class DetectPlayer : public Selector
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn DetectPlayer(EntityID id) : id_(id)

		  \brief Constructor, sets children nodes relating to player detection
		*/
		/******************************************************************************/
		DetectPlayer(EntityID id) : id_(id) {
			addChild(new PlayerWithinDistance(id_, 2.0f));
			addChild(new PlayerWithinVision(id_));
		}
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

		float detectdistance_ = 4.0f;
	public:
		/******************************************************************************/
		/*!
		  \fn CheckSentry(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		PlayerWithinVision(EntityID id);
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

	/******************************************************************************/
	/*!
	  \class AttackSelector

	  \brief AttackSelector Node, contains nodes relating to attack
	*/
	/******************************************************************************/
	class AttackSelector :public Selector
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn AttackSelector(EntityID id)

		  \brief Constructor, sets nodes relating to attack
		*/
		/******************************************************************************/
		AttackSelector(EntityID id) : id_(id) {
			addChild(new DetectAnim(id_));
			addChild(new ChaseSequence(id_));
			addChild(new AttackAnim(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class DetectAnim

	  \brief DetectAnim Node, plays detected animation
	*/
	/******************************************************************************/
	class DetectAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		/******************************************************************************/
		/*!
		  \fn DetectAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		DetectAnim(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Plays detected animation
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class ChaseSequence

	  \brief ChaseSequence Node, contains nodes relating to the run sequence
	*/
	/******************************************************************************/
	class ChaseSequence :public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn ChaseSequence(EntityID id)

		  \brief Constructor, sets nodes relating to the run sequence
		*/
		/******************************************************************************/
		ChaseSequence(EntityID id) : id_(id) {
			addChild(new NotAtkRange(id_));
			addChild(new ChasePath(id_));
			addChild(new Move(id_, 400.0f));
			addChild(new ChaseAnim(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class NotAtkRange

	  \brief NotAtkRange Node, inverts node relating to player detection
	*/
	/******************************************************************************/
	class NotAtkRange :public Inverter
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn NotAtkRange(EntityID id) :id_(id)

		  \brief Sets child node relating to AI proximity detection
		*/
		/******************************************************************************/
		NotAtkRange(EntityID id) :id_(id) {
			setChild(new PlayerWithinDistance(id_, 0.5f));
		}
	};

	/******************************************************************************/
	/*!
	  \class ChasePath

	  \brief ChasePath Node, Checks whether path to player is viable and sets it
	*/
	/******************************************************************************/
	class ChasePath :public Node
	{
		EntityID id_;
		AMap* map_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Transform* player_rigidbody_;
		Vector2D SetDes;
	public:
		/******************************************************************************/
		/*!
		  \fn ChasePath(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		ChasePath(EntityID id);
		/******************************************************************************/
		/*!
		  \fn PlayerInit()

		  \brief Sets nodes private variables relating to player
		*/
		/******************************************************************************/
		void PlayerInit();
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Checks whether path to player is viable and sets it
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class ChaseAnim

	  \brief ChaseAnim Node, plays run animation
	*/
	/******************************************************************************/
	class ChaseAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		/******************************************************************************/
		/*!
		  \fn ChaseAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		ChaseAnim(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Plays run animation
		*/
		/******************************************************************************/
		bool run() override;
	};

	/******************************************************************************/
	/*!
	  \class AttackAnim

	  \brief AttackAnim Node, plays chase/attack animation
	*/
	/******************************************************************************/
	class AttackAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		/******************************************************************************/
		/*!
		  \fn AttackAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		AttackAnim(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Plays attack animation
		*/
		/******************************************************************************/
		bool run() override;
	};
};

#endif
