#pragma once

#include <SDL.h>

#include "Actor.h"
#include "Utils.h"

/*
class containing the information of a single tile
*/

namespace Velce {
	class Tile {
	public:
		Tile();
		Tile(SDL_Rect clip, int spritesheetID, int scale = 1);
		Vec2 GetGridPos();

	private:
		// the id of the spritesheet in the sector it belongs to
		int spritesheetID;
		SDL_Rect clip;
	};
}

