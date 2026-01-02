#include "render.h"

#include <assert.h>
#include <stdint.h>

#include "raymath.h"
#include "tomlc17.h"

#include "autotileset.h"
#include "tileset.h"

#include "map.h"
#include "vec2.h"
#include "world.h"

// clang-format off
static Color _depth_tint[] = { 
    WHITE, 
    (Color){ 100, 100, 100, 255 }, 
    (Color){ 50, 50, 50, 255 },
    (Color){ 25, 25, 25, 255 }
};
// clang-format on
#define MAX_RENDER_DEPTH sizeof(_depth_tint) / sizeof(typeof(_depth_tint[0]))

#define REDRAW_LOCAL_AREA_RADIUS 1  // 1 tile around the updated tile will get redrawn

void render_init(struct renderer* ren, struct map* map) {
    ren->max_z = MAP_SIZE / 2;

    ren->map_tex = LoadRenderTexture(map->size * RENDER_TILE_SIZE, map->size * RENDER_TILE_SIZE);

    ren->dirty = false;
}

void render_tile(struct renderer* ren, struct tile* tile, uint8_t floor_adjacency, uint8_t wall_adjacency,
                 uint32_t depth, struct vec2f world_pos) {
    struct render_tile_kind* wall_kind = &ren->render_kinds[tile->wall];
    struct render_tile_kind* floor_kind = &ren->render_kinds[tile->floor];

    if (floor_kind->has_tileset) {
        autotileset_draw(&floor_kind->tileset, floor_adjacency, _depth_tint[depth], world_pos);
    }
    if (wall_kind->has_tileset) {
        autotileset_draw(&wall_kind->tileset, wall_adjacency, _depth_tint[depth], world_pos);
    }
}

struct vec2f render_map_coords_to_world_pos(struct map_coords map_coords) {
    return (struct vec2f){ .x = map_coords.x * RENDER_TILE_SIZE, .y = map_coords.y * RENDER_TILE_SIZE };
}

struct map_coords render_world_pos_to_map_coords(struct renderer* ren, struct vec2f world_pos) {
    return (struct map_coords){ .x = world_pos.x / (float)RENDER_TILE_SIZE,
                                .y = world_pos.y / (float)RENDER_TILE_SIZE,
                                .z = ren->max_z };
}

/* Renders the area from min (inclusive) to max (exclusive). */
void render_chunk(struct renderer* ren, struct map* map, struct map_coords min, struct map_coords max) {
    int32_t max_render_depth = MAX_RENDER_DEPTH;

    for (int32_t z = min.z; z < max.z; z++) {
        if ((z <= ren->max_z) && (z > ren->max_z - max_render_depth)) {
            for (int32_t x = min.x; x < max.x; x++) {
                for (int32_t y = min.y; y < max.y; y++) {
                    struct map_coords map_coords = (struct map_coords){ x, y, z };

                    if (map_coords_in_bounds(map, map_coords)) {
                        struct tile* tile = map_get(map, map_coords);

                        uint8_t floor_adjacency = autotileset_get_adjacency_from_map(map, map_coords, true);
                        uint8_t wall_adjacency = autotileset_get_adjacency_from_map(map, map_coords, false);

                        render_tile(ren, tile, floor_adjacency, wall_adjacency, ren->max_z - z,
                                    render_map_coords_to_world_pos(map_coords));
                    }
                }
            }
        }
    }

    // Draw depths
    // for (int32_t x = min.x; x < max.x; x++) {
    //     for (int32_t y = min.y; y < max.y; y++) {
    //         struct map_coords map_coords = (struct map_coords){ x, y, ren->max_z };

    //         if (map_coords_in_bounds(map, map_coords)) {
    //             struct vec2f world_pos = render_map_coords_to_world_pos(map_coords);

    //             DrawText(TextFormat("%d", map_find_z_distance_to_solid(map, map_coords)),
    //                      world_pos.x + (RENDER_TILE_SIZE / 2), world_pos.y + (RENDER_TILE_SIZE / 2), 20, RED);
    //         }
    //     }
    // }
}

/* Render the visible z-slices of the map. This is a heavy function! */
void render_map(struct renderer* ren, struct map* map) {
    render_chunk(ren, map,
                 (struct map_coords){
                     0, 0, ren->max_z - MAX_RENDER_DEPTH - 1  //
                 },
                 (struct map_coords){
                     map->size, map->size, ren->max_z + 1  // Make sure we include the max_z slice
                 });
}

void _redraw_whole_map(struct renderer* ren, struct world* world) {
    BeginTextureMode(ren->map_tex);
    {
        ClearBackground(BLACK);
        render_map(ren, &world->map);
    }
    EndTextureMode();
}

void _redraw_local_area(struct renderer* ren, struct world* world, struct map_coords coords) {
    BeginTextureMode(ren->map_tex);
    {
        struct map_coords min = (struct map_coords){
            coords.x - REDRAW_LOCAL_AREA_RADIUS,
            coords.y - REDRAW_LOCAL_AREA_RADIUS,
            coords.z - REDRAW_LOCAL_AREA_RADIUS,
        };
        struct map_coords max = (struct map_coords){
            coords.x + REDRAW_LOCAL_AREA_RADIUS + 1,  // Maximum is exclusive, hence +1.
            coords.y + REDRAW_LOCAL_AREA_RADIUS + 1,
            coords.z + REDRAW_LOCAL_AREA_RADIUS + 1,
        };

        // Clear the area.
        struct vec2f min_world = render_map_coords_to_world_pos(min);
        struct vec2f max_world = render_map_coords_to_world_pos(max);
        DrawRectangle(min_world.x, min_world.y, max_world.x - min_world.x, max_world.y - min_world.y, BLACK);

        // Redraw.
        render_chunk(ren, &world->map, min, max);
    }
    EndTextureMode();
}

void render_prepare_world(struct renderer* ren, struct world* world) {
    // If the map isn't dirty (requiring a total redraw), then only redraw the updated areas.
    // Otherwise, redraw the whole map.
    if (world->map.dirty || ren->dirty) {
        printf("Map repaint (dirty).\n");
        _redraw_whole_map(ren, world);
        map_clear_events(&world->map);
        world->map.dirty = false;
        ren->dirty = false;
    }
    else {
        struct map_event evt;
        while (map_poll_event(&world->map, &evt)) {
            switch (evt.type) {
                case MAP_EVENT_CLEARED: {
                    printf("Map repaint.\n");
                    _redraw_whole_map(ren, world);
                    map_clear_events(&world->map);  // We don't need to update anything else if we redraw the whole map.
                    break;
                }

                case MAP_EVENT_TILE_CHANGED: {
                    printf("Tile changed.\n");
                    _redraw_local_area(ren, world, evt.u.tile_changed.coords);
                    break;
                }

                default:
                    assert(false && "Bad map event type!");
                    break;
            }
        }
    }
}

void render_world(struct renderer* ren) {
    Rectangle src = (Rectangle){
        0, 0, ren->map_tex.texture.width,
        -ren->map_tex.texture.height  // Flip the render texture!
    };
    Rectangle dst = (Rectangle){ 0, 0, ren->map_tex.texture.width, ren->map_tex.texture.height };
    DrawTexturePro(ren->map_tex.texture, src, dst, Vector2Zero(), 0.f, WHITE);
}