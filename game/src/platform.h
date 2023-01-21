#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "vec3.h"
#include "settings.h"
#include "rect.h"

struct PlatformButton {
    int keycode;
    bool held = false;
    bool just_pressed = false;
    bool released = false;

    PlatformButton(int keycode) : keycode(keycode) {}
};

// TODO: Too specific for a platform layer. Figure something out.
struct PlatformInput {
    PlatformButton left = PlatformButton(KEY_A);
    PlatformButton right = PlatformButton(KEY_D);
    PlatformButton up = PlatformButton(KEY_W);
    PlatformButton down = PlatformButton(KEY_S);
    PlatformButton jump = PlatformButton(KEY_SPACE);
    PlatformButton pause = PlatformButton(KEY_ESCAPE);
};

struct PlatformSprite {
    int texture_handle;
    IRect dest;
    int x;
    int y;
    bool is_flipped;

    PlatformSprite(int texture_handle, IRect dest, int x, int y, bool is_flipped) : texture_handle(texture_handle), dest(dest), x(x), y(y), is_flipped(is_flipped) {}
};

// TODO: Refactor for pixel art text with atlas
struct PlatformText {
    std::string text;
    double font_size;
    int x;
    int y;
    Vec3 color;

    PlatformText(std::string text, double font_size, int x, int y, Vec3 color) :
        text(text), font_size(font_size), x(x), y(y), color(color) {}
};

struct Platform {
    int actual_width;
    int actual_height;
    int logical_width;
    int logical_height;
    bool window_should_close = false;
    PlatformInput input;
    std::vector<Texture2D> textures;
    std::vector<PlatformSprite> sprites;
    std::vector<PlatformText> texts;
};

// Platform management
void init_platform(Platform& platform);
void update_platform(Platform& platform);
void deinit_platform();

// Input handling
void query_button(PlatformButton& button);

// Text loading
const char* get_file_text(const char* fname);

// Sprite handling
int new_texture_handle(Platform& platform, const char* fname);
void put_sprite(Platform& platform, int atlas_texture, const IRect dest, int x, int y);
void put_sprite(Platform& platform, int atlas_texture, const IRect dest, int x, int y, bool is_flipped);