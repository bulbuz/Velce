#pragma once

#include <SDL.h>

#include <iostream>
#include <vector>

#include "Gate.h"
#include "Tile.h"
#include "Utils.h"

namespace Velce {
    
    class Sector {
    public:
        // size of the sector in tiles
        Sector(SDL_Renderer* renderer, Vec2 size);
        ~Sector();

        void RemoveTile(Vec2 grid_pos);
        void SetTile(Tile tile, Vec2 grid_pos);
        void AddSpritesheet(Spritesheet sheet);
        int GetSpritesheetID(Spritesheet* sheet);
        Vec2 GetSize();

        void AddGate(Gate gate);
        std::vector<Gate>* GetGates();

        SDL_Rect* GetRect();
        void SetRect(SDL_Rect rect);
        
        void RenderGrid(Vec2 scroll, double zoom, double TILE_SIZE);

        // debugging
        void PrintGrid();

    private:
        SDL_Renderer* renderer;
        Vec2 size;
        // the grid storing all the tiles
        std::vector<std::vector<Tile>> grid;
        std::vector<Spritesheet> spritesheets;

        std::vector<Gate> gates;

        // rect for editor representation
        SDL_Rect rect;
    };
}
