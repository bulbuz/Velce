#include "Engine.h"

namespace Velce {

	Engine::Engine(int w, int h) : WIN_WIDTH(w), WIN_HEIGHT(h) {
		deltatime = 0;

		// todo: check for exceptions
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Failed to initialize SDL! SDL_Error: " << SDL_GetError() << std::endl;
			exit(-1);
		}

		#ifdef SDL_HINT_IME_SHOW_UI
			SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
		#endif

		IMG_Init(IMG_INIT_PNG);

		window = SDL_CreateWindow("Velce", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer_Init(renderer);

		game_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
		editor_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);

		CWD = std::filesystem::current_path().string() + "/../";

		SDL_SetWindowResizable(window, SDL_TRUE);

		game = nullptr;
		editor = new Editor(renderer, WIN_WIDTH, WIN_HEIGHT);
		is_running = true;
		run_game = false;
	}

	void Engine::HandleEvents() {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				is_running = false;
			}

			editor->Input(&event);
		}
	}

	void Engine::Run() {
		Uint64 now = SDL_GetPerformanceCounter();
		Uint64 last = 0;
		deltatime = 0;

		while (is_running) {
			last = now;
			now = SDL_GetPerformanceCounter();
			deltatime = (double)((now - last) / (double)(SDL_GetPerformanceFrequency()));

			// Start the Dear ImGui frame
			ImGui_ImplSDLRenderer_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			HandleEvents();

			{
				ImGui::Begin("Editor", NULL);
				SDL_SetRenderTarget(renderer, editor_buffer);
				editor->Run();
				ImGui::Image((void*)editor_buffer, ImVec2(WIN_WIDTH, WIN_HEIGHT));
				ImGui::End();
			}

			{
				SDL_SetRenderTarget(renderer, game_buffer);

				ImGui::Begin("Game");
				if (ImGui::Button("Play")) {
					delete game;
					game = nullptr;
					game = new Game(renderer, WIN_WIDTH, WIN_HEIGHT, CWD);
					run_game = 1;
				}

				ImGui::SameLine();
				if (ImGui::Button("Stop")) {
					run_game = 0;
					delete game;
					game = nullptr;
				}

				if (run_game) {
					game->Run(deltatime);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderClear(renderer);
				}
				ImGui::Image((void*)game_buffer, ImVec2(WIN_WIDTH, WIN_HEIGHT));
				ImGui::End();
			}

			// Render imgui
			SDL_SetRenderTarget(renderer, NULL);
			SDL_SetRenderDrawColor(renderer, 44, 43, 46, 255);
			SDL_RenderClear(renderer);
			ImGui::Render();
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
			SDL_RenderPresent(renderer);
		}
	}

	// Cleanup resources
	void Engine::OnClose() {
		delete game;
		delete editor;
		SDL_DestroyTexture(game_buffer);
		SDL_DestroyTexture(editor_buffer);

		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	Engine::~Engine() {
		OnClose();
	}
}