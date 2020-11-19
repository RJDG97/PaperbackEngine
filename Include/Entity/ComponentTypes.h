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
	MAX
};

#endif