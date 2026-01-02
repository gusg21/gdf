#include "autotileset.h"

#include "tileset.h"

// This maps a given adjacency bitmap (01101001) to a tile index in the standard
// autotile tileset.
static uint32_t _adjacency_to_tile_index_map[] = {
    // clang-format off
    36, 36, 24, 24, 36, 36, 24, 24, 37, 37, 25, 25, 37, 37, 44, 44, 36, 36, 24, 24, 36, 36, 24, 24, 37, 37, 25, 25, 37, 37, 44, 44, 0, 0, 12, 12, 0, 0, 12, 12, 1, 1, 13, 13, 1, 1, 28, 28, 0, 0, 12, 12, 0, 0, 12, 12, 8, 8, 16, 16, 8, 8, 20, 20, 36, 36, 24, 24, 36, 36, 24, 24, 37, 37, 25, 25, 37, 37, 44, 44, 36, 36, 24, 24, 36, 36, 24, 24, 37, 37, 25, 25, 37, 37, 44, 44, 0, 0, 12, 12, 0, 0, 12, 12, 1, 1, 13, 13, 1, 1, 28, 28, 0, 0, 12, 12, 0, 0, 12, 12, 8, 8, 16, 16, 8, 8, 20, 20, 39, 39, 27, 47, 39, 39, 27, 47, 38, 38, 26, 42, 38, 38, 41, 45, 39, 39, 27, 47, 39, 39, 27, 47, 38, 38, 26, 42, 38, 38, 41, 45, 3, 3, 15, 31, 3, 3, 15, 31, 2, 2, 14, 4, 2, 2, 7, 46, 3, 3, 15, 31, 3, 3, 15, 31, 5, 5, 43, 34, 43, 5, 32, 29, 39, 39, 27, 47, 39, 39, 27, 47, 38, 38, 26, 42, 38, 38, 41, 45, 39, 39, 27, 47, 39, 39, 27, 47, 38, 38, 26, 42, 38, 38, 41, 45, 11, 11, 19, 35, 11, 11, 19, 35, 6, 6, 40, 23, 6, 6, 21, 30, 11, 11, 19, 35, 11, 11, 19, 35, 10, 10, 9, 18, 10, 10, 17, 33
    // clang-format on
};

uint8_t autotileset_get_adjacency_from_map(struct map* map, struct map_coords map_coords, bool check_floor) {
    uint32_t x = map_coords.x;
    uint32_t y = map_coords.y;
    uint32_t z = map_coords.z;

    // Adjacency bits start in the top left and spiral around, ending at the left bit.
    // 0 1 2
    // 7 x 3
    // 6 5 4
    if (check_floor) {
        return ((map_get_floor_kind(map, (struct map_coords){x - 1, y - 1, z}).solid ? 1 : 0) << 0) |
               ((map_get_floor_kind(map, (struct map_coords){x + 0, y - 1, z}).solid ? 1 : 0) << 1) |
               ((map_get_floor_kind(map, (struct map_coords){x + 1, y - 1, z}).solid ? 1 : 0) << 2) |
               ((map_get_floor_kind(map, (struct map_coords){x + 1, y + 0, z}).solid ? 1 : 0) << 3) |
               ((map_get_floor_kind(map, (struct map_coords){x + 1, y + 1, z}).solid ? 1 : 0) << 4) |
               ((map_get_floor_kind(map, (struct map_coords){x + 0, y + 1, z}).solid ? 1 : 0) << 5) |
               ((map_get_floor_kind(map, (struct map_coords){x - 1, y + 1, z}).solid ? 1 : 0) << 6) |
               ((map_get_floor_kind(map, (struct map_coords){x - 1, y + 0, z}).solid ? 1 : 0) << 7);
    } else {
        return ((map_get_wall_kind(map, (struct map_coords){x - 1, y - 1, z}).solid ? 1 : 0) << 0) |
               ((map_get_wall_kind(map, (struct map_coords){x + 0, y - 1, z}).solid ? 1 : 0) << 1) |
               ((map_get_wall_kind(map, (struct map_coords){x + 1, y - 1, z}).solid ? 1 : 0) << 2) |
               ((map_get_wall_kind(map, (struct map_coords){x + 1, y + 0, z}).solid ? 1 : 0) << 3) |
               ((map_get_wall_kind(map, (struct map_coords){x + 1, y + 1, z}).solid ? 1 : 0) << 4) |
               ((map_get_wall_kind(map, (struct map_coords){x + 0, y + 1, z}).solid ? 1 : 0) << 5) |
               ((map_get_wall_kind(map, (struct map_coords){x - 1, y + 1, z}).solid ? 1 : 0) << 6) |
               ((map_get_wall_kind(map, (struct map_coords){x - 1, y + 0, z}).solid ? 1 : 0) << 7);
    }
}

uint32_t autotileset_adjacency_to_tile_index(uint8_t adjacency) { return _adjacency_to_tile_index_map[adjacency]; }

void autotileset_decrement_adjacency(uint8_t adjacency) {
    if (_adjacency_to_tile_index_map[adjacency] != 0) {
        _adjacency_to_tile_index_map[adjacency] = (_adjacency_to_tile_index_map[adjacency] - 1);
    } else {
        _adjacency_to_tile_index_map[adjacency] = 47;
    }
}

void autotileset_increment_adjacency(uint8_t adjacency) {
    _adjacency_to_tile_index_map[adjacency] = (_adjacency_to_tile_index_map[adjacency] + 1) % 47;
}

const char* autotileset_get_map() {
    const char* output = "";
    for (int i = 0; i < 256; i++) {
        output = TextFormat("%s, %d", output, _adjacency_to_tile_index_map[i]);
    }
    return output;
}

void autotileset_draw(struct tileset* tileset, uint8_t adjacency, Color tint, struct vec2f world_pos) {
    uint32_t tile_index = autotileset_adjacency_to_tile_index(adjacency);
    tileset_draw(tileset, tile_index, tint, world_pos);
}