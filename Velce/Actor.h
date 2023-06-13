#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "imgui.h"

#include <string>

#include "Debug.h"
#include "Utils.h"

namespace Velce {
	class Actor {
	public:
		Actor();
		Actor(SDL_Renderer* r, int x, int y, std::string path = "", Color color_key = EMPTY_COLOR);
		~Actor();

		void Render(double scale = 1);

		SDL_Renderer* renderer;
		SDL_Texture* texture;
		SDL_Rect rect;
		int x;
		int y;
		int w;
		int h;

		void OnDestroy();

	private:
	};
}

