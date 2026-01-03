#include "sfx.h"

#include "tomlc17.h"

void sfx_load(struct sfx* sfx, const char* config_path) {
    toml_result_t result = toml_parse_file_ex(config_path);

    const char* base_path = toml_seek(result.toptab, "config.base_path").u.s;
    toml_datum_t sounds = toml_seek(result.toptab, "sounds");
    for (uint32_t i = 0; i < sounds.u.arr.size; i++) {
        toml_datum_t sound = sounds.u.arr.elem[i];
        struct sfx_entry new_entry;

        char* sound_path = toml_seek(sound, "path").u.s;
        char* sound_full_path = TextFormat("%s%s", base_path, sound_path);

        new_entry.music = false;
        new_entry.id_name = toml_seek(sound, "id_name").u.s;
        new_entry.data.sound = LoadSound(sound_full_path);
        new_entry.valid = true;
    }


}

void sfx_set_mute(struct sfx* sfx, bool muted) { SetMasterVolume(muted ? 0.f : 1.f); }

void sfx_add_entry(struct sfx* sfx, struct sfx_entry entry) {
    sfx->entries[]
}
