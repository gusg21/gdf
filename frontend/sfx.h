#if !defined(SFX_H)
#define SFX_H

#include "raylib.h"

#define SFX_MAX_ENTRIES 64

struct sfx_entry {
    /* Is this entry valid? */
    bool valid : 1;
    /* True = music, false = sound. */
    bool music : 1;
    /* The unique name of the sound effect. */
    char* id_name;
    /* The data stored in this entry. */
    union {
        Sound sound;
        Music music;
    } data;
};

struct sfx {
    struct sfx_entry entries[SFX_MAX_ENTRIES];
};

void sfx_load(struct sfx* sfx, const char* config_path);
void sfx_set_mute(struct sfx* sfx, bool muted);
void sfx_add_entry(struct sfx* sfx, struct sfx_entry entry);

#endif // SFX_H
