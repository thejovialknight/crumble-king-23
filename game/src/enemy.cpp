#include "enemy.h"

void update_enemies(std::vector<Enemy>& enemies, King& king, std::vector<Tile>& tiles, SurfaceMap& surface_map, Sequences& sequences, double delta_time) {
	const double patrol_speed = 4; // TODO: Settings!
	const double edge_tolerance = 0;
	const double edge_pause_length = 1.5;

	Surface* king_surface = surface_from_entity_position(king.position, tiles, surface_map);

	for(Enemy& enemy : enemies) {
		Surface* enemy_surface = surface_from_entity_position(enemy.position, tiles, surface_map);
		if(enemy_surface == nullptr) {
			enemy.state = EnemyState::LOST;
		}

		// PATROL STATE
		if(enemy.state == EnemyState::PATROL) {
			// Move if stopped (from PAUSE state)
			if(epsilon_equals(enemy.velocity.x, 0)) {
				enemy.velocity.x = patrol_speed * enemy.stored_x_direction;
			}
			
			// Check if at edges
			bool is_at_edge = false;
			if(enemy.velocity.x < 0 
			&& enemy.position.x < tiles[enemy_surface->left_edge_tile].position.x - edge_tolerance) {
				is_at_edge = true;
				enemy.stored_x_direction = 1;
			}
			else if(enemy.velocity.x > 0 
			&& enemy.position.x > tiles[enemy_surface->right_edge_tile].position.x + edge_tolerance) {
				is_at_edge = true;
				enemy.stored_x_direction = -1;
			}
			if(is_at_edge) {
				enemy.velocity.x = 0;
				enemy.state = EnemyState::PAUSE;
				enemy.time_to_unpause = edge_pause_length;
			}
		}
		else if(enemy.state == EnemyState::CHASE) {
			// CHASE LOGIC
		}
		else if(enemy.state == EnemyState::PAUSE) {
			enemy.time_to_unpause -= delta_time;
			if(enemy.time_to_unpause <= 0) {
				enemy.state = EnemyState::PATROL;
			}
		}

		enemy.position.x += enemy.velocity.x * delta_time;

		enemy.animator.sequence = &sequences.guard_idle;
		iterate_animator(enemy.animator, delta_time);
	}
}

bool is_king_caught(std::vector<Enemy>& enemies, King& king) {
	const double distance_to_catch = 12;

	for(Enemy& enemy : enemies) {
		if(abs((king.position - enemy.position).magnitude()) <= distance_to_catch) {
			return true;
		}
	}

	return false;
}

Surface* surface_from_entity_position(Vec2& position, std::vector<Tile>& tiles, SurfaceMap& surface_map) {
	const double x_tolerance = 12; // TODO: Settings
	const double y_tolerance = 8; // TODO: Settings

	Vec2 projected_position = Vec2(position.x, position.y + 16);
	for(int i = 0; i < tiles.size(); ++i) {
		if(epsilon_equals(projected_position.x, tiles[i].position.x, x_tolerance)
		&& epsilon_equals(projected_position.y, tiles[i].position.y, y_tolerance)) {
			return surface_map.tile_surface_map[i];
		}
	}

	return nullptr;
}
