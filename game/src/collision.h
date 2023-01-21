#pragma once
#include "rect.h"
#include "tiles.h"
#include "king.h"

// Returns whether two rects are overlapping (colliding).
bool AABB(Rect a, Rect b);
void resolve_king_velocity(King& king, bool tiles[]);
