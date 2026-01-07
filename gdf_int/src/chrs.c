#include "chrs.h"

#include <string.h>

void chrs_init(struct chrs* chrs) {
    // Initialize all characters.
    for (uint32_t i = 0; i < CHRS_MAX_COUNT; i++) {
        struct chr* chr = &chrs->all[i];
        chr_init(chr);
    }

    // Clear kinds.
    memset(chrs->kinds, 0, sizeof(struct chr_kind) * CHRS_KINDS_MAX_COUNT);
}

void chrs_step(struct chrs* chrs, struct world* world) {
    // Step all characters.
    for (uint32_t i = 0; i < CHRS_MAX_COUNT; i++) {
        struct chr* chr = &chrs->all[i];
        chr_step(chr, world);
    }
}

void chrs_add_kind(struct chrs* chrs, uint32_t kind_id, struct chr_kind kind) {
    chrs->kinds[kind_id] = kind;
}

void chrs_add_chr(struct chrs* chrs, struct chr chr) {
    for (uint32_t i = 0; i < CHRS_MAX_COUNT; i++) {
        struct chr* chr_slot = &chrs->all[i];
        if (!chr_slot->alive) {
            *chr_slot = chr;
            return;
        }
    }
}
