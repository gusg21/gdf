#if !defined(CHARACTER_H)
#define CHARACTER_H

#include <stdbool.h>

#include "vec2.h"

#define CHARACTER_NAME_MAX_LENGTH 128

enum character_kind : uint32_t {
    CK_DWARF,
    CK_RABBIT
};

struct character {
    /* Do we exist? */
    bool alive;
    /* Grid position */
    struct vec2i pos;
    /* What are we? */    
    enum character_kind kind;
    /* Name */
    char name[CHARACTER_NAME_MAX_LENGTH];
};

void character_step(struct character* character);

#endif // CHARACTER_H
