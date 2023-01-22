#include "animation.h"

void iterate_animator(Animator& animator, double delta_time) {
	animator.time_till_next_frame -= delta_time;
	if (animator.time_till_next_frame <= 0) {
		animator.time_till_next_frame = animator.frame_length;
		animator.frame++;
	}

	// Loop iteration outside of first if statement in case animation is changed to one with less frames
	if (animator.frame >= animator.sequence->frames.size()) {
		animator.frame = 0;
	}
}

PlatformSprite sprite_from_animator(int atlas_texture, Animator& animator, const Vec2& position) {
	return sprite_from_sequence(atlas_texture, *animator.sequence, animator.frame, position, animator.is_flipped);
}

PlatformSprite sprite_from_sequence(int atlas_texture, const Sequence& sequence, int frame, const Vec2& position, bool is_flipped) {
	return PlatformSprite(
		atlas_texture,
		sequence.frames[frame],
		(int)position.x,
		(int)position.y,
		(int)sequence.origin.x,
		(int)sequence.origin.y,
		is_flipped
	);
}

void populate_sequences(const std::string text, Sequences& sequences) {
	for(int i = 0; i < text.length(); ++i) {
		if(!try_iterate_past_char('@', text, i)) break;
		std::string sequence_name = pull_string_before_char(',', text, i);
		Sequence sequence;
		int width = std::stod(pull_string_before_char(',', text, i));
		int height = std::stod(pull_string_before_char(',', text, i));
		sequence.origin.x = std::stod(pull_string_before_char(',', text, i));
		sequence.origin.y = std::stod(pull_string_before_char(':', text, i));
		while(text[i] != '\n' && text[i] != '\0') {
			int pos_x = pull_int_before_char(',', text, i);
			int pos_y = pull_int_before_char(';', text, i);
			sequence.frames.emplace_back(IRect(
				pos_x,
				pos_y,
				width,
				height
			));
		}
		
		if(sequence_name == "TILE_ISLAND") { sequences.tile_island = sequence; }
		if(sequence_name == "TILE_CENTER") { sequences.tile_center = sequence; }
		if(sequence_name == "TILE_LEFT") { sequences.tile_left = sequence; }
		if(sequence_name == "TILE_RIGHT") { sequences.tile_right = sequence; }
		if(sequence_name == "BUBBLING_POT") { sequences.bubbling_pot = sequence; }
		if(sequence_name == "PLATTER") { sequences.platter = sequence; }
		if(sequence_name == "FOOD_POTATO") { sequences.food_potato = sequence; }
		if(sequence_name == "FOOD_WATERMELON") { sequences.food_watermelon = sequence; }
		if(sequence_name == "FOOD_GRAPE") { sequences.food_grape = sequence; }
		if(sequence_name == "FOOD_CHICKEN") { sequences.food_chicken = sequence; }
		if(sequence_name == "KING_IDLE") { sequences.king_idle = sequence; }
		if(sequence_name == "KING_RUN") { sequences.king_run = sequence; }
		if(sequence_name == "KING_JUMP") { sequences.king_jump = sequence; }
		if(sequence_name == "KING_FLOAT") { sequences.king_float = sequence; }
		if(sequence_name == "GUARD_IDLE") { sequences.guard_idle = sequence; }
		if(sequence_name == "GUARD_RUN") { sequences.guard_idle = sequence; }
		if(sequence_name == "GUARD_JUMP") { sequences.guard_run = sequence; }
		if(sequence_name == "GUARD_END") { sequences.guard_end = sequence; }
	}
}

// Iterates i to the position after the specified character index
// Returns bool if reached the end of file first
bool try_iterate_past_char(const char c, const std::string& text, int& i) {
	while(text[i] != c) { 
		i++;
		if(i < text.length()) {
			return false;
		}
	}
	i++;
	return true;
}

// Returns the string from i to the index of the specified character - 1
// Iterates i to the position after the specified character index
std::string pull_string_before_char(const char c, const std::string& text, int& i) {
	std::string data_str;
	while(text[i] != c) { 
		data_str += text[i];
		i++;
	}
	i++;
	return data_str;
}

int pull_int_before_char(const char c, const std::string& text, int& i) {
	return std::stoi(pull_string_before_char(c, text, i));
}
