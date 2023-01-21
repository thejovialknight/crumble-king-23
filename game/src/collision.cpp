#include "collision.h"

bool AABB(Rect a, Rect b) {
	if (a.position.x < b.position.x + b.size.x &&
		a.position.x + a.size.x > b.position.x &&
		a.position.y < b.position.y + b.size.y &&
		a.position.y + a.size.y > b.position.y)
	{
		return true;
	}

	return false;
}

void resolve_king_velocity(King& king, bool tiles[]) {
	Rect player_collider(Vec2(king.position.x, king.position.y), Vec2(16, 16));
	king.is_grounded = false;

	// Loop through collision checks
	for (int i = 0; i < ROWS * COLUMNS; ++i) {
		if (!tiles[i]) {
			continue;
		}

		Vec2 tile_position = tile_position_from_index(tiles, i);
		Rect tile_collider(Vec2(tile_position.x, tile_position.y), Vec2(16, 16));

		Rect player_after_x_movement = player_collider;
		player_after_x_movement.position.x += king.velocity.x;
		Rect player_after_y_movement = player_collider;
		player_after_y_movement.position.y += king.velocity.y;
		Rect ground_check_rect = player_collider;
		ground_check_rect.position.y += 0.75;

		if(AABB(player_after_x_movement, tile_collider)) {
			king.velocity.x = 0;
		}

		if(AABB(player_after_y_movement, tile_collider)) {
			king.velocity.y = 0;
		}

		if(king.velocity.y >= 0 && AABB(ground_check_rect, tile_collider)) {
			king.is_grounded = true;
		}
	}

	king.position.x += king.velocity.x;
	king.position.y += king.velocity.y;
}
