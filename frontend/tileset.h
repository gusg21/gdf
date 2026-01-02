#if !defined(TILESET_H)
#define TILESET_H

#include <stdint.h>

#include "raylib.h"

#include "vec2.h"

struct tileset {
    /* The texture of the whole tileset. */
    Texture tex;
    /* This is one side length of a tile; tiles are always square. */
    uint32_t tile_size;
    /* The number of tiles wide the tileset is. This is calculated for you. */
    uint32_t tiles_wide;
};

void tileset_set_texture(struct tileset* tileset, Texture tex, uint32_t tile_size);
Rectangle tileset_get_rect(struct tileset* tileset, uint32_t tile_id);
void tileset_draw(struct tileset* tileset, uint32_t tile_id, Color tint, struct vec2f world_pos);

#endif // TILESET_H
