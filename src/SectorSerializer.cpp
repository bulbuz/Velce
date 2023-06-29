#include "SectorSerializer.h"
#include <string>

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


SectorSerializer::SectorSerializer(const Sector& sector) 
    : sector(sector) {
}

static void SerializeTile(YAML::Emitter& out, Tile& tile) {
}

void SectorSerializer::Serialize(const std::string& path) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Sector" << YAML::Value << "Untitled";
    out << YAML::Key << "Size" << YAML::Value << sector.size;
    out << YAML::Key << "GUID" << YAML::Value << sector.ID;
    out << YAML::EndMap;
}

void SectorSerializer::Deserialize(const std::string& path) {

}
