#include "sfx.h"

#include <string.h>
#include <string.h>

#include "tomlc17.h"

#include "defs.h"

void sfx_init(struct sfx* sfx) {
    sfx->current_music = NULL;
    
    memset(sfx->entries, 0, sizeof(struct sfx_entry) * SFX_MAX_ENTRIES);
}

void sfx_load(struct sfx* sfx, const char* config_path) {
    toml_result_t result = toml_parse_file_ex(config_path);

    const char* base_path = toml_seek(result.toptab, "config.base_path").u.s;

    // Load sounds.
    {
        toml_datum_t sounds = toml_seek(result.toptab, "sounds");
        for (int32_t i = 0; i < sounds.u.arr.size; i++) {
            toml_datum_t sound = sounds.u.arr.elem[i];
            struct sfx_entry new_entry;

            const char* sound_path = toml_seek(sound, "path").u.s;
            const char* sound_full_path = TextFormat("%s%s", base_path, sound_path);

            new_entry.music = false;
            new_entry.id_name = toml_seek(sound, "id_name").u.s;
            new_entry.data.sound = LoadSound(sound_full_path);
            new_entry.valid = true;

            sfx_add_entry(sfx, new_entry);
        }
    }

    // Load music.
    {
        toml_datum_t music = toml_seek(result.toptab, "music");
        for (int32_t i = 0; i < music.u.arr.size; i++) {
            toml_datum_t music_datum = music.u.arr.elem[i];
            struct sfx_entry new_entry;

            const char* music_path = toml_seek(music_datum, "path").u.s;
            const char* music_full_path = TextFormat("%s%s", base_path, music_path);

            new_entry.music = true;
            new_entry.id_name = toml_seek(music_datum, "id_name").u.s;
            new_entry.data.music = LoadMusicStream(music_full_path);
            new_entry.valid = true;

            sfx_add_entry(sfx, new_entry);
        }
    }
}

void sfx_set_mute(struct sfx* sfx, bool muted) {
    GDF_UNUSED(sfx);
    SetMasterVolume(muted ? 0.f : 1.f);
}

void sfx_add_entry(struct sfx* sfx, struct sfx_entry new_entry) {
    for (uint32_t i = 0; i < SFX_MAX_ENTRIES; i++) {
        struct sfx_entry* entry = &sfx->entries[i];
        if (!entry->valid) {
            *entry = new_entry;
            return;
        }
    }
}

struct sfx_entry* sfx_find_entry_by_id_name(struct sfx* sfx, const char* id_name) {
    for (uint32_t i = 0; i < SFX_MAX_ENTRIES; i++) {
        struct sfx_entry* entry = &sfx->entries[i];
        if (strcmp(entry->id_name, id_name) == 0) {
            return entry;
        }
    }

    return NULL;  // Uh-oh!
}

void sfx_play_sound(struct sfx* sfx, const char* id_name) {
    struct sfx_entry* entry = sfx_find_entry_by_id_name(sfx, id_name);
    if (entry != NULL && !entry->music) {
        PlaySound(entry->data.sound);
    }
}

void sfx_play_music(struct sfx* sfx, const char* id_name) {
    struct sfx_entry* entry = sfx_find_entry_by_id_name(sfx, id_name);
    if (entry != NULL && entry->music) {
        // If any music is already playing, stop it.
        if (sfx->current_music != NULL) {
            StopMusicStream(sfx->current_music->data.music);
        }

        // Play the new music.
        PlayMusicStream(entry->data.music);
        sfx->current_music = entry;
    }
}

void sfx_update(struct sfx* sfx) {
    if (sfx->current_music != NULL) {
        UpdateMusicStream(sfx->current_music->data.music);
    }
}
