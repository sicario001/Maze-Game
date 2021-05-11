#pragma once

#include <vector>
#include <functional>

#include <SDL2/SDL_image.h>

#include "engine/graphics/LTexture.hpp"
#include "engine/net/ClientNet.hpp"
#include "engine/net/ServerNet.hpp"

using namespace std;

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

    void render(double pre_scale = 1.0);
    void renderCustom(SDL_Rect* renderQuad);
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
    bool canMove = true;
public:
    KinematicBody(int x, int y, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* clip, double angle = -10);

    void setPosVel(int pX, int pY, int pVelX, int pVelY);
    void setRotation(int deg);
    void move();
    void stopMovement();
    void allowMovement();
    // handle collision with a body, move back a step
    bool handleCollision(RigidBody* rb);
    // check if outside window
    bool handleOutOfBounds();
    bool inVicinity(std::pair<int, int> object, int pix_dis = 30);
};
