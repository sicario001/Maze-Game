#pragma once

#include "engine/graphics/PhysicsObject.hpp"
#include "engine/audio/AudioSource.hpp"

const int BULLET_SPRITE_H = 16;
const int BULLET_SPRITE_W = 16;
const int BULLET_COLLIDER_H = 8;
const int BULLET_COLLIDER_W = 14;

const int SLASH_SPRITE_H = 129;
const int SLASH_SPRITE_W = 110;
const int SLASH_COLLIDER_H = 20;
const int SLASH_COLLIDER_W = 20;

class Throwable : public KinematicBody{
private:
    int numFramesEnd = 0;
    AudioSource* hitSound;
public:
    ThrowableType throwableType;
    // if collided yet
    bool collided = false;
    int damage;
    Throwable();
    Throwable(int x, int y, int pSpeed, double rotation,int damage,LTexture* pTexture, ThrowableType type);
    void move();
    void release();
    // start animation when hit
    void onHit();
};