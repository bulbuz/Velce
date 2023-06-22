#include "Editor.h"
#include "Utils.h"
#include "imgui.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <algorithm>
#include <fstream>

using namespace Velce;

Editor::Editor(SDL_Renderer* renderer, int w, int h, std::string* CWD) : 
    renderer(renderer), VIEWPORT_WIDTH(w), VIEWPORT_HEIGHT(h) {

    we.mode = Mode::MOVE;
    se.mode = Mode::MOVE;
    context = Context::WORLD_EDITOR;

    we.WORLD_WIDTH = 80;
    we.WORLD_HEIGHT = 40;
    TILE_SIZE = 24;

    we.zoom = 1;
    se.zoom = 1;
    zoom_speed = 1 / 10.0;

    cur_sector = nullptr;
    blocks_per_tile = 4;

    background_color = Color(34, 34, 34);
    grid_color = Color(80, 80, 80);
    select_color = Color(0, 188, 212);
    sector_color = Color(76, 175, 80);

    this->CWD = CWD;

    se.tileset_buffer = NULL;
}

void Editor::Run() {
    Input();

    dbg("mouse.rel_pos: ", mouse.rel_pos.x, mouse.rel_pos.y);
    dbg("mouse.abs_pos: ", mouse.abs_pos.x, mouse.abs_pos.y);
    dbg("grid_pos: ", mouse.grid_pos.x, mouse.grid_pos.y);
    dbg("sector delta: ", we.grabbed_delta.x, we.grabbed_delta.y);

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
            // don't select another sector while moving the selected sector
            if (we.mode == Mode::SELECT && we.grabbed_delta == Vec2(-1, -1)) {
                Sector* res = ClickedOnSector();
                if (res != nullptr)
                    cur_sector = res;
            }
        }
    }

    if (context == Context::SECTOR_EDITOR && ImGui::IsWindowFocused()) {

    }

    zoomed = io.MouseWheel;
}

