#if !defined(KIND_LOAD_H)
#define KIND_LOAD_H

#include "map.h"

#include "gdf_config.h"
#include "render.h"

void load_tile_kinds(struct map* map, struct gdf_config config);
void load_render_tile_kinds(struct renderer* ren, struct map* map, struct gdf_config config);
void load_chr_kinds(struct chrs* chrs, struct gdf_config config);
void load_render_chr_kinds(struct renderer* ren, struct gdf_config config);

#endif  // KIND_LOAD_H
