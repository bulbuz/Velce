#include "Tile.h"

namespace Velce {
	Tile::Tile() : clip{0, 0, 0, 0} {
	}

	Tile::Tile(SDL_Rect clip, xg::Guid spritesheetID, int scale) : spritesheetID(spritesheetID), clip(clip) {
	}

	Vec2 Tile::GetGridPos() {
		return Vec2(clip.x, clip.y);
	}
    xg::Guid Tile::GetSpritesheetID() {
		return spritesheetID;
	}

    SDL_Rect Tile::GetClip() {
        return clip;
    }
}
