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

// TODO: Hash function eventually for speed.
SurfaceMap get_surface_map(std::vector<Tile>& tiles) {
	SurfaceMap surface_map;
	std::vector<Surface>& surfaces = surface_map.surfaces;
	std::unordered_map<int, Surface*>& tile_surface_map = surface_map.tile_surface_map;
	std::vector<int> already_traversed_tiles;
	// Associate tiles with surfaces
	for(int i = 0; i < tiles.size(); ++i) {
		// Check if already traversed
		bool already_traversed = false;
		for(int j = 0; j < already_traversed_tiles.size(); ++j) {
			if(i == already_traversed_tiles[j]) {
				already_traversed = true;
				break;
			}
		}
		if(already_traversed) { continue; }

		// If not already traversed, carry on!
		surfaces.push_back(Surface());
		Surface* surface = &surfaces[surfaces.size() - 1];

		tile_surface_map.insert({i, surface});
		already_traversed_tiles.push_back(i);
		// Look left
		Vec2 check_position = tiles[i].position;
		// TODO: Initial fencepost necessitates function
		tile_surface_map.insert({i, surface});
		already_traversed_tiles.push_back(i);

		// TODO: Get rid of this later! ONLY FOR TEST RUN
		surface->left_edge_tile = i;
		int current_tile = i;
		bool found_tile = true;
		/*
		// While there is a tile at the checking position
		// TODO: These two while loops can be made a function easily.
		while(found_tile) {
			found_tile = false;
			check_position.x -= 16;
			for(int j = 0; j < tiles.size(); ++j) {
				// This should be a function too. Check if already traversed.
				bool already_traversed = false;
				for (int k = 0; k < already_traversed_tiles.size(); ++k) {
					if (j == already_traversed_tiles[k]) {
						already_traversed = true;
						break;
					}
				}
				if (already_traversed) { continue; }

				// Check if tile is at position
				if(tiles[j].position == check_position) {
					tile_surface_map.insert({ j, surface });
					already_traversed_tiles.push_back(j);
					current_tile = j;
					found_tile = true;
					break;
				}
			}
		}
		surface->left_edge_tile = current_tile */
		//check_position = Vec2(tiles[i].position.x + 16, tiles[i].position.y); // TODO: Hardcoded!
		while(found_tile) {
			found_tile = false;
			check_position.x += 16;
			for (int j = 0; j < tiles.size(); ++j) {
				// This should be a function too. Check if already traversed.
				bool already_traversed = false;
				for (int k = 0; k < already_traversed_tiles.size(); ++k) {
					if (j == already_traversed_tiles[k]) {
						already_traversed = true;
						break;
					}
				}
				if (already_traversed) { continue; }

				// Check if tile is at position
				if (tiles[j].position == check_position) {
					tile_surface_map.insert({ j, surface });
					already_traversed_tiles.push_back(j);
					current_tile = j;
					found_tile = true;
					break;
				}
			}
		}
		surface->right_edge_tile = current_tile;
	}
	return surface_map;
}

/* first attempt at hashed version
SurfaceMap get_surface_map(std::vector<Tile>& tiles) {
	SurfaceMap surface_map;
	std::vector<Surface>& surfaces = surface_map.surfaces;
	std::unordered_map<IVec2, Surface*, IVec2Hasher>& tile_surface_map = surface_map.tile_surface_map;
	std::unordered_set<IVec2, IVec2Hasher> tile_set;
	// Get all tile positions accounted for
	for (const Tile& tile : tiles) {
		tile_set.insert(ivec2_from_vec2(tile.position));
		//tile_surface_map.insert({ivec2_from_vec2(tile.position), nullptr});
	}

	// Associate tiles with surfaces
	for (const Tile& tile : tiles) {
		IVec2 tile_position = ivec2_from_vec2(tile.position);
		//if(tile_surface_map.count(tile_position) != 0) {
		if (tile_set.count(tile_position)) {
			continue;
		}
		surfaces.push_back(Surface());
		Surface* surface = &surfaces[surfaces.size() - 1];
		tile_surface_map.insert({ tile_position, surface });
		// Look left
		IVec2 checking_position(tile_position.x - 16, tile_position.y); // TODO: Not hardcode tile sizes?
		// While there is a tile at the checking position
		// TODO: These two while loops can be made a function easily.
		while (tile_surface_map.count(checking_position) != 0) {
			tile_surface_map[checking_position] = surface;
			checking_position.x -= 16;
		}
		checking_position.x += 16;
		surface->left_edge = checking_position;
		checking_position = IVec2(tile_position.x + 16, tile_position.y); // TODO: Hardcoded!
		while (tile_surface_map.count(checking_position) != 0) {
			tile_surface_map[checking_position] = surface;
			checking_position.x += 16;
		}
		checking_position.x -= 16;
		surface->right_edge = checking_position;
	}
	return surface_map;
}
*/
