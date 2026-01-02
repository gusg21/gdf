#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "autotileset.h"
#include "world.h"
#include "world_gen.h"

#include "defs.h"
#include "kind_load.h"
#include "render.h"

int main(int argc, char* argv[]) {
    GDF_UNUSED(argc);
    GDF_UNUSED(argv);

    InitWindow(1600, 900, "gus dwarves");

    // Create world.
    struct world* world = malloc(sizeof(struct world));
    world_empty(world);

    // Set up renderer.
    struct renderer ren;
    render_init(&ren, &world->map);

    // Set up camera.
    Camera2D cam = (Camera2D){ .offset = (Vector2){ 1600 / 2, 900 / 2 },  //
                               .rotation = 0.f,
                               .target = Vector2Zero(),
                               .zoom = 2.f };
    Vector2 cam_velocity = Vector2Zero();

    // Load the tile kinds into the world map.
    load_tile_kinds(&world->map);

    // Load the render kinds based of the map's tile kinds.
    load_render_kinds(&ren, &world->map);

    // Generate world.
    world_gen(world, (struct world_gen_params){ .seed = time(NULL) });

    while (!WindowShouldClose()) {
        // Input:
        Vector2 rlib_mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), cam);
        struct vec2f mouse_world_pos = (struct vec2f){ rlib_mouse_world_pos.x, rlib_mouse_world_pos.y };
        struct map_coords mouse_map_coords = render_world_pos_to_map_coords(&ren, mouse_world_pos);

        // Camera x/y motion.
        cam_velocity = Vector2Zero();
        if (IsKeyDown(KEY_LEFT)) cam_velocity.x = -1;
        if (IsKeyDown(KEY_RIGHT)) cam_velocity.x = 1;
        if (IsKeyDown(KEY_UP)) cam_velocity.y = -1;
        if (IsKeyDown(KEY_DOWN)) cam_velocity.y = 1;
        cam.target = Vector2Add(cam.target, Vector2Scale(cam_velocity, GetFrameTime() * 200.0f));

        // Zooming and vertical motion.
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            cam.zoom += GetMouseWheelMove();
        }
        else {
            ren.max_z += GetMouseWheelMove();
            if (GetMouseWheelMove() != 0) {
                ren.dirty = true;
            }
        }

        // Map editing.
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            map_set(&world->map, mouse_map_coords,
                    (struct tile){ .floor = world->map.empty_kind_index, .wall = world->map.empty_kind_index });
        }
        if (IsKeyPressed(KEY_Z)) {
            map_set(&world->map, (struct map_coords){ 1, 1, ren.max_z },
                    (struct tile){ .floor = world->map.empty_kind_index, .wall = world->map.empty_kind_index });
            _redraw_local_area(&ren, world, (struct map_coords){ 1, 1, ren.max_z });
        }

        // Autotiling editing.
        if (IsKeyPressed(KEY_A)) {
            uint8_t adjacency = autotileset_get_adjacency_from_map(&world->map, mouse_map_coords, false);
            autotileset_increment_adjacency(adjacency);
            _redraw_local_area(&ren, world, mouse_map_coords);
        }
        if (IsKeyPressed(KEY_D)) {
            uint8_t adjacency = autotileset_get_adjacency_from_map(&world->map, mouse_map_coords, false);
            autotileset_decrement_adjacency(adjacency);
            _redraw_local_area(&ren, world, mouse_map_coords);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            SaveFileText("autotileset_data.txt", autotileset_get_map());
        }

        // Prepare the renderer:
        render_prepare_world(&ren, world);

        // Drawing:
        BeginDrawing();
        {
            // World-space drawing:
            BeginMode2D(cam);
            {
                ClearBackground(BLACK);

                render_world(&ren);

                DrawLineEx(Vector2Zero(), (Vector2){ 32, 0 }, 5, RED);
                DrawLineEx(Vector2Zero(), (Vector2){ 0, 32 }, 5, GREEN);

                struct vec2f cursor_pos = render_map_coords_to_world_pos(mouse_map_coords);
                DrawRectangleLines(cursor_pos.x, cursor_pos.y, RENDER_TILE_SIZE, RENDER_TILE_SIZE, RED);
            }
            EndMode2D();

            // Screen-space drawing:
            DrawText(TextFormat("Camera: %.2f, %.2f", cam.target.x, cam.target.y), 10, 10, 20, DARKGRAY);
            DrawFPS(10, 30);
            DrawText(TextFormat("%u Map Events", map_count_events(&world->map)), 10, 50, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}