#ifndef _IMGUI_SYSTEM_H_
#define _IMGUI_SYSTEM_H_

#include <Windows.h>
#include <map>
#include <string>
#include <memory>

#include "Entity/Entity.h"
#include "Engine/Core.h"

#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/Debug.h"
#include "Systems/Collision.h"

#include "ImguiWindows/IWindow.h"
#include "Systems/WindowsSystem.h"


class ImguiSystem : public ISystem
{
public:
	
	//bool b_lock_entity;


	ImguiSystem() {};

/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui System
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates all ImGui Windows in the system
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
	\fn Draw()

	\brief 
*/
/******************************************************************************/
	void Draw() override;

/******************************************************************************/
/*!
  \fn AddWindow<T>()

  \brief Used to add a ImGui Window to the ImGui System by passing in the template
		 parameter
*/
/******************************************************************************/

	template <typename WindowType>
	void AddWindow() {

		DEBUG_ASSERT((imgui_window_arr_.find(typeid(WindowType).name()) == imgui_window_arr_.end()), "Window already exists");
		// Log system message to "Source/Debug.txt"
		std::stringstream str;
		str << "Adding Window: " << typeid(WindowType).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());

		imgui_window_arr_[typeid(WindowType).name()] = std::make_shared<WindowType>();
	}

/******************************************************************************/
/*!
  \fn GetWindow<T>()

  \brief Used to retrieve a ImGui Window that was attached to the ImGui System by
		 passing in the template parameter
*/
/******************************************************************************/

	template <typename WindowType>
	std::shared_ptr<WindowType> GetWindow() {

		DEBUG_ASSERT((imgui_window_arr_.find(typeid(WindowType).name()) != imgui_window_arr_.end()), "Manager does not exist");

		WindowIt return_val = imgui_window_arr_.find(typeid(WindowType).name());
		return std::dynamic_pointer_cast<WindowType>(return_val->second);
	}

/******************************************************************************/
/*!
	\fn GetName()

	\brief Returns the name of the system for debug use
*/
/******************************************************************************/
	virtual std::string GetName();

/******************************************************************************/
/*!
	\fn SendMessageD(Message* m)

	\brief Function more akin to "What to do when message is received"
		   for internal logic
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m);

/******************************************************************************/
/*!
	\fn DockSpaceFlagSet()

	\brief Toggles the flags needed for the ImGui Dock Space 
*/
/******************************************************************************/
	void DockSpaceFlagSet();

/******************************************************************************/
/*!
	\fn ImGuiRender()

	\brief Handles the rendering of the ImGui Windows
*/
/******************************************************************************/
	void ImguiRender();

/******************************************************************************/
/*!
	\fn ~ImguiSystem()

	\brief Handles the cleanup of Dear ImGui
*/
/******************************************************************************/	
	~ImguiSystem();

/******************************************************************************/
/*!
	\fn GetSelectedEntity()

	\brief Retrieves the data selected_entity_
*/
/******************************************************************************/	
	std::pair<Entity*, std::vector<ComponentTypes>> GetSelectedEntity();

/******************************************************************************/
/*!
	\fn ResetSelectedEntity()

	\brief sets the Entity* of the selected entity to nullptr
*/
/******************************************************************************/
	void ResetSelectedEntity();

	bool GetDebugBool();

	void SetDebugBool(bool debug);

	bool GetLockEntity();

	void SetLockEntity(bool debug);


private:

	// map to store all imgui windows added to the system
	using WindowIt = std::map<std::string, std::shared_ptr<IWindow>>::iterator;
	std::map<std::string, std::shared_ptr<IWindow>> imgui_window_arr_;

	// to access the window pointer in the window system
	WindowsSystem* win;

	// bools for the docking space
	bool b_dock_space_open;
	bool b_fullscreen_persistant;
	bool b_fullscreen;

	bool b_imguimode;
	bool b_debug;
	bool b_lock_entity;

	// imGui flags for the docking space
	ImGuiDockNodeFlags dock_space_flags_;
	ImGuiWindowFlags window_flags_;

	std::pair<Entity*, std::vector<ComponentTypes>> selected_entity_;
	Collision* collision_system_;
};


#endif
