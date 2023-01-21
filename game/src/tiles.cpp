#include "tiles.h"

Vec2 tile_position_from_index(bool tiles[], int i) {
    Vec2 position;
    int grid_y = (i / COLUMNS);
    position.y = grid_y * 16;
    position.x = (i - (grid_y * COLUMNS)) * 16;
    return position;
}

void put_tile(bool tiles[], int x, int y) {
    tiles[y * COLUMNS + x] = true;
}
