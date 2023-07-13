#pragma once

#include <SDL.h>
#include "SectorSerializer.h"
#include "imgui.h"
#include <crossguid/guid.hpp>

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Camera.h"
#include "Gate.h"
#include "Tile.h"
#include "Utils.h"

namespace Velce {
    class Gate;
    class Tile;

    class UISector {
    public:
        // size of the sector in tiles
        UISector();
        UISector(SDL_Renderer* renderer, Vec2 size);
        ~UISector();

        void RemoveTile(std::string layer, Vec2 grid_pos);
        void SetTile(std::string layer, Tile tile, Vec2 grid_pos);
        void AddSpritesheet(Spritesheet sheet);
        xg::Guid GetSpritesheetID(Spritesheet* sheet);
        Vec2 GetSize();

        SDL_Rect GetCameraSize();

        void AddGate(Gate gate);
        std::list<Gate>* GetGates();

        SDL_Rect* GetRect();
        void SetRect(SDL_Rect rect);
        
        void RenderGrid(Vec2 scroll, double zoom, double TILE_SIZE, std::string active_layer);

        void DestroyGate(std::list<Gate>::iterator it);

        // debugging
        void PrintGrid();
        void PrintID();

        std::vector<std::string>& GetLayers();
        void SetLayers(std::vector<std::string> layers);

        // give the serializer access to all members
        friend class SectorSerializer;

    private:
        SDL_Renderer* renderer;
        Vec2 size;
        
        // a map storing all the tiles and their sector positions
        std::vector<std::string> layers;
        std::map<std::string, std::map<std::pair<int, int>, Tile>> tiles;
        std::map<xg::Guid, Spritesheet> spritesheets;

        std::list<Gate> gates;

        // rect for editor representation
        SDL_Rect rect;

        xg::Guid ID;

        Camera camera;

        std::string title;
    };
}