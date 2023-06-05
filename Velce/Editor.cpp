#include "Editor.h"

namespace Velce
{
    Editor::Editor(SDL_Renderer* renderer, int w, int h) : renderer(renderer), WIN_WIDTH(w), WIN_HEIGHT(h),
        WORLD_WIDTH(80), WORLD_HEIGHT(40), TILE_SIZE(24) {
        world_editor.mode = Mode::MOVE;
        world_editor.context = Context::WORLD_EDITOR;
        world_editor.zoom = 1;
        world_editor.zoom_speed = 1 / 10.0;

        world_editor.create_start_pos.x = -1; world_editor.create_start_pos.y = -1;
        world_editor.selected_sector = nullptr;
        world_editor.selection_box = { 0, 0, 0, 0 };
        blocks_per_tile = 4;
    }
    void Editor::Run() {

        WIN_WIDTH = ImGui::GetIO().DisplaySize.x;
        WIN_HEIGHT = ImGui::GetIO().DisplaySize.y;

        Input();

        if (world_editor.context == Context::WORLD_EDITOR) {
            WorldEditor();
        }
        else if (world_editor.context == Context::SECTOR_EDITOR) {
            SectorEditor();
        }
    }

    void Editor::WorldEditor() {

        // imgui stuff
        // --------------

        ImGui::Begin("Editor prop");
        // mode buttons
        if (ImGui::Button("Move"))
            world_editor.mode = Mode::MOVE;
        ImGui::SameLine();
        if (ImGui::Button("Create room"))
            world_editor.mode = Mode::CREATE;
        ImGui::SameLine();
        if (ImGui::Button("Select"))
            world_editor.mode = Mode::SELECT;

        std::string tbx_mode = "Move";
        if (world_editor.mode == Mode::CREATE)
            tbx_mode = "Creation";
        else if (world_editor.mode == Mode::SELECT)
            tbx_mode = "Selection";
        ImGui::Text(("Mode: " + tbx_mode).c_str());
        ImGui::Text(("Sector count: " + std::to_string(world_editor.sector_rects.size())).c_str());
        // ImGui::SliderInt("Sector tile scale", )
        ImGui::End();

        // reset selected sector if mode was switched
        if (world_editor.mode != Mode::SELECT && world_editor.selected_sector != nullptr)
            world_editor.selected_sector = nullptr;

        // check if there is a selected sector
        if (world_editor.selected_sector != nullptr) {
            ImGui::Begin("Editor prop");
            if (ImGui::Button("Edit sector")) {
                world_editor.context = Context::SECTOR_EDITOR;
            }

            if (ImGui::Button("Delete")) {
                for (int i = 0; i < world_editor.sector_rects.size(); i++) {
                    if (world_editor.selected_sector == &world_editor.sector_rects[i]) {
                        world_editor.sector_rects.erase(world_editor.sector_rects.begin() + i);
						world_editor.selected_sector = nullptr;
                        break;
                    }
                }
            }
            ImGui::End();
        }

        // update
        // ------
        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        world_editor.mouse.rel_pos = Vec2(world_editor.mouse.abs_pos.x - window_pos.x, world_editor.mouse.abs_pos.y - window_pos.y);
        world_editor.mouse.grid_pos = world_editor.mouse.rel_pos - world_editor.scroll;
        world_editor.mouse.grid_pos.x /= TILE_SIZE * world_editor.zoom; world_editor.mouse.grid_pos.y /= TILE_SIZE * world_editor.zoom;


        // behavior code
        if (ImGui::IsWindowFocused() && world_editor.mouse.holding_left_click && world_editor.mouse.rel_pos.x > 0 && world_editor.mouse.rel_pos.x <= WIN_WIDTH 
            && world_editor.mouse.rel_pos.y >= 0 && world_editor.mouse.rel_pos.y <= WIN_HEIGHT) {
            switch (world_editor.mode) {
            case Mode::MOVE:
                world_editor.scroll = world_editor.scroll + world_editor.mouse.delta;
                break;

            case Mode::CREATE:
                if (world_editor.create_start_pos == Vec2(-1, -1)) {
                    if (world_editor.mouse.grid_pos.x == std::clamp(world_editor.mouse.grid_pos.x, 0, WORLD_WIDTH) && 
                        world_editor.mouse.grid_pos.y == std::clamp(world_editor.mouse.grid_pos.y, 0, WORLD_HEIGHT)) {
                        world_editor.create_start_pos = world_editor.mouse.grid_pos;
                        world_editor.selection_box = { 0, 0, 0, 0 };
                    }
                } else {
                    // empty selection grid
                    world_editor.selection_box = { world_editor.create_start_pos.x, world_editor.create_start_pos.y,
                        (world_editor.mouse.grid_pos.x - world_editor.create_start_pos.x) + 1, (world_editor.mouse.grid_pos.y - world_editor.create_start_pos.y) + 1 };
                }
                break;

            case Mode::SELECT:
                if (world_editor.selected_sector != nullptr) {
                    if (world_editor.grabbed_delta == Vec2(-1, -1)) {
                        world_editor.grabbed_delta = world_editor.mouse.grid_pos - Vec2(world_editor.selected_sector->x, world_editor.selected_sector->y);
                    } else {
                        int dx = world_editor.mouse.grid_pos.x - world_editor.grabbed_delta.x;
                        int dy = world_editor.mouse.grid_pos.y - world_editor.grabbed_delta.y;
                        if (dx >= 0 && dx + world_editor.selected_sector->w <= WORLD_WIDTH)
                            world_editor.selected_sector->x = dx;
                        if (dy >= 0 && dy + world_editor.selected_sector->h <= WORLD_HEIGHT)
                            world_editor.selected_sector->y = dy;
                    }
                }
                break;
            default:
                break;
            }
        }

        if (ImGui::IsWindowFocused() && !world_editor.mouse.holding_left_click) {
            if (world_editor.mode == Mode::CREATE) {
                // create the actual sector
                if (world_editor.create_start_pos != Vec2(-1, -1)) {
                    // correct negative dimensions
                    if (world_editor.selection_box.w < 0) {
                        world_editor.selection_box.x += world_editor.selection_box.w;
                        world_editor.selection_box.w = abs(world_editor.selection_box.w);
                    }

                    if (world_editor.selection_box.h < 0) {
                        world_editor.selection_box.y += world_editor.selection_box.h;
                        world_editor.selection_box.h = abs(world_editor.selection_box.h);
                    }

                    if (world_editor.selection_box.w && world_editor.selection_box.h)
                        if (world_editor.selection_box.x >= 0 && world_editor.selection_box.x + world_editor.selection_box.w <= WORLD_WIDTH &&
                            world_editor.selection_box.y >= 0 && world_editor.selection_box.y + world_editor.selection_box.h <= WORLD_HEIGHT)
                            world_editor.sector_rects.push_back(world_editor.selection_box);
                }
                // reset selection grid
                world_editor.create_start_pos = Vec2(-1, -1);
                world_editor.selection_box = { 0, 0, 0, 0 };
            }
            world_editor.grabbed_delta = Vec2(-1, -1);
        }

        RenderWorldEditor();
    }

