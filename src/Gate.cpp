#include "Gate.h"
#include "Utils.h"

using namespace Velce;

Gate::Gate() {
    
}

Gate::Gate(SDL_Rect r) : rect(r) {

}

void Gate::AddEndpoint(Gate* gate) {
    endpoints.insert(gate);
}

void Gate::RemoveEndpoint(Gate* gate) {
    endpoints.erase(gate);
}

std::set<Gate*> Gate::GetEndpoints() {
    return endpoints;
}

SDL_Rect* Gate::GetSectorRect() {
    return sector_rect;
}

void Gate::SetSectorRect(SDL_Rect* rect) {
    sector_rect = rect;
}

SDL_Rect* Gate::GetRect() {
    return &rect;
}
