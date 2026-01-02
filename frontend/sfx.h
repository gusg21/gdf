#if !defined(SFX_H)
#define SFX_H

#include "raylib.h"

struct sfx {
    Sound stone_strike;
};

void sfx_load(struct sfx* sfx);
void sfx_stone_strike(struct sfx* sfx);

#endif // SFX_H
