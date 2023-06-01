#pragma once

#include <SDL.h>

#include "Utils.h"

namespace Velce {
	class InputHandler {
	public:
		InputHandler();

		void HandleEvents();
		Vec2 GetMouseDelta();
		Vec2 GetMousePos();

		bool input_state[(int)Input::_END - (int)Input::_START - 1];
		
	private:
		SDL_Event event;
		Vec2 MousePos;
	};

	enum class Input {
		_START,
		KEY_W,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_SPACE,
		MOUSE_LEFT,
		MOUSE_RIGHT,
		WINDOW_QUIT,
		_END,
	};
}