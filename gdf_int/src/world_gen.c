#include "world_gen.h"

#include <stdlib.h>

#define MAX_ROCK_KI_COUNT 16

void world_gen(struct world* world, struct world_gen_params params) {
    struct map* map = &world->map;

    // Random noise for now.
    srand(params.seed);

    uint32_t empty_ki = map_get_kind_index_from_id(map, "empty");

    // Find viable rock [k]ind [i]ndices.
    uint32_t rock_kis[MAX_ROCK_KI_COUNT];
    uint32_t rock_ki_count = 0;
    for (uint32_t kind_index = 0; kind_index < MAP_MAX_TILE_KIND_COUNT; kind_index++) {
        struct tile_kind kind = map_get_kind(map, kind_index);
        if (kind.valid && kind.rock) {
            rock_kis[rock_ki_count] = kind_index;
            rock_ki_count++;

            if (rock_ki_count >= MAX_ROCK_KI_COUNT) {
                continue;
            }
        }
    }

    for (uint32_t z = 0; z <= MAP_SIZE / 2; z++) {
        for (uint32_t x = 0; x < MAP_SIZE; x++) {
            for (uint32_t y = 0; y < MAP_SIZE; y++) {
                struct map_coords coords = (struct map_coords){ x, y, z };

                if (rand() % 2 == 0) {
                    map_set(map, coords, (struct tile){ empty_ki, empty_ki });
                }
                else {
                    uint32_t random_rock_ki = rock_kis[rand() % rock_ki_count];
                    map_set(map, coords, (struct tile){ random_rock_ki, random_rock_ki });
                }
            }
        }
    }

    // Ignore the event list, just redraw the whole thing.
    map_clear_events(map);
    map->dirty = true;
}