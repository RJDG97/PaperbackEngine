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
#include "Systems/WindowsSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/Factory.h"
#include "Systems/Game.h"

#include "ImguiWindows/IWindow.h"
#include "GameStates/GameState.h"
#include "GameStates/MenuState.h"

#include "Manager/EntityManager.h"
#include "Imgui/IconsFontAwesome4.h"

//colours for axis buttons
#define REDDEFAULT ImVec4{ 0.773f, 0.027f, 0.067f, 1.0f }
#define REDHOVERED ImVec4{ 0.965f, 0.075f, 0.118f, 1.0f }
#define REDACTIVE  ImVec4{ 0.773f, 0.027f, 0.067f, 1.0f }
#define GREENDEFAULT ImVec4{ 0.294f, 0.804f, 0.075f, 1.0f }
#define GREENHOVERED ImVec4{ 0.361f, 0.918f, 0.122f, 1.0f }
#define GREENACTIVE  ImVec4{ 0.216f, 0.584f, 0.055f, 1.0f }
#define BLUEDEFAULT ImVec4{ 0.141f, 0.176f, 0.839f, 1.0f }
#define BLUEHOVERED ImVec4{ 0.318f, 0.345f, 0.882f, 1.0f }
#define BLUEACTIVE  ImVec4{ 0.118f, 0.145f, 0.682f, 1.0f }
#define AQUAMARINE ImVec4{ 0.498f, 1.0f, 0.831f, 1.0f }

class ImguiSystem : public ISystem
{
public:
	ImFont* bold_font_, *img_font_;

	bool b_entitywin, b_archetypewin, b_component, b_display, b_editpath;

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
	EntityID GetSelectedEntity();

/******************************************************************************/
/*!
	\fn ResetSelectedEntity()

	\brief Resets the Entity* of the selected entity to nullptr
*/
/******************************************************************************/
	void ResetSelectedEntity();

/******************************************************************************/
/*!
	\fn GetLockBool()

	\brief Returns the Lock Entity Bool of the ImGui System
*/
/******************************************************************************/
	bool GetLockBool();

/******************************************************************************/
/*!
	\fn SetLockBool()

	\brief Sets the Lock Entity Bool of the ImGui System
*/
/******************************************************************************/
	void SetLockBool(bool debug);

/******************************************************************************/
/*!
	\fn ImguiHelp()

	\brief A function to provide infomation of the imgui windows
*/
/******************************************************************************/

	Entity* GetEntity();
	
	void SetEntity(Entity* newentity);

	bool GetImguiBool();

	void SetImguiBool(bool mode);

	void DeletePopUp(const char* windowName, std::string objName, Entity* entity = nullptr, std::shared_ptr<Component> component = nullptr);

	void ImguiHelp(const char* description);

	void ImguiInput();

	std::string OpenSaveDialog(const char* filter, int save);
	
	void SaveArchetype();

	void LoadArchetype();

	void OpenFile();

	void SaveFile();

	void ImguiMenuBar();

	void ImGuiCustomStyle(); // may not be used

	void CustomImGuiButton(ImVec4 ButtonCol, ImVec4 HoveredCol, ImVec4 SelectCol);

	std::string EditString(std::string filepath, const char* startpos = "Resources");

	void PopUpMessage(const char* windowName, const char* message);

private:

	// map to store all imgui windows added to the system
	using WindowIt = std::map<std::string, std::shared_ptr<IWindow>>::iterator;
	std::map<std::string, std::shared_ptr<IWindow>> imgui_window_arr_;

	// get the selected entity ID
	EntityID selected_entity_;

	// to access the window pointer in the window system
	WindowsSystem* win_;
	Collision* collision_system_;
	InputSystem* input_sys_;
	
	Entity* new_entity_; // entity* to store selected entity
	EntityManager* entities_;
	EntityFactory* factory_;

	const char* scene_filter_;
	const char* texture_filter_;

	// bools for the docking space
	bool b_dock_space_open;
	bool b_fullscreen_persistant;
	bool b_fullscreen;

	bool b_imguimode;
	bool b_windows;
	bool b_lock_entity;

	bool b_showpop;

	// imGui flags for the docking space
	ImGuiDockNodeFlags dock_space_flags_;
	ImGuiWindowFlags window_flags_;

};


#endif
