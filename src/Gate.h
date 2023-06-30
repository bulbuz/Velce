#pragma once

#include "SectorSerializer.h"
#include "Utils.h"
#include "crossguid/guid.hpp"
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
        void RemoveEndpoint(Gate* gate);

        void SetSectorRect(SDL_Rect* rect);
        SDL_Rect* GetSectorRect();

        std::set<Gate*> GetEndpoints();

        friend class SectorSerializer;

    private:
        // set of the endpoints
        std::set<Gate*> endpoints;

        SDL_Rect* sector_rect;

        // editor repr
        SDL_Rect rect;
        xg::Guid ID;
    };
}
