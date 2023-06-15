#include "Actor.h"
namespace Velce {
	Actor::Actor() {
	}

	Actor::Actor(SDL_Renderer* r, int x, int y, std::string path, Color color_key) {
		this->x = x;
		this->y = y;
		this->renderer = r;
		texture = NULL;

		// todo: check if loaded correctly
		if (path != "") {
			if (color_key == EMPTY_COLOR) {
				texture = IMG_LoadTexture(renderer, path.c_str());
			}
			else {
				SDL_Surface* temp = IMG_Load(path.c_str());
				if (temp == NULL) {
					std::cerr << "Failed to load image in Actor.h!" << "SDL_Error: " << SDL_GetError() << std::endl;
				}
				SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, color_key.r, color_key.g, color_key.b));
				texture = SDL_CreateTextureFromSurface(renderer, temp);

				if (texture == NULL) {
					std::cerr << "Failed to load texture in Actor.h!" << "SDL_Error: " << SDL_GetError() << std::endl;
					exit(-1);
				}

				w = temp->w;
				h = temp->h;

				rect.x = x;
				rect.y = y;
				rect.w = w;
				rect.h = h;

				SDL_FreeSurface(temp);
			}
		}
	}

	void Actor::Render(double scale) {
		if (scale != 1) {
			SDL_Rect r = rect;
			r.w *= scale;
			r.h *= scale;
			SDL_RenderCopy(renderer, texture, NULL, &r);
		}
		else {
			SDL_RenderCopy(renderer, texture, NULL, &rect);
		}

	}
	Actor::~Actor() {
		OnDestroy();
	}

	void Actor::OnDestroy() {
		SDL_DestroyTexture(texture);
	}
}
