#pragma once
#include <vector>
#include <string>
#include "level.h"
#include "menu.h"
#include "game.h"
#include "game_state.h"
#include "settings.h"
#include "animation.h"

struct Game {
    // Scenes
    GameState state;
    Level* level;
    MainMenu* menu;

    // State
    int lives_remaining = 3;
    int level_index = 0;

    // Data
    Settings settings;
    std::vector<LevelData> levels;
    Sequences sequences;
    int sprite_atlas; // Texture index
};

void init_game(Game& game, Platform& platform);
void update_game(Game& game, Platform& platform, double delta_time);
void return_to_menu(Game& game);
