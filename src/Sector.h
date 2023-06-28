#pragma once

#include <SDL.h>
#include "SectorSerializer.h"
#include "imgui.h"
#include <crossguid/guid.hpp>

#include <iostream>
#include <list>
#include <vector>

#include "Gate.h"
#include "Tile.h"
#include "Utils.h"

namespace Velce {
    
    class Sector {
    public:
        // size of the sector in tiles
        Sector();
        Sector(SDL_Renderer* renderer, Vec2 size);
        ~Sector();

        void RemoveTile(Vec2 grid_pos);
        void SetTile(Tile tile, Vec2 grid_pos);
        void AddSpritesheet(Spritesheet sheet);
        int GetSpritesheetID(Spritesheet* sheet);
        Vec2 GetSize();

        void AddGate(Gate gate);
        std::list<Gate>* GetGates();

        SDL_Rect* GetRect();
        void SetRect(SDL_Rect rect);
        
        void RenderGrid(Vec2 scroll, double zoom, double TILE_SIZE);

        void DestroyGate(std::list<Gate>::iterator it);

        // debugging
        void PrintGrid();
        void PrintID();

        friend class SectorSerializer;

    private:
        SDL_Renderer* renderer;
        Vec2 size;
        // the grid storing all the tiles
        std::vector<std::vector<Tile>> grid;
        std::vector<Spritesheet> spritesheets;

        std::list<Gate> gates;

        // rect for editor representation
        SDL_Rect rect;

        xg::Guid ID;
    };
}
