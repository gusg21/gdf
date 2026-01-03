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
    const char* id_name;
    /* The data stored in this entry. */
    union {
        Sound sound;
        Music music;
    } data;
};

struct sfx {
    /* List of all possible sound effects and music. */
    struct sfx_entry entries[SFX_MAX_ENTRIES];
    /* The current entry playing as music. */
    struct sfx_entry* current_music;
};

void sfx_init(struct sfx* sfx);
void sfx_load(struct sfx* sfx, const char* config_path);
void sfx_set_mute(struct sfx* sfx, bool muted);
void sfx_add_entry(struct sfx* sfx, struct sfx_entry new_entry);
struct sfx_entry* sfx_find_entry_by_id_name(struct sfx* sfx, const char* id_name);
void sfx_play_sound(struct sfx* sfx, const char* id_name);
void sfx_play_music(struct sfx* sfx, const char* id_name);
void sfx_update(struct sfx* sfx);

#endif // SFX_H
