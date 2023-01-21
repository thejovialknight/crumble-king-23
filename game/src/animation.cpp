#include "animation.h"

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
			sequence.frames.emplace_back(IRect(
				std::stoi(pull_string_before_char(',', text, i)),
				std::stoi(pull_string_before_char(';', text, i)),
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
