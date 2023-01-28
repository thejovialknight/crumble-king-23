#pragma once
#include <vector>
#include "rect.h"
#include "king.h"

enum class EnemyState {
	PATROL,
	CHASE,
	PAUSE
};

struct Enemy {
	Vec2 position;
	Vec2 velocity;
	Animator animator;

	Enemy(Vec2 position) : position(position) {}
};

void update_enemies(std::vector<Enemy>& enemies, King& king, Sequences& sequences, double delta_time);
bool is_king_caught(std::vector<Enemy>& enemies, King& king);
