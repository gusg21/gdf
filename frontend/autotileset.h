#if !defined(AUTOTILESET_H)
#define AUTOTILESET_H

#include <stdbool.h>
#include <stdint.h>

#include "map.h"
#include "vec2.h"

#include "tileset.h"

uint8_t autotileset_get_adjacency_from_map(struct map* map, struct map_coords map_coords, bool check_floor);
uint32_t autotileset_adjacency_to_tile_index(uint8_t adjacency);
void autotileset_decrement_adjacency(uint8_t adjacency);
void autotileset_increment_adjacency(uint8_t adjacency);
const char* autotileset_get_map();
void autotileset_draw(struct tileset* tileset, uint8_t adjacency, Color tint, struct vec2f world_pos);

#endif // AUTOTILESET_H
