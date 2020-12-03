/**********************************************************************************
*\file         IManager.h
*\brief        Contains interface class for Managers
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _IMANAGER_H_
#define _IMANAGER_H_

class IManager {

public:

	virtual void Init() = 0;

	virtual void Update(float frametime) { (void)frametime; }

	virtual ~IManager() = default;
};
#endif