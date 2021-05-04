#pragma once
#include <SDL2/SDL_image.h>
#include <vector>
#include <functional>
#include "LTexture.hpp"
#include "ClientNet.hpp"
#include "ServerNet.hpp"

using namespace std;

const int PLAYER_SPRITE_SIZE = 71;
const int PLAYER_COLLIDER_SIZE = 45;
const int BULLET_SPRITE_H = 20;
const int BULLET_SPRITE_W = 34;
const int BULLET_COLLIDER_H = 18;
const int BULLET_COLLIDER_W = 36;

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

    bool isActive = true;
    
    int getPosX(){return x;}
    int getPosY(){return y;}

    void render();
    void renderCustom(SDL_Rect* renderQuad);
};

extern GameEngine* gEngine;

class CollisionRect{
    private:
        int x,y,w,h,uw,uh;
        double angle;
        pair<int,int> getVertex(int i);
    public:
        CollisionRect();
        CollisionRect(int px,int py, int pw,int ph, int puw=-1,int puh=-1){
            x=px;
            y=py;
            w=pw;
            h=ph;
            uw = puw;
            uh = puh;
            if(puw==-1){
                uw=w;
            }
            if(puh==-1){
                uh=h;
            }
        }
        void shift(int x,int y, double angle);
        int getH(){
            return h;
        }
        int getW(){
            return w;
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
    bool handleCollision(RigidBody* rb);
    // check if outside window
    bool handleOutOfBounds();
};

class Bullet : public KinematicBody{
private:
    int damage;
public:
    Bullet();
    Bullet(int x, int y, int pSpeedX, int pSpeedY,int damage,LTexture* pTexture, BulletType pType);
};

class Player : public KinematicBody{
private:
    function <void(int x,int y, int speed, double angle, BulletType bt)> shoot;

public:
    Player(int health, int x, int y, LTexture* pbt,SDL_Rect* pClip,function <void(int x,int y, int speed, double angle, BulletType bt)> shootFunc);
    
    void damage(int x);
    int getHealth();
    void sendUpdate(ClientNet* clientObj, ServerNet* serverObj);
    void handleEvent( SDL_Event& e );
    void resetCamera();
};