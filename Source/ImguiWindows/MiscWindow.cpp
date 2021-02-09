#include "ImguiWindows/MiscWindow.h"

void MiscWindow::Init() {

    imgui_ = &*CORE->GetSystem<ImguiSystem>();
    layer_ = &*CORE->GetManager<LayerManager>();
    graphics_ = &*CORE->GetSystem<GraphicsSystem>();
    input_ = &*CORE->GetSystem<InputSystem>();

	b_draw = false;
	b_grid = false;
	b_light = false;

    mousePos_ = { 0,0 };
}

void MiscWindow::Update() {

	GraphicsLayerWindow();
	EditorSettingsWindow();
}

void MiscWindow::EditorSettingsWindow() {

	if (imgui_->b_settings) {

		ImGui::Begin("Editor Settings", &imgui_->b_settings);

		DragEntityCheckBox();

		ImGui::SameLine(0, 7);

		if (ImGui::Checkbox("Bounding Boxes", &b_draw)) {

			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
		}

		ImGui::Checkbox("Toggle Lighting", &b_light);
		graphics_->EnableLighting(b_light);

		ImGui::SameLine(0, 3);

		if (imgui_->GetCamera()) {

			ImGui::Checkbox("Draw Grid", &b_grid);

			if (!imgui_->EditorMode()) {

				if (input_->IsMousePressed(0))
					mousePos_ = input_->GetUpdatedCoords();

				ImGui::Text("Current Cursor Position: %.2f, %.2f", mousePos_.x, mousePos_.y);
			}
		}

		if (b_grid)
			imgui_->DrawGrid();
		
		ImGui::End();
	}
}

void MiscWindow::GraphicsLayerWindow() {
	static bool swap = false;
	std::vector<RenderLayer> layers;

	if (imgui_->b_layers) {

		ImGui::Begin("Layer Hierarchy", &imgui_->b_layers, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {

			ImGui::PushFont(imgui_->img_font_);

			if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE)) {
		
				// add new layer
			}
		
			ImGui::MenuItem(ICON_FA_TRASH);
				// deleting layers TBC

			ImGui::Checkbox(ICON_FA_EXCHANGE_ALT, &swap);

			ImGui::PopFont();

		ImGui::EndMenuBar();

		}

		if (!layer_->GetRenderLayers()->empty())

			for (auto it = layer_->GetRenderLayers()->begin(); it != layer_->GetRenderLayers()->end(); ++it )
				layers.push_back(it->second);

		ImGui::Text("Display Order: Layer Number, Layer Name");

		auto renderLayers = layer_->GetRenderLayers();

		for (auto it = renderLayers->begin(); it != renderLayers->end(); ++it ) {

			ImGui::Text("%s, Layer: %d", it->second.GetName().c_str(), it->first);

			ImGui::SameLine(0, 5);

			bool layervisibility = it->second.GetVisible();

			if (ImGui::SmallButton(imgui_->VisibleIcon(layervisibility).c_str()))
				layervisibility = !layervisibility;

			it->second.SetVisible(layervisibility);
			
		}

		for (int i = 0; i < layers.size(); ++i) {
			
			ImGui::Text("Layer %d: %s", i, layers[i].GetName());
		}
		ImGui::End();
	}
}

void MiscWindow::DragEntityCheckBox() {

	bool lock = imgui_->GetLockBool();
	ImGui::Checkbox("Drag Entity", &lock);
	ImGui::SameLine(); imgui_->ImguiHelp("Check this box\nto Drag Entities around");
	imgui_->SetLockBool(lock);

}