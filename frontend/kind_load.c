#include "kind_load.h"

#include <assert.h>

#include "tomlc17.h"

#include "defs.h"

void load_tile_kinds(struct map* map, struct gdf_config config) {
    toml_result_t result = toml_parse_file_ex(config.tile_kinds_config_path);
    assert(result.ok);

    // Load the tile kind information from each array item.
    toml_datum_t kinds = toml_seek(result.toptab, "tile_kinds");
    for (int32_t i = 0; i < kinds.u.arr.size; i++) {
        toml_datum_t kind_datum = kinds.u.arr.elem[i];

        struct tile_kind kind;
        kind.solid = toml_seek(kind_datum, "solid").u.boolean;
        kind.rock = toml_seek(kind_datum, "rock").u.boolean;
        kind.id_name = toml_seek(kind_datum, "id_name").u.s;
        kind.display_name = toml_seek(kind_datum, "display_name").u.s;

        map_add_kind(map, kind);
    }
}

void load_render_tile_kinds(struct renderer* ren, struct map* map, struct gdf_config config) {
    toml_result_t result = toml_parse_file_ex(config.render_tile_kinds_config_path);
    assert(result.ok);

    const char* base_path = toml_seek(result.toptab, "config.base_path").u.s;

    toml_datum_t kinds = toml_seek(result.toptab, "render_kinds");
    for (int32_t i = 0; i < kinds.u.arr.size; i++) {
        toml_datum_t kind_datum = kinds.u.arr.elem[i];

        // Find the id name and convert that to a kind index.
        const char* kind_id_name = toml_seek(kind_datum, "id_name").u.s;
        uint32_t kind_index = map_get_kind_index_from_id(map, kind_id_name);

        // Fill out the render kind at the same index.
        struct render_tile_kind* render_kind = &ren->render_tile_kinds[kind_index];

        // Load and add the tilesets, if we have both.
        toml_datum_t wall_datum = toml_seek(kind_datum, "wall_tileset");
        toml_datum_t floor_datum = toml_seek(kind_datum, "floor_tileset");
        if (wall_datum.type != TOML_UNKNOWN && floor_datum.type != TOML_UNKNOWN) {
            render_kind->has_tileset = true;

            {
                const char* wall_tileset_path = TextFormat("%s%s", base_path, wall_datum.u.s);
                tileset_set_texture_from_file(&render_kind->wall_tileset, wall_tileset_path, RENDER_TILE_SIZE);
            }

            {
                const char* floor_tileset_path = TextFormat("%s%s", base_path, floor_datum.u.s);
                tileset_set_texture_from_file(&render_kind->floor_tileset, floor_tileset_path, RENDER_TILE_SIZE);
            }
        }
        else {
            render_kind->has_tileset = false;
        }
    }
}

void load_chr_kinds(struct chrs* chrs, struct gdf_config config) {
    toml_result_t result = toml_parse_file_ex(config.render_tile_kinds_config_path);
    assert(result.ok);

    toml_datum_t kinds = toml_seek(result.toptab, "chr_kinds");
    for (int32_t i = 0; i < kinds.u.arr.size; i++) {
        toml_datum_t kind_datum = kinds.u.arr.elem[i];
        struct chr_kind kind;

        int64_t kind_id = toml_seek(kind_datum, "id").u.int64;
        const char* name = toml_seek(kind_datum, "name").u.s;

        kind.name = name;

        chrs_add_kind(chrs, kind_id, kind);
    }
}

void load_render_chr_kinds(struct renderer* ren, struct gdf_config config) {
    toml_result_t result = toml_parse_file_ex(config.render_chr_kinds_config_path);
    assert(result.ok);

    const char* base_path = toml_seek(result.toptab, "config.base_path").u.s;

    toml_datum_t kinds = toml_seek(result.toptab, "render_chr_kinds");
    for (int32_t i = 0; i < kinds.u.arr.size; i++) {
        toml_datum_t kind_datum = kinds.u.arr.elem[i];
        struct render_chr_kind kind;

        uint32_t kind_id = toml_seek(kind_datum, "id").u.int64;
        kind.tex = LoadTexture(TextFormat("%s%s", base_path, toml_seek(kind_datum, "texture").u.s));

        ren->render_chr_kinds[kind_id] = kind;
    }
}
