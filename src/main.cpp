/*******************************************************************************************
*
*   raylib - sample game: asteroids survival
*
*   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <math.h>
#include <vector>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define PLAYER_BASE_SIZE    15.0f
#define PLAYER_SPEED        6.0f
#define PLAYER_MAX_SHOOTS   10

#define METEORS_SPEED       1
#define MAX_MEDIUM_METEORS  0
#define MAX_SMALL_METEORS   0


#define XBOX360_NAME_ID     "Xbox 360 Controller"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

struct Player {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Vector3 collider;
    Color color;
};

struct Meteor {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
    Color color;
};

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static int screenWidth = 320;
static int screenHeight = 240;

static int framesCounter;
static bool gameOver;
static bool pause;

// NOTE: Defined triangle is isosceles with common angles of 70 degrees.
static float shipHeight;

static Player player;
static Meteor mediumMeteor[MAX_MEDIUM_METEORS];
static Meteor smallMeteor[MAX_SMALL_METEORS];

Texture2D playerSprite;
Rectangle sourceRec;
Rectangle destRec;
Vector2 origin;
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "sample game: asteroids survival");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    int posx, posy;
    int velx, vely;
    bool correctRange = false;

    pause = false;

    playerSprite = LoadTexture("../assets/jugador.png");
    sourceRec = {0.0f, 0.0f, (float)playerSprite.width, (float)playerSprite.height};


    origin = {(float)playerSprite.width/2, (float)playerSprite.height/2};

    framesCounter = 0;

    shipHeight = (PLAYER_BASE_SIZE/2)/tanf(20*DEG2RAD);

    // Initialization player
    player.position = (Vector2){screenWidth/2, screenHeight/2};
    player.speed = (Vector2){0, 0};
    player.acceleration = 0;
    player.rotation = 0;
    player.collider = (Vector3){player.position.x + sinf(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cosf(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};
    player.color = LIGHTGRAY;

    for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
    {
        posx = GetRandomValue(0, screenWidth);

        while(!correctRange)
        {
            if (posx > screenWidth/2 - 80 && posx < screenWidth/2 + 80) posx = GetRandomValue(0, screenWidth);
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, screenHeight);

        while(!correctRange)
        {
            if (posy > screenHeight/2 - 60 && posy < screenHeight/2 + 60)  posy = GetRandomValue(0, screenHeight);
            else correctRange = true;
        }

        correctRange = false;
        velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

        while(!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
                vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
            }
            else correctRange = true;
        }
        mediumMeteor[i].position = (Vector2){posx, posy};
        mediumMeteor[i].speed = (Vector2){velx, vely};
        mediumMeteor[i].radius = 16;
        mediumMeteor[i].active = true;
        mediumMeteor[i].color = GREEN;
    }

    for (int i = 0; i < MAX_SMALL_METEORS; i++)
    {
        posx = GetRandomValue(0, screenWidth);

        while(!correctRange)
        {
            if (posx > screenWidth/2 - 80 && posx < screenWidth/2 + 80) posx = GetRandomValue(0, screenWidth);
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, screenHeight);

        while(!correctRange)
        {
            if (posy > screenHeight/2 - 60 && posy < screenHeight/2 + 60)  posy = GetRandomValue(0, screenHeight);
            else correctRange = true;
        }

        correctRange = false;
        velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

        while(!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
                vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
            }
            else correctRange = true;
        }
        smallMeteor[i].position = (Vector2){posx, posy};
        smallMeteor[i].speed = (Vector2){velx, vely};
        smallMeteor[i].radius = 8;
        smallMeteor[i].active = true;
        smallMeteor[i].color = YELLOW;
    }
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            framesCounter++;

            // Player logic

            // Rotation
            if (IsKeyDown(KEY_LEFT)) player.rotation -= 5;
            if (IsKeyDown(KEY_RIGHT)) player.rotation += 5;

            // Joystick rotation
            if (IsGamepadAvailable(GAMEPAD_PLAYER1))
            {
                float size = GetGamepadAxisMovement(GAMEPAD_PLAYER1, 0);

                if(IsGamepadButtonDown(GAMEPAD_PLAYER1, 18)) player.rotation -= 5;
                if(IsGamepadButtonDown(GAMEPAD_PLAYER1, 16)) player.rotation += 5;

                // Controller
                if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 0))
                {
                    if (player.acceleration < 1) player.acceleration += 0.04f;
                }
                else
                {
                    if (player.acceleration > 0) player.acceleration -= 0.02f;
                    else if (player.acceleration < 0) player.acceleration = 0;
                }

            }


            // Speed
            player.speed.x = sinf(player.rotation*DEG2RAD)*PLAYER_SPEED;
            player.speed.y = cosf(player.rotation*DEG2RAD)*PLAYER_SPEED;

            // Controller
            if (IsKeyDown(KEY_UP))
            {
                if (player.acceleration < 1) player.acceleration += 0.04f;
            }
            else
            {
                if (player.acceleration > 0) player.acceleration -= 0.02f;
                else if (player.acceleration < 0) player.acceleration = 0;
            }
//            if (IsKeyDown(KEY_DOWN))
//            {
//                if (player.acceleration > 0) player.acceleration -= 0.04f;
//                else if (player.acceleration < 0) player.acceleration = 0;
//            }

            // Movement
            player.position.x += (player.speed.x * player.acceleration);
            player.position.y -= (player.speed.y * player.acceleration);

            // Wall behaviour for player
            if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
            else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
            if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
            else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

            // Collision Player to meteors
            player.collider = (Vector3){player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};

            for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active) gameOver = true;
            }

            for (int a = 0; a < MAX_SMALL_METEORS; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active) gameOver = true;
            }

            // Meteor logic

            for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
            {
                if (mediumMeteor[i].active)
                {
                    // movement
                    mediumMeteor[i].position.x += mediumMeteor[i].speed.x;
                    mediumMeteor[i].position.y += mediumMeteor[i].speed.y;

                    // wall behaviour
                    if  (mediumMeteor[i].position.x > screenWidth + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = screenWidth + mediumMeteor[i].radius;
                    if (mediumMeteor[i].position.y > screenHeight + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = screenHeight + mediumMeteor[i].radius;
                }
            }

            for (int i = 0; i < MAX_SMALL_METEORS; i++)
            {
                if (smallMeteor[i].active)
                {
                    // movement
                    smallMeteor[i].position.x += smallMeteor[i].speed.x;
                    smallMeteor[i].position.y += smallMeteor[i].speed.y;

                    // wall behaviour
                    if  (smallMeteor[i].position.x > screenWidth + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = screenWidth + smallMeteor[i].radius;
                    if (smallMeteor[i].position.y > screenHeight + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = screenHeight + smallMeteor[i].radius;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }

        // Joystick rotation
        if (IsGamepadAvailable(GAMEPAD_PLAYER1))
        {
            if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 7))
            {
                InitGame();
                gameOver = false;
            }
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        // Draw spaceship
        Vector2 v1 = { player.position.x + sinf(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cosf(player.rotation*DEG2RAD)*(shipHeight) };
        Vector2 v2 = { player.position.x - cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
        Vector2 v3 = { player.position.x + cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y + sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
        //
//        DrawTriangle(v1, v2, v3, MAROON);
        // draw ship
        destRec = {player.position.x,// - playerSprite.width/2,
                   player.position.y,// - playerSprite.height/2,
                   (float)playerSprite.width,
                   (float)playerSprite.height};


        DrawTexturePro(playerSprite, sourceRec, destRec, origin, (float)player.rotation, WHITE);
        // Draw meteor
        for (int i = 0;i < MAX_MEDIUM_METEORS; i++)
        {
            if (mediumMeteor[i].active) DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, GRAY);
            else DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
        }

        for (int i = 0;i < MAX_SMALL_METEORS; i++)
        {
            if (smallMeteor[i].active) DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, DARKGRAY);
            else DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
        }

        DrawText(FormatText("TIME: %.02f", (float)framesCounter/60), 10, 10, 20, BLACK);

        if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN",
                  GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 10)/2,
                  GetScreenHeight()/2,
                  10,
                  GRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}