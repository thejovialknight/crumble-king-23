#pragma once
#include "platform.h"
#include "game_state.h"
#include "level.h"

struct MenuButton {
    std::string text;

    MenuButton(std::string text) : text(text) {}
};

struct MenuList {
    int selection = 0;
    std::vector<MenuButton> buttons;
};

enum class MainMenuState {
    MAIN,
    LEVEL_SELECT
};

struct MainMenu {
    MainMenuState state = MainMenuState::MAIN;
    MenuList list;
    int level_index_to_load = 0;
    bool ready_to_load_level = false;
};

void handle_menu_list(MenuList& list, Platform& platform);
void update_main_menu(MainMenu& menu, std::vector<LevelData> levels, Platform& platform);
void populate_main_menu(MenuList& list);
void populate_level_select_menu(std::vector<LevelData> levels, MenuList& list);
