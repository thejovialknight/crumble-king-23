#include "platform.h"

void init_platform(Platform& platform) {
    platform.actual_width = 1920;
    platform.actual_height = 1080;
    platform.logical_width = 640;
    platform.logical_height = 360;
    InitWindow(platform.actual_width, platform.actual_height, "Crumble King");
    InitAudioDevice();
    SetExitKey(KEY_F4);
    SetTargetFPS(60);
}

void update_platform(Platform& platform) {
    // Draw sprites 
    BeginDrawing();
    ClearBackground(ColorFromNormalized(Vector4{ 
        (float)platform.background_color.r, 
        (float)platform.background_color.g, 
        (float)platform.background_color.b 
    }));
    for(PlatformSprite& sprite : platform.sprites) {
        float pixel_scalar = platform.actual_height / platform.logical_height;
        float x_scalar = 1;
        if (sprite.is_flipped) { x_scalar = -1; }
        DrawTexturePro(
            platform.texture_assets[sprite.atlas], // Atlas texture
            Rectangle{  // Source
                (float)sprite.source.position.x,
                (float)sprite.source.position.y,
                (float)sprite.source.size.x * x_scalar,
                (float)sprite.source.size.y,
            },
            Rectangle{ // Dest
                (float)sprite.x * pixel_scalar,
                (float)sprite.y * pixel_scalar,
                (float)sprite.source.size.x * pixel_scalar,
                (float)sprite.source.size.y * pixel_scalar,
            },
            Vector2{ // origin
                (float)sprite.origin_x,
                (float)sprite.origin_y
            },
            0,
            WHITE
        );
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

    // Play sounds
    for(PlatformSound& sound : platform.sounds) {
        PlaySound(platform.sound_assets[sound.handle]);
    }
    
    // Clear sounds
    platform.sounds.clear();

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
    Texture2D texture_asset = LoadTexture(fname);
    platform.texture_assets.emplace_back(texture_asset);
    return platform.texture_assets.size() - 1;
}

int new_sound_handle(Platform& platform, const char* fname) {
    Sound sound_asset = LoadSound(fname);
    platform.sound_assets.emplace_back(sound_asset);
    return platform.sound_assets.size() - 1;
}

void put_sprite(Platform& platform, PlatformSprite sprite) {
    platform.sprites.push_back(sprite);
}

void buffer_sound(Platform& platform, int handle, double volume) {
    platform.sounds.push_back(PlatformSound(handle, volume));
}

void stop_sound(Platform& platform, int handle) {
    StopSound(platform.sound_assets[handle]);
}