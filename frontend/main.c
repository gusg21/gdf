#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"
#include "tomlc17.h"

#include "autotileset.h"
#include "world.h"
#include "world_gen.h"

#include "defs.h"
#include "gdf_config.h"
#include "kind_load.h"
#include "render.h"
#include "sfx.h"

#define FRAMES_PER_SECOND 60.f
#define FRAME_TIME (1.f / FRAMES_PER_SECOND)
#define STEP_FRAME_TIME (FRAME_TIME * 5.f)

int main(int argc, char* argv[]) {
    struct gdf_config config;
    gdf_config_init(&config);
    gdf_config_apply_file(&config, "resources/gdf_config.toml");
    gdf_config_apply_args(&config, argc, argv);

    InitWindow(1600, 900, "gus dwarves");
    InitAudioDevice();
    SetTargetFPS(60);

    // Create world.
    struct world* world = malloc(sizeof(struct world));
    world_empty(world);

    // Set up renderer.
    struct renderer ren;
    render_init(&ren, &world->map);

    // Set up camera.
    Camera2D cam = (Camera2D){ .offset = (Vector2){ 1600 / 2, 900 / 2 },  //
                               .rotation = 0.f,
                               .target = (Vector2){ world->map.size / 2 * RENDER_TILE_SIZE,
                                                    world->map.size / 2 * RENDER_TILE_SIZE },
                               .zoom = 2.f };
    Vector2 cam_velocity = Vector2Zero();

    // Load the tile kinds into the world map.
    load_tile_kinds(&world->map, config);

    // Load the render kinds based of the map's tile kinds.
    load_render_tile_kinds(&ren, &world->map, config);

    // Load the chr kinds.
    load_chr_kinds(&world->chrs, config);

    // Load the render information for the chr kinds.
    load_render_chr_kinds(&ren, config);

    // Generate world.
    world_gen(world, (struct world_gen_params){ .seed = time(NULL) });

    // Load our SFX.
    struct sfx sfx;
    sfx_init(&sfx);
    sfx_load(&sfx, config.sfx_config_path);
    // sfx_play_music(&sfx, "sift");
    sfx_set_mute(&sfx, config.mute_audio);

    // Timer for stepping.
    float time_since_step = 0.f;

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
            if (map_get_kind(&world->map, map_get(&world->map, mouse_map_coords)->wall).solid) {
                map_set_wall_kind(&world->map, mouse_map_coords, world->map.empty_kind_index);
                sfx_play_sound(&sfx, "stone_strike");
            }
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

        // Update:
        sfx_update(&sfx);

        // Step if we should.
        time_since_step += GetFrameTime();
        if (time_since_step > STEP_FRAME_TIME) {
            time_since_step = 0.f;
            world_step(world);
        }

        // Prepare the renderer.
        render_prepare_world(&ren, world);

        // Drawing:
        BeginDrawing();
        {
            // World-space drawing:
            BeginMode2D(cam);
            {
                // Clear the screen.
                ClearBackground(BLACK);
                
                // Render!
                render_world(&ren, world);

                // Origin marker.
                DrawLineEx(Vector2Zero(), (Vector2){ RENDER_TILE_SIZE, 0 }, 5, RED);
                DrawLineEx(Vector2Zero(), (Vector2){ 0, RENDER_TILE_SIZE }, 5, GREEN);

                // Draw the hovered tile.
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

    CloseAudioDevice();
    CloseWindow();

    return 0;
}