#pragma once

#include <SDL.h>

#include "Utils.h"

namespace Velce {
    class Sector {
    public:
        Sector(int width, int height);
    private:
        Vec2 win_dimensions;
    };

} // namespace Velce
