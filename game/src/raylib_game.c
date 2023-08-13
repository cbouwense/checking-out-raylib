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
static const int screen_width  = 600;
static const int screen_height = 600;

static uint64_t frames_counter = 0;

const int square_1_mass = 1;
const int square_2_mass = 1;
// const float gravitational_constant = 6.67408e-11;
const float gravitational_constant = 100;

// TODO: Maybe have a struct for these
static float square_1_position_x = (screen_width/2)  - 128;
static float square_1_position_y = (screen_height/2) - 128;
static float square_2_position_x = (screen_width/2)  + 128;
static float square_2_position_y = (screen_height/2) + 128;

static int square_side_length = 16;

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
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
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
static void update_game_state() {
    frames_counter++;

    float square_1_distance_x = square_2_position_x - square_1_position_x;
    float square_1_distance_y = square_2_position_y - square_1_position_y;
    float square_2_distance_x = square_1_position_x - square_2_position_x;
    float square_2_distance_y = square_1_position_y - square_2_position_y;

    float square_1_gravitational_force = gravitational_constant * square_1_mass * square_2_mass / (pow(square_1_distance_x, 2) + pow(square_1_distance_y, 2));
    float square_2_gravitational_force = gravitational_constant * square_1_mass * square_2_mass / (pow(square_2_distance_x, 2) + pow(square_2_distance_y, 2));

    float square_1_velocity_x = square_1_gravitational_force * square_1_distance_x;
    float square_1_velocity_y = square_1_gravitational_force * square_1_distance_y;
    float square_2_velocity_x = square_2_gravitational_force * square_2_distance_x;
    float square_2_velocity_y = square_2_gravitational_force * square_2_distance_y;

    square_1_position_x += square_1_velocity_x;
    square_1_position_y += square_1_velocity_y;
    square_2_position_x += square_2_velocity_x;
    square_2_position_y += square_2_velocity_y;
}

static void draw_game_frame() {
    // Draw
    BeginDrawing();
        DrawCircle(square_1_position_x, square_1_position_y, 4.0f, BLACK);
        DrawCircle(square_2_position_x, square_2_position_y, 4.0f, BLACK);
        // Draw text displaying the frames_counter in the top left corner
        DrawText(TextFormat("Frames: %i", frames_counter), 10, 10, 20, LIME);
        ClearBackground(RAYWHITE);
    EndDrawing();
}
