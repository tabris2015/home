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

        sprite_.SetColor(RED);
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
        sprite_.SetPose(position_, 0);
    }
    void Draw()
    {
        sprite_.Draw();
    }
    // ~Enemy();
};


#endif // !ENEMY_H