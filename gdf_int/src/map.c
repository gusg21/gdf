#include "map.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Empty magic tile for out of bounds access.
static struct tile _magic_tile = { .wall = 0, .floor = 0 };

void map_empty(struct map* map) {
    map->size = MAP_SIZE;  // Fixed map sizes for now.

    // Reset the tiles.
    memset(map->tiles, 0, sizeof(struct tile) * map->size * map->size);

    // Default tile kinds.
    map_reset_kinds(map);

    // Clear events.
    map_clear_events(map);

    // Add clear event.
    map_push_event(map, (struct map_event){ .type = MAP_EVENT_CLEARED });
}

bool map_coords_in_bounds(struct map* map, struct map_coords coords) {
    return !(coords.x < 0 || coords.x >= (int32_t)map->size || coords.y < 0 || coords.y >= (int32_t)map->size ||
             coords.z < 0 || coords.z >= (int32_t)map->size);
}

struct tile* map_get(struct map* map, struct map_coords coords) {
    if (!map_coords_in_bounds(map, coords)) {
        return &_magic_tile;
    }

    return &map->tiles[(coords.z * map->size * map->size) + (coords.y * map->size) + coords.x];
}

void map_reset_kinds(struct map* map) {
    // Clear kinds.
    memset(map->kinds, 0, sizeof(struct tile_kind) * MAP_MAX_TILE_KIND_COUNT);

    // Add "Empty" kind.
    map_add_kind(map, (struct tile_kind){ .id_name = "empty", .display_name = "Empty", .solid = false });
}

struct tile_kind map_get_wall_kind(struct map* map, struct map_coords coords) {
    return map_get_kind(map, map_get(map, coords)->wall);
}

struct tile_kind map_get_floor_kind(struct map* map, struct map_coords coords) {
    return map_get_kind(map, map_get(map, coords)->floor);
}

uint32_t map_find_z_distance_to_solid(struct map* map, struct map_coords coords) {
    uint32_t dist = 0;
    struct map_coords pointer = coords;
    while (!map_is_solid(map, pointer)) {
        pointer.z--;
        dist++;

        if (pointer.z <= 0) break;
    }
    return dist;
}

bool map_is_solid(struct map* map, struct map_coords coords) { 
    struct tile* tile = map_get(map, coords);
    struct tile_kind wall_tk = map_get_kind(map, tile->wall);
    struct tile_kind floor_tk = map_get_kind(map, tile->floor);
    
    return wall_tk.solid || floor_tk.solid;
 }

void map_set(struct map* map, struct map_coords coords, struct tile tile) {
    map_set_raw(map, coords, tile);

    // Push the notification events.
    map_push_event(map, (struct map_event){
                            .type = MAP_EVENT_TILE_CHANGED,
                            .u = { .tile_changed = { .coords = coords, .floor = true, .kind = map_get_kind(map, tile.floor) } } });
    map_push_event(map, (struct map_event){
                            .type = MAP_EVENT_TILE_CHANGED,
                            .u = { .tile_changed = { .coords = coords, .floor = false, .kind = map_get_kind(map, tile.wall) } } });
}

void map_set_raw(struct map* map, struct map_coords coords, struct tile tile) {
    struct tile* tile_ptr = map_get(map, coords);
    *tile_ptr = tile;
}

void map_add_kind(struct map* map, struct tile_kind new_kind) {
    for (uint32_t kind_index = 0; kind_index < MAP_MAX_TILE_KIND_COUNT; kind_index++) {
        struct tile_kind* kind = &map->kinds[kind_index];
        if (!kind->valid) {
            *kind = new_kind;    // Copy in the data
            kind->valid = true;  // Ensure the valid flag is set
            return;
        }
    }

    assert(false && "Ran out of slots for tile kinds!");
}

struct tile_kind map_get_kind(struct map* map, uint32_t kind_index) { return map->kinds[kind_index]; }

uint32_t map_get_kind_index_from_id(struct map* map, const char* id) {
    for (uint32_t kind_index = 0; kind_index < MAP_MAX_TILE_KIND_COUNT; kind_index++) {
        struct tile_kind* kind = &map->kinds[kind_index];
        if (kind->valid) {
            if (strcmp(id, kind->id_name) == 0) {
                return kind_index;
            }
        }
    }

    return 0;
}

void map_push_event(struct map* map, struct map_event evt) {
    struct map_event* new_evt = malloc(sizeof(struct map_event));
    *new_evt = evt;

    new_evt->next = map->events;  // Grab the old head as our next
    map->events = new_evt;        // Make ourselves the new head
}

bool map_poll_event(struct map* map, struct map_event* o_evt) {
    if (map->events != NULL) {
        struct map_event* popped_evt = map->events;
        struct map_event* next_evt = popped_evt->next;
        popped_evt->next = NULL;
        map->events = next_evt;

        *o_evt = *popped_evt;  // Output the event we just popped off

        free(popped_evt);  // We copied the data; the original can be freed.

        return true;
    }

    return false;
}

void map_clear_events(struct map* map) {
    struct map_event* evt = map->events;
    while (evt != NULL) {
        struct map_event* dead_evt = evt;
        evt = evt->next;
        free(dead_evt);
    }

    map->events = NULL;
}

uint32_t map_count_events(struct map* map) {
    uint32_t count = 0;
    struct map_event* evt = map->events;
    while (evt != NULL) {
        count++;
        evt = evt->next;
    }
    return count;
}
