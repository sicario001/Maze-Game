#pragma once
#include <SDL2/SDL_image.h>
#include "LTexture.hpp"
#include "ClientNet.hpp"
#include "ServerNet.hpp"

const int PLAYER_SPRITE_W = 71;
const int PLAYER_SPRITE_H = 71;
const int COLLIDER_SIZE = 45;

enum PlayerSpriteType{
	HITMAN,
	MAN_BLUE,
	MAN_BROWN,
	MAN_OLD,
	ROBOT,
	SOLDIER,
	SURVIVOR,
	WOMAN,
	ZOMBIE
};

class Entity
{
protected:
    int x,y;

    LTexture* texture;
    double rotation=0.0;
public:
    SDL_Rect* clip = NULL;
    Entity();
    ~Entity();
    Entity(int x, int y, LTexture* pTexture, SDL_Rect* clip);

    bool isActive;
    
    int getPosX(){return x;}
    int getPosY(){return y;}

    void render();
    void renderCustom(SDL_Rect* renderQuad);
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
        void shift(int x,int y);
        int getH(){
            return rect.h;
        }
        int getW(){
            return rect.w;
        }
        bool intersects(CollisionRect* collider);
        // to debug collision areas
        void render();
};

class RigidBody : public Entity{
protected:
    CollisionRect* collisionRect;
public:
    RigidBody(int x, int y, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip);

    CollisionRect* getCollisionRect(){
        return collisionRect;
    }
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
    // reset rotation based on current velocity
    void resetRotation();
public:
    KinematicBody(int x, int y, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip);

    void setPosVel(int pX, int pY, int pVelX, int pVelY);
    void move();
    // handle collision with a body, move back a step
    void handleCollision(RigidBody* rb);
    // check if outside window
    void handleOutOfBounds();
};


class Player : public KinematicBody{
private:

public:
    Player(int health, int x, int y, LTexture* pTexture,SDL_Rect* pClip);
    
    void damage(int x);
    int getHealth();
    void sendUpdate(ClientNet* clientObj, ServerNet* serverObj);
    void handleEvent( SDL_Event& e );
    void resetCamera();
};