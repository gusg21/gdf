#if !defined(CHRS_H)
#define CHRS_H

#include "chr.h"

#define CHRS_MAX_COUNT 256
#define CHRS_KINDS_MAX_COUNT 64

struct chr_kind {
    /* The name of a single instance of this kind. */
    const char* name;
};

struct chrs {
    /* All the characters. */
    struct chr all[CHRS_MAX_COUNT];
    /* All character kinds, indexed by their id. */
    struct chr_kind kinds[CHRS_KINDS_MAX_COUNT];
};

struct world;

void chrs_init(struct chrs* chrs);
void chrs_step(struct chrs* chrs, struct world* world);
void chrs_add_kind(struct chrs* chrs, uint32_t kind_id, struct chr_kind kind);
void chrs_add_chr(struct chrs* chrs, struct chr chr);

#endif  // CHRS_H
