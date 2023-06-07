#include "Editor.h"

namespace Velce {

    Editor::Editor(SDL_Renderer* renderer, int w, int h) : renderer(renderer), VIEWPORT_WIDTH(w), VIEWPORT_HEIGHT(h),
        WORLD_WIDTH(80), WORLD_HEIGHT(40), TILE_SIZE(24) {
        we.mode = Mode::MOVE;
        we.context = Context::WORLD_EDITOR;
        we.zoom = 1;
        we.zoom_speed = 1 / 10.0;

        we.create_start_pos.x = -1; we.create_start_pos.y = -1;
        we.selected_sector = nullptr;
        we.selection_box = { 0, 0, 0, 0 };
        blocks_per_tile = 4;

        background_color = Color(34, 34, 34);
        grid_color = Color(80, 80, 80);
        select_color = Color(0, 188, 212);
        sector_color = Color(76, 175, 80);
    }

    void Editor::Run() {
        Input();

        WIN_WIDTH = std::min((int)ImGui::GetIO().DisplaySize.x, VIEWPORT_WIDTH);
        WIN_HEIGHT = std::min((int)ImGui::GetIO().DisplaySize.y, VIEWPORT_HEIGHT);

        if (we.context == Context::WORLD_EDITOR) {
            WorldEditor();
        }
        else if (we.context == Context::SECTOR_EDITOR) {
            SectorEditor();
        }
    }

    void Editor::Input() {
        ImGuiIO io = ImGui::GetIO();

        if (we.context == Context::WORLD_EDITOR) {
			we.mouse.delta = Vec2(io.MouseDelta.x, io.MouseDelta.y);
			we.mouse.abs_pos = Vec2(io.MousePos.x, io.MousePos.y);

			if (io.MouseDown[0]) {
				if (we.mode == Mode::SELECT && we.context == Context::WORLD_EDITOR) {
					SDL_Point p = { we.mouse.grid_pos.x, we.mouse.grid_pos.y };

					for (int i = 0; i < we.sector_rects.size(); i++) {
						if (SDL_PointInRect(&p, &we.sector_rects[i])) {
							we.selected_sector = &we.sector_rects[i];
						}
					}
				}
			}

			we.mouse.holding_left_click = io.MouseDown[0];

			we.zoom += io.MouseWheel * we.zoom_speed;
			we.zoom = std::max(we.zoom, 0.6);
        }
    }

    void Editor::WorldEditor() {
        // imgui stuff
        // --------------

        // mode buttons
        ImGui::Begin("Properties");
        if (ImGui::Button("Move")) {
            we.mode = Mode::MOVE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Create room")) {
            we.mode = Mode::CREATE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Select")) {
            we.mode = Mode::SELECT;
        }

        std::string tbx_mode = "Move";
        if (we.mode == Mode::CREATE)
            tbx_mode = "Creation";
        else if (we.mode == Mode::SELECT)
            tbx_mode = "Selection";
        ImGui::Text(("Mode: " + tbx_mode).c_str());
        ImGui::Text(("Sector count: " + std::to_string(we.sector_rects.size())).c_str());
        ImGui::End();

        // reset selected sector if mode was switched
        if (we.mode != Mode::SELECT && we.selected_sector != nullptr)
            we.selected_sector = nullptr;

        // check if there is a selected sector
        if (we.selected_sector != nullptr) {
			ImGui::Begin("Properties");
            if (ImGui::Button("Edit sector")) {
                we.context = Context::SECTOR_EDITOR;
            }

            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                for (int i = 0; i < we.sector_rects.size(); i++) {
                    if (we.selected_sector == &we.sector_rects[i]) {
                        we.sector_rects.erase(we.sector_rects.begin() + i);
						we.selected_sector = nullptr;
                        break;
                    }
                }
            }
            ImGui::End();
        }

        // update
        // ------
        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        we.mouse.rel_pos = Vec2(we.mouse.abs_pos.x - window_pos.x, we.mouse.abs_pos.y - window_pos.y);
        we.mouse.grid_pos = we.mouse.rel_pos - we.scroll;
        we.mouse.grid_pos.x /= TILE_SIZE * we.zoom; we.mouse.grid_pos.y /= TILE_SIZE * we.zoom;

        ImGui::Begin("debug");
        ImGui::Text(("grid_pos: " + std::to_string(we.mouse.grid_pos.x) + " " + std::to_string(we.mouse.grid_pos.y)).c_str());
        ImGui::End();

