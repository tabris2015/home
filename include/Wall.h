//
// Created by pepe on 27-01-19.
//

#ifndef HOME_WALL_H
#define HOME_WALL_H

#include "raylib.h"
#include "Sprite.h"
#include <string>


class Wall
{
private:
    /* data */
    Vector2 position_;
    Sprite sprite_;

public:
    Wall(Vector2 position, std::string file)
            : position_(position), sprite_(file)
    {
        sprite_.SetColor(BLUE);
        sprite_.AddBoxCollider(BoxCollider({position_.x - sprite_.GetWidth()/2, position_.y - sprite_.GetHeight() / 2, sprite_.GetWidth(), sprite_.GetHeight()}));
        sprite_.SetPose(position_, 0);
    }

    Sprite * GetSprite()
    {
        return &sprite_;
    }

    Vector2 GetPosition(){ return position_;}
    void Draw()
    {
        sprite_.Draw();
    }
    // ~Enemy();
};

#endif //HOME_WALL_H
