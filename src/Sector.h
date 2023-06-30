#pragma once

#include <SDL.h>
#include "SectorSerializer.h"
#include "imgui.h"
#include <crossguid/guid.hpp>

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

#include "Gate.h"
#include "Tile.h"
#include "Utils.h"

namespace Velce {
    class Gate;
    class Tile;

    class Sector {
    public:
        // size of the sector in tiles
        Sector();
        Sector(SDL_Renderer* renderer, Vec2 size);
        ~Sector();

        void RemoveTile(Vec2 grid_pos);
        void SetTile(Tile tile, Vec2 grid_pos);
        void AddSpritesheet(Spritesheet sheet);
        xg::Guid GetSpritesheetID(Spritesheet* sheet);
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

        // give the serializer access to all members
        friend class SectorSerializer;

    private:
        SDL_Renderer* renderer;
        Vec2 size;
        
        // a map storing all the tiles and their sector positions
        std::map<std::pair<int, int>, Tile> tiles;
        // std::vector<Spritesheet> spritesheets;
        std::map<xg::Guid, Spritesheet> spritesheets;

        std::list<Gate> gates;

        // rect for editor representation
        SDL_Rect rect;

        xg::Guid ID;

        std::string title;
    };
}
