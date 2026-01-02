#if !defined(TILE_H)
#define TILE_H

#include <stdint.h>
#include <stdbool.h>

struct tile_kind {
    /* Is this a real tile kind? */
    bool valid : 1;
    /* Is this tile considered solid? */
    bool solid : 1;
    /* Is this tile a rock? */
    bool rock : 1;
    /* The unique "ID name" of this tile kind. */
    const char* id_name;
    /* The display name of this tile kind. */
    const char* display_name;
};

struct tile {
    /* Represents an index into the map's tile_kind list. */
    uint32_t wall;
    /* Represents an index into the map's tile_kind list. */
    uint32_t floor;
};

#endif // TILE_H
