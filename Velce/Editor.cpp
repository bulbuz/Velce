#include "Editor.h"

namespace Velce
{
    Editor::Editor(SDL_Renderer* renderer, int w, int h) : renderer(renderer), WIN_WIDTH(w), WIN_HEIGHT(h),
        WORLD_WIDTH(80), WORLD_HEIGHT(40), TILE_SIZE(24), zoom(1), zoom_speed(1 / 10.0), grid(WORLD_HEIGHT, std::vector<Tile>(WORLD_WIDTH, Tile::EMTPY)) {
        mode = MOVE;
        context = WORLD_EDITOR;
        mouse_motion = false;
        create_start_pos.x = -1; create_start_pos.y = -1;
        selection_grid = std::vector<std::vector<Tile>>(WORLD_HEIGHT, std::vector<Tile>(WORLD_WIDTH, Tile::EMTPY));
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

        // update
        // ------

        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
        mouse.grid_pos = mouse.rel_pos - scroll;

        mouse.grid_pos.x /= TILE_SIZE * zoom; mouse.grid_pos.y /= TILE_SIZE * zoom;
        mouse.grid_pos.x = (int)mouse.grid_pos.x; mouse.grid_pos.y = (int)mouse.grid_pos.y;

        // behavior code
        if (ImGui::IsWindowFocused() && mouse.holding_left_click && mouse.grid_pos.x >= 0 && mouse.grid_pos.x <= WORLD_WIDTH && mouse.grid_pos.y >= 0 && mouse.grid_pos.y <= WORLD_HEIGHT) {
            switch (mode) {
            case MOVE:
                scroll = scroll + mouse.delta;
                break;

            case CREATE:
                if (create_start_pos == Vec2(-1, -1)) {
                    create_start_pos = mouse.grid_pos;
                    selection_box = { 0, 0, 0, 0 };
                }
                else {
                    // empty selection grid
                    selection_grid = std::vector<std::vector<Tile>>(WORLD_HEIGHT, std::vector<Tile>(WORLD_WIDTH, Tile::EMTPY));

                    float w = (mouse.grid_pos.x - create_start_pos.x) * TILE_SIZE * zoom;
                    float h = (mouse.grid_pos.y - create_start_pos.y) * TILE_SIZE * zoom;
                    selection_box = { (float)(create_start_pos.x * TILE_SIZE * zoom + scroll.x), (float)(create_start_pos.y * TILE_SIZE * zoom + scroll.y), w, h };
                    // for (int i = selection_box.y; i <= selection_box.y + selection_box.h; i++) {
                    //     for (int j = selection_box.x; j <= selection_box.x + selection_box.w; j++) {
                    //         selection_grid[i][j] = Tile::CREATED;
                    //     }
                    // }
                }
                break;

            case SELECT:
                break;

            default:
                break;
            }
        }

        if (ImGui::IsWindowFocused() && !mouse.holding_left_click) {
            if (mode == Mode::CREATE) {

                // create sector in grid
                if (create_start_pos != Vec2(-1, -1)) {
                    for (int i = selection_box.y; i <= selection_box.y + selection_box.h; i++) {
                        for (int j = selection_box.x; j <= selection_box.x + selection_box.w; j++) {
                            grid[i][j] = Tile::CREATED;
                        }
                    }
                    SDL_Rect r{ selection_box.x, selection_box.y, selection_box.x + selection_box.w, selection_box.y + selection_box.h };
                    sector_rects.push_back(r);
                }
                create_start_pos = Vec2(-1, -1);
            }
        }

        // render
        // ---------
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRectF(renderer, &selection_box);

        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                SDL_FRect tile{ j * TILE_SIZE * zoom + scroll.x, i * TILE_SIZE * zoom + scroll.y, TILE_SIZE * zoom, TILE_SIZE * zoom };

                if (selection_grid[i][j] == Tile::CREATED || grid[i][j] == Tile::CREATED) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRectF(renderer, &tile);
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRectF(renderer, &tile);
            }
        }

        // remove
        int idx = 0;
        for (auto r : sector_rects) {
            std::cout << idx++ << ": " << r.x << " " << r.y << " " << r.w << " " << r.h << "\n";
        }

        // reset mouse motion
        mouse_motion = false;

        // imgui stuff
        // --------------

        if (selected_sector != nullptr) {
            ImGui::Begin("Sector props");
            if (ImGui::Button("Delete")) {
                // std::remove(sector_rects.begin(), sector_rects.end(), *selected_sector);
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

        // debugging
        ImGui::Text(("dt pos: " + std::to_string(mouse.delta.x) + " " + std::to_string(mouse.delta.y)).c_str());
        ImGui::Text(("relpos: " + std::to_string(mouse.rel_pos.x) + " " + std::to_string(mouse.rel_pos.y)).c_str());
        ImGui::Text(("gridpos: " + std::to_string(mouse.grid_pos.x) + " " + std::to_string(mouse.grid_pos.y)).c_str());
        ImGui::Text(("scroll: " + std::to_string(scroll.x) + " " + std::to_string(scroll.y)).c_str());
        ImGui::Text(("zoom: " + std::to_string(zoom)).c_str());
        ImGui::Text(("mode: " + std::to_string(mode)).c_str());
        ImGui::Text(("left mouse btn: " + std::to_string(mouse.holding_left_click)).c_str());
        ImGui::Text(("gridpos: " + std::to_string(create_start_pos.x) + " " + std::to_string(create_start_pos.y)).c_str());
        ImGui::End();
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
                    for (auto rect : sector_rects) {
                        if (SDL_PointInRect(&p, &rect)) {
                            selected_sector = &rect;
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
