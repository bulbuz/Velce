#include "Engine.h"
#include "Logger.h"
#include "imgui.h"

using namespace Velce;

Engine::Engine(int w, int h) : WIN_WIDTH(w), WIN_HEIGHT(h) {
    deltatime = 0;

    // todo: check for exceptions
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }
    Logger::LOG(Logger::MODE::SYSTEM, "Initialized SDL");

    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    IMG_Init(IMG_INIT_PNG);
    uint32_t window_flags = SDL_WINDOW_SHOWN;
    window_flags |= SDL_WINDOW_MAXIMIZED;
    window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow("Velce", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

#ifndef RELEASE
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

    SDL_SetWindowResizable(window, SDL_TRUE);
#endif // RELEASE
    Logger::LOG(Logger::MODE::SYSTEM, "Created ImGui context");

    game_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
    editor_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);

    CWD = std::filesystem::current_path().string() + "/../";

    game = nullptr;
    editor = new Editor(renderer, WIN_WIDTH, WIN_HEIGHT, &CWD);

    run_engine = true;
    run_editor = true;
    run_game = false;
}

void Engine::HandleEvents() {
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            run_engine= false;
        }
    }
}

void Engine::Layout() {
    ImGui::Begin("Console");
    ImGui::Text("%s", Logger::GetBuffer().c_str());
    ImGui::End();

    if (run_editor) {
        ImGui::Begin("Editor", NULL, ImGuiWindowFlags_MenuBar);
        SDL_SetRenderTarget(renderer, editor_buffer);
        editor->Run();


        ImGui::Image((void*)editor_buffer, ImVec2(WIN_WIDTH, WIN_HEIGHT));
        ImGui::End();
    }

    {
        ImGui::Begin("Game");
        if (ImGui::Button("Play")) {
            run_game = 1;
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            run_game = 0;
        }

        ImGui::End();
    }
}

void Engine::Update() {
    Layout();
    if (run_game) {
        UISector* sector = editor->GetSector();
        if (game == nullptr && sector != nullptr) {
            game = new Game(renderer, WIN_WIDTH, WIN_HEIGHT, CWD);
            run_editor = false;
        } else if (game != nullptr) {
            ImGui::Begin("Game");
            SDL_SetRenderTarget(renderer, game_buffer);
            ImGui::Image((void*)game_buffer, ImVec2(WIN_WIDTH, WIN_HEIGHT));
            ImGui::End();
            game->Run(deltatime);
        } else {
            run_game = false;
        }
    } else {
        run_editor = true;
        delete game;
        game = nullptr;
    }
}

void Engine::Render() {
    // Render imgui
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 44, 43, 46, 255);
    SDL_RenderClear(renderer);
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void Engine::Start() {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;

    Logger::LOG(Logger::MODE::SYSTEM, "Initialized.");
    while (run_engine) {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltatime = (double)((now - last) / (double)(SDL_GetPerformanceFrequency()));

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        HandleEvents();
        Update();
        Render();
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
    IMG_Quit();
    SDL_Quit();
}

Engine::~Engine() {
    OnClose();
}
