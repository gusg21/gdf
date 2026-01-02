#if !defined(KIND_LOAD_H)
#define KIND_LOAD_H

#include "map.h"
#include "render.h"

void load_tile_kinds(struct map* map);
void load_render_kinds(struct renderer* ren, struct map* map);

#endif // KIND_LOAD_H
