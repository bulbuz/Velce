#include "Editor.h"

namespace Velce
{
    Editor::Editor(SDL_Renderer* renderer, int w, int h) : renderer(renderer), WIN_WIDTH(w), WIN_HEIGHT(h),
        WORLD_WIDTH(80), WORLD_HEIGHT(40), TILE_SIZE(24), zoom(1), zoom_speed(1 / 10.0), grid(WORLD_HEIGHT, std::vector<Tile>(WORLD_WIDTH, Tile::EMTPY)) {
        mode = MOVE;
        context = WORLD_EDITOR;
        mouse_motion = false;
        create_start_pos.x = -1; create_start_pos.y = -1;
        selected_sector = nullptr;
        selection_box = { 0, 0, 0, 0 };
    }

    void Editor::Run() {
        // mouse calculations
        if (!mouse_motion) {
            mouse.delta = Vec2(0, 0);
        }

        if (context == WORLD_EDITOR) {
            WorldEditor();
        }
        else if (context == SECTOR_EDITOR) {
            SectorEditor();
        }
    }

    void Editor::WorldEditor() {

        // imgui stuff
        // --------------
        
        if (selected_sector != nullptr) {
            ImGui::Begin("Sector props");
            if (ImGui::Button("Delete")) {
                for (int i = 0; i < sector_rects.size(); i++) {
                    if (selected_sector->x == sector_rects[i].x && selected_sector->y == sector_rects[i].y && selected_sector->w == sector_rects[i].w && selected_sector->h == sector_rects[i].h) {
                        sector_rects.erase(sector_rects.begin() + i);
                        break;
                    }
                }
                selected_sector = nullptr;
            }
            ImGui::End();
        }

        ImGui::Begin("Editor props");
        // mode buttons
        if (ImGui::Button("Move"))
            mode = MOVE;
        ImGui::SameLine();
        if (ImGui::Button("Create room"))
            mode = CREATE;
        ImGui::SameLine();
        if (ImGui::Button("Select"))
            mode = SELECT;

        std::string tbx_mode = "Move";
        if (mode == CREATE)
            tbx_mode = "Creation";
        else if (mode == SELECT)
            tbx_mode = "Selection";
        ImGui::Text(("Mode: " + tbx_mode).c_str());
        ImGui::Text(("Sector count: " + std::to_string(sector_rects.size())).c_str());
        ImGui::End();

        // update
        // ------

        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
        mouse.grid_pos = mouse.rel_pos - scroll;

        mouse.grid_pos.x /= TILE_SIZE * zoom; mouse.grid_pos.y /= TILE_SIZE * zoom;
        mouse.grid_pos.x = (int)mouse.grid_pos.x; mouse.grid_pos.y = (int)mouse.grid_pos.y;

        // behavior code
        if (ImGui::IsWindowFocused() && mouse.holding_left_click && mouse.rel_pos.x > 0 && mouse.rel_pos.x <= WIN_WIDTH && mouse.rel_pos.y >= 0 && mouse.rel_pos.y <= WIN_WIDTH) {
            switch (mode) {
            case MOVE:
                scroll = scroll + mouse.delta;
                break;

            case CREATE:
                if (create_start_pos == Vec2(-1, -1)) {
                    create_start_pos = mouse.grid_pos;
                    selection_box = { 0, 0, 0, 0 };
                } else {
                    // empty selection grid
                    selection_box = { (int) create_start_pos.x, (int) create_start_pos.y, (int) (mouse.grid_pos.x - create_start_pos.x) + 1, (int) (mouse.grid_pos.y - create_start_pos.y) + 1 };
                }
                break;

            default:
                break;
            }
        }

        if (ImGui::IsWindowFocused() && !mouse.holding_left_click) {
            if (mode == Mode::CREATE) {
                // create the actual sector
                if (create_start_pos != Vec2(-1, -1)) {
                    if (selection_box.w < 0) {
                        selection_box.x += selection_box.w;
                        selection_box.w = abs(selection_box.w);
                    }

                    if (selection_box.h < 0) {
                        selection_box.y += selection_box.h;
                        selection_box.h = abs(selection_box.h);
                    }
                    if (selection_box.w && selection_box.h)
                        sector_rects.push_back(selection_box);
                }
                // reset selection grid
                create_start_pos = Vec2(-1, -1);
                selection_box = { 0, 0, 0, 0 };
            }
        }

        // render
        // ---------
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        {   // draw the selection box 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_FRect selection_box_render{ selection_box.x * TILE_SIZE * zoom + scroll.x, selection_box.y * TILE_SIZE * zoom + scroll.y, selection_box.w * TILE_SIZE * zoom, selection_box.h * TILE_SIZE * zoom };
            SDL_RenderFillRectF(renderer, &selection_box_render);
        }

        // draw created sectors
        for (const auto& rect : sector_rects) {
            SDL_FRect sector_rect{ rect.x * TILE_SIZE * zoom + scroll.x, rect.y * TILE_SIZE * zoom + scroll.y, rect.w * TILE_SIZE * zoom, rect.h * TILE_SIZE * zoom };
            SDL_RenderFillRectF(renderer, &sector_rect);
        }

        // render grid
        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                SDL_FRect tile{ j * TILE_SIZE * zoom + scroll.x, i * TILE_SIZE * zoom + scroll.y, TILE_SIZE * zoom, TILE_SIZE * zoom };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRectF(renderer, &tile);
            }
        }

        // reset mouse motion
        mouse_motion = false;
    }

    void Editor::SectorEditor() {

    }

    void Editor::Input(SDL_Event* event) {
        if (event->type == SDL_MOUSEMOTION) {
            mouse_motion = true;
            mouse.delta = Vec2(event->motion.xrel, event->motion.yrel);
            mouse.abs_pos = Vec2(event->motion.x, event->motion.y);
        }

        if (event->type == SDL_MOUSEBUTTONDOWN) {
            if (event->button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
                mouse.holding_left_click = true;

                if (mode == Mode::SELECT && context == Context::WORLD_EDITOR) {
                    SDL_Point p = { mouse.grid_pos.x, mouse.grid_pos.y };

                    for (int i = 0; i < sector_rects.size(); i++) {
                        if (SDL_PointInRect(&p, &sector_rects[i])) {
                            selected_sector = &sector_rects[i];
                        }
                    }
                }
            }
        }
        if (event->type == SDL_MOUSEBUTTONUP) {
            if (event->button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
                mouse.holding_left_click = false;
        }

        if (event->type == SDL_MOUSEWHEEL) {
            zoom += event->wheel.y * zoom_speed;
            zoom = std::max(zoom, 0.6);
        }
    }

} // namespace Velce
