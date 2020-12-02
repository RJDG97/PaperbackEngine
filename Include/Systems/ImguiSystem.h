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
#include "Systems/SoundSystem.h"

#include "ImguiWindows/IWindow.h"

#include "Manager/EntityManager.h"
#include "Manager/AMap.h"
#include "Imgui/IconsFontAwesome5.h"

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
#define GOLDENORANGE ImVec4{ 1.0f, 0.843f, 0.0f, 1.0f }
#define SKYBLUE ImVec4{ 0.0f, 0.749f, 1.0f, 1.0f }

class ImguiSystem : public ISystem
{
public:
	ImFont* bold_font_, *img_font_;

	bool b_entitywin, b_archetypewin, b_component, b_display, b_editpath, b_asset, b_editcomp, b_addtexture;
	bool b_showtex;

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
	\fn GetEntity()

	\brief Retrieve the entity*
*/
/******************************************************************************/

	Entity* GetEntity();

/******************************************************************************/
/*!
	\fn  SetEntity(Entity* newentity)

	\brief Set the entity* to the selected entity
*/
/******************************************************************************/

	void SetEntity(Entity* newentity);

/******************************************************************************/
/*!
	\fn  GetImguiBool()

	\brief Retrieve the imguimode bool
*/
/******************************************************************************/
	bool GetImguiBool();

/******************************************************************************/
/*!
	\fn SetImguiBool(bool mode);

	\brief Set the imguimode bool
*/
/******************************************************************************/
	void SetImguiBool(bool mode);

/******************************************************************************/
/*!
	\fn GetSceneFilter()

	\brief Get the Json Filter for the filedialog
*/
/******************************************************************************/
	const char* GetSceneFilter();

/******************************************************************************/
/*!
	\fn GetGenericFilter()

	\brief Get the Generic Filter for the filedialog
*/
/******************************************************************************/
	const char* GetGenericFilter();

/******************************************************************************/
/*!
	\fn GetExistingSceneCamera()

	\brief Get the Curreny Active Camera in the Scene
*/
/******************************************************************************/
	Camera* GetExistingSceneCamera();

/******************************************************************************/
/*!
	\fn DeletePopUp(const char* windowName, std::string objName, 
	Entity* entity = nullptr, std::shared_ptr<Component> component = nullptr)

	\brief Manages the pop up for deletion of entities/archetype/components
*/
/******************************************************************************/
	void DeletePopUp(const char* windowName, std::string objName, Entity* entity = nullptr, 
					std::shared_ptr<Component> component = nullptr);

/******************************************************************************/
/*!
	\fn OpenSaveDialog(const char* filter, int save)

	\brief Runs the win32 API for the file dialog
*/
/******************************************************************************/
	std::string OpenSaveDialog(const char* filter, int save, int multiselect = 0);

/******************************************************************************/
/*!
	\fn SaveArchetype()

	\brief Saves the list of avaliable archetypes
*/
/******************************************************************************/
	void SaveArchetype(std::string path);

/******************************************************************************/
/*!
	\fn LoadArchetype()

	\brief Load a list of avaliable archetype
*/
/******************************************************************************/
	void LoadArchetype();

/******************************************************************************/
/*!
	\fn OpenFile()

	\brief Runs the filedialog to load file into Deserialize
*/
/******************************************************************************/
	void OpenFile();

/******************************************************************************/
/*!
	\fn SaveFile()

	\brief Saves the entities into their corresponding json files
*/
/******************************************************************************/
	void SaveFile(std::string dest_path);

/******************************************************************************/
/*!
	\fn CloseCurrentScene();

	\brief Closes the current scene and runs Editor.json
*/
/******************************************************************************/
	void NewScene();

/******************************************************************************/
/*!
	\fn ImguiMenuBar()

	\brief Manages the options of the menu bar
*/
/******************************************************************************/
	void ImguiMenuBar();

/******************************************************************************/
/*!
	\fn ImGuiCustomStyle()

	\brief Set a custom imgui style
*/
/******************************************************************************/
	void ImGuiCustomStyle();

/******************************************************************************/
/*!
	\fn CustomImGuiButton(ImVec4 ButtonCol, ImVec4 HoveredCol, ImVec4 SelectCol)

	\brief Set the 3 types of colours of imgui buttons
*/
/******************************************************************************/
	void CustomImGuiButton(ImVec4 ButtonCol, ImVec4 HoveredCol, ImVec4 SelectCol);

/******************************************************************************/
/*!
	\fn ImguiHelp(const char* description)

	\brief to generate hints if needed
*/
/******************************************************************************/
	void ImguiHelp(const char* description, int symbol = 1);

/******************************************************************************/
/*!
	\fn EditString(std::string filepath, const char* startpos = "Resources")

	\brief Edits the string for usage of Serialize and Deserialize
*/
/******************************************************************************/
	std::string EditString(std::string filepath, const char* startpos = "Resources");

/******************************************************************************/
/*!
	\fn PopUpMessage(const char* windowName, const char* message)

	\brief Shows a pop message with a simple confirmation from the user
*/
/******************************************************************************/
	void PopUpMessage(const char* windowName, const char* message);

/******************************************************************************/
/*!
	\fn DrawGrid()

	\brief Draws the world grid (wip)
*/
/******************************************************************************/
	void DrawGrid();

/******************************************************************************/
/*!
	\fn GetCamera()

	\brief Get the current active camera in the scene
*/
/******************************************************************************/
	Camera* GetCamera();

/******************************************************************************/
/*!
	\fn EditorMode()

	\brief Checks if the window is in focus or hovered
*/
/******************************************************************************/	
	bool EditorMode();

/******************************************************************************/
/*!
	\fn GetAssetAdd();

	\brief Gets the path of the new Asset
*/
/******************************************************************************/
	std::string GetAssetAdd();

/******************************************************************************/
/*!
	\fn SetAssetAdd(std::string image)

	\brief Sets the path of the new Asset to be added in
*/
/******************************************************************************/
	void SetAssetAdd(std::string image);

/******************************************************************************/
/*!
	\fn LoadJsonPaths(std::string path)

	\brief Load the paths of the archetypes from the json 
*/
/******************************************************************************/
	void LoadJsonPaths(std::string path);

/******************************************************************************/
/*!
	\fn CheckString(std::string path, const char* key)

	\brief Checks if the string contains the key
*/
/******************************************************************************/
	bool CheckString(std::string path, const char* key);

/******************************************************************************/
/*!
	\fn SetPopupPosition()

	\brief Sets the position of the next window/pop up to be the centre
	of the dock
*/
/******************************************************************************/
	void SetPopupPosition();

/******************************************************************************/
/*!
	\fn GetArchetypePath()

	\brief Get the path of the loaded archetype file
*/
/******************************************************************************/
	std::string GetArchetypePath();

/******************************************************************************/
/*!
	\fn SetArchetypePath(std::string new_path)

	\brief Set the path of the archetype file
*/
/******************************************************************************/
	void SetArchetypePath(std::string new_path);

private:

	// map to store all imgui windows added to the system
	using WindowIt = std::map<std::string, std::shared_ptr<IWindow>>::iterator;
	std::map<std::string, std::shared_ptr<IWindow>> imgui_window_arr_;

	// get the selected entity ID
	EntityID selected_entity_id_;

	// to access the window pointer in the window system
	WindowsSystem* win_;
	Collision* collision_;
	InputSystem* input_;
	SoundSystem* sound_;
	GraphicsSystem* graphics_;
	
	Entity* selected_entity_; // entity* to store selected entity
	EntityManager* entities_;
	EntityFactory* factory_;

	CMap<Camera>* cam_arr_;
	Camera* camera_;

	Level* editor_;
	AMap* amap_;

	std::string img_to_add_;

	std::string current_loaded_path_;

	std::string archetype_path_;

	const char* scene_filter_, *generic_filter_;

	// bools for the docking space
	bool b_dock_space_open, b_fullscreen_persistant, b_fullscreen;

	bool b_imgui_mode, b_lock_entity, b_show_pop, b_add_path, b_save_check, b_close_confirm, b_editor;

	// imGui flags for the docking space
	ImGuiDockNodeFlags dock_space_flags_;
	ImGuiWindowFlags window_flags_;
};

#endif