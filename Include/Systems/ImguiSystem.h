#ifndef _IMGUI_SYSTEM_H_
#define _IMGUI_SYSTEM_H_

#include "Systems/ISystem.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

class ImguiSystem : public ISystem
{
	bool b_showdemo = true;
	bool b_showanother = false;

	ImVec4 clearcolor_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float f = 0.0f;
	int counter = 0;

public:

	bool b_imguimode = false;
	ImguiSystem() {};
	void Init() override;
	void Update(float frametime) override;
	void Draw() override;

	std::string GetName() override;
	void SendMessageD(Message* m) override;

	~ImguiSystem();
};


#endif
