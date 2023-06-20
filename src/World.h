#pragma once

#include "Sector.h"
#include "Utils.h"

#include <vector>


/*
 *
 * class containing the game world
 *
 */

namespace Velce {

    class World {
    public:
        World(Vec2 size);

        void AddSector(Sector* sector);
        void RemoveSector(Sector* sector);
    private:
        Vec2 size;
        std::vector<Sector*> sectors;
    };
} // namespace Velce
