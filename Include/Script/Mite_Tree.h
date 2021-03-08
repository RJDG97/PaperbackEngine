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
#include "Script/Common_Nodes.h"
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
			addChild(new IdleSequence(id_));
			addChild(new PatrolSequence(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class IdleSequence

	  \brief Idle Sequence Node, Contains nodes for Idle actions
	*/
	/******************************************************************************/
	class IdleSequence : public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn IdleSequence(EntityID id)

		  \brief Constructor, setting multiple children related to Idle actions
		*/
		/******************************************************************************/
		IdleSequence(EntityID id) : id_(id) {
			addChild(new Common::CheckSentry(id_));
			addChild(new SentrySequence(id_));
			addChild(new SentryAnim(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class SentrySequence

	  \brief Sentry Sequence Node, Contains nodes for Sentry and overall actions
	*/
	/******************************************************************************/
	class SentrySequence : public Sequence
	{
		EntityID id_;
	public:
		/******************************************************************************/
		/*!
		  \fn SentrySequence(EntityID id)

		  \brief Constructor, setting multiple children related to Idle actions
			and returning to destination
		*/
		/******************************************************************************/
		SentrySequence(EntityID id) : id_(id) {
			addChild(new Common::SentryReturn(id_));
			addChild(new Common::CheckPath(id_));
			addChild(new Common::Move(id_, 300.0f));
			addChild(new WalkAnim(id_));
		}
	};

	/******************************************************************************/
	/*!
	  \class SentryAnim

	  \brief Sentry Anim Node, Runs Sentry animation (walk)
	*/
	/******************************************************************************/
	class SentryAnim :public Node
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
		  \fn SentryAnim(EntityID id)

		  \brief Constructor, sets nodes private variables
		*/
		/******************************************************************************/
		SentryAnim(EntityID id);
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
			addChild(new Common::CheckWaypoint(id_));
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
		AI* ai_;
		ComponentManager* component_mgr;
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
			std::cout << "Mite Level: " << ai_->GetLevel() << std::endl;
			switch (ai_->GetLevel()) {
			case 1:
				addChild(new Common::PlayerWithinDistance(id_, 2.0f));
				addChild(new Common::PlayerWithinVision(id_, 4.0f));
				break;
			case 2:
				addChild(new Common::PlayerWithinDistance(id_, 3.0f));
				addChild(new Common::PlayerWithinVision(id_, 6.0f));
				break;
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
			setChild(new Common::PlayerWithinDistance(id_, 0.5f));
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
