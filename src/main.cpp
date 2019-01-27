#include <cstdlib>
#include "raylib.h"
#include "Sprite.h"
#include "Game.h"

// screen splash
Texture2D menu_bck;
Texture2D start;
Vector2 position;
Rectangle frameRec;
int currentFrame;
int framesCounter;
int framesSpeed; 
bool is_splash;


int GetRandom(int min, int max)
{
    return min + (std::rand() % (max - min + 1));
}

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 320;
    int screenHeight = 240;

    Game game(screenWidth, screenHeight);

    menu_bck = LoadTexture("../assets/menu.png");
    start = LoadTexture("../assets/start.png");
    position = { 18.0f, 170.0f };
    frameRec = { 0.0f, 0.0f, (float)start.width/4, (float)start.height };
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6; 
    is_splash = true;



    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Screen splash
        //----------------------------------------------------------------------------------
        //
        if (is_splash)
        {
			framesCounter++;
			if (framesCounter >= (60/framesSpeed))
			{
				framesCounter = 0;
				currentFrame++;
				
				if (currentFrame > 5)
                    currentFrame = 0;
				
				frameRec.x = (float)currentFrame*(float)start.width/4;
			}
			if (IsKeyPressed(KEY_ENTER))
                is_splash = false;
            if (IsGamepadAvailable(GAMEPAD_PLAYER1))
            {
                if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 10))
//                IsGamepadButtonPressed()
                {
                    is_splash = false;
                }
            }
			BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawTexture(menu_bck, 0, 0, WHITE);
				DrawTextureRec(start, frameRec, position, WHITE);  // Draw part of the texture
			EndDrawing();
		}
        else
        {
            // Update
            //----------------------------------------------------------------------------------
            //
            game.Update();
            //----------------------------------------------------------------------------------

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);

            game.Draw();
            EndDrawing();
            //----------------------------------------------------------------------------------
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}