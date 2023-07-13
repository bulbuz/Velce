#pragma once

#include "Utils.h"

namespace Velce {

    struct Camera {
        Camera(Vec2 position, Vec2 size) : position(position), size(size) {}
        Vec2 position;
        const Vec2 size;
    };

    /*
    class Camera {
    public:
        Camera(Vec2 position, Vec2 size);

        friend class SectorSerializer;
    private:
        Vec2 position;
        const Vec2 size;
    };
    */
}
