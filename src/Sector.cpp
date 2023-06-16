#include "Sector.h"
#include "Utils.h"
#include <SDL_render.h>
#include <string>

using namespace Velce;

Sector::Sector(SDL_Renderer* renderer, Vec2 size) : renderer(renderer), size(size) {
    LOG("sector created!");
    grid.resize(size.y, std::vector<Tile>(size.x, Tile()));
}

Sector::~Sector() {
    LOG("sector removed!");
}

void Sector::RemoveTile(Vec2 grid_pos) {
    grid[grid_pos.y][grid_pos.x] = Tile();
}

void Sector::SetTile(Tile tile, Vec2 grid_pos) {
    grid[grid_pos.y][grid_pos.x] = tile;
}

void Sector::AddSpritesheet(Spritesheet sheet) {
    spritesheets.push_back(sheet);
}

int Sector::GetSpritesheetID(Spritesheet* sheet) {
    int i = 0;
    assert(sheet != nullptr);
    for (auto sh : spritesheets) {
        if (sh.path == sheet->path)
            return i;
        i++;
    }
    return -1;
}

Vec2 Sector::GetSize() {
    return size;
}

void Sector::RenderGrid(Vec2 scroll, double zoom, double TILE_SIZE) {
    for (int i = 0; i < size.y; i++) {
        for (int j = 0; j < size.x; j++) {
            int id = grid[i][j].GetSpritesheetID();
            if (id != -1) {
                Vec2 pos = grid[i][j].GetGridPos();
                SDL_Rect src_rect{pos.x * spritesheets[id].tile_size.x, pos.y * spritesheets[id].tile_size.y, spritesheets[id].tile_size.x, spritesheets[id].tile_size.y};
                SDL_FRect dst_rect{(float) (j * TILE_SIZE * zoom + scroll.x), (float) (i * TILE_SIZE * zoom + scroll.y), (float) (TILE_SIZE * zoom), (float) (TILE_SIZE * zoom)};
                SDL_RenderCopyF(renderer, spritesheets[id].texture, &src_rect, &dst_rect);
            }
        }
    }
}

// debugging purposes
void Sector::PrintGrid() {

}
