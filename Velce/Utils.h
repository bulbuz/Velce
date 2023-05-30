#pragma once

namespace Velce
{
#define EMPTY_COLOR Color(-1, -1, -1)

	// 2D Vector
	struct Vec2 {
		Vec2() { }
		Vec2(int x, int y) : x(x), y(y) { }

		inline Vec2 operator+(Vec2 that) {
			return Vec2(x + that.x, y + that.y);
		}

		inline Vec2 operator-(Vec2 that) {
			return Vec2(x - that.x, y - that.y);
		}

		inline Vec2 operator*(Vec2 that) {
			return Vec2(x * that.x, y * that.y);
		}

		inline Vec2 operator/(Vec2 that) {
			return Vec2(x / that.x, y / that.y);
		}

		inline bool operator==(Vec2 that) {
			return x == that.x && y == that.y;
		}

		inline bool operator!=(Vec2 that) {
			return (x != that.x || y != that.y);
		}

		double x = 0;
		double y = 0;
	};

	// color representation in RGB-format
	struct Color {
		Color(int r, int g, int b) : r(r), g(g), b(b) { }

		int r = -1;
		int g = -1;
		int b = -1;

		bool operator == (const Color& rhs) const
		{
			return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b);
		}
	};

	// structure for animation settings
	struct AnimConfig {
		int width;
		int height;

		int frame_count; // number of frames for the animation
		int frame_duration; // duration of a single frame
		int start_frame = 1; // which frame to start animate from (1-indexed)
		double scale = 1;

		int padding_x = 0; // horizontal space between sprites 
		int padding_y = 0; // vertical space between sprites

		int margin_x = 0; // horizontal surrounding space
		int margin_y = 0; // vertical surrounding space

		bool show_hitbox = false; // debugging purposes (hitbox)

		SDL_RendererFlip flip = SDL_FLIP_NONE;
	};

	// enumeration of actor states
	enum ACTOR_STATE {
		LEFT,
		RIGHT,
		JUMPING,
		FALLING,
		IDLE
	};

} // namespace Velce