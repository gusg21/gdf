#include "world.h"

#include <stdlib.h>
#include <string.h>

void world_empty(struct world* world) {
    // Reset all the characters.
    memset(world->characters, 0, sizeof(struct character) * WORLD_MAX_CHARACTERS);

    // Reset the map.
    map_empty(&world->map);
}

void world_step(struct world* world) {
    for (uint32_t i = 0; i < WORLD_MAX_CHARACTERS; i++) {
        struct character* character = &world->characters[i];
        character_step(character);
    }
}
