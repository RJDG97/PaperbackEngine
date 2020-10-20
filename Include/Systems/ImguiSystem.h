#ifndef _IMGUI_SYSTEM_H_
#define _IMGUI_SYSTEM_H_

#include <map>
#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/Debug.h"

#include "Imgui/IWindow.h"
#include "Systems/WindowsSystem.h"
#include <string>
#include <memory>


class ImguiSystem : public ISystem
{
	using WindowIt = std::map<std::string, std::shared_ptr<IWindow>>::iterator;
	std::map<std::string, std::shared_ptr<IWindow>> imgui_window_arr_;

	WindowsSystem* win;


public:

	bool b_imguimode = false;
	ImguiSystem() {};
	void Init() override;
	void Update(float frametime) override;
	void Draw() override;

	template <typename WindowType>
	void AddWindow() {

		DEBUG_ASSERT((imgui_window_arr_.find(typeid(WindowType).name()) == imgui_window_arr_.end()), "Window already exists");
		// Log system message to "Source/Debug.txt"
		std::stringstream str;
		str << "Adding Window: " << typeid(WindowType).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());

		imgui_window_arr_[typeid(WindowType).name()] = std::make_shared<WindowType>();
	}

	template <typename WindowType>
	std::shared_ptr<WindowType> GetManager() {

		DEBUG_ASSERT((imgui_window_arr_.find(typeid(WindowType).name()) != imgui_window_arr_.end()), "Manager does not exist");

		WindowIt return_val = imgui_window_arr_.find(typeid(WindowType).name());

		/*
		if (debug_) {
			// Log system message to "Source/Debug.txt"
			std::stringstream str;
			str << "Getting Window: " << typeid(WindowType).name() << "\n";
			M_DEBUG->WriteDebugMessage(str.str());
		}
		*/
		return std::dynamic_pointer_cast<WindowType>(return_val->second);
	}

	std::string GetName() override;
	void SendMessageD(Message* m) override;

	~ImguiSystem();
};


#endif
