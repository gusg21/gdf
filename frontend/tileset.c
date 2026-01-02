#include "tileset.h"

#include <assert.h>

#include "raymath.h"

void tileset_set_texture(struct tileset* tileset, Texture tex, uint32_t tile_size) {
    assert(IsTextureValid(tex) && "Invalid tileset texture!");

    tileset->tex = tex;
    tileset->tile_size = tile_size;
    tileset->tiles_wide = (uint32_t)(tex.width / tile_size);
}

Rectangle tileset_get_rect(struct tileset* tileset, uint32_t tile_id) {
    assert(IsTextureValid(tileset->tex) && "Invalid tileset texture!");
    assert(tileset->tiles_wide > 0 && "Invalid tileset texture!");
    assert(tileset->tex.width > 0 && "Invalid tileset texture!");

    return (Rectangle){ .x = (tile_id % tileset->tiles_wide) * tileset->tile_size,
                        .y = (tile_id / tileset->tiles_wide) * tileset->tile_size,
                        .width = tileset->tile_size,
                        .height = tileset->tile_size };
}

void tileset_draw(struct tileset* tileset, uint32_t tile_id, Color tint, struct vec2f world_pos) {
    Rectangle src = tileset_get_rect(tileset, tile_id);
    // src.height = -src.height;
    Rectangle dst =
        (Rectangle){ .x = world_pos.x, .y = world_pos.y, .width = tileset->tile_size, .height = tileset->tile_size };

    // Vector2 origin = (Vector2){ .x = ((float)tileset->tile_size) / 2.f, .y = ((float)tileset->tile_size) / 2.f };
    DrawTexturePro(tileset->tex, src, dst, Vector2Zero(), 0.f, tint);
}
