#pragma once

#include <SDL.h>

#include <map>
#include <string>

#include "Actor.h"
#include "Utils.h"

/*

Statemachine kinda?

usage:
set all states and their respective configurations with SetStateConfig()
and then SetState() to choose what state to play

call Play() to actually play the animation

Restart() to reset currently selected animation
*/

namespace Velce
{
    class Animator {
    public:
        Animator(SDL_Renderer* renderer, std::string path);
        ~Animator();

        // todo: error catching (must set state and configs before playing)
        void Play(Vec2 pos);
        void Update(double dt);
        void Restart();

        void SetStateConfig(ACTOR_STATE state, AnimConfig config);
        void SetState(ACTOR_STATE state);

    private:
        SDL_Texture* spritesheet;
        SDL_Renderer* renderer;
        ACTOR_STATE cur_state;
        double elapsed;
        int cur_frame;

        std::map<ACTOR_STATE, AnimConfig> state_config;
    };
} // namespace Velce