#pragma once

#include <SDL.h>
#include <set>
#include <stdint.h>
#include <vector>

namespace Velce {
    class Gate {
    public:
        Gate();
        Gate(SDL_Rect r);
        SDL_Rect* GetRect();
        void AddEndpoint(Gate* gate);
        void SetSectorRect(SDL_Rect* rect);
        SDL_Rect* GetSectorRect();
        std::set<Gate*> GetEndpoints();
    private:
        SDL_Rect* sector_rect = nullptr;
        
        // vector of the endpoints for this gate
        std::set<Gate*> endpoints;

        // editor repr
        SDL_Rect rect;
    };
}
