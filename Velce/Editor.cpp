#include "Editor.h"

namespace Velce {

    Editor::Editor(SDL_Renderer* renderer, int w, int h, std::string* CWD) : renderer(renderer), VIEWPORT_WIDTH(w), VIEWPORT_HEIGHT(h) {
        we.mode = Mode::MOVE;
        se.mode = Mode::MOVE;
        context = Context::WORLD_EDITOR;

        we.WORLD_WIDTH = 80;
        we.WORLD_HEIGHT = 40;
        TILE_SIZE = 24;

        we.zoom = 1;
        se.zoom = 1;
        zoom_speed = 1 / 10.0;

        we.create_start_pos.x = -1; we.create_start_pos.y = -1;
        we.selected_sector = nullptr;
        we.selection_box = { 0, 0, 0, 0 };
        blocks_per_tile = 4;

        background_color = Color(34, 34, 34);
        grid_color = Color(80, 80, 80);
        select_color = Color(0, 188, 212);
        sector_color = Color(76, 175, 80);

        this->CWD = CWD;

        se.tileset_buffer = NULL; //SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    }

    void Editor::Run() {
        Input();
        ImGui::Begin("debug");
        ImGui::End();

        WIN_WIDTH = std::min((int)ImGui::GetIO().DisplaySize.x, VIEWPORT_WIDTH);
        WIN_HEIGHT = std::min((int)ImGui::GetIO().DisplaySize.y, VIEWPORT_HEIGHT);
        if (context == Context::WORLD_EDITOR) {
            WorldEditor();
        } else if (context == Context::SECTOR_EDITOR) {
            SectorEditor();
        }
    }

    void Editor::Input() {
        ImGuiIO io = ImGui::GetIO();

		mouse.holding_left_click = io.MouseDown[0];
		mouse.delta = Vec2(io.MouseDelta.x, io.MouseDelta.y);
		mouse.abs_pos = Vec2(io.MousePos.x, io.MousePos.y);


        if (context == Context::WORLD_EDITOR && ImGui::IsWindowFocused()) {
			if (io.MouseDown[0] && ImGui::IsWindowHovered()) {
				if (we.mode == Mode::SELECT && context == Context::WORLD_EDITOR) {
					SDL_Point p = { mouse.grid_pos.x, mouse.grid_pos.y };

					for (int i = 0; i < we.sector_rects.size(); i++) {
						if (SDL_PointInRect(&p, &we.sector_rects[i])) {
							we.selected_sector = &we.sector_rects[i];
						}
					}
				}
			}
        }

        if (context == Context::SECTOR_EDITOR && ImGui::IsWindowFocused()) {
        }

        zoomed = io.MouseWheel;
    }

    void Editor::WorldEditor() {
        // imgui stuff
        // --------------

        // mode buttons
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
        ImGui::SameLine();
        ImGui::Button("Save map");

        std::string tbx_mode = "Move";
        if (we.mode == Mode::CREATE)
            tbx_mode = "Create";
        else if (we.mode == Mode::SELECT)
            tbx_mode = "Select";

        ImGui::Text(("mode: " + tbx_mode).c_str());
        ImGui::Text(("Sectors: " + std::to_string(we.sector_rects.size())).c_str());

        // reset selected sector if mode was switched
        if (we.mode != Mode::SELECT && we.selected_sector != nullptr)
            we.selected_sector = nullptr;

        // check if there is a selected sector
        if (we.selected_sector != nullptr) {
			ImGui::Begin("Properties");

            if (ImGui::Button("Edit sector")) {
				// enter the sector editor and create a sector
                context = Context::SECTOR_EDITOR;
                cur_sector = new Sector(Vec2(we.selected_sector->w, we.selected_sector->w));
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
                delete cur_sector;
                cur_sector = nullptr;
            }
            ImGui::End();
        }

        // update
        // ------
        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
        mouse.grid_pos = mouse.rel_pos - we.scroll;
        mouse.grid_pos.x /= TILE_SIZE * we.zoom; mouse.grid_pos.y /= TILE_SIZE * we.zoom;

        if (mouse.rel_pos.x >= 0 && mouse.rel_pos.x <= WIN_WIDTH && mouse.rel_pos.y >= 0 && mouse.rel_pos.y <= WIN_HEIGHT) {
            we.zoom += zoomed * zoom_speed;
            we.zoom = std::max(we.zoom, 0.6);
        }

        ImGui::Begin("debug");
        ImGui::Text(("grid_pos: " + std::to_string(mouse.grid_pos.x) + " " + std::to_string(mouse.grid_pos.y)).c_str());
        ImGui::End();

        // behavior code
        if (ImGui::IsWindowFocused() && mouse.holding_left_click && mouse.rel_pos.x >= 0 && mouse.rel_pos.x <= WIN_WIDTH 
            && mouse.rel_pos.y >= 0 && mouse.rel_pos.y <= WIN_HEIGHT) {
            switch (we.mode) {
            case Mode::MOVE:
                we.scroll = we.scroll + mouse.delta;
                break;

            case Mode::CREATE:
                if (we.create_start_pos == Vec2(-1, -1)) {
                    if (mouse.grid_pos.x == std::clamp(mouse.grid_pos.x, 0, we.WORLD_WIDTH) && 
                        mouse.grid_pos.y == std::clamp(mouse.grid_pos.y, 0, we.WORLD_HEIGHT)) {
                        we.create_start_pos = mouse.grid_pos;
                        we.selection_box = { 0, 0, 0, 0 };
                    }
                } else {
                    // empty selection grid
                    we.selection_box = { we.create_start_pos.x, we.create_start_pos.y,
                        (mouse.grid_pos.x - we.create_start_pos.x) + 1, (mouse.grid_pos.y - we.create_start_pos.y) + 1 };
                }
                break;

            case Mode::SELECT:
                if (we.selected_sector != nullptr) {
                    if (we.grabbed_delta == Vec2(-1, -1)) {
                        we.grabbed_delta = mouse.grid_pos - Vec2(we.selected_sector->x, we.selected_sector->y);
                    } else {
                        Vec2 delta = mouse.grid_pos - we.grabbed_delta;
                        if (delta.x >= 0 && delta.x + we.selected_sector->w <= we.WORLD_WIDTH)
                            we.selected_sector->x = delta.x;
                        if (delta.y >= 0 && delta.y + we.selected_sector->h <= we.WORLD_HEIGHT)
                            we.selected_sector->y = delta.y;
                    }
                }
                break;
            default:
                break;
            }
        }

        if (ImGui::IsWindowFocused() && !mouse.holding_left_click) {
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
                        if (we.selection_box.x >= 0 && we.selection_box.x + we.selection_box.w <= we.WORLD_WIDTH &&
                            we.selection_box.y >= 0 && we.selection_box.y + we.selection_box.h <= we.WORLD_HEIGHT)
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

        RenderGrid(we.WORLD_WIDTH, we.WORLD_HEIGHT);

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

    void Editor::RenderGrid(int WIDTH, int HEIGHT) {
        // render grid
        Vec2 scroll;
        double zoom;
        if (context == Context::WORLD_EDITOR) {
            scroll = we.scroll;
            zoom = we.zoom;
        } else {
            scroll = se.scroll; 
            zoom = se.zoom;
        }

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                SDL_FRect tile{ j * TILE_SIZE * zoom + scroll.x, i * TILE_SIZE * zoom + scroll.y, TILE_SIZE * zoom, TILE_SIZE * zoom };
                SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, 255);
                SDL_RenderDrawRectF(renderer, &tile);
            }
        }
    }

