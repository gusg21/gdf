#include "gdf_config.h"

#include <string.h>

#include "tomlc17.h"

#include "defs.h"

void gdf_config_init(struct gdf_config* config) {
    // Set up default values for a gdf_config.
    config->mute_audio = false;
    config->sfx_config_path = "resources/sfx.toml";
    config->render_tile_kinds_config_path = "resources/render_tile_kinds.toml";
    config->tile_kinds_config_path = "resources/tile_kinds.toml";
    config->render_chr_kinds_config_path = "resources/render_chr_kinds.toml";
    config->chr_kinds_config_path = "resources/chr_kinds.toml";
    config->gameplay_music_id_name = "sift";
}

/* Load data from a config file into the config struct. */
void gdf_config_apply_file(struct gdf_config* config, const char* path) {
    toml_result_t result = toml_parse_file_ex(path);

    TRY_BOOLEAN("config.mute_audio", config->mute_audio);
    TRY_STRING("config.sfx_config_path", config->sfx_config_path);
    TRY_STRING("config.render_tile_kinds_config_path", config->render_tile_kinds_config_path);
    TRY_STRING("config.tile_kinds_config_path", config->tile_kinds_config_path);
    TRY_STRING("config.render_chr_kinds_config_path", config->render_chr_kinds_config_path);
    TRY_STRING("config.chr_kinds_config_path", config->chr_kinds_config_path);
    TRY_STRING("config.gameplay_music_id_name", config->gameplay_music_id_name);
}

void gdf_config_apply_args(struct gdf_config* config, int argc, char* argv[]) {
    for (int32_t i = 0; i < argc; i++) {
        char* arg = argv[i];
        if (strcmp(arg, "--mute-audio") == 0) {
            config->mute_audio = true;
        }
    }
}