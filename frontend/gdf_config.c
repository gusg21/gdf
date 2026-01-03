#include "gdf_config.h"

#include <string.h>

#include "tomlc17.h"

void gdf_config_init(struct gdf_config* config) {
    // Set up default values for a gdf_config.
    config->mute_audio = false;
    config->sfx_config_path = "resources/sfx.toml";
    config->gameplay_music_id_name = "sift";
}

/* Load data from a config file into the config struct. */
void gdf_config_apply_file(struct gdf_config* config, const char* path) {
    toml_result_t result = toml_parse_file_ex(path);

    config->mute_audio = toml_seek(result.toptab, "config.mute_audio").u.boolean;
    config->sfx_config_path = toml_seek(result.toptab, "config.sfx_config_path").u.s;
    config->gameplay_music_id_name = toml_seek(result.toptab, "config.gameplay_music_id_name").u.s;
}

void gdf_config_apply_args(struct gdf_config* config, int argc, char* argv[]) {
    for (int32_t i = 0; i < argc; i++) {
        char* arg = argv[i];
        if (strcmp(arg, "--mute-audio") == 0) {
            config->mute_audio = true;
        }
    }
}