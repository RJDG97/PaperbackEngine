/**********************************************************************************
*\file         Stag_Tree.h
*\brief        Contains declaration of functions and variables used for
*			   the Stag Beetle AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Script/Common_Nodes.h"

/******************************************************************************/
/*!
  \class Stag_Tree

  \brief Class containing behaviour nodes for StagBeetle AI
*/
/******************************************************************************/
class Stag_Tree: public Behaviour
{
public:
	/******************************************************************************/
	/*!
	  \class StagRoot

	  \brief StagRoot Node, Starting node that points to one child
	*/
	/******************************************************************************/
	class StagRoot :public Root
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn StagRoot(EntityID id)

		  \brief Constructor, setting child
		*/
		/******************************************************************************/
		StagRoot(EntityID id) :id_(id) {
			Alert::level = 0;
			setChild(new StagSequence(id));
		}
		/******************************************************************************/
		/*!
		  \fn CollisionResponse(EntityID obj)

		  \brief Stag response upon ai collision
		*/
		/******************************************************************************/
		void CollisionResponse(EntityID obj) override;
	};

	/******************************************************************************/
	/*!
	  \class StagSequence

	  \brief Stag Sequence Node, Contains nodes for respawn and overall actions 
	*/
	/******************************************************************************/
	class StagSequence : public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn StagSequence(EntityID id)

		  \brief Constructor, setting multiple children
		*/
		/******************************************************************************/
		StagSequence(EntityID id) : id_(id) {
			addChild(new Common::CheckAlive(id));
			addChild(new ActionSelector(id));
		}
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
			addChild(new ConfusedSelector(id_));
			addChild(new IdleSequence(id_));
		}
	};

	class ConfusedSelector : public Sequence
	{
		EntityID id_;
	public:
		ConfusedSelector(EntityID id) : id_(id) {
			addChild(new ConfusedAnim(id_));
			addChild(new SearchCheck(id_));
		}
	};

	class SearchCheck : public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;
		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
		bool pass;
	public:
		SearchCheck(EntityID id);

		bool run() override;
	};

	class IdleSequence : public Sequence
	{
		EntityID id_;
		ComponentManager* component_mgr;
		AI* ai_;
	public:
		IdleSequence(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			if (ai_->GetNumDes())
				addChild(new PatrolSequence(id_));
			else
				addChild(new SentrySelector(id_));
		}
	};

	class SentrySelector : public Selector
	{
	public:
		SentrySelector(EntityID id)
		{
			addChild(new SentrySequence(id));
			addChild(new SentryReturn(id));
		}
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
		ComponentManager* component_mgr;
		AI* ai_;
	public:
		/******************************************************************************/
		/*!
		  \fn DetectPlayer(EntityID id) : id_(id)

		  \brief Constructor, sets children nodes relating to player detection
		*/
		/******************************************************************************/
		DetectPlayer(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			if (ai_->GetLevel()) {
				addChild(new Common::PlayerWithinDistance(id_, 3.0f));
				addChild(new Common::PlayerWithinVision(id_, 5.0f));
			}
			else {
				addChild(new Common::PlayerWithinDistance(id_, 2.0f));
				addChild(new Common::PlayerWithinVision(id_, 4.0f));
			}
		}
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
			addChild(new AttackSequence(id_));
			addChild(new ConfusedAnim(id_));
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
		Time_Channel Detect_timer_;
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

		  \brief Constructor, sets nodes relating to the chase/attack sequence
		*/
		/******************************************************************************/
		ChaseSequence(EntityID id) : id_(id) {
			addChild(new NotAtkRange(id_));
			addChild(new Common::ChasePath(id_));
			addChild(new Common::Move(id_, 400.0f));
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
			setChild(new Common::PlayerWithinDistance(id_, 1.0f));
		}
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
	  \class AttackSequence

	  \brief AttackSequence Node, contains nodes relating to attack/charge
	*/
	/******************************************************************************/
	class AttackSequence :public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn AttackSequence(EntityID id)

		  \brief Constructor, sets nodes relating to attack/charge
		*/
		/******************************************************************************/
		AttackSequence(EntityID id) :id_(id) {
			addChild(new Charge(id_));
			addChild(new AttackAnim(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class Charge

	  \brief Charge Node, Moves AI towards the player
	*/
	/******************************************************************************/
	class Charge :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		ForcesManager* forces_;
		float Speed_ = 1000.0f;
	public:
		/******************************************************************************/
		/*!
		  \fn Charge(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		Charge(EntityID id);

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

		  \brief Moves AI towards the player
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

	/******************************************************************************/
	/*!
	  \class ConfusedAnim

	  \brief ConfusedAnim Node, plays confused animation
	*/
	/******************************************************************************/
	class ConfusedAnim :public Node
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
		  \fn ConfusedAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		ConfusedAnim(EntityID id);
		/******************************************************************************/
		/*!
		  \fn run()

		  \brief Plays confused animation
		*/
		/******************************************************************************/
		bool run() override;
	};

	class SentrySequence : public Sequence
	{
	public:
		SentrySequence(EntityID id)
		{
			addChild(new Common::AtWaypoint(id));
			addChild(new IdleAnim(id));
		}
	};

	class SentryReturn : public Sequence
	{
	public:
		SentryReturn(EntityID id)
		{
			addChild(new Common::SentryReturn(id));
			addChild(new Common::CheckPath(id));
			addChild(new Common::Move(id, 300.0f));
			addChild(new WalkAnim(id));
		}
	};

	/******************************************************************************/
	/*!
	  \class IdleAnim

	  \brief IdleAnim Node, Runs idle animation (walk)
	*/
	/******************************************************************************/
	class IdleAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai;
		bool running;
	public:
		/******************************************************************************/
		/*!
		  \fn IdleAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		IdleAnim(EntityID id);
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
			addChild(new Common::CheckPath(id_));
			addChild(new Common::Move(id_, 300.0f));
			addChild(new WalkAnim(id_));
		}
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
			addChild(new Common::AtWaypoint(id_));
			addChild(new CheckIdle(id_));
		}
	};

	class CheckIdle : public Sequence
	{
		EntityID id_;
	public:
		CheckIdle(EntityID id) : id_(id) {
			addChild(new IdleAnim(id_));
			addChild(new Common::ChangeWaypoint(id_));
		}
	};
};

#endif