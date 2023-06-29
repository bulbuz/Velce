#pragma once

#include <yaml-cpp/yaml.h>

#include "Sector.h"

#include <string>

namespace Velce {    
    class Sector;
    
    class SectorSerializer {
    public:
        SectorSerializer(const Sector& sector);

        void Serialize(const std::string& path);
        void Deserialize(const std::string& path);

    private:
        const Sector& sector;
    };
}