    void Editor::AddTileset() {
		ImGui::Begin("Tileset Settings");
		ImGui::SliderInt("Tile Width", &se.cur_sheet.tile_size.x, 0, 64);
		ImGui::SliderInt("Tile Height", &se.cur_sheet.tile_size.y, 0, 64);
		se.cur_sheet.tile_size.x = 16;
		se.cur_sheet.tile_size.y = 16;

		ImGui::SliderInt("Tile Margin X", &se.cur_sheet.margin.x, 0, 64);
		ImGui::SliderInt("Tile Margin Y", &se.cur_sheet.margin.y, 0, 64);

		ImGui::SliderInt("Tile Padding X", &se.cur_sheet.padding.x, 0, 64);
		ImGui::SliderInt("Tile Padding Y", &se.cur_sheet.padding.y, 0, 64);
		ImGui::SliderInt("Scaling", &se.cur_sheet.scale, 0, 16);

		std::string p = *CWD + "res/assets/art/Tileset_Pink.png";
		char* path = &p[0];
		ImGui::InputText("spritesheet path", path, sizeof(path));

        // MAX 50 CHARACTERS
		ImGui::InputText("Name", se.name, sizeof(se.name));

		if (ImGui::Button("Add")) {
            // load image and store in table of tilesets
			se.cur_sheet.LoadImage(renderer, path);
            int scaling = se.TILESET_TILE_SIZE / se.cur_sheet.tile_size.x;
			se.tileset_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, se.cur_sheet.size.x * scaling, se.cur_sheet.size.y * scaling);
            se.cur_sheet.path = p;
            cur_sector->AddSpritesheet(se.cur_sheet);
			se.show_tile_settings = false;
		}
        ImGui::End();
    }

    void Editor::SectorEditor() {
        // clear screen
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
        SDL_RenderClear(renderer);

        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
        if (mouse.rel_pos.x >= 0 && mouse.rel_pos.x <= WIN_WIDTH && mouse.rel_pos.y >= 0 && mouse.rel_pos.y <= WIN_HEIGHT) {
            se.zoom += zoomed * zoom_speed;
            se.zoom = std::max(se.zoom, 0.6);
        }

		// top bar buttons (different modes)
        { 
            if (ImGui::Button("Play")) {

            }
            if (ImGui::Button("Move")) {
                se.mode = Mode::MOVE;
            }
            ImGui::SameLine();
            if (ImGui::Button("Draw")) {
                se.mode = Mode::CREATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("Erase")) {
                se.mode = Mode::DELETE;
            }
            ImGui::SameLine();
            if (ImGui::Button("Save sector")) {

            }
            ImGui::SameLine();
            if (ImGui::Button("Exit")) {
                context = Context::WORLD_EDITOR;
            }
            if (ImGui::Button("Add tileset")) {
                se.show_tile_settings = true;
            }
        }

        std::string tbx_mode = "Move";
        if (se.mode == Mode::CREATE)
            tbx_mode = "Create";

        ImGui::Text(("mode: " + tbx_mode).c_str());

		if (se.show_tile_settings) {
			AddTileset();
		}

        if (ImGui::IsWindowFocused() && mouse.holding_left_click && mouse.rel_pos.x >= 0 && mouse.rel_pos.x <= WIN_WIDTH 
            && mouse.rel_pos.y >= 0 && mouse.rel_pos.y <= WIN_HEIGHT) {
            switch (se.mode) {
            case Mode::MOVE:
                se.scroll = se.scroll + mouse.delta;
                break;

            case Mode::CREATE:
                break;
            }
        }

        // render grid
        RenderGrid(we.selected_sector->w * blocks_per_tile, we.selected_sector->h * blocks_per_tile);

        // render tileset window
        if (se.cur_sheet.texture != NULL) {
            ImGui::Begin("Tileset");

            if (ImGui::IsWindowFocused() && mouse.holding_left_click) {
                ImVec2 window_pos = ImGui::GetCursorScreenPos();
                Vec2 grid_pos((mouse.abs_pos.x - window_pos.x) / se.TILESET_TILE_SIZE, (mouse.abs_pos.y - window_pos.y) / se.TILESET_TILE_SIZE);

                // add tile to sector if drawing
                int spritesheetID = cur_sector->GetSpritesheetID(&se.cur_sheet);
                assert(spritesheetID != -1);
                se.cur_tile = Tile(SDL_Rect{grid_pos.x, grid_pos.y, se.cur_sheet.tile_size.x,se. cur_sheet.tile_size.y}, spritesheetID);

				// debug window
				ImGui::Begin("debug");
				ImGui::Text(("grid_pos: " + std::to_string(grid_pos.x) + " " + std::to_string(grid_pos.y)).c_str());
				ImGui::End();
            }

            RenderTileset();
            int scaling = se.TILESET_TILE_SIZE / se.cur_sheet.tile_size.x;
            ImGui::Image((void*)se.tileset_buffer, ImVec2(se.cur_sheet.size.x * scaling, se.cur_sheet.size.y * scaling));
            ImGui::End();
        }
    }

    void Editor::RenderTileset() {
        SDL_SetRenderTarget(renderer, se.tileset_buffer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < se.cur_sheet.size.y / se.cur_sheet.tile_size.y; i++) {
			for (int j = 0; j < se.cur_sheet.size.x / se.cur_sheet.tile_size.x; j++) {
                SDL_Rect src_rect{j * se.cur_sheet.tile_size.x, i * se.cur_sheet.tile_size.y, se.cur_sheet.tile_size.x, se.cur_sheet.tile_size.y};
                SDL_Rect dst_rect{j * se.TILESET_TILE_SIZE, i * se.TILESET_TILE_SIZE, se.TILESET_TILE_SIZE, se.TILESET_TILE_SIZE};

                SDL_RenderCopy(renderer, se.cur_sheet.texture, &src_rect, &dst_rect);

                SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, 255);
                SDL_RenderDrawRect(renderer, &dst_rect);
			}
        }
    }

    Editor::~Editor() {
        delete CWD;
        delete cur_sector;
        delete we.selected_sector;
        delete se.tileset_buffer;
    }
} // namespace Velce