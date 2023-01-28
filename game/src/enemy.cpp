#include "enemy.h"

void update_enemies(std::vector<Enemy>& enemies, King& king, Sequences& sequences, double delta_time) {
	for(Enemy& enemy : enemies) {
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

