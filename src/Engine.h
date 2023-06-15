#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "Editor.h"
#include "Game.h"
#include "Player.h"

namespace Velce {
	class Engine {
	public:
		void Run();
		Engine(int w, int h);
		~Engine();

	private:
		const int WIN_WIDTH;
		const int WIN_HEIGHT;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;

		Editor* editor;
		Game* game;
		double deltatime;
		bool is_running;

		void Render();
		void Update();
		void HandleEvents();
		void OnClose();

		SDL_Texture* game_buffer;
		SDL_Texture* editor_buffer;

		bool run_game;

		// current working directory path
		std::string CWD;
	};
}

