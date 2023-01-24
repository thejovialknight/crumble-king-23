#include "food.h"

void update_food(int& points, Food& food, King& king, Sounds& sounds, Platform& platform, Settings& settings, double delta_time) {
	const double expiration_length = settings.food_expiration_length;
	
	// Handle countdown
	food.time_to_next_phase -= delta_time;
	if(food.is_active) {
		if(food.time_to_next_phase <= 0) { 
			buffer_sound(platform, sounds.food_expired, 1);
			hide_food(food, settings);
		}
	}
	else {
		if(food.time_to_next_phase <= 0) {
			buffer_sound(platform, sounds.food_cooked, 1);
			food.time_to_next_phase = expiration_length;
			food.is_active = true;
			food.position = food.spawn_positions[random_int(food.spawn_positions.size())];
			food.animator.sequence = food.sequences[random_int(food.sequences.size())];
		}
	}

	// Handle king eating food
	const double distance_to_eat = 16;
	if(food.is_active && abs((king.position - food.position).magnitude()) <= distance_to_eat) {
		buffer_sound(platform, sounds.food_pickup, 1);
		hide_food(food, settings);
		points++;
		king.acceleration_mod *= 0.75;
	}

	iterate_animator(food.animator, delta_time);
};

void hide_food(Food& food, Settings& settings) {
	const double cook_length = settings.food_cook_length;
	food.time_to_next_phase = cook_length;
	food.is_active = false;
}