    void Editor::Input() {
        ImGuiIO io = ImGui::GetIO();

        if (world_editor.context == Context::WORLD_EDITOR) {
			world_editor.mouse.delta = Vec2(io.MouseDelta.x, io.MouseDelta.y);
			world_editor.mouse.abs_pos = Vec2(io.MousePos.x, io.MousePos.y);

			if (io.MouseClicked[0]) {
				if (world_editor.mode == Mode::SELECT && world_editor.context == Context::WORLD_EDITOR) {
					SDL_Point p = { world_editor.mouse.grid_pos.x, world_editor.mouse.grid_pos.y };

					for (int i = 0; i < world_editor.sector_rects.size(); i++) {
						if (SDL_PointInRect(&p, &world_editor.sector_rects[i])) {
							world_editor.selected_sector = &world_editor.sector_rects[i];
						}
					}
				}
			}

			world_editor.mouse.holding_left_click = io.MouseDown[0];

			world_editor.zoom += io.MouseWheel * world_editor.zoom_speed;
			world_editor.zoom = std::max(world_editor.zoom, 0.6);
        }
    }

    void Editor::RenderWorldEditor() {
        // render
        // ---------
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        {   // draw the selection box 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_FRect selection_box_render{ world_editor.selection_box.x * TILE_SIZE * world_editor.zoom + world_editor.scroll.x, world_editor.selection_box.y * TILE_SIZE * world_editor.zoom + world_editor.scroll.y,
                world_editor.selection_box.w * TILE_SIZE * world_editor.zoom, world_editor.selection_box.h * TILE_SIZE * world_editor.zoom };
            SDL_RenderFillRectF(renderer, &selection_box_render);
        }

        // render grid
        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                SDL_FRect tile{ j * TILE_SIZE * world_editor.zoom + world_editor.scroll.x, i * TILE_SIZE * world_editor.zoom + world_editor.scroll.y, TILE_SIZE * world_editor.zoom, TILE_SIZE * world_editor.zoom };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRectF(renderer, &tile);
            }
        }
        // draw created sectors
        for (const auto& rect : world_editor.sector_rects) {
            SDL_FRect sector_rect{ rect.x * TILE_SIZE * world_editor.zoom + world_editor.scroll.x, rect.y * TILE_SIZE * world_editor.zoom + world_editor.scroll.y,
                rect.w * TILE_SIZE * world_editor.zoom, rect.h * TILE_SIZE * world_editor.zoom };

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            if (&rect == world_editor.selected_sector) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            }
            SDL_RenderFillRectF(renderer, &sector_rect);
        }
    }

    void Editor::SectorEditor() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

    }

} // namespace Velce
