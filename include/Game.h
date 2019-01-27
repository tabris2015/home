//
// Created by pepe on 27-01-19.
//

#ifndef HOME_GAME_H
#define HOME_GAME_H

#include "raylib.h"

#include <vector>
#include <iostream>
#include "Sprite.h"

#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
class Game
{
private:
    int width_;
    int height_;
    bool pause_;
    unsigned long frames_counter_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<Wall> walls_;
    Camera2D camera_;
    Rectangle buildings[100];
    Color buildColors[100];
    Rectangle tiles[5];
    Color tilesColors[5];
    
public:
    bool game_over_;
    bool victory_;
    bool is_splash;
    void setSplash(bool splash){is_splash = splash;}
    bool getSplash(){ return is_splash;}
    Game(int w, int h) : width_(w), height_(h), game_over_(false), pause_(false), frames_counter_(0), is_splash(true)
    {
        InitWindow(width_, height_, "Home");
//        ToggleFullscreen();
        Init();
    };

    void Init()
    {

        // hola bola
        enemies_.clear();
        walls_.clear();
        victory_ = false;

        Vector2 init_pos = {150, 950};
        player_ = Player(init_pos, {0, 0}, 0, "../assets/personaje.png");
//        AddEnemy(Enemy({20, 20}, {0.5, -0.5},  "../assets/jugador.png"));
        // enemigos
        Vector2 small_init_pos[8] = {
                {329, 740},
                {671, 260},
                {935, 665},
                {1283, 935},
                {1366, 565},
                {400, 555},
                {700, 100},
                {445, 935}
        };
        std::string small_sprites[8] = {
            "../assets/enemigo_pequeno_1.png",
            "../assets/enemigo_pequeno_2.png",
            "../assets/enemigo_pequeno_3.png",
            "../assets/enemigo_pequeno_4.png",
            "../assets/enemigo_pequeno_1.png",
            "../assets/enemigo_pequeno_2.png",
            "../assets/enemigo_pequeno_3.png",
            "../assets/enemigo_pequeno_5.png"
        };

        for(int i= 0; i < 8; i++)
        {
            float x_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 1.75f;
            float y_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 1.75f;

            AddEnemy(Enemy(small_init_pos[i], {x_vel, y_vel},small_sprites[i].c_str()));
        }

        Vector2 medium_init_pos[7] = {
                {73 , 568},
                {622 , 490},
                {1182, 790},
                {1746, 590},
                {222 , 49},
                {182, 790},
                {49 , 499}
        };
        std::string medium_sprites[7] = {
            "../assets/enemigo_mediano_1.png",
            "../assets/enemigo_mediano_2.png",
            "../assets/enemigo_mediano_3.png",
            "../assets/enemigo_mediano_4.png",
            "../assets/enemigo_mediano_1.png",
            "../assets/enemigo_mediano_2.png",
            "../assets/enemigo_mediano_3.png",
        };
        for(int i= 0; i < 7; i++)
        {
            float x_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 1.2f;
            float y_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 1.2f;

            AddEnemy(Enemy(medium_init_pos[i], {x_vel, y_vel},medium_sprites[i].c_str()));
        }

        Vector2 large_init_pos[6] = {
                {570 , 970},
                {180 , 190},
                {1280, 185},
                {1780, 970},
                {73 , 568},
                {622 , 490}
        };

        std::string large_sprites[6] = {
            "../assets/enemigo_grande_1.png",
            "../assets/enemigo_grande_2.png",
            "../assets/enemigo_grande_3.png",
            "../assets/enemigo_grande_4.png",
            "../assets/enemigo_grande_1.png",
            "../assets/enemigo_grande_2.png"
        };

        for(int i= 0; i < 6; i++)
        {
            float x_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 0.7f;
            float y_vel = ((float)GetRandomValue(-100, 100) / 100.0f) * 0.7f;

            AddEnemy(Enemy(large_init_pos[i], {x_vel, y_vel},medium_sprites[i].c_str()));
        }

        // --------------------



        Vector2 wall_init_[7] = {
                {0, 450},
                {522, 806},
                {522, 91},
                {960, 450},
                {960, 1046},
                {1514, 806},
                {1672 , 0}
        };
        for(auto pos: wall_init_)
        {
            AddWall(Wall(pos, "../assets/paredgrande.png"));
        }
        // poner camara
        camera_.target = player_.GetPosition();
        camera_.offset = {init_pos.x - width_/2 + player_.GetRadius(), -init_pos.y + height_/2};
        camera_.rotation = 0.0f;
        camera_.zoom = 1.0f;



        tiles[0] =       {0, 0, 1920, 1080};
        tiles[1] =       {709, 0, 1210, 905};
        tiles[2] =       {1017, 0, 902, 673};
        tiles[3] =       {1375, 0, 545, 404};
        tiles[4] =       {1600, 0, 320, 240};


        tilesColors[0] = {0x02, 0x86, 0xba, 0xff};
        tilesColors[1] = {0x0d, 0xa0, 0xd6, 0xff};
        tilesColors[2] = {0x12, 0xb7, 0xe0, 0xff};
        tilesColors[3] = {0x1c, 0xc3, 0xed, 0xff};
        tilesColors[4] = {0x37, 0xd7, 0xff, 0xff};

        is_splash = true;
    }

