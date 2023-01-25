#pragma once
#include "vec2.h"
#include <vector>

#define COLUMNS 40
#define ROWS 23

struct Tile {
	int health = 3;
	Vec2 position;
	bool is_crumbling = false;
	double time_till_crumble = 0;

	Tile(Vec2 position) : position(position) {}
};

Vec2 grid_position_from_index(int i);
void put_tile(bool tiles[], int x, int y);
void update_tiles(std::vector<Tile>& tiles, double delta_time);
