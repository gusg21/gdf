#if !defined(RENDER_H)
#define RENDER_H

#include <stdbool.h>

#include "raylib.h"

#include "world.h"

#include "tileset.h"

#define RENDER_TILE_SIZE 32

// Stores the renderer specific information for a tile kind.
struct render_tile_kind {
    /* Does this tile kind have a tileset? */
    bool has_tileset;
    /* All the tiles that could be drawn for this tile. */
    struct tileset tileset;
};

struct renderer {
    /* All the info for rendering tile kinds. */
    struct render_tile_kind render_kinds[MAP_MAX_TILE_KIND_COUNT]; // Same count as the map
    /* The current top layer of the map. */
    int32_t max_z;
    /* The latest version of the map. When the map becomes dirty, this gets re-rendered. See render_world() */
    RenderTexture map_tex;
    /* Has the information about the renderer changed in a way that requires a redraw? */
    bool dirty;
};

void render_init(struct renderer* ren, struct map* map);
void load_render_kinds(struct renderer* ren, struct map* map);
void render_chunk(struct renderer* ren, struct map* map, struct map_coords min, struct map_coords max);
void render_prepare_world(struct renderer* ren, struct world* world);
void render_world(struct renderer* ren);
struct map_coords render_world_pos_to_map_coords(struct renderer* ren, struct vec2f world_pos);
struct vec2f render_map_coords_to_world_pos(struct map_coords map_coords);

void _redraw_local_area(struct renderer* ren, struct world* world, struct map_coords coords); //! DEBUG

#endif // RENDER_H
