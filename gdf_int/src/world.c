#include "world.h"

#include <stdlib.h>
#include <string.h>

void world_empty(struct world* world) {
    chrs_init(&world->chrs);

    // Reset the map.
    map_empty(&world->map);
}

void world_step(struct world* world) {
    chrs_step(&world->chrs, world);
}
