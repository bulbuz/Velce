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

        enum class Context { WORLD_EDITOR, SECTOR_EDITOR };
        enum class Mode { SELECT, MOVE, CREATE, DELETE };

    private:

        SDL_Renderer* renderer;
        const int WIN_WIDTH;
        const int WIN_HEIGHT;

        // dimensions in tiles
        const int WORLD_WIDTH;
        const int WORLD_HEIGHT;
        double TILE_SIZE;
        int blocks_per_tile; // number of blocks one world tile corresponds to

        std::vector<SDL_Rect> sector_rects;

        double zoom;
        double zoom_speed;
        bool mouse_motion;

        // preview of sector creation
        SDL_Rect selection_box;

        SDL_Rect* selected_sector;

        Mode mode;
        Context context;

        Mouse mouse;
        Vec2 scroll;

        Vec2 create_start_pos;

        Vec2 grabbed_delta;

    };
} // namespace Velce
