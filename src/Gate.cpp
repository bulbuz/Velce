#include "Gate.h"


using namespace Velce;

Gate::Gate(SDL_Rect r) : rect(r) {

}

void Gate::AddEndpoint(Gate* gate) {
    endpoints.insert(gate);
}

void Gate::SetSectorRect(SDL_Rect* rect) {
    sector_rect = rect;
}

SDL_Rect* Gate::GetSectorRect() {
    return sector_rect;
}

std::set<Gate*> Gate::GetEndpoints() {
    return endpoints;
}

SDL_Rect* Gate::GetRect() {
    return &rect;
}
