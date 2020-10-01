#pragma once
#ifndef _IMANAGER_H_
#define _IMANAGER_H_

class IManager {
public:
	virtual void Init() = 0;

	virtual ~IManager() = default;
};
#endif