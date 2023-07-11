#pragma once

#include <SDL.h>

#include "UISector.h"
#include "SectorSerializer.h"
#include "Utils.h"
#include "crossguid/guid.hpp"

/*
class containing the information of a single tile
*/

namespace Velce {
	class Tile {
	public:
		Tile();
		Tile(SDL_Rect clip, xg::Guid spritesheetID, int scale = 1);
		Vec2 GetGridPos();
        xg::Guid GetSpritesheetID();
        SDL_Rect GetClip();

        friend class SectorSerializer;

	private:
		// the id of the spritesheet in the sector it belongs to
        xg::Guid spritesheetID;
		SDL_Rect clip;
	};
}

