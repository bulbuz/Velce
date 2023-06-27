#include "Sector.h"
#include "Logger.h"
#include "Utils.h"
#include "imgui.h"
#include <string>

using namespace Velce;

Sector::Sector() : ID(xg::newGuid()) {
}

Sector::Sector(SDL_Renderer* renderer, Vec2 size) : renderer(renderer), size(size), ID(xg::newGuid()) {
    Logger::LOG(Logger::MODE::INFO, "sector created!");
    grid.resize(size.y, std::vector<Tile>(size.x, Tile()));
}

Sector::~Sector() {
    Logger::LOG(Logger::MODE::INFO, "sector removed!");
    
    for (auto it = gates.begin(); it != gates.end(); it++) {
        DestroyGate(it--);
    }
}

void Sector::PrintID() {
    ImGui::Begin("debug");
    ImGui::Text("guid: ");
    ImGui::SameLine();
    ImGui::Text("%s", ID.str().c_str());
    ImGui::End();
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

void Sector::DestroyGate(std::list<Gate>::iterator it) {
    std::set<Gate*> endpoints = it->GetEndpoints();
    for (auto endpoint : endpoints) {
        endpoint->RemoveEndpoint(&*it);
    }
    gates.erase(it);
}

Vec2 Sector::GetSize() {
    return size;
}

SDL_Rect* Sector::GetRect() {
    return &rect;
}

void Sector::SetRect(SDL_Rect rect) {
    this->rect = rect;
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

void Sector::AddGate(Gate gate) {
    gates.push_back(gate);
}


std::list<Gate>* Sector::GetGates() {
    return &gates;
}

// debugging purposes
void Sector::PrintGrid() {

}
