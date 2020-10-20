#pragma once
#ifndef _ANOTHER_WINDOW_H_
#define _ANOTHER_WINDOW_H_

#include "ImGui/IWindow.h"


class AnotherWindow : public IWindow{

public:
	void Init() override;
	void Update() override;
};
#endif