Sector* Editor::ClickedOnSector() {

    SDL_Point p = { mouse.grid_pos.x, mouse.grid_pos.y };
    // check which sector was selected
    for (auto& sector : we.sectors) {
        if (SDL_PointInRect(&p, sector.GetRect()))
            return &sector;
    }

    return nullptr;
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
    if (ImGui::Button("Save map")) {
        SaveMap();
    }
    ImGui::SameLine();
    if (ImGui::Button("Connect gates")) {
        we.mode = Mode::CONNECT;
    }

    std::string tbx_mode = "Move";
    if (we.mode == Mode::CREATE)
        tbx_mode = "Create";
    else if (we.mode == Mode::SELECT)
        tbx_mode = "Select";
ImGui::Text("%s", ("Mode: " + tbx_mode).c_str());
    ImGui::Text("%s", ("Sectors: " + std::to_string(we.sectors.size())).c_str());

    // reset selected sector if mode was switched
    if (we.mode != Mode::SELECT && cur_sector != nullptr)
        cur_sector = nullptr;

    // check if there is a selected sector
    if (cur_sector != nullptr) {
        ImGui::Begin("Properties");

        if (ImGui::Button("Edit sector")) {
            // enter the sector editor and create a sector
            context = Context::SECTOR_EDITOR;
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete")) {

            // remove sector if deleted
            for (size_t i = 0; i < we.sectors.size(); i++) {
                if (&we.sectors[i] == cur_sector) {
                    cur_sector = nullptr;
                    we.sectors.erase(we.sectors.begin() + i);
                    break;
                }
            }
        }
        ImGui::End();
    }

    // update
    // ------
    ImVec2 window_pos = ImGui::GetCursorScreenPos();
    mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
    mouse.grid_pos = mouse.rel_pos - we.scroll;
    mouse.grid_pos.x /= TILE_SIZE * we.zoom; mouse.grid_pos.y /= TILE_SIZE * we.zoom;

    if (InRange(mouse.rel_pos.x, 0, WIN_WIDTH) && InRange(mouse.rel_pos.y, 0, WIN_HEIGHT)) {
        we.zoom += zoomed * zoom_speed;
        we.zoom = std::max(we.zoom, 0.6);
        
    }

    // OnClick behaviors code
    if (ImGui::IsWindowFocused() && mouse.holding_left_click && 
            InRange(mouse.rel_pos.x, 0, WIN_WIDTH) && InRange(mouse.rel_pos.y, 0, WIN_HEIGHT)) {


        switch (we.mode) {
        case Mode::MOVE:
            we.scroll = we.scroll + mouse.delta;
            break;

        case Mode::CREATE:
            if (InRange(mouse.grid_pos.x, 0, we.WORLD_WIDTH) && 
                    InRange(mouse.grid_pos.y, 0, we.WORLD_HEIGHT)) {
                mouse.OnClick();
            }
            break;

        case Mode::SELECT:
            if (cur_sector != nullptr && we.selected_gate == nullptr) {
                SDL_Rect* rect = cur_sector->GetRect();
                if (we.grabbed_delta == Vec2(-1, -1)) {
                    we.grabbed_delta = mouse.grid_pos - Vec2(rect->x, rect->y);
                } else {
                    Vec2 delta = mouse.grid_pos - we.grabbed_delta;
                    if (delta.x >= 0 && delta.x + rect->w <= we.WORLD_WIDTH)
                        rect->x = delta.x;
                    if (delta.y >= 0 && delta.y + rect->h <= we.WORLD_HEIGHT)
                        rect->y = delta.y;
                }
            }
            break;
        case Mode::CONNECT:
            if (we.selected_gate != nullptr)
                break;

            for (auto& sector : we.sectors) {
                if (InRange(mouse.grid_pos.x, sector.GetRect()->x, sector.GetRect()->x + sector.GetRect()->w)
                    && InRange(mouse.grid_pos.y, sector.GetRect()->y, sector.GetRect()->y + sector.GetRect()->h)) {
                    for (auto& gate : *sector.GetGates()) {
                        SDL_FPoint m_pos{(float) mouse.rel_pos.x, (float) mouse.rel_pos.y};
                        SDL_FRect r = GetWorldGateRect(&gate, sector.GetRect());
                        if (SDL_PointInFRect(&m_pos, &r)) {
                            we.selected_gate = &gate;
                            we.gate_sector = &sector;
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && !mouse.holding_left_click) {
        if (we.mode == Mode::CREATE) {
            SDL_Rect box = mouse.GetSelection();
            if (box.w && box.h)
                if (box.x >= 0 && box.x + box.w <= we.WORLD_WIDTH &&
                    box.y >= 0 && box.y + box.h <= we.WORLD_HEIGHT) {
                    
                    // CREATE THE ACTUAL SECTOR
                    we.sectors.push_back(Sector(renderer, Vec2(box.w * blocks_per_tile, box.h * blocks_per_tile)));
                    we.sectors.back().SetRect(box);
                }
            mouse.OnRelease();
        }

        if (we.mode == Mode::CONNECT && we.selected_gate != nullptr) {
            for (auto& sector : we.sectors) {
                if (InRange(mouse.grid_pos.x, sector.GetRect()->x, sector.GetRect()->x + sector.GetRect()->w)
                    && InRange(mouse.grid_pos.y, sector.GetRect()->y, sector.GetRect()->y + sector.GetRect()->h)) {
                    for (auto& gate : *sector.GetGates()) {
                        SDL_FPoint m_pos{(float) mouse.rel_pos.x, (float) mouse.rel_pos.y};
                        SDL_FRect r = GetWorldGateRect(&gate, sector.GetRect());
                        if (SDL_PointInFRect(&m_pos, &r) && &gate != we.selected_gate) {
                            we.selected_gate->AddEndpoint(&gate);
                            we.selected_gate = nullptr;
                            we.gate_sector = nullptr;
                            break;
                        }
                    }
                }
            }
        }


        we.grabbed_delta = Vec2(-1, -1);
    }

    RenderWorldEditor();
}

void Editor::SaveMap() {
    std::ofstream map_file(*CWD + "res/data/world_map.txt");

    /*
     *
     * FORMAT OF world_map.txt
     * ----------------------
     *  int N - number of sectors
     *
     */

    map_file.close();
}

void Editor::RenderWorldEditor() {
    // render
    // ---------
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
    SDL_RenderClear(renderer);

    {   // draw the selection box 
        SDL_SetRenderDrawColor(renderer, sector_color.r, sector_color.g, sector_color.b, 255);

        SDL_Rect box = mouse.GetSelection();
        float x = box.x * TILE_SIZE * we.zoom + we.scroll.x;
        float y = box.y * TILE_SIZE * we.zoom + we.scroll.y;
        float w = box.w * TILE_SIZE * we.zoom;
        float h = box.h * TILE_SIZE * we.zoom;
        SDL_FRect selection_box_render{ x, y, w, h};
        SDL_RenderFillRectF(renderer, &selection_box_render);
    }

    RenderGrid(we.WORLD_WIDTH, we.WORLD_HEIGHT);

    // draw created sectors

    for (auto& sector : we.sectors) {
        SDL_Rect* rect = sector.GetRect();
        SDL_FRect sector_rect{ (float)(rect->x * TILE_SIZE * we.zoom + we.scroll.x), 
            (float)(rect->y * TILE_SIZE * we.zoom + we.scroll.y),
            (float)(rect->w * TILE_SIZE * we.zoom), (float)(rect->h * TILE_SIZE * we.zoom)};

        SDL_SetRenderDrawColor(renderer, sector_color.r, sector_color.g, sector_color.b, 255);
        SDL_RenderFillRectF(renderer, &sector_rect);

        if (rect == cur_sector->GetRect()) {
            SDL_SetRenderDrawColor(renderer, select_color.r, select_color.g, select_color.b, 255);
            SDL_RenderDrawRectF(renderer, &sector_rect);
        }


        // draw gates relative to the sectors
        std::vector<Gate>* gates = sector.GetGates();
        for (auto& gate : *gates) {
            SDL_Rect* gate_rect = gate.GetRect();
            SDL_FRect gate_world_rect = GetWorldGateRect(&gate, rect);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRectF(renderer, &gate_world_rect);
        }
    }

    if (we.selected_gate != nullptr) {
        SDL_FRect r = GetWorldGateRect(we.selected_gate, we.gate_sector->GetRect());
        Vec2 start = Vec2(r.x + r.w / 2, r.y + r.h / 2);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, start.x, start.y, mouse.rel_pos.x, mouse.rel_pos.y);
    }


    for (auto& sector : we.sectors) {
        for (auto& gate : *sector.GetGates()) {
            for (auto& endpoint : gate.GetEndpoints()) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // RENDER CONNECTIONS

                SDL_FRect start_rect = GetWorldGateRect(&gate, gate.GetSectorRect());
                SDL_FRect end_rect = GetWorldGateRect(endpoint, endpoint->GetSectorRect());
                Vec2 start(start_rect.x + start_rect.w / 2, start_rect.y + start_rect.h / 2);
                Vec2 end(end_rect.x + end_rect.w / 2, end_rect.y + end_rect.h / 2);
                SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
            }
        }
    }
}

SDL_FRect Editor::GetWorldGateRect(Gate* gate, SDL_Rect* sector_rect) {
    assert(gate != nullptr);
    assert(sector_rect != nullptr);
    SDL_Rect* gate_rect = gate->GetRect();
    SDL_FRect gate_world_rect;
    gate_world_rect.x = sector_rect->x + (float) gate_rect->x / blocks_per_tile;
    gate_world_rect.y = sector_rect->y + (float) gate_rect->y / blocks_per_tile;
    gate_world_rect.w = (float) gate_rect->w / blocks_per_tile;
    gate_world_rect.h = (float) gate_rect->h / blocks_per_tile;

    gate_world_rect.x = gate_world_rect.x * TILE_SIZE * we.zoom + we.scroll.x;
    gate_world_rect.y = gate_world_rect.y * TILE_SIZE * we.zoom + we.scroll.y;
    gate_world_rect.w *= TILE_SIZE * we.zoom;
    gate_world_rect.h *= TILE_SIZE * we.zoom;

    return gate_world_rect;
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
            float x = j * TILE_SIZE * zoom + scroll.x;
            float y = i * TILE_SIZE * zoom + scroll.y;
            float w = TILE_SIZE * zoom;
            float h = TILE_SIZE * zoom;
            SDL_FRect tile{ x, y, w, h};
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
    ImGui::SameLine();
    if (ImGui::Button("Close"))
        se.show_tile_settings = false;

    ImGui::End();
}

void Editor::SectorEditor() {
    // clear screen
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
    SDL_RenderClear(renderer);

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
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            context = Context::WORLD_EDITOR;
            ResetSectorEditor();
        }
        if (ImGui::Button("Add gate")) {
            se.mode = Mode::ADD_GATE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove gate")) {
            se.mode = Mode::REMOVE_GATE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Add tileset")) {
            se.show_tile_settings = true;
        }
    }

    std::string tbx_mode = "Move";
    if (se.mode == Mode::CREATE)
        tbx_mode = "Create";
    else if (se.mode == Mode::DELETE)
        tbx_mode = "Erase";
    else if (se.mode == Mode::ADD_GATE)
        tbx_mode = "Add gate";
    else if (se.mode == Mode::REMOVE_GATE)
        tbx_mode = "Remove gate";

    ImGui::Text("%s", ("mode: " + tbx_mode).c_str());

    if (se.show_tile_settings) {
        AddTileset();
    }

    ImVec2 window_pos = ImGui::GetCursorScreenPos();
    mouse.rel_pos = Vec2(mouse.abs_pos.x - window_pos.x, mouse.abs_pos.y - window_pos.y);
    mouse.grid_pos = mouse.rel_pos - se.scroll;
    mouse.grid_pos.x /= TILE_SIZE * se.zoom; mouse.grid_pos.y /= TILE_SIZE * se.zoom;
    if (ImGui::IsWindowFocused() && mouse.holding_left_click && 
            InRange(mouse.rel_pos.x, 0, WIN_WIDTH) && InRange(mouse.rel_pos.y, 0, WIN_HEIGHT)) {

        if (se.mode == Mode::MOVE) {
            se.scroll = se.scroll + mouse.delta;
        }

        if (InRangeEx(mouse.grid_pos.x, 0, cur_sector->GetSize().x) && 
                InRangeEx(mouse.grid_pos.y, 0, cur_sector->GetSize().y)) {
            if (se.mode == Mode::CREATE) {
                if (se.cur_tile.GetSpritesheetID() != -1) {
                    cur_sector->SetTile(se.cur_tile, mouse.grid_pos);
                }
            } else if (se.mode == Mode::DELETE) {
                cur_sector->RemoveTile(mouse.grid_pos);
            }
        }

        if (se.mode == Mode::ADD_GATE)
            mouse.OnClick();

        if (se.mode == Mode::REMOVE_GATE) {
            SDL_Point p{mouse.grid_pos.x, mouse.grid_pos.y};
            std::vector<Gate>* gates = cur_sector->GetGates();
            for (auto it = gates->begin(); it != gates->end(); it++) {
                if (SDL_PointInRect(&p, it->GetRect())) {
                    gates->erase(it--);
                }
            }
        }
    }

    if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && !mouse.holding_left_click) {
        if (se.mode == Mode::ADD_GATE) {
            SDL_Rect box = mouse.GetSelection();
            if (box.w && box.h)
                if (box.x >= 0 && box.x + box.w <= cur_sector->GetSize().x &&
                    box.y >= 0 && box.y + box.h <= cur_sector->GetSize().y) {
                    Gate g(box);
                    g.SetSectorRect(cur_sector->GetRect());
                    cur_sector->AddGate(g);
                }

            mouse.OnRelease();
        }     
    }

    // render grid
    RenderGrid(cur_sector->GetSize().x, cur_sector->GetSize().y);

    // render drawn tiles
    cur_sector->RenderGrid(se.scroll, se.zoom, TILE_SIZE);
    
    {   // draw the selection box 
        SDL_Rect box = mouse.GetSelection();
        if (box.w || box.h) {
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            float x = box.x * TILE_SIZE * se.zoom + se.scroll.x;
            float y = box.y * TILE_SIZE * se.zoom + se.scroll.y;
            float w = box.w * TILE_SIZE * se.zoom;
            float h = box.h * TILE_SIZE * se.zoom;
            SDL_FRect selection_box_render{ x, y, w, h};
            SDL_RenderDrawRectF(renderer, &selection_box_render);
        }
    }

    {
        std::vector<Gate>* gates = cur_sector->GetGates();
        for (auto& gate : *gates) {
            SDL_Rect* rect = gate.GetRect();
            SDL_FRect r{ (float)(rect->x * TILE_SIZE * se.zoom + se.scroll.x), (float)(rect->y * TILE_SIZE * se.zoom + se.scroll.y),
                (float)(rect->w * TILE_SIZE * se.zoom), (float)(rect->h * TILE_SIZE * se.zoom)};

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &r);
        }
    }

    // render tileset window if a tileset is available
    if (se.cur_sheet.texture != NULL) {
        TilesetWindow();
    }
}

