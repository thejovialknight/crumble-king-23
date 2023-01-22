#include "game.h"

void init_game(Game& game, Platform& platform) {
    populate_settings(get_file_text("resources/config/config.txt"), game.settings);
    populate_sequences(get_file_text("resources/sprites/sequences.txt"), game.sequences);
    game.sprite_atlas = new_texture_handle(platform, "resources/sprites/sprite_atlas.png");
    std::string level_str = get_file_text("resources/levels/levels.txt");
    LevelData level_data;
    int start = 0;
    int length = 0; // must equal start at beginning
    for(int i = 0; i < level_str.length(); ++i) {
        if(level_str[i] == '@') {
            i++;
            // If this is the first level in string
            if(length != start) {
                game.levels.push_back(level_data);
            }

            // Get title
            const int max_title_length = 50;
            int current_title_length = 0;
            level_data.name = "";
            while(current_title_length < max_title_length && level_str[i] != ':') {
                level_data.name += level_str[i];
                i++;
            }
            start = i + 1;
            length = 0;
        }
        else {
            if (level_str[i] == '\n') { continue; }
            level_data.tilemap[length] = level_str[i];
            length++;
        }
    }
    game.levels.push_back(level_data);
    int j = 4;
}

void update_game(Game& game, Platform& platform, double delta_time) {
    populate_settings(get_file_text("resources/config/config.txt"), game.settings);
    switch(game.state) {
        case GameState::MENU :
            update_main_menu(*game.menu, game.levels, platform);\
            if(game.menu->ready_to_load_level && game.menu->level_index_to_load < game.levels.size()) {
                int level_index_to_load = game.menu->level_index_to_load;
                delete game.menu;
                game.state = GameState::LEVEL;
                game.level = new Level(&game.levels[level_index_to_load], game.sequences, platform);
            }
            break;
        case GameState::LEVEL :
            update_level(*game.level, game.sprite_atlas, game.sequences, platform, game.settings, delta_time);
            if(game.level->post_level_info.ready_to_exit) {
                if(game.level->post_level_info.is_advancing) { // if won
                    if(game.level_index >= game.levels.size() - 1) {
                        return_to_menu(game);
                    }
                    else {
                        game.level_index++;
                        game.level->data = &game.levels[level_index];
                        load_level(*game.level);
                    }
                }
                else { // if died
                    game.lives_remaining -= 1;
                    if(game.lives_remaining < 0) {
                        return_to_menu(game);
                    }
                    else {
                        load_level(Level& level);
                    }
                }
            }
            break;
        default :
            break;
    }
}

void return_to_menu(Game& game) {
    delete game.level;
    game.state = GameState::MENU;
    game.menu = new MainMenu();
    game.menu->state = MainMenuState::LEVEL_SELECT;
    populate_level_select_menu(game.levels, game.menu->list);
}
