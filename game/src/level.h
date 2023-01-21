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

struct Level { 
    King king;
    bool tiles[ROWS * COLUMNS];
    int points = 0;
    int brick_texture = 0;
    Food food;
    bool ready_to_exit = false;

    Level(LevelData& data, Platform& platform);
};

void update_level(Level& level, int sprite_atlas_texture, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void display_points(Level& level, Platform& platform);