void Editor::TilesetWindow() {
    ImGui::Begin("Tileset");

    if (ImGui::IsWindowFocused() && mouse.holding_left_click && ImGui::IsWindowHovered()) {
        se.mode = Mode::CREATE;

        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        Vec2 grid_pos((mouse.abs_pos.x - window_pos.x) / se.TILESET_TILE_SIZE, 
                (mouse.abs_pos.y - window_pos.y) / se.TILESET_TILE_SIZE);

        // add tile to sector if drawing
        int spritesheetID = cur_sector->GetSpritesheetID(&se.cur_sheet);
        assert(spritesheetID != -1);
        se.cur_tile = Tile(SDL_Rect{grid_pos.x, grid_pos.y,
                se.cur_sheet.tile_size.x,se. cur_sheet.tile_size.y}, spritesheetID);
    }

    RenderTileset();
    int scaling = se.TILESET_TILE_SIZE / se.cur_sheet.tile_size.x;
    ImGui::Image((void*)se.tileset_buffer, ImVec2(se.cur_sheet.size.x * scaling, se.cur_sheet.size.y * scaling));
    ImGui::End();
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

            // highlight tile if selected
            Vec2 pos = se.cur_tile.GetGridPos();
            int id = se.cur_tile.GetSpritesheetID();
            if (id != -1 && pos.x == j && pos.y == i) {
                SDL_SetRenderDrawColor(renderer, select_color.r, select_color.g, select_color.b, 255);
                SDL_RenderDrawRect(renderer, &dst_rect);
            }
        }
    }
}

void Editor::ResetSectorEditor() {
    se.cur_sheet = Spritesheet();
    se.scroll = Vec2();
    se.show_tile_settings = false;
    SDL_DestroyTexture(se.tileset_buffer);
    se.tileset_buffer = NULL;
    se.mode = Mode::MOVE;
    se.zoom = 1;
    se.cur_tile = Tile();
}

Editor::~Editor() {

}
