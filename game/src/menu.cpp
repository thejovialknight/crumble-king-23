#include "menu.h"

void handle_menu_list(MenuList& list, Platform& platform) {
    // Iterate selection based on input
    if (platform.input.up.just_pressed) { 
        list.selection--; 
        if (list.selection < 0) { list.selection = list.buttons.size() - 1; }
    }
    if (platform.input.down.just_pressed) { 
        list.selection++; 
        if (list.selection >= list.buttons.size()) { list.selection = 0; }
    }

    // Draw menu items
    for(int i = 0; i < list.buttons.size(); ++i) {
        Vec3 color(0.8, 0.8, 0.8);
        if(i == list.selection) { color = Vec3(0.9, 0.2, 0.2); };
        platform.texts.emplace_back(PlatformText(
            list.buttons[i].text, 
            64,
            100,
            100 + (i * 64),
            color
        ));
    }
}

void update_main_menu(MainMenu& menu, std::vector<LevelData> levels, Platform& platform) {
    handle_menu_list(menu.list, platform);

    if(menu.state == MainMenuState::MAIN) {
        // Handle selection
        if (platform.input.jump.just_pressed) {
            switch (menu.list.selection) {
            case 0:
                menu.state = MainMenuState::LEVEL_SELECT;
                menu.list.selection = 0;
                populate_level_select_menu(levels, menu.list);
                break;
            case 1:
                platform.window_should_close = true;
                break;
            default:
                break;
            }
        }

        // Quit on escape
        if (platform.input.pause.just_pressed) { platform.window_should_close = true; }
    }
    else if(menu.state == MainMenuState::LEVEL_SELECT) {
        // Handle selection
        if (platform.input.jump.just_pressed) {
            menu.level_index_to_load = menu.list.selection;
            menu.ready_to_load_level = true;
        }

        // Back to menu on escape
        if (platform.input.pause.just_pressed) {
            menu.state = MainMenuState::MAIN;
            menu.list.selection = 0;
            populate_main_menu(menu.list);
        }
    }
}

void populate_main_menu(MenuList& list) {
    list.buttons.clear();
    list.buttons.emplace_back(MenuButton("Levels"));
    list.buttons.emplace_back(MenuButton("Quit"));
}

void populate_level_select_menu(std::vector<LevelData> levels, MenuList& list) {
    list.buttons.clear();
    for (LevelData& level : levels) {
        list.buttons.emplace_back(MenuButton(level.name.c_str()));
    }
}

