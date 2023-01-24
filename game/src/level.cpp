#include "level.h"
#include <iostream>

Level::Level(LevelData* data, Sequences& sequences, Platform& platform) : data(data) {
    // Initialize state
    food.sequences.push_back(&sequences.food_chicken);
    food.sequences.push_back(&sequences.food_grape);
    food.sequences.push_back(&sequences.food_watermelon);
    food.sequences.push_back(&sequences.food_potato);
    food.animator.sequence = food.sequences[random_int(food.sequences.size())];
    tile_sequence = &sequences.tile_center;
    king.animator.sequence = &sequences.king_idle;
}

void load_level(Level& level, Sounds& sounds, Platform& platform) {
    // Populate tiles from tilemap
    int i = 0;
    for (const char c : level.data->tilemap) {
        if (c == '\n') { continue; }

        level.tiles[i] = false;
        if (c == 'K') { level.king.position = tile_position_from_index(level.tiles, i); }
        if (c == 'F') { level.food.spawn_positions.emplace_back(tile_position_from_index(level.tiles, i)); }
        if (c == '#') { level.tiles[i] = true; }
        i++;
    }

    // Initialize data
    level.score = 0;
    level.food.is_active = false;
    level.food.time_to_next_phase = 4;
    level.food.animator.sequence = level.food.sequences[random_int(level.food.sequences.size())];
    level.king.velocity = Vec2(0, 0);
    level.king.jump_state = JumpState::GROUND;
    level.king.acceleration_mod = 1;
    // Initiate pre level
    level.state = LevelState::PRE;
    level.time_to_next_state = 2;
    level.post_level_info.ready_to_exit = false;
    level.post_level_info.behavior = PostLevelBehavior::RESTART;
    // Music
    stop_sound(platform, sounds.music_victory);
}

void update_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time) {
    switch(level.state) {
        case LevelState::PRE:
            handle_pre_level(level, atlas, sequences, sounds, platform, settings, delta_time);
            break;
        case LevelState::ACTIVE:
            handle_active_level(level, atlas, sequences, sounds, platform, settings, delta_time);
            break;
        case LevelState::POST:
            handle_post_level(level, atlas, sequences, platform, settings, delta_time);
            break;
    }
    
    // Menu on escape
    if (platform.input.pause.just_pressed) {
        buffer_sound(platform, sounds.menu_select, 1);
        goto_post_level(level, PostLevelBehavior::QUIT);
    }

    render_level(level, atlas, platform);
}

void handle_active_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time) {
    platform.background_color = Vec3(0, 0, 0);
    // Level logic
    update_king(level.king, platform, sequences, sounds, settings, delta_time);
    resolve_king_velocity(level.king, level.tiles);
    update_food(level.score, level.food, level.king, sounds, platform, settings, delta_time);
    if(is_king_dead(level.king, platform)) {
        level.ready_to_play_dead_sound = true;
        platform.background_color = Vec3(0.5, 0, 0);
        goto_post_level(level, PostLevelBehavior::RESTART);
    }
    else if (level.score >= 2) {
        stop_sound(platform, music_from_level_name(level.data->name, sounds));
        buffer_sound(platform, sounds.music_victory, 1);
        goto_post_level(level, PostLevelBehavior::ADVANCE);
    }
}

void handle_pre_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time) {  
    platform.background_color = Vec3(0, 0, 0);
    level.time_to_next_state -= delta_time;
    if(level.time_to_next_state <= 0 || platform.input.jump.just_pressed) {
        buffer_sound(platform, music_from_level_name(level.data->name, sounds), 1);
        level.state = LevelState::ACTIVE;
    }
}

void handle_post_level(Level& level, int atlas, Sequences& sequences, Platform& platform, Settings& settings, double delta_time) {
    level.time_to_next_state -= delta_time;
    if(level.time_to_next_state <= 0 || platform.input.jump.just_pressed) {
        level.post_level_info.ready_to_exit = true;
    }
} 

void render_level(Level& level, int atlas, Platform& platform) {
    // Draw tiles
    for (int i = 0; i < ROWS * COLUMNS; ++i) { 
        if (level.tiles[i] == true) {
            put_sprite(platform, sprite_from_sequence(
                atlas,
                *level.tile_sequence,
                0,
                tile_position_from_index(level.tiles, i),
                false
            ));
        }
    }
    // Draw king
    put_sprite(platform, sprite_from_animator(
        atlas, 
        level.king.animator,
        level.king.position
    ));
    // Draw food if active
    if (level.food.is_active) {
        put_sprite(platform, sprite_from_animator(
            atlas, 
            level.food.animator,
            level.food.position
        ));
    }

    // Draw points
    platform.texts.emplace_back(PlatformText(
        "Points: " + std::to_string(level.score),
        64,
        100,
        100,
        Vec3(0.9, 0.2, 0.2)
    ));
}

void goto_post_level(Level& level, PostLevelBehavior behavior) {
    level.time_to_next_state = 3;
    if(behavior == PostLevelBehavior::ADVANCE) {
        // TODO: lol
        level.time_to_next_state = 100000000;
    }
    level.state = LevelState::POST;
    level.post_level_info.behavior = behavior;
}

int music_from_level_name(std::string& name, Sounds& sounds) {
    if(name == "Archipelago") {
        return sounds.music_level1;
    }
    if(name == "Out of the Frying Pan...") {
        return sounds.music_level2;
    }
    if(name == "Xavier's Cave") {
        return sounds.music_level3;
    }
    return sounds.music_menu;
}
