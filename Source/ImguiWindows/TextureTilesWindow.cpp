/**********************************************************************************
*\file         TextureTilesWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the TextureTilesWindow class to display loaded textures in the
*			   engine

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ImguiWindows/TextureTilesWindow.h"

void TextureTilesWindow::Init() {

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	entities_ = &*CORE->GetManager<EntityManager>();
	texture_ = &*CORE->GetManager<TextureManager>();
	animation_ = &*CORE->GetManager<AnimationManager>();
	graphics_ = &*CORE->GetSystem<GraphicsSystem>();

	selectedtex_ = {};
}

void TextureTilesWindow::Update() {

	int counter = 0;

	if (imgui_->b_showtex) {
		ImGui::Begin("Texture Tiles", &imgui_->b_showtex, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		static ImGuiTextFilter filter;
		filter.Draw(ICON_FA_FILTER, 250.0f);

		if (ImGui::Selectable(ICON_FA_PLUS ICON_FA_IMAGE))
			imgui_->b_addtexture = true;
		if (ImGui::IsItemHovered())
			imgui_->ImguiHelp("Add New Texture", 0);
		ImGui::EndMenuBar();

		for (auto it = texture_->GetTextureMap().begin(); it != texture_->GetTextureMap().end(); ++it) {
			++counter;
			if (filter.PassFilter(it->first.c_str())) {

				Texture* texture = texture_->GetTexture(it->first.c_str());
				std::vector<glm::vec2>* tex_vtx = texture->GetTexVtx();

				ImTextureID texID = (void*)(intptr_t)texture->GetTilesetHandle();

				ImGuiStyle& style = ImGui::GetStyle();
				float win_vis = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
				ImVec2 buttonsize = ImVec2{ 64, 64 };

				ImGui::PushID(counter);
				if (!tex_vtx->size()) {

					ImGui::PopID();
					continue;
				}	
				else 
					ImGui::ImageButton(texID, buttonsize, ImVec2{ (*tex_vtx)[2].x, (*tex_vtx)[2].y }, ImVec2{ (*tex_vtx)[1].x, (*tex_vtx)[1].y });

				float last_button_size = ImGui::GetItemRectMax().x;
				float next_button_size = last_button_size + style.ItemSpacing.x + buttonsize.x;
				if (counter + 1 <= texture_->GetTextureMap().size() && next_button_size < win_vis)
					ImGui::SameLine();

				if (ImGui::IsItemHovered())
					imgui_->ImguiHelp(it->first.c_str(), 0);

				ImGui::PopID();
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_SourceNoDisableHover)) {

				selectedtex_.assign(it->first.c_str());
				ImGui::SetDragDropPayload("TEXTURE", &selectedtex_, sizeof(std::string));
				ImGui::Text(selectedtex_.c_str());
				ImGui::EndDragDropSource();
			}
		}
		ImGui::End();
	}
}
