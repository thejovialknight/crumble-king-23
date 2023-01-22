#pragma once
#include <vector>
#include "level.h"

struct Tower {
	Level level;
	std::vector<LevelData*> levels;
	int level_index = 0;
	int lives_remaining = 3; // TODO: Settings
};
