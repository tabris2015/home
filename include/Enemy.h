#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Sprite.h"
#include <string>

#define MAX_ENEMY_SPEED 1

class Enemy
{
private:
    /* data */
    Vector2 position_;
    Vector2 speed_;
    Sprite sprite_;

public:
    Enemy(Vector2 position, Vector2 speed, std::string file)
    : position_(position), speed_(speed), sprite_(file)
    {

        sprite_.SetColor(WHITE);
        sprite_.AddCircleCollider(CircleCollider(sprite_.GetWidth() / 2));
        sprite_.SetPose(position_, 0);
    }

    Sprite * GetSprite()
    {
        return &sprite_;
    }

    void SetSpeed(Vector2 speed)
    {
        speed_ = speed;
    }

    void Update()
    {
        position_.x += speed_.x;
        position_.y -= speed_.y;
        
        // wall behaviour
        float radius = sprite_.GetHeight()/2;
        
        if  (position_.x > 1920 + radius) position_.x = -(radius);
        else if (position_.x < 0 - radius) position_.x = 1920 + radius;
        if (position_.y > 1080 + radius) position_.y = -(radius);
        else if (position_.y < 0 - radius) position_.y = 1080 + radius;

        sprite_.SetPose(position_, 0);
    }
    void Draw()
    {
        sprite_.Draw();
    }
    // ~Enemy();
};


#endif // !ENEMY_H