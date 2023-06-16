#pragma once

#include <SDL.h>

#include <iostream>
#include <vector>

#include "Tile.h"
#include "Utils.h"

using namespace Velce;

class Sector {
public:
    // size of the sector in tiles
    Sector(SDL_Renderer*, Vec2 size);
    ~Sector();

    void RemoveTile(Vec2 grid_pos);
    void SetTile(Tile tile, Vec2 grid_pos);
    void AddSpritesheet(Spritesheet sheet);
    int GetSpritesheetID(Spritesheet* sheet);
    Vec2 GetSize();

    void RenderGrid(Vec2 scroll, double zoom, double TILE_SIZE);

    // debugging
    void PrintGrid();

private:
    SDL_Renderer* renderer;
    Vec2 size;
    // the grid storing all the tiles
    std::vector<std::vector<Tile>> grid;
    std::vector<Spritesheet> spritesheets;
};
