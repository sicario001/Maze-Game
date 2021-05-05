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
const int BULLET_SPRITE_H = 16;
const int BULLET_SPRITE_W = 16;
const int BULLET_COLLIDER_H = 8;
const int BULLET_COLLIDER_W = 14;

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
    // angle in radians
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
};

extern GameEngine* gEngine;

class CollisionRect{
    private:
        // uw and uh are the textures dimensions
        int x,y,w,h,uw,uh;
        // initial angle in radians
        double angle;
        pair<int,int> getVertex(int i);
    public:
        CollisionRect();
        CollisionRect(int px,int py, int pw,int ph, double pangle = 0.0, int puw=-1,int puh=-1){
            x=px;
            y=py;
            w=pw;
            h=ph;
            uw = puw;
            uh = puh;
            angle = pangle;
            if(puw==-1){
                uw=w;
            }
            if(puh==-1){
                uh=h;
            }
        }
        void shift(int x,int y);
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
public:
    KinematicBody(int x, int y, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip);

    void setPosVel(int pX, int pY, int pVelX, int pVelY);
    void setRotation(int deg);
    void move();
    // handle collision with a body, move back a step
    bool handleCollision(RigidBody* rb);
    // check if outside window
    bool handleOutOfBounds();
};

class Bullet : public KinematicBody{
private:
    int numFramesEnd = 0;
public:
    // if collided yet
    bool collided = false;
    int damage;
    Bullet();
    Bullet(int x, int y, int pSpeed, double rotation,int damage,LTexture* pTexture);
    void move();
    // start animation when hit
    void onHit();
};

class Player : public KinematicBody{
private:
    function <void(int x,int y, int speed, double angle, int damage)> shoot;
    int xMouse,yMouse;
    int health = 100;
    void resetRotation();
public:
    Player(int health, int x, int y, LTexture* pbt,SDL_Rect* pClip,function <void(int x,int y, int speed, double angle, int damage)> shootFunc);
    
    int getHealth();
    void damage(int x);
    // send position,velocity and rotation
    void sendUpdate(ClientNet* clientObj, ServerNet* serverObj);
    void handleEvent( SDL_Event& e );
    void resetCamera();
};