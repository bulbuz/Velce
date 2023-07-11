#pragma once

#include <SDL.h>

#include <filesystem>
#include <string>

#include "Player.h"
//#include "Sector.h"

namespace Velce {

    class Game {

    public:

        Game(SDL_Renderer* renderer, int w, int h, std::string CWD);
        void Run(double dt);

    private:

        // UISector* sector;
        SDL_Renderer* renderer;

        // display dimension
        const int WIN_WIDTH;
        const int WIN_HEIGHT;

        Player player;

        std::string CWD;

        friend class Engine;

    private:
        void Render();
        void Input();
        void Update(double dt);
    };
} // namespace Velce
