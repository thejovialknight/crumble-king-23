#include "level.h"
#include <iostream>

Level::Level(LevelData* data, Sequences& sequences, Platform& platform) : data(data) {
    // Initialize state
    food.sequences.push_back(&sequences.food_chicken);
    food.sequences.push_back(&sequences.food_grape);
    food.sequences.push_back(&sequences.food_watermelon);
    food.sequences.push_back(&sequences.food_potato);
    food.platter_sequence = &sequences.platter;
    food.bubbling_pot_sequence = &sequences.bubbling_pot;
    food.animator.sequence = food.sequences[random_int(food.sequences.size())];
    tile_sequence = &sequences.tile_center;
    window_sequence = &sequences.window;
    king.animator.sequence = &sequences.king_idle;
}

void load_level(Level& level, Sounds& sounds, Platform& platform) {
    level.tiles.clear();
    level.food.windows.clear();
    // Populate tiles from tilemap
    int i = 0;
    for (const char c : level.data->tilemap) {
        if (c == '\n') { continue; }
        if (c == 'K') { level.king.position = grid_position_from_index(i); }
        if (c == 'F') { level.food.windows.emplace_back(Window(grid_position_from_index(i))); }
        if (c == '#') { level.tiles.emplace_back(grid_position_from_index(i)); }
        i++;
    }

    // Initialize data
    level.score = 0;
    level.food.state = FoodState::INACTIVE;
    level.food.time_to_next_phase = 4; // TODO: noop
    level.food.level_complete = false;
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
    resolve_king_velocity(level.king, level.tiles, sounds, platform);
    update_food(level.score, level.food, level.king, sounds, platform, settings, delta_time);
    update_tiles(level.tiles, delta_time);

    if(is_king_dead(level.king, platform)) {
        level.ready_to_play_dead_sound = true;
        platform.background_color = Vec3(0.5, 0, 0);
        goto_post_level(level, PostLevelBehavior::RESTART);
    }
    else if(level.food.level_complete) {
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
    const double level_complete_float_speed = 300; // TODO: Settings!

    level.king.position.y -= level_complete_float_speed * delta_time;
    level.time_to_next_state -= delta_time;
    if(level.time_to_next_state <= 0 || platform.input.jump.just_pressed) {
        level.post_level_info.ready_to_exit = true;
    }
} 

void render_level(Level& level, int atlas, Platform& platform) {
    // Draw windows
    for(const Window& window : level.food.windows) {
        int frame = 0;
        if(!window.is_active) { frame = 1; }
        put_sprite(platform, sprite_from_sequence(
            atlas,
            *level.window_sequence,
            frame,
            window.spawn_position,
            false
        ));
    }
    // Draw food if active
    if (level.food.state != FoodState::INACTIVE) {
        put_sprite(platform, sprite_from_animator(
            atlas, 
            level.food.animator,
            level.food.position
        ));
    }
    // Draw tiles
    for (const Tile& tile : level.tiles) {
        int visible_health = tile.health;
        if (tile.is_crumbling) { visible_health--; }
        if (visible_health <= 0) { continue; }
        put_sprite(platform, sprite_from_sequence(
            atlas,
            *level.tile_sequence,
            visible_health,
            tile.position,
            false // TODO: is_flipped could be randomized? Hmm, probably not.
        ));
    }
    // Draw king
    put_sprite(platform, sprite_from_animator(
        atlas,
        level.king.animator,
        level.king.position
    ));
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
