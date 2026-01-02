#include "kind_load.h"

#include <assert.h>

#include "tomlc17.h"

void load_tile_kinds(struct map* map) {
    toml_result_t result = toml_parse_file_ex("resources/data/tile_kinds.toml");
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

void load_render_kinds(struct renderer* ren, struct map* map) {
    toml_result_t result = toml_parse_file_ex("resources/data/render_kinds.toml");
    assert(result.ok);

    const char* base_path = toml_seek(result.toptab, "config.base_path").u.s;

    toml_datum_t kinds = toml_seek(result.toptab, "render_kinds");
    for (int32_t i = 0; i < kinds.u.arr.size; i++) {
        toml_datum_t kind_datum = kinds.u.arr.elem[i];

        // Find the id name and convert that to a kind index.
        const char* kind_id_name = toml_seek(kind_datum, "id_name").u.s;
        uint32_t kind_index = map_get_kind_index_from_id(map, kind_id_name);

        // Fill out the render kind at the same index.
        struct render_tile_kind* render_kind = &ren->render_kinds[kind_index];

        // Load and add the tileset, if we need it.
        if (toml_seek(kind_datum, "tileset").type != TOML_UNKNOWN) {
            render_kind->has_tileset = true;

            const char* tileset_path = TextFormat("%s%s", base_path, toml_seek(kind_datum, "tileset").u.s);
            tileset_set_texture(&render_kind->tileset, LoadTexture(tileset_path), RENDER_TILE_SIZE);
        } else {
            render_kind->has_tileset = false;
        }
    }
}