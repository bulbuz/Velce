#include "InputHandler.h"

namespace Velce {
	InputHandler::InputHandler() {
		for (int i = (int) Input::_START + 1; Input(i) != Input::_END; i = (int) Input(i + 1)) {
			input_state[i] = false;
		}
	}
	void InputHandler::HandleEvents() {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				input_state[(int)Input::WINDOW_QUIT] = true;

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					input_state[(int)Input::KEY_W] = true;
				case SDLK_a:
					input_state[(int)Input::KEY_A] = true;
				case SDLK_s:
					input_state[(int)Input::KEY_S] = true;
				case SDLK_d:
					input_state[(int)Input::KEY_D] = true;
				default:
					break;
				}
			}

			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					input_state[(int)Input::KEY_W] = false;
				case SDLK_a:						 
					input_state[(int)Input::KEY_A] = false;
				case SDLK_s:						 
					input_state[(int)Input::KEY_S] = false;
				case SDLK_d:						 
					input_state[(int)Input::KEY_D] = false;
				default:
					break;
				}
			}

			if (event.type == SDL_MOUSEMOTION) {
			}
		}
	}
	Vec2 InputHandler::GetMouseDelta() {

		return Vec2();
	}

	Vec2 InputHandler::GetMousePos() {
		return Vec2();
	}
}