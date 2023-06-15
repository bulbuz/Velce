#pragma once

#include "Actor.h"
#include "Animator.h"
#include "Utils.h"
#include "Debug.h"

#include <math.h>

namespace Velce {
	class Player : public Actor {
	public:
		Player(SDL_Renderer* r, int x, int y, std::string path);
		~Player();
		void Update(double dt);
		void Render();
		void Input();

	private:

		ACTOR_STATE state;
		Animator animator;

		// physics
		Vec2 vel;
		int walk_speed;
		int gravity;
		int init_jump_vel;
		int jump_height;
		float apex_time;
		bool grounded;

		void OnClose();

		// temporary
		int WIN_WIDTH = 960;
		int WIN_HEIGHT = 540;
	};
}
