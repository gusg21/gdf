#if !defined(WORLD_GEN_H)
#define WORLD_GEN_H

#include "world.h"

struct world_gen_params {
    uint32_t seed;
};

void world_gen(struct world* world, struct world_gen_params params);

#endif // WORLD_GEN_H
