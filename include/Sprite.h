//
// Created by pepe on 26-01-19.
//

#ifndef HOME_SPRITE_H
#define HOME_SPRITE_H

#include "raylib.h"
#include <string>
#include <iostream>
#include <vector>
#include "Collider.h"

class Sprite
{
private:
    Vector2 position_;
    float rotation_;
    Texture2D texture_;
    float scale_;
    Rectangle source_rec_;
    Rectangle dest_rec_;
    Vector2 origin_;
    Color color_;



    std::vector<Vector2> collider_offsets_;

public:
    CircleCollider circle_collider_;
    BoxCollider box_collider_;
    Vector2 collider_offset_;
    ColliderType collider_type;
    Sprite(){};
    Sprite(const std::string& file) : position_({0, 0}), rotation_(0), scale_(1), color_(WHITE)
    {
        texture_ = LoadTexture(file.c_str());
        // take complete texture
        source_rec_ = {0.0f, 0.0f, (float)texture_.width, (float)texture_.height};
        origin_ = {(float)texture_.width/2, (float)texture_.height/2};
        std::cout << texture_.id << std::endl;

    }

    void AddCircleCollider(CircleCollider collider, Vector2 relative_pos={0, 0})
    {
        circle_collider_ = collider;
        collider_offset_ = relative_pos;
        collider_type = CIRCLE;
    }
    void AddBoxCollider(BoxCollider collider, Vector2 relative_pos={0, 0})
    {
        box_collider_ = collider;
        collider_offset_ = relative_pos;
        collider_type = RECT;
    }

    float GetWidth(){return (float)texture_.width;}
    float GetHeight(){return (float)texture_.height;}

    void SetPose(Vector2 new_position, float new_rotation)
    {
        position_ = new_position;
        rotation_ = new_rotation;
        // recalculate colliders position
        float x_coll = position_.x + collider_offset_.x;
        float y_coll = position_.y + collider_offset_.y;
        circle_collider_.SetPosition({x_coll, y_coll});
        box_collider_.SetPosition({x_coll, y_coll});

    }

    void SetPose(float x, float y, float new_rotation)
    {
        position_ = {x, y};
        rotation_ = new_rotation;
        // recalculate colliders position
        float x_coll = position_.x + collider_offset_.x;
        float y_coll = position_.y + collider_offset_.y;
        circle_collider_.SetPosition({x_coll, y_coll});
        box_collider_.SetPosition({x_coll, y_coll});

    }
    void Draw()
    {
        dest_rec_ = {position_.x, position_.y, (float)texture_.width * scale_, (float)texture_.height * scale_};
        DrawTexturePro(texture_, source_rec_, dest_rec_, origin_, rotation_, color_);
    }

    void SetColor(Color new_color)
    {
        color_ = new_color;
    }

    void SetScale(float new_scale)
    {
        scale_ = new_scale;
    }


    bool CheckCollision(Sprite& other)
    {
        if(collider_type == CIRCLE && other.collider_type == CIRCLE)
            return CheckCollisionCircles(circle_collider_.GetPosition(),
                    circle_collider_.GetRadius(),
                    other.circle_collider_.GetPosition(),
                    other.circle_collider_.GetRadius());

        if(collider_type == CIRCLE && other.collider_type == RECT)
            return CheckCollisionCircleRec(circle_collider_.GetPosition(),
                    circle_collider_.GetRadius(),
                    other.box_collider_.GetRect());

        if(collider_type == RECT && other.collider_type == CIRCLE)
            return CheckCollisionCircleRec(other.circle_collider_.GetPosition(),
                                           other.circle_collider_.GetRadius(),
                                           box_collider_.GetRect());
        if(collider_type == RECT && other.collider_type == RECT)
            return CheckCollisionRecs(box_collider_.GetRect(), other.box_collider_.GetRect());
    }

};
#endif //HOME_SPRITE_H
