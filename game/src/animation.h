// Should we change sequences to a vector of sequences and have the Sequence struct have either an enum or GASP a string to reference the right one.
// Then, the king can just get the integer index into the vector on load by asking where in the vector is the string or whatnot.
// Might be better and more robust, in all honesty
//
// We have a problem with the pulling in animations from the file. The parsing is getting caught up.
// Debug that to see whats the problem. Breakpoints have already been set.
// 
// The next thing after that to do is get the food and king to control their current frame and whatnot so that the level renderer doesnt have to do it.
// In fact, there should be a draw_sprite function that takes a platform, sequence, sprite atlas, and current frame and does that work.
// Then the render_level() bit calls this function with data from the data.

#pragma once
#include <vector>
#include <string>
#include "vec2.h"
#include "rect.h"

struct Sequence {
	Vec2 origin = Vec2(0, 0);
	std::vector<IRect> frames;
};

struct Sequences {
	Sequence tile_island;
	Sequence tile_center;
	Sequence tile_left;
	Sequence tile_right;
	Sequence bubbling_pot;
	Sequence platter;
	Sequence food_potato;
	Sequence food_watermelon;
	Sequence food_grape;
	Sequence food_chicken;
	Sequence king_idle;
	Sequence king_run;
	Sequence king_jump;
	Sequence king_float;
	Sequence guard_idle;
	Sequence guard_run;
	Sequence guard_jump;
	Sequence guard_end;
};

void populate_sequences(const std::string text, Sequences& sequences);
bool try_iterate_past_char(const char c, const std::string& text, int& i);
std::string pull_string_before_char(const char c, const std::string& text, int& i);
