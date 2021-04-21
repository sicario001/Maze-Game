#pragma once
#include "LTexture.hpp"
#include <SDL2/SDL_image.h>

const int SQUARE_SIZE = 20;
const int WALL_W = 40;
const int WALL_H = 400;

class Entity
{
protected:
    int x,y;
    LTexture* texture;
    SDL_Rect* clip;
public:
    Entity();
    Entity(int x, int y, LTexture* pTexture, SDL_Rect* clip);

    bool isActive;
    
    int getPosX(){return x;}
    int getPosY(){return y;}

    void render();
};

extern GameEngine* gEngine;

class CollisionRect{
    private:
        SDL_Rect rect;
    public:
        CollisionRect();
        CollisionRect(int x,int y, int w,int h){
            rect.x=x;
            rect.y=y;
            rect.w=w;
            rect.h=h;
        }
        void shift(int x,int y){
            rect.x=x;
            rect.y=y;
        }
        int getH(){
            return rect.h;
        }
        int getW(){
            return rect.w;
        }
        bool intersects(CollisionRect* collider);
        void render();
};

class RigidBody : public Entity{
protected:
    CollisionRect* collisionRect;
public:
    RigidBody(int x, int y, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip);

    CollisionRect* getCollisionRect();
    void handleCollision(RigidBody* rb){}
    bool collided(RigidBody* rb);
};

class KinematicBody : public RigidBody
{
protected:
    int velX;
    int velY;
    int lastVelX=0;
    int lastVelY=0;
    int speed;
public:
    KinematicBody(int x, int y, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip);

    void move();
    void handleCollision(RigidBody* rb);
    void handleOutOfBounds(int scrWidth,int scrHeight);
};

class Character : public KinematicBody{
protected:
    int health;
public:
    Character(int health, int x, int y, LTexture* pTexture, SDL_Rect* clip);
        
    void damage(int x);
    int getHealth();
};


class Player : public Character{
private:

public:
    Player(int health, int x, int y, LTexture* pTexture);
    
    void handleEvent( SDL_Event& e );
};