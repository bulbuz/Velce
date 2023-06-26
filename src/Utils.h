#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "imgui.h"

#include "Debug.h"

#include <iostream>
#include <string>

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

    inline Vec2& operator=(Vec2 that) {
        x = that.x;
        y = that.y;
        return *this;
    }

    int x = 0;
    int y = 0;
};

// color representation in RGB-format
struct Color {
    Color() : r(0), g(0), b(0) { }
    Color(int r, int g, int b) : r(r), g(g), b(b) { }

    int r = -1;
    int g = -1;
    int b = -1;

    bool operator == (const Color& that) const {
        return (this->r == that.r && this->g == that.g && this->b == that.b);
    }
};

// OBS! MANUALLY CALL SDL_DestroyTexture() on the texture!!!
struct Spritesheet {
    Vec2 size;

    Vec2 tile_size; // dimensions of a single item

    int scale = 1;

    Vec2 margin; // surrounding space
    Vec2 padding; // internal space (between sprites)

    std::string path;

    SDL_Texture* texture = NULL;

    void LoadImage(SDL_Renderer* renderer, std::string path) {
        texture = IMG_LoadTexture(renderer, path.c_str());
        LOG(texture);
        if (texture == NULL)
            std::cerr << "Failed to load spritesheet! SDL_Error: " << SDL_GetError() << std::endl;

        SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    }

    void RenderAt(SDL_Renderer* renderer, SDL_Rect* src_rect, SDL_Rect* dst_rect, SDL_RendererFlip flip) {
        if (SDL_RenderCopyEx(renderer, texture, src_rect, dst_rect, 0, NULL, flip) < 0) {
            std::cerr << SDL_GetError() << std::endl;
        }
    }
};

// structure for animation settings
struct AnimConfig {
    int width;
    int height;

    int frame_count; // number of frames for the animation
    int frame_duration; // duration of a single frame
    int start_frame = 1; // which frame to start animate from (1-indexed)

    Spritesheet sheet;

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


template<typename T>
void dbg(T t) {
    ImGui::Begin("debug");
    ImGui::Text( std::to_string(t).c_str());
    ImGui::End();
}

template<typename T, typename... Args>
void dbg(T t, Args... args) {
    ImGui::Begin("debug");
    ImGui::Text( std::to_string(t).c_str());
    ImGui::SameLine();
    ImGui::End();

    dbg(args...);
}

inline void dbg(const char* s) {
    ImGui::Begin("debug");
    ImGui::Text("%s", s);
    ImGui::End();
}

template<typename T, typename... Args>
void dbg(const char* s, T t, Args... args) {
    ImGui::Begin("debug");
    ImGui::Text( (s + std::to_string(t)).c_str());
    ImGui::SameLine();
    ImGui::End();

    dbg(args...);
}

// checks if value is in the given range (excluded the upper bound)
template<typename T>
inline bool InRangeEx(T val, T lo, T hi) {
    return val >= lo && val < hi;
}

// checks if value is in the given range (inclusive)
template<typename T>
inline bool InRange(T val, T lo, T hi) {
    return val >= lo && val <= hi;
}
