//
// Created by pepe on 26-01-19.
//

#ifndef HOME_COLLIDER_H
#define HOME_COLLIDER_H

#include "raylib.h"

enum ColliderType
{
    POINT,
    CIRCLE,
    RECT
};

class BoxCollider;

class CircleCollider
{
private:
    Vector2 position_;
    ColliderType type_;
    float radius_;

public:
    CircleCollider(){};
    CircleCollider(float radius) : position_({0,0}), type_(CIRCLE), radius_(radius) {};

    float GetRadius(){ return  radius_; }
    Vector2 GetPosition(){ return position_; };
    ColliderType GetType(){ return type_; };
    void SetPosition(Vector2 new_pos){ position_ = new_pos; };


};

class BoxCollider
{
private:
    Vector2 position_;
    ColliderType type_;
    Rectangle box_;

public:
    BoxCollider(){};
    BoxCollider(Rectangle box) : position_({0, 0}), type_(RECT) , box_(box) {};

    Vector2 GetPosition(){ return position_; };
    ColliderType GetType(){ return type_; };
    Rectangle GetRect(){ return  box_; }
    void SetPosition(Vector2 new_pos){ position_ = new_pos; };


};


#endif //HOME_COLLIDER_H
