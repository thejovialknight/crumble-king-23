#pragma once
#include "king.h"
#include "game_state.h"
#include "collision.h"
#include "tiles.h"
#include "food.h"
#include "animation.h"

struct LevelData {
    std::string name;
    char tilemap[ROWS * COLUMNS];
};

enum class LevelState {
    PRE,
    ACTIVE,
    POST
};

enum class PostLevelBehavior {
    RESTART,
    ADVANCE,
    QUIT
};

struct PostLevelInfo {
    bool ready_to_exit = false; // Needed for Game to advance
    PostLevelBehavior behavior = PostLevelBehavior::RESTART; // True if won level, false if died
};

struct Level { 
    // State
    LevelState state = LevelState::PRE;
    PostLevelInfo post_level_info;
    double time_to_next_state;
    int points = 0;

    // "Entitites"
    King king;
    bool tiles[ROWS * COLUMNS];
    Food food;

    // Data
    LevelData* data;
    Sequence* tile_sequence;

    Level(LevelData* data, Sequences& sequences, Platform& platform);
};

void load_level(Level& level);
void update_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void handle_pre_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void handle_active_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void handle_post_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void render_level(Level& level, int sprite_atlas_texture, Platform& platform);
void goto_post_level(Level& level, PostLevelBehavior behavior);
