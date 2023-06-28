#pragma once

#include <SDL.h>
#include "imgui.h"

#include <crossguid/guid.hpp>

#include <algorithm>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Debug.h"
#include "Gate.h"
#include "Sector.h"
#include "Utils.h"
#include "World.h"

using namespace Velce;

struct Mouse {

    // general
    // --------------
    
    Vec2 grid_pos;
    Vec2 rel_pos;
    Vec2 abs_pos;
    Vec2 delta;
    bool holding_left_click = false;


    // selection stuff
    // --------------

    Vec2 start_pos = Vec2(-1, -1);
    SDL_Rect selection_box = { 0, 0, 0, 0 }; // preview of sector creation

    void OnClick() {
        if (start_pos == Vec2(-1, -1)) {
            start_pos = grid_pos;
            selection_box = { 0, 0, 0, 0 };
        } else {
            selection_box.x = start_pos.x;
            selection_box.y = start_pos.y;
            selection_box.w = grid_pos.x - start_pos.x + 1;
            selection_box.h = grid_pos.y - start_pos.y + 1;
        }

    }

    // returns empty rect if nothing is selected
    SDL_Rect GetSelection() {
        if (start_pos != Vec2(-1, -1)) {
            
            // TODO: fix reverse offset

            // correct negative dimensions
            if (selection_box.w < 0) {
                selection_box.x += selection_box.w;
                selection_box.w = abs(selection_box.w);
            }

            if (selection_box.h < 0) {
                selection_box.y += selection_box.h - 1;
                selection_box.h = abs(selection_box.h);
            }
        } else {
            selection_box = {0, 0, 0, 0};
        }
        return selection_box;
    }

    void OnRelease() {
        selection_box = { 0, 0, 0, 0 };
        start_pos = Vec2(-1, -1);
    }
};


class Editor {
public:
    Editor(SDL_Renderer* renderer, int w, int h, std::string* CWD);
    ~Editor();
    void Run();
    void WorldEditor();
    void SectorEditor();
    void Input();

private:
    enum class Context { WORLD_EDITOR, SECTOR_EDITOR, NONE};
    enum class Mode { SELECT, MOVE, CREATE, DELETE, CREATE_GATE, REMOVE_GATE, CONNECT, NONE };

    World* world;

    SDL_Renderer* renderer;
    const int VIEWPORT_WIDTH;
    const int VIEWPORT_HEIGHT;

    double TILE_SIZE;

    int WIN_WIDTH;
    int WIN_HEIGHT;

    double zoom_speed;

    int blocks_per_tile; // number of blocks one world tile corresponds to

    std::string* CWD;

    // colors
    Color background_color;
    Color grid_color;
    Color select_color;
    Color sector_color;

    Context context;

    Sector* cur_sector = nullptr;

    Mouse mouse;

    int zoomed = 0;

    // World Editor struct
    struct We {
        // dimensions in tiles
        int WORLD_WIDTH;
        int WORLD_HEIGHT;

        Vec2 scroll;
        std::list<Sector> sectors;

        double zoom;

        Mode mode;

        Vec2 grabbed_delta;

        // testing vars
        bool unselect = false;
        Gate* selected_gate = nullptr;
        Sector* gate_sector = nullptr;
    };

    // Sector editor struct
    struct Se {
        Spritesheet cur_sheet;
        Vec2 scroll;
        bool show_tile_settings = false;
        SDL_Texture* tileset_buffer;
        int TILESET_TILE_SIZE = 64;
        Mode mode;
        double zoom;
        char name[15] = "";
        Tile cur_tile;
    };

    We we;
    Se se;

private:
    void AddTileset();
    void RenderWorldEditor();
    void RenderTileset();
    void RenderGrid(int WIDTH, int HEIGHT);
    void RenderSectorTiles();
    void TilesetWindow();
    void SaveWorld();
    void ResetSectorEditor();
    Sector* ClickedOnSector();
    SDL_FRect GetWorldGateRect(Gate* gate, SDL_Rect* sector_rect);
};
