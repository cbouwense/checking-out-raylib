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
GameScreenType current_screen = GUPPY;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screen_width = 600;
static const int screen_height = 600;

static uint64_t frames_counter = 0;

static int square_position_x = screen_width/2;
static int square_position_y = screen_height/2;

static int square_speed_x = 3;
static int square_speed_y = 4;

static int square_side_length = 16;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreenType transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void update_and_draw_game_frame(void);

int main(void)
{
    InitWindow(screen_width, screen_height, "Checking out raylib");
    
    InitAudioDevice();

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    // music = LoadMusicStream("resources/ambient.ogg");
    // fxCoin = LoadSound("resources/coin.wav");

    // SetMusicVolume(music, 0.0f);
    // PlayMusicStream(music);

    // Setup and init first screen
    current_screen = GUPPY;

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_and_draw_game_frame, 60, 1);
#else
    SetTargetFPS(144);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        update_and_draw_game_frame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // Unload global data loaded
    UnloadFont(font);
    // UnloadMusicStream(music);1
    // UnloadSound(fxCoin);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
static void update_and_draw_game_frame(void) {
    // Update
    frames_counter++;

    if ((square_position_x + square_side_length) > screen_width || square_position_x < 0) square_speed_x *= -1;
    if ((square_position_y + square_side_length) > screen_height || square_position_y < 0) square_speed_y *= -1;

    square_position_x += square_speed_x;
    square_position_y += square_speed_y;

    // Draw
    BeginDrawing();
        DrawRectangle(square_position_x, square_position_y, 16, 16, BLACK);
        // Draw text displaying the frames_counter in the top left corner
        DrawText(TextFormat("Frames: %i", frames_counter), 10, 10, 20, LIME);
        ClearBackground(RAYWHITE);
    EndDrawing();
}
