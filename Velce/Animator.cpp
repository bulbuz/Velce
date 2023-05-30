#include "Animator.h"

namespace Velce
{
    Animator::Animator(SDL_Renderer* renderer, std::string path) {
        this->renderer = renderer;
        elapsed = 0;
        spritesheet = NULL;
        spritesheet = IMG_LoadTexture(renderer, path.c_str());
        if (spritesheet == NULL) {
            std::cout << "Failed to load player spritesheet! SDL_Error: " << SDL_GetError() << std::endl;
        }
    }

    void Animator::Play(Vec2 pos) {
        AnimConfig config = state_config[cur_state];

        if (elapsed >= config.frame_duration / 1000.0) {
            elapsed -= config.frame_duration / 1000.0;
            cur_frame = (cur_frame + 1) % config.frame_count + 1;
        }
        int x = (cur_frame - 1) * (config.width + config.padding_x) + config.margin_x;
        int y = config.margin_y;
        SDL_Rect src_rect{ x, y, config.width, config.height };
        SDL_Rect dst_rect{ pos.x, pos.y, config.width * config.scale, config.height * config.scale };
        if (SDL_RenderCopyEx(renderer, spritesheet, &src_rect, &dst_rect, 0, NULL, config.flip) < 0) {
            std::cout << "ERROR! SDL_Error: " << SDL_GetError() << std::endl;
            exit(-1);
        }

        // debugging
        if (config.show_hitbox) {
            // draws a red border around the "hitbox"
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &dst_rect);
        }
    }

    void Animator::Update(double dt) {
        elapsed += dt;
    }

    void Animator::Restart() {
        elapsed = 0;
        cur_frame = state_config[cur_state].start_frame;
    }

    void Animator::SetState(ACTOR_STATE state) {
        cur_state = state;
    }

    void Animator::SetStateConfig(ACTOR_STATE state, AnimConfig config) {
        state_config[state] = config;
    }

    Animator::~Animator() {
        SDL_DestroyTexture(spritesheet);
    }
} // namespace Velce