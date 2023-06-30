#include "Game.h"

namespace Velce
{
    Game::Game(SDL_Renderer* renderer, int w, int h, std::string CWD, Sector* sector) :
        renderer(renderer),  WIN_WIDTH(w), WIN_HEIGHT(h), CWD(CWD), 
        player(renderer, 0, 0, CWD + "res/assets/art/character/player_sheet.png"), sector(sector) {
    }

    void Game::Run(double dt) {
        Input();
        Update(dt);
        Render();
    }

    void Game::Render() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        player.Render();
    }

    void Game::Update(double dt) {
        player.Update(dt);
    }

    void Game::Input() {

    }

} // namespace Velce
