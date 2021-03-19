/**********************************************************************************
*\file         ComponentTypes.h
*\brief        Contains definition of ENUMs for Components
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _COMPONENT_TYPES_H_
#define _COMPONENT_TYPES_H_

// Contains the enums defining a component type
enum class ComponentTypes {

	NONE = 0,
	NAME,
	MOTION,
	TRANSFORM,
	HEALTH,
	CAMERA, //not in yet
	CONTROLLER, // apparently not used
	TEXTURERENDERER,
	ANIMATIONRENDERER,
	TEXTRENDERER,
	AABB,
	AI, // not in yet
	SCALE,
	STATUS,
	POINTLIGHT,
	CONELIGHT, // not in yet
	BASICAI,
	CLICKABLE,
	INPUTCONTROLLER,
	PARENTCHILD,
	LOGICCOMPONENT,
	INVENTORY,
	PARTICLE,
	EMITTER,
	SOUNDEMITTER,
	COLLECTIBLE,
	UNLOCKABLE,
	INTERACTABLE,
	DIALOGUETRIGGER,
	CHILD,
	PUZZLE,
	STOPANIMATION,
	DESTINATION,
	UIPARTICLE,
	MAX
};

#endif