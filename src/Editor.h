#pragma once

#include <SDL.h>
#include "imgui.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "Debug.h"
#include "Sector.h"
#include "Utils.h"

struct Mouse {
    Vec2 grid_pos;
    Vec2 rel_pos;
    Vec2 abs_pos;
    Vec2 delta;
    bool holding_left_click = false;
};

class Action {
public:
    Mouse mouse;
    void Update();

private:

};

class Select : Action {
public:

private:
    Vec2 start_pos;
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
    enum class Mode { SELECT, MOVE, CREATE, DELETE, ADD_GATE, NONE };

    std::string* CWD;

    SDL_Renderer* renderer;
    const int VIEWPORT_WIDTH;
    const int VIEWPORT_HEIGHT;

    double TILE_SIZE;

    int WIN_WIDTH;
    int WIN_HEIGHT;

    double zoom_speed;

    int blocks_per_tile; // number of blocks one world tile corresponds to

    // colors
    Color background_color;
    Color grid_color;
    Color select_color;
    Color sector_color;

    Context context;

    Sector* cur_sector = nullptr;

    Mouse mouse;

    int zoomed = 0;

    struct WorldEditor {
        // dimensions in tiles
        int WORLD_WIDTH;
        int WORLD_HEIGHT;

        Vec2 scroll;
        std::vector<SDL_Rect> sector_rects;
        double zoom;

        // preview of sector creation
        SDL_Rect selection_box;

        SDL_Rect* selected_sector;

        Mode mode;

        Vec2 create_start_pos;
        Vec2 grabbed_delta;

        // testing vars
        bool unselect = false;
    } we;

    struct SectorEditor {
        Spritesheet cur_sheet;
        Vec2 scroll;
        bool show_tile_settings = false;
        SDL_Texture* tileset_buffer;
        int TILESET_TILE_SIZE = 64;
        Mode mode;
        double zoom;
        char name[15] = "";
        Tile cur_tile;
    } se;

private:
    void AddTileset();
    void RenderWorldEditor();
    void RenderTileset();
    void RenderGrid(int WIDTH, int HEIGHT);
    void RenderSectorTiles();
    void TilesetWindow();
    void SaveMap();
};
