#if !defined(WORLD_H)
#define WORLD_H

#include "chrs.h"
#include "map.h"

struct world {
    /* The map data. Very large! */
    struct map map;
    /* Character data. */
    struct chrs chrs;
};

void world_empty(struct world* world);
void world_step(struct world* world);

#endif // WORLD_H
