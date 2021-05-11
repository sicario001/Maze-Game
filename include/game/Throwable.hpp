#pragma once

#include "engine/graphics/PhysicsObject.hpp"
#include "engine/audio/AudioSource.hpp"

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
    void free();
    // start animation when hit
    void onHit();
};