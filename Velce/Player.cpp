#include "Player.h"

namespace Velce {

	Player::Player(SDL_Renderer* r, int x, int y, std::string path) : Actor(r, x, y, path, Color(255, 255, 255)), animator(renderer, path) { // remove white colorkey
		// physics constants 
		// source: https://2dengine.com/p/platformers
		walk_speed = 350;
		apex_time = 0.3;
		jump_height = 200;
		gravity = (2 * jump_height) / (apex_time * apex_time);
		init_jump_vel = sqrt(2 * gravity * jump_height);

		state = IDLE;

		// temporary
		// ---------------------------------------

		// idle animation config
		AnimConfig ic;
		ic.width = 22;
		ic.height = 32;
		ic.margin_x = 18;
		ic.margin_y = 24;
		ic.padding_x = 34;
		ic.padding_y = 32;
		ic.frame_count = 6;
		ic.start_frame = 1;
		ic.frame_duration = 200;
		ic.show_hitbox = true;
		ic.scale = 8;
		animator.SetStateConfig(IDLE, ic);

		rect.w = ic.width * ic.scale;
		rect.h = ic.height * ic.scale;
		// ---------------------------------------
	}

	void Player::Update(double dt) {
		grounded = false;

		ImGui::Begin("Player vars");
		ImGui::SliderInt("Walk speed", &walk_speed, 0, 600);
		ImGui::SliderFloat("Jump apex time", &apex_time, 0.0, 1);
		ImGui::SliderInt("Jump height", &jump_height, 0, 600);
		gravity = (2 * jump_height) / (apex_time * apex_time);
		init_jump_vel = sqrt(2 * gravity * jump_height);
		ImGui::End();

		if (y >= WIN_HEIGHT - rect.h) {
			y = WIN_HEIGHT - rect.h;
			vel.y = 0;
			grounded = true;
		}

		const Uint8* key_states = SDL_GetKeyboardState(NULL);
		vel.x = 0;
		if (key_states[SDL_SCANCODE_D]) {
			vel.x = walk_speed;
		}
		if (key_states[SDL_SCANCODE_A]) {
			vel.x = -walk_speed;
		}
		if (key_states[SDL_SCANCODE_SPACE] && grounded) {
			vel.y = -init_jump_vel;
		}

		vel.y += (int)round(gravity * dt);

		x += (int)round(vel.x * dt);
		y += (int)round(vel.y * dt);

		animator.SetState(state);
		animator.Update(dt);
	}

	void Player::Render() {
		rect.x = x;
		rect.y = y;
		animator.Play(Vec2(x, y));
	}

	void Player::Input() {

	}

	Player::~Player() {
		OnClose();
	}

	void Player::OnClose() {
	}
}
