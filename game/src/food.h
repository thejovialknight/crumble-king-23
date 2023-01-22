#pragma once
#include <vector>
#include "random.h"
#include "king.h"

struct Food {
	Vec2 position = Vec2(0,0);
	double is_active = false;
	double time_to_next_phase = 0;
	Animator animator;
	std::vector<Vec2> spawn_positions;
	std::vector<Sequence*> sequences;
};

void update_food(int& points, Food& food, King& king, Platform& platform, Settings& settings, double delta_time);
void hide_food(Food& food, Settings& settings);
