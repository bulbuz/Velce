#pragma once

#include <yaml-cpp/yaml.h>

#include "UISector.h"
#include "Camera.h"
#include <string>

namespace Velce {    
    class UISector;

    class SectorSerializer {
    public:
        SectorSerializer(const UISector& sector);

        void Serialize(const std::string& path);
        void Deserialize(const std::string& path);

    private:
        const UISector& sector;
    };
}

