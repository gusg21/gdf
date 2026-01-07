#include "world_gen.h"

#include <stdbool.h>
#include <stdlib.h>

#define MAX_ROCK_KI_COUNT 16

static bool world_gen_is_spawn_friendly(struct map* map, struct map_coords coords) {
    return map_is_walkable(map, coords);
}

void world_gen(struct world* world, struct world_gen_params params) {
    struct map* map = &world->map;

    // Random noise for now.
    srand(params.seed);

    int32_t midlayer_z = MAP_SIZE / 2;
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

    // Generate solid underground.
    for (int32_t z = 0; z < MAP_SIZE / 2; z++) {
        for (int32_t x = 0; x < MAP_SIZE; x++) {
            for (int32_t y = 0; y < MAP_SIZE; y++) {
                struct map_coords coords = (struct map_coords){ x, y, z };

                uint32_t random_rock_ki = rock_kis[rand() % rock_ki_count];
                map_set(map, coords, (struct tile){ random_rock_ki, random_rock_ki });
            }
        }
    }

    // Generate randomized midlayer.
    for (int32_t x = 0; x < MAP_SIZE; x++) {
        for (int32_t y = 0; y < MAP_SIZE; y++) {
            struct map_coords coords = (struct map_coords){ x, y, midlayer_z };

            uint32_t random_rock_ki = rock_kis[rand() % rock_ki_count];

            if (rand() % 2 == 0) {
                map_set(map, coords, (struct tile){ empty_ki, random_rock_ki }); // Wall first, floor second.
            }
            else {
                map_set(map, coords, (struct tile){ random_rock_ki, random_rock_ki });
            }
        }
    }

    // Ignore the event list, just redraw the whole thing.
    map_clear_events(map);
    map->dirty = true;

    // Find a spot close to the center of the world.
    struct map_coords coords = (struct map_coords){ MAP_SIZE / 2, MAP_SIZE / 2, midlayer_z };
    while (!world_gen_is_spawn_friendly(map, coords)) {
        coords.x++;
    }
    
    struct chr chr;
    chr.alive = true;
    chr.coords = coords;
    chr.kind = CK_DWARF;
    chr.name = "Debuggicus";
    chrs_add_chr(&world->chrs, chr);
}