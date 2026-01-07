#include "chr.h"

#include "stdio.h"

#include "world.h"

/* Step this character like it's a dwarf. */
void _dwarf_step(struct chr* chr, struct world* world) {
    struct map_coords next_coords = chr->coords;
    next_coords.x += 1;
    if (map_is_walkable(&world->map, next_coords)) {
        chr->coords = next_coords;
    }
}

void _unimplemented_step(struct chr* chr) {
    printf("Unimplemented step for character kind %u!\n", chr->kind);
}

void chr_init(struct chr* chr) {
    chr->alive = false;
    chr->kind = CK_UNKNOWN;
    chr->name = "Unknowable Boye";
    chr->coords = (struct map_coords){ 0, 0, 0 };
}

void chr_step(struct chr* chr, struct world* world) {
    switch (chr->kind) {
        case CK_DWARF:
            _dwarf_step(chr, world);
            break;

        default:

            break;
    }
}