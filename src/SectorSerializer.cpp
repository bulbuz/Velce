#include "SectorSerializer.h"
#include <fstream>
#include <string>
#include <yaml-cpp/emittermanip.h>

using namespace Velce;

YAML::Emitter& operator << (YAML::Emitter& out, const Vec2& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, const SDL_Rect& r) {
    out << YAML::Flow;
    out << YAML::BeginSeq << r.x << r.y << r.w << r.h << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, const std::pair<int, int> pair) {
    out << YAML::Flow;
    out << YAML::BeginSeq << pair.first << pair.second << YAML::EndSeq;
    return out;
}

SectorSerializer::SectorSerializer(const Sector& sector) 
    : sector(sector) {
}

void SectorSerializer::Serialize(const std::string& path) {
    /*
    YAML::Emitter out;
    out << YAML::BeginMap; // Sector
    out << YAML::Key << "Sector" << YAML::Value << sector.ID;
    out << YAML::Key << "Size" << YAML::Value << sector.size;
    out << YAML::Key << "Rect" << YAML::Value << sector.rect;

    {
        out << YAML::Key << "Tiles"; 
        out << YAML::Value << YAML::BeginSeq; // Tiles
        for (const auto& [pos, tile] : sector.tiles) {
            out << YAML::BeginMap;
            out << YAML::Key << "Position" << YAML::Value << pos;
            out << YAML::Key << "Clip" << YAML::Value << tile.clip;
            out << YAML::Key << "SpritesheetID" << YAML::Value << tile.spritesheetID;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq; // Tiles
    }
    {
        out << YAML::Key << "Spritesheets"; 
        out << YAML::Value << YAML::BeginSeq; // Spritesheets
        for (const auto& [id, sheet] : sector.spritesheets) {
            out << YAML::BeginMap;
            out << YAML::Key << "GUID" << YAML::Value << id;
            out << YAML::Key << "Size" << YAML::Value << sheet.size;
            out << YAML::Key << "Tile size" << YAML::Value << sheet.tile_size;
            out << YAML::Key << "Padding" << YAML::Value << sheet.padding;
            out << YAML::Key << "Margin" << YAML::Value << sheet.margin;
            out << YAML::Key << "Path" << YAML::Value << sheet.path;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq; // Spritesheets
    }
    {
        out << YAML::Key << "Gates";
        out << YAML::Value << YAML::BeginSeq; // Gates
        for (const auto gate : sector.gates) {
            out << YAML::BeginMap;
            out << YAML::Key << "Gate" << YAML::Value << gate.ID;
            out << YAML::Key << "Sector rect" << YAML::Value << *gate.sector_rect;
            out << YAML::Key << "Rect" << YAML::Value << gate.rect;

            out << YAML::Key << "Endpoints";
            out << YAML::Value << YAML::Flow << YAML::BeginSeq; // Endpoints
            for (const auto& endpoint : gate.endpoints) {
                out << endpoint->ID;
            }
            out << YAML::EndSeq; // Endpoints
        }
        out << YAML::EndSeq; // Gates
    }
                         
    out << YAML::EndMap; // Sector

    std::ofstream fout(path);
    fout << out.c_str();
    Logger::LOG(Logger::MODE::INFO, "Saved sector!");
    fout.close();
    */
}

void SectorSerializer::Deserialize(const std::string& path) {

}
