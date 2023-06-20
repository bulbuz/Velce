#pragma once

#include <SDL.h>

namespace Velce {

    class Gate {
    public:
        Gate();
        Gate(SDL_Rect r);
        SDL_Rect* GetRect();
    private:
        SDL_Rect rect;
    };
}
