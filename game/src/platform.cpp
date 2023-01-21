#include "platform.h"

void init_platform(Platform& platform) {
    platform.actual_width = 1920;
    platform.actual_height = 1080;
    platform.logical_width = 640;
    platform.logical_height = 360;
    InitWindow(platform.actual_width, platform.actual_height, "Crumble King");
    SetExitKey(KEY_F4);
    SetTargetFPS(60);
}

void update_platform(Platform& platform) {
    // Draw sprites 
    BeginDrawing();
    ClearBackground(BLACK);
    for(PlatformSprite& sprite : platform.sprites) {
        float pixel_scalar = platform.actual_height / platform.logical_height;
        float x_scalar = 1;
        if (sprite.is_flipped) { x_scalar = -1; }
        // TODO: Implement x_scalar stuff. Maybe when we switch to SDL.
        DrawTextureEx(platform.textures[sprite.texture_handle], Vector2{ sprite.x * pixel_scalar, sprite.y * pixel_scalar }, 0, pixel_scalar, WHITE);
    }
    EndDrawing();

    // Clear sprites
    platform.sprites.clear();

    // Draw texts
    for(PlatformText& text : platform.texts) {
        DrawText(text.text.c_str(), text.x, text.y, text.font_size,
            ColorFromNormalized(Vector4{ 
                (float)text.color.r, 
                (float)text.color.g, 
                (float)text.color.b, 1.0 
            })
        );
    }
    
    // Clear texts
    platform.texts.clear();

    // Get next frame input
    query_button(platform.input.left);
    query_button(platform.input.right);
    query_button(platform.input.up);
    query_button(platform.input.down);
    query_button(platform.input.jump);
    query_button(platform.input.pause);

    // Get window_should_close
    if(!platform.window_should_close) {
        platform.window_should_close = WindowShouldClose();
    }
}

void deinit_platform() {
    CloseWindow();
}

void query_button(PlatformButton& button) {
    button.held = IsKeyDown(button.keycode);
    button.just_pressed = IsKeyPressed(button.keycode);
    button.released = IsKeyReleased(button.keycode);
}

const char* get_file_text(const char* fname) {
    return LoadFileText(fname);
}

int new_texture_handle(Platform& platform, const char* fname) {
    Texture2D texture = LoadTexture(fname);
    platform.textures.emplace_back(texture);
    return platform.textures.size() - 1;
}

void put_sprite(Platform& platform, int atlas_texture, const IRect dest, int x, int y) {
    put_sprite(platform, atlas_texture, dest, x, y, false);
}

void put_sprite(Platform& platform, int atlas_texture, const IRect dest, int x, int y, bool is_flipped) {
    platform.sprites.emplace_back(PlatformSprite(atlas_texture, dest, x, y, is_flipped));
}
