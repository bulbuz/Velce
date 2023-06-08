#include "Animator.h"

namespace Velce
{
    Animator::Animator(SDL_Renderer* renderer) {
        this->renderer = renderer;
        elapsed = 0;
    }

    void Animator::Play(Vec2 pos) {
        AnimConfig config = state_config[cur_state];

        if (elapsed >= config.frame_duration / 1000.0) {
            elapsed -= config.frame_duration / 1000.0;
            cur_frame = (cur_frame + 1) % config.frame_count + 1;
        }
        int x = (cur_frame - 1) * (config.width + config.sheet.padding_x) + config.sheet.margin_x;
        int y = config.sheet.margin_y;
        SDL_Rect src_rect{ x, y, config.width, config.height };
        SDL_Rect dst_rect{ pos.x, pos.y, config.width * config.sheet.scale, config.height * config.sheet.scale };

        config.sheet.RenderAt(renderer, &src_rect, &dst_rect, config.flip);

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

    void Animator::ConfigureState(ACTOR_STATE state, AnimConfig config) {
        state_config[state] = config;
    }

    Animator::~Animator() {
    }
} // namespace Velce