    float constrainAngle(float x){
        x = fmod(x,360);
        if (x < 0)
            x += 360;
        return x;
    }
    void AddEnemy(Enemy enemy)
    {
        enemies_.push_back(enemy);
    }

    void AddWall(Wall wall)
    {
        walls_.push_back(wall);
    }
    void Update()
    {
        if(!game_over_)
        {
            if (IsGamepadAvailable(GAMEPAD_PLAYER1))
            {
                if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 10))
//                IsGamepadButtonPressed()
                {
                    pause_ = !pause_;
                }
            }

            if(!pause_)
            {
                frames_counter_++;
                // Joystick rotation
                if (IsGamepadAvailable(GAMEPAD_PLAYER1))
                {

                    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, 4))
                        player_.Rotate(-5);
                    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, 5))
                        player_.Rotate(5);
                    // Controller
                    if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 9))
                    {
                        player_.Accelerate(0.04f);
                    }
                    else
                    {
                        player_.Accelerate(-0.02f);
                    }

                }

                for(auto& enemy: enemies_)
                    enemy.Update();



                bool wall_hit = false;
                for(auto wall: walls_)
                {
                    if(player_.CheckWall(wall))
                    {
                        // candidato
                        wall_hit = true;

                        // si el osbstaculo esta arriba
                        float norm_angle = constrainAngle(player_.GetRotation());

                        // la pared esta arriba y miramos abajo
                        if(wall.GetPosition().y < player_.GetPosition().y && norm_angle > 180)
                        {
                            // liberamos
                            wall_hit = false;
                        }
                        // la pared esta abajo y miramos arriba
                        else if(wall.GetPosition().y > player_.GetPosition().y && norm_angle <= 180)
                        {
                            // liberamos
                            wall_hit = false;
                        }
                        // la pared esta a la derecha y miramos izquierda
                        else if(wall.GetPosition().x > player_.GetPosition().x && (norm_angle > 90 && norm_angle <= 270))
                        {
                            // liberamos
                            wall_hit = false;
                        }
                        // la pared esta a la izquierda y miramos derecha
                        else if(wall.GetPosition().x < player_.GetPosition().x &&
                                (norm_angle <= 90 || norm_angle > 270))
                        {
                            // liberamos
                            wall_hit = false;
                        }

                    }

                }

                if(!wall_hit)
                {
                    player_.Update();
                    if (CheckCollisionCircleRec(player_.GetPosition(), player_.GetRadius(), {1870, 30, 50, 50}))
                    {
                        victory_ = true;
                        game_over_ = true;
                    }
                    // update de la camara
                    Vector2 d_pos = player_.GetDeltaPosition();
                    camera_.offset.x -= d_pos.x;
                    camera_.offset.y += d_pos.y;
                    camera_.target = player_.GetPosition();
                }


                // check collisions
                for(auto enemy: enemies_)
                {
                    if(player_.GetHealth() <= 0)
                    {
                        game_over_ = true;
//                        CloseWindow();
                        break;
                    }
                    else
                    {
                        player_.CheckDamage(enemy);
                    }
                }


            }
            else
            {
//                ClearBackground(RAYWHITE);
                DrawText("PRESS [o] TO CONTINUE",
                     GetScreenWidth()/2 - MeasureText("PRESS [o] TO CONTINUE", 10)/2,
                     GetScreenHeight()/2,
                     10,
                     GRAY);
            }
            
        }
        else
        {
            is_splash = true;

            // limpeza
            for(auto enemy: enemies_)
            {
                enemy.ResetPosition({-10, -10});
            }
            enemies_.clear();

            // Joystick rotation
            if (IsGamepadAvailable(GAMEPAD_PLAYER1))
            {
                if (IsGamepadButtonDown(GAMEPAD_PLAYER1, 10))
                {

                    game_over_ = false;

                    walls_ = std::vector<Wall>();
                    Init();


                }
            }
        }
        
    }
    void DrawBackground()
    {

        for (int i = 0; i < 5; i++)
            DrawRectangleRec(tiles[i], tilesColors[i]);
//        DrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);
//
//        for (int i = 0; i < 100; i++) DrawRectangleRec(buildings[i], buildColors[i]);


    }
    void Draw()
    {
        if(!game_over_)
        {

            BeginMode2D(camera_);
            DrawBackground();

            player_.Draw();

            for(auto wall: walls_) wall.Draw();

            for(auto enemy: enemies_) enemy.Draw();

            EndMode2D();
            Vector2 p_pos = player_.GetPosition();
            DrawText(FormatText("{%.01f},{%.01f}", p_pos.x, p_pos.y), 250, 15, 10, BLACK);
            if (GetGamepadButtonPressed() != -1) DrawText(FormatText("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 43, 10, RED);
        }
        else
        {
            DrawText("PRESS [o] TO PLAY AGAIN",
                     GetScreenWidth()/2 - MeasureText("PRESS [o] TO PLAY AGAIN", 10)/2,
                     GetScreenHeight()/2,
                     10,
                     GRAY);
        }
    }
};
#endif //HOME_GAME_H
