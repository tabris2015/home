//
// Created by pepe on 26-01-19.
//

#ifndef HOME_PLAYER_H
#define HOME_PLAYER_H

#include "raylib.h"
#include "Sprite.h"
#include <string>
#include <math.h>
#include "Enemy.h"
#include <iostream>
#include "Wall.h"

#define MAX_ACCELERATION 1.0
#define MIN_ACCELERATION 0.0

class Player
{
private:
    Vector2 position_;
    Vector2 speed_;
    float accel_;
    float rotation_;
    Sprite sprite_;
    int health_;
    std::vector<Enemy> * enemies_;
    Vector2 delta_position_;


public:
    Player(){};

    Player(Vector2 position, Vector2 speed, float rotation, std::string file)
    : position_(position), speed_(speed), rotation_(rotation), sprite_(file), health_(5), enemies_(nullptr)
    {

        CircleCollider coll = CircleCollider(sprite_.GetWidth() / 2);
        sprite_.AddCircleCollider(coll);
        sprite_.SetPose(position_, rotation_);
    }

    int GetRadius()
    {
        return sprite_.GetHeight() / 2;
    }
    int GetHealth(){return health_;};
    void SetEnemies(std::vector<Enemy> * enemies)
    {
        enemies_ = enemies;
    }

    void SetRotation(float rotation)
    {
        rotation_ = rotation;
        sprite_.SetPose(position_, rotation_);
    }

    Vector2 GetPosition(){return position_;}
    float GetRotation(){ return rotation_;}
    Vector2 GetDeltaPosition()
    {
        return delta_position_;
    }

    void Rotate(float delta)
    {
        rotation_ += delta;
        sprite_.SetPose(position_, rotation_);
    }

    void Accelerate(float delta)
    {
        if(accel_ > MAX_ACCELERATION) accel_ = MAX_ACCELERATION;
        if(accel_ <= MAX_ACCELERATION)
            accel_ += delta;
        
        if(accel_ < MIN_ACCELERATION)
            accel_ = 0.0;
    }
    
    bool CheckDamage(Enemy& enemy)
    {
        bool has_collision = sprite_.CheckCollision(*enemy.GetSprite());
        if(has_collision)
        {
            health_--;
            std::cout << "hit" << std::endl;
            return true;
        }
        return false;
    }

    bool CheckWall(Wall& wall)
    {
        bool has_collision = sprite_.CheckCollision(*wall.GetSprite());
        if(has_collision)
        {
            speed_ = {0,0};
            return true;
        }
        return false;
    }

    void Update()
    {
        speed_.x = sinf(rotation_ * DEG2RAD) * 3.0f;
        speed_.y = cosf(rotation_ * DEG2RAD) * 3.0f;

        delta_position_.x = (speed_.x * accel_);
        delta_position_.y = (speed_.y * accel_);

        position_.x += delta_position_.x;
        position_.y -= delta_position_.y;

        sprite_.SetPose(position_, rotation_);
//        // collisions
//        int collisions = 0;
//        if(!enemies_->empty())
//        {
//            for(auto enemy: *enemies_)
//            {
//                if(CheckDamage(enemy)) collisions++;
//            }
//        }
//
//        if(collisions > 0) health_--;

    }
    void Draw()
    {
        sprite_.Draw();
    }

};
#endif //HOME_PLAYER_H
