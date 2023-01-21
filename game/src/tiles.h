#pragma once
#include "vec2.h"

#define COLUMNS 40
#define ROWS 23

Vec2 tile_position_from_index(bool tiles[], int i);
void put_tile(bool tiles[], int x, int y);
