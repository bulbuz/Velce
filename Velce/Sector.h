#pragma once

#include <SDL.h>

#include <iostream>
#include <vector>

#include "Tile.h"
#include "Utils.h"

namespace Velce {
    class Sector {
    public:
        // size of the sector in tiles
        Sector(Vec2 size);
        ~Sector();

        void SetTile(Tile tile);
        void AddSpritesheet(Spritesheet sheet);
        int GetSpritesheetID(Spritesheet* sheet);

        // debugging
        void PrintGrid();

    private:
        Vec2 size;
        // the grid storing all the tiles
        std::vector<std::vector<Tile>> grid;
        std::vector<Spritesheet> spritesheets;
    };

} // namespace Velce
