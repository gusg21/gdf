#if !defined(GDF_CONFIG_H)
#define GDF_CONFIG_H

#include <stdbool.h>

struct gdf_config {
    /* Whether the audio should be muted when gdf starts. */
    bool mute_audio;
    /* The path to the sfx TOML. */
    const char* sfx_config_path;
    /* The gameplay background music. TODO: probably needs to be changed later. */
    const char* gameplay_music_id_name;
};

void gdf_config_init(struct gdf_config* config);
void gdf_config_apply_file(struct gdf_config* config, const char* path);
void gdf_config_apply_args(struct gdf_config* config, int argc, char* argv[]);

#endif // GDF_CONFIG_H
