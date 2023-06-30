#include "Sector.h"
#include "Logger.h"
#include "Utils.h"
#include "crossguid/guid.hpp"
#include "imgui.h"
#include <string>
#include <utility>

using namespace Velce;

Sector::Sector() : ID(xg::newGuid()) {
}

Sector::Sector(SDL_Renderer* renderer, Vec2 size) : renderer(renderer), size(size), ID(xg::newGuid()) {
    Logger::LOG(Logger::MODE::INFO, "sector created!");
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
    tiles.erase(std::make_pair(grid_pos.x, grid_pos.y));
}

void Sector::SetTile(Tile tile, Vec2 grid_pos) {
    tiles[{grid_pos.x, grid_pos.y}] = tile;
}

void Sector::AddSpritesheet(Spritesheet sheet) {
    // spritesheets.push_back(sheet);

    // assign identifier to spritesheet
    spritesheets[xg::newGuid()] = sheet;
}

xg::Guid Sector::GetSpritesheetID(Spritesheet* sheet) {
    assert(sheet != nullptr);

    for (const auto& [id, spritesheet] : spritesheets) {
        if (spritesheet.path == sheet->path) {
            return id;
        }
    }
    /*
    int i = 0;
    for (auto sh : spritesheets) {
        if (sh.path == sheet->path)
            return i;
        i++;
    }
    */

    // check if id is valid with xg::Guid.isValid()
    return xg::Guid();
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
    for (auto& [sector_pos, tile] : tiles) {
        xg::Guid id = tile.GetSpritesheetID();
        if (id.isValid()) {
            Vec2 pos = tile.GetGridPos();
            SDL_Rect src_rect{pos.x * spritesheets[id].tile_size.x, 
                pos.y * spritesheets[id].tile_size.y, 
                spritesheets[id].tile_size.x, spritesheets[id].tile_size.y};

            SDL_Rect r{sector_pos.first, sector_pos.second, 1, 1};
            SDL_FRect dst_rect = transform_rect(r, scroll, zoom, TILE_SIZE);
            SDL_RenderCopyF(renderer, spritesheets[id].texture, &src_rect, &dst_rect);
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
