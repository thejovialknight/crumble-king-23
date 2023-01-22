#include "level.h"
#include <iostream>

Level::Level(const LevelData& data, Sequences& sequences, Platform& platform) {
    // Populate tiles from tilemap
    int i = 0;
    for (const char c : data.tilemap) {
        if (c == '\n') { continue; }

        tiles[i] = false;
        if (c == 'K') { king.position = tile_position_from_index(tiles, i); }
        if (c == 'F') { food.spawn_positions.emplace_back(tile_position_from_index(tiles, i)); }
        if (c == '#') { tiles[i] = true; }
        i++;
    }
    tile_sequence = &sequences.tile_center;

    // Initialize state
    points = 0;
    food.time_to_next_phase = 4;
    food.sequences.push_back(&sequences.food_chicken);
    food.sequences.push_back(&sequences.food_grape);
    food.sequences.push_back(&sequences.food_watermelon);
    food.sequences.push_back(&sequences.food_potato);
    food.animator.sequence = food.sequences[random_int(food.sequences.size())];
}

void update_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time) {
    // Level logic
    update_king(level.king, platform, sequences, settings, delta_time);
    resolve_king_velocity(level.king, level.tiles);
    update_food(level.points, level.food, level.king, platform, settings, delta_time);
    if(is_king_dead(level.king, platform)) {
        level.ready_to_exit = true;
    }

    // Menu on escape
    if (platform.input.pause.just_pressed) { 
        level.ready_to_exit = true;
    }

    // Render to platform
    // Draw tiles
    for (int i = 0; i < ROWS * COLUMNS; ++i) { // put some sprites
        if (level.tiles[i] == true) {
            put_sprite(platform, sprite_from_sequence(
                sprite_atlas_texture,
                *level.tile_sequence,
                0,
                tile_position_from_index(level.tiles, i),
                false
            ));
        }
    }
    // Draw king
    put_sprite(platform, sprite_from_animator(
        sprite_atlas_texture, 
        level.king.animator,
        level.king.position
    ));
    // Draw food if active
    if (level.food.is_active) {
        put_sprite(platform, sprite_from_animator(
            sprite_atlas_texture, 
            level.food.animator,
            level.food.position
        ));
    }

    std::string points_word = "Points: ";
    std::string points_string_text = std::to_string(level.points);
    std::string pre_converted_concat_str = points_word + points_string_text;
    platform.texts.emplace_back(PlatformText(
        //"Points!",
        "Points: " + std::to_string(level.points),
        64,
        100,
        100,
        Vec3(0.9, 0.2, 0.2)
    ));
}

void display_points(Level& level, Platform& platform) {
    std::string points_display = "Points: " + level.points;
    platform.texts.emplace_back(points_display.c_str(), 32, 32, 32, Vec3(0.8, 0.8, 0.8));
}
