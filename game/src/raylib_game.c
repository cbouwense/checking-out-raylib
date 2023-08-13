/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screen_width  = 1000;
static const int screen_height = 1000;

static uint64_t frames_counter = 0;

const int circle_1_mass = 1;
const int circle_2_mass = 1;

const float G = 200;

// TODO: Maybe have a struct for these
static float circle_1_position_x = (screen_width/2)  - 256;
static float circle_1_position_y = (screen_height/2) - 256;
static float circle_2_position_x = (screen_width/2)  + 256;
static float circle_2_position_y = (screen_height/2) + 256;

static int square_side_length = 16;
static float circle_radius = 128.0f;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void update_game_state();
static void draw_game_frame();

int main()
{
    InitWindow(screen_width, screen_height, "Checking out raylib");
    
    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_and_draw_game_frame, 60, 1);
#else
    SetTargetFPS(144);

    // Main game loop
    while (!WindowShouldClose())
    {
        frames_counter++;
        update_game_state();
        draw_game_frame();
    }
#endif

    // De-Initialization

    // Unload global data loaded
    UnloadFont(font);
    // UnloadMusicStream(music);1
    // UnloadSound(fxCoin);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

//--------------------------------------------------------------------------------------------------
// Module specific Functions Definition
//--------------------------------------------------------------------------------------------------

// TODO: I need to implement momentum as well, will probably need a notion of forces instead of just manual velocity.
static void update_game_state() {
    // TODO: Obviously, this should be DRY'd up. I just wanna make sure this makes sense first.
    float x_diff = abs(circle_2_position_x - circle_1_position_x);
    float y_diff = abs(circle_2_position_y - circle_1_position_y);

    float gravitational_force_between_the_circles = G * circle_1_mass * circle_2_mass / pow(x_diff + y_diff, 2);

    float circle_1_velocity_x = (gravitational_force_between_the_circles * x_diff);
    float circle_1_velocity_y = (gravitational_force_between_the_circles * y_diff);
    float circle_2_velocity_x = (gravitational_force_between_the_circles * -x_diff);
    float circle_2_velocity_y = (gravitational_force_between_the_circles * -y_diff);

    // Close enough ya nammsayin.
    const float sqrt_of_8 = 2.82842712474619f;

    // Figure out if (radius_a + radius_b)^2 = x_diff^2 + y_diff^2
    const float circle_1_circle_2_collision_distance = abs(2 * circle_radius);

    // TODO: this looks very inefficient.
    const float circle_origins_diff = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
    const bool are_squares_colliding = circle_origins_diff <= circle_1_circle_2_collision_distance;

    printf("are_squares_colliding: %d\n", are_squares_colliding);

    if (!are_squares_colliding) {
        circle_1_position_x += circle_1_velocity_x;
        circle_1_position_y += circle_1_velocity_y;
        circle_2_position_x += circle_2_velocity_x;
        circle_2_position_y += circle_2_velocity_y;
    }
}

static void draw_game_frame() {
    // Draw
    BeginDrawing();
        DrawCircle(circle_1_position_x, circle_1_position_y, circle_radius, BLACK);
        DrawCircle(circle_2_position_x, circle_2_position_y, circle_radius, BLACK);

        DrawText(TextFormat("Frames: %i", frames_counter), 10, 10, 20, LIME);

        ClearBackground(RAYWHITE);
    EndDrawing();
}
