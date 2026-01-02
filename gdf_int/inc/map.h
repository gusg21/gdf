#if !defined(MAP_H)
#define MAP_H

#include "tile.h"
#include "vec2.h"

#define MAP_SIZE 256
#define MAP_TILE_COUNT MAP_SIZE * MAP_SIZE * MAP_SIZE  // 3D baybee!
#define MAP_MAX_TILE_KIND_COUNT 256

struct map_coords {
    int32_t x, y, z;
};

enum map_event_type {
    MAP_EVENT_CLEARED,
    MAP_EVENT_TILE_CHANGED,
};

struct map_event {
    /* The type of the event. */
    enum map_event_type type;

    /* The data union for the data this event might contain. */
    union {
        // (No data needed for MAP_EVENT_CLEARED.)

        struct {
            /* The coordinates the event happened at. */
            struct map_coords coords;
            /* The tile kind that was added. */
            struct tile_kind kind;
            /* Was the change on the floor? (Or if not, the wall)*/
            bool floor : 1;
        } tile_changed;
    } u;

    /* The next event in the list. Should be alright with a singly-linked list. */
    /* Hello future me nullifying the above assumption! */
    struct map_event* next;
};

struct map {
    /* All the tiles in the map. Read row by row, left to right. */
    struct tile tiles[MAP_TILE_COUNT];
    /* All the different kinds of tiles this map can contain. All tiles are indices into this list. */
    struct tile_kind kinds[MAP_MAX_TILE_KIND_COUNT];
    /* Side length of the square map */
    uint32_t size;
    /* The list of events that haven't been dealt with yet. */
    struct map_event* events;
    /* If the map is dirty, the whole thing needs a redraw. This circumvents having to iterate through a huge event
     * list.*/
    bool dirty;
    /* The kind index of the "Empty" kind (pre-generated.) */
    uint32_t empty_kind_index;
};

// Tile functions:
void map_empty(struct map* map);
bool map_coords_in_bounds(struct map* map, struct map_coords coords);
struct tile* map_get(struct map* map, struct map_coords coords);
void map_set(struct map* map, struct map_coords coords, struct tile tile);
void map_set_raw(struct map* map, struct map_coords coords, struct tile tile);
struct tile_kind map_get_wall_kind(struct map* map, struct map_coords coords);
struct tile_kind map_get_floor_kind(struct map* map, struct map_coords coords);
uint32_t map_find_z_distance_to_solid(struct map* map, struct map_coords coords);
bool map_is_solid(struct map* map, struct map_coords coords);

// Kind functions:
void map_reset_kinds(struct map* map);
void map_add_kind(struct map* map, struct tile_kind new_kind);
struct tile_kind map_get_kind(struct map* map, uint32_t kind_index);
uint32_t map_get_kind_index_from_id(struct map* map, const char* id);

// Event functions:
void map_push_event(struct map* map, struct map_event evt);
/* Returns true if `o_evt` was populated with a new event. */
bool map_poll_event(struct map* map, struct map_event* o_evt);
void map_clear_events(struct map* map);
uint32_t map_count_events(struct map* map);

#endif  // MAP_H