        // behavior code
        if (ImGui::IsWindowFocused() && we.mouse.holding_left_click && we.mouse.rel_pos.x > 0 && we.mouse.rel_pos.x <= WIN_WIDTH 
            && we.mouse.rel_pos.y >= 0 && we.mouse.rel_pos.y <= WIN_HEIGHT) {
            switch (we.mode) {
            case Mode::MOVE:
                we.scroll = we.scroll + we.mouse.delta;
                break;

            case Mode::CREATE:
                if (we.create_start_pos == Vec2(-1, -1)) {
                    if (we.mouse.grid_pos.x == std::clamp(we.mouse.grid_pos.x, 0, WORLD_WIDTH) && 
                        we.mouse.grid_pos.y == std::clamp(we.mouse.grid_pos.y, 0, WORLD_HEIGHT)) {
                        we.create_start_pos = we.mouse.grid_pos;
                        we.selection_box = { 0, 0, 0, 0 };
                    }
                } else {
                    // empty selection grid
                    we.selection_box = { we.create_start_pos.x, we.create_start_pos.y,
                        (we.mouse.grid_pos.x - we.create_start_pos.x) + 1, (we.mouse.grid_pos.y - we.create_start_pos.y) + 1 };
                }
                break;

            case Mode::SELECT:
                if (we.selected_sector != nullptr) {
                    if (we.grabbed_delta == Vec2(-1, -1)) {
                        we.grabbed_delta = we.mouse.grid_pos - Vec2(we.selected_sector->x, we.selected_sector->y);
                    } else {
                        Vec2 delta = we.mouse.grid_pos - we.grabbed_delta;
                        if (delta.x >= 0 && delta.x + we.selected_sector->w <= WORLD_WIDTH)
                            we.selected_sector->x = delta.x;
                        if (delta.y >= 0 && delta.y + we.selected_sector->h <= WORLD_HEIGHT)
                            we.selected_sector->y = delta.y;
                    }
                }
                break;
            default:
                break;
            }
        }

        if (ImGui::IsWindowFocused() && !we.mouse.holding_left_click) {
            if (we.mode == Mode::CREATE) {
                // create the actual sector
                if (we.create_start_pos != Vec2(-1, -1)) {
                    // correct negative dimensions
                    if (we.selection_box.w < 0) {
                        we.selection_box.x += we.selection_box.w;
                        we.selection_box.w = abs(we.selection_box.w);
                    }

                    if (we.selection_box.h < 0) {
                        we.selection_box.y += we.selection_box.h;
                        we.selection_box.h = abs(we.selection_box.h);
                    }

                    if (we.selection_box.w && we.selection_box.h)
                        if (we.selection_box.x >= 0 && we.selection_box.x + we.selection_box.w <= WORLD_WIDTH &&
                            we.selection_box.y >= 0 && we.selection_box.y + we.selection_box.h <= WORLD_HEIGHT)
                            we.sector_rects.push_back(we.selection_box);
                }
                // reset selection grid
                we.create_start_pos = Vec2(-1, -1);
                we.selection_box = { 0, 0, 0, 0 };
            }
            we.grabbed_delta = Vec2(-1, -1);
        }

        RenderWorldEditor();
    }

    void Editor::RenderWorldEditor() {
        // render
        // ---------
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
        SDL_RenderClear(renderer);

        {   // draw the selection box 
            SDL_SetRenderDrawColor(renderer, sector_color.r, sector_color.g, sector_color.b, 255);
            SDL_FRect selection_box_render{ we.selection_box.x * TILE_SIZE * we.zoom + we.scroll.x, we.selection_box.y * TILE_SIZE * we.zoom + we.scroll.y,
                we.selection_box.w * TILE_SIZE * we.zoom, we.selection_box.h * TILE_SIZE * we.zoom };
            SDL_RenderFillRectF(renderer, &selection_box_render);
        }

        // render grid
        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                SDL_FRect tile{ j * TILE_SIZE * we.zoom + we.scroll.x, i * TILE_SIZE * we.zoom + we.scroll.y, TILE_SIZE * we.zoom, TILE_SIZE * we.zoom };
                SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, 255);
                SDL_RenderDrawRectF(renderer, &tile);
            }
        }
        // draw created sectors
        for (const auto& rect : we.sector_rects) {
            SDL_FRect sector_rect{ rect.x * TILE_SIZE * we.zoom + we.scroll.x, rect.y * TILE_SIZE * we.zoom + we.scroll.y,
                rect.w * TILE_SIZE * we.zoom, rect.h * TILE_SIZE * we.zoom };

            SDL_SetRenderDrawColor(renderer, sector_color.r, sector_color.g, sector_color.b, 255);
            SDL_RenderFillRectF(renderer, &sector_rect);

            if (&rect == we.selected_sector) {
                SDL_SetRenderDrawColor(renderer, select_color.r, select_color.g, select_color.b, 255);
                SDL_RenderDrawRectF(renderer, &sector_rect);
            }
        }
    }

    void Editor::SectorEditor() {
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
        SDL_RenderClear(renderer);
    
        ImGui::Begin("Sector editor");
        char spritesheet_path[50] = "/../res/assets/art/";
        ImGui::InputText("test", spritesheet_path, 100);
        ImGui::End();
    }

} // namespace Velce
