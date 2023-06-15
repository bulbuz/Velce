#pragma once

#include <SDL.h>

#include <filesystem>
#include <string>

#include "Player.h"
#include "World.h"

namespace Velce
{
    class Game {
    public:
        Game(SDL_Renderer* renderer, int w, int h, std::string CWD);
        void Run(double dt);
    private:
        void Render();
        void Input();
        void Update(double dt);

        const int WIN_WIDTH;
        const int WIN_HEIGHT;

        SDL_Renderer* renderer;
        Player player;

        std::string CWD;
    };
} // namespace Velce
