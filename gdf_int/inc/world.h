#if !defined(WORLD_H)
#define WORLD_H

#include "character.h"
#include "map.h"

#define WORLD_MAX_CHARACTERS 2048

struct world {
    /* The map data. Very large! */
    struct map map;
    /* Character data. */
    struct character characters[WORLD_MAX_CHARACTERS];
};

void world_empty(struct world* world);
void world_step(struct world* world);

#endif // WORLD_H
