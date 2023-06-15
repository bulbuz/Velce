#include "Tile.h"

namespace Velce {
	Tile::Tile() {
		spritesheetID = -1;
	}

	Tile::Tile(SDL_Rect clip, int spritesheetID, int scale) : spritesheetID(spritesheetID), clip(clip) {
	}

	Vec2 Tile::GetGridPos() {
		return Vec2(clip.x, clip.y);
	}
	int Tile::GetSpritesheetID() {
		return spritesheetID;
	}
}
