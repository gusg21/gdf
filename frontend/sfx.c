#include "sfx.h"

void sfx_load(struct sfx* sfx) {
    sfx->stone_strike = LoadSound("resources/sounds/AbilityPlaced01.wav");
}

void sfx_stone_strike(struct sfx* sfx) {
    PlaySound(sfx->stone_strike);
}