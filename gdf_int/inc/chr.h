#if !defined(CHARACTER_H)
#define CHARACTER_H

#include <stdbool.h>

#include "map.h"

#define CHARACTER_NAME_MAX_LENGTH 128

enum chr_kind_id : uint32_t {
    CK_UNKNOWN,
    CK_DWARF,
    CK_RABBIT
};

struct chr {
    /* Do we exist? */
    bool alive;
    /* Map position */
    struct map_coords coords;
    /* What are we? */    
    enum chr_kind_id kind;
    /* Name */
    const char* name;
};

struct world;

void chr_init(struct chr* chr);
void chr_step(struct chr* chr, struct world* world);

#endif // CHARACTER_H
