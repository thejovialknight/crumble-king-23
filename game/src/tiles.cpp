#include "tiles.h"

Vec2 grid_position_from_index(int i) {
    Vec2 position;
    int grid_y = (i / COLUMNS);
    position.y = grid_y * 16;
    position.x = (i - (grid_y * COLUMNS)) * 16;
    return position;
}

void put_tile(bool tiles[], int x, int y) {
    tiles[y * COLUMNS + x] = true;
}

void update_tiles(std::vector<Tile>& tiles, double delta_time) {
    for(Tile& tile : tiles) {
        if(tile.is_crumbling) {
            tile.time_till_crumble -= delta_time;
            if(tile.time_till_crumble <= 0) {
                tile.health--;
                tile.is_crumbling = false;
            }
        }
    }
}
