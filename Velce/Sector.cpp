#include "Sector.h"

namespace Velce {
    Sector::Sector(Vec2 size) : size(size) {
        LOG("sector created!");
    }

    Sector::~Sector() {
        LOG("sector removed!");
    }

    void Sector::SetTile(Tile tile) {
        Vec2 pos = tile.GetGridPos();
        grid[pos.y][pos.x] = tile;
    }

    void Sector::AddSpritesheet(Spritesheet sheet) {
        spritesheets.push_back(sheet);
    }

    int Sector::GetSpritesheetID(Spritesheet* sheet) {
        int i = 0;
        for (auto sh : spritesheets) {
            if (sh.path == sheet->path)
                return i;
            i++;
        }
        return -1;
    }

    // debugging purposes
    void Sector::PrintGrid() {
    }

} // namespace Velce
