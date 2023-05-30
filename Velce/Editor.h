#pragma once

#include <SDL.h>
#include "imgui.h"

#include <algorithm>
#include <string>
#include <vector>

#include "Debug.h"
#include "Utils.h"

namespace Velce
{
    struct Mouse {
        Vec2 grid_pos;
        Vec2 rel_pos;
        Vec2 abs_pos;
        Vec2 delta;
        bool holding_left_click = false;
    };

    class Editor {
    public:
        Editor(SDL_Renderer* renderer, int w, int h);
        void Run();
        void WorldEditor();
        void SectorEditor();
        void Input(SDL_Event* event);

        enum Context { WORLD_EDITOR, SECTOR_EDITOR };
        enum Mode { SELECT, MOVE, CREATE, DELETE };
        enum Tile { EMTPY, SELECTED, CREATED };

    private:
        SDL_Renderer* renderer;
        const int WIN_WIDTH;
        const int WIN_HEIGHT;

        // dimensions in tiles
        const int WORLD_WIDTH;
        const int WORLD_HEIGHT;
        double TILE_SIZE;

        std::vector<std::vector<Tile>> grid;
        std::vector<std::vector<Tile>> selection_grid;
        std::vector<SDL_Rect> sector_rects;

        double zoom;
        double zoom_speed;
        bool mouse_motion;

        // preview of sector creation
        SDL_FRect selection_box;

        SDL_Rect* selected_sector;

        Mode mode;
        Context context;

        Mouse mouse;
        Vec2 scroll;

        Vec2 create_start_pos;

        int cnt = 0;
    };
} // namespace Velce
