#pragma once
#include "vec2.h"
#include "platform.h"
#include "settings.h"
#include "utils.h"

enum class JumpState {
    GROUND,
    JUMP,
    FLOAT
};

struct King {
    JumpState jump_state = JumpState::GROUND;
    Vec2 position = Vec2(0, 0);
    Vec2 velocity = Vec2(0, 0);
    double jump_buffer = 0;
    double coyote_time = 0;
    double gravity_scale = 1;
    bool is_grounded = false;
    bool is_facing_right = true;

    // List indices
    int current_frame = 0;
    int idle_sequence = 0;
    int run_sequence = 0;
};

void update_king(King& king, Platform& platform, Settings& settings, double delta_time);
bool is_king_dead(King& king, Platform& platform);
