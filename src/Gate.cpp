#include "Gate.h"

Velce::Gate::Gate(SDL_Rect r) : rect(r) {
    
}

SDL_Rect* Velce::Gate::GetRect() {
    return &rect;
}
