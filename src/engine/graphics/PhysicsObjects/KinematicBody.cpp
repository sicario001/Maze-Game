#include "engine/graphics/PhysicsObject.hpp"

pair<int,int> getPosOnCamera(pair<int,int> pos){
    int x_p = (pos.first - gEngine->camera->x)*SCREEN_WIDTH/gEngine->camera->w;
    int y_p = (pos.second - gEngine->camera->y)*SCREEN_HEIGHT/gEngine->camera->h;
    return {x_p,y_p};
}


KinematicBody::KinematicBody(int pX, int pY, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* pClip, double angle):RigidBody(pX,pY,pCollisionRect,pTexture,pClip){
    x = pX;
    y = pY;
    velX = pSpeedX;
    velY = pSpeedY;
    speed = pSpeed;
    if (angle==-10){
        if(pSpeedX!=0 || pSpeedY!=0){
            rotation = atan2(pSpeedY,pSpeedX);
        }
    }
    else{
        rotation = angle;
    }
    
}

void KinematicBody::move()
{
    if (canMove){
        //Move the body left or right
        x += velX;

        //Move the body up or down
        y += velY;

        lastVelX = velX;
        lastVelY = velY;
    }
    collisionRect->shift(x,y);
}

void KinematicBody::stopMovement(){
    canMove = false;
}
void KinematicBody::allowMovement(){
    canMove = true;
}
bool KinematicBody::inVicinity(std::pair<int, int>obj, int pix_dis){
    int distance_sq = (x-obj.first)*(x-obj.first)+ (y-obj.second)*(y-obj.second);
    if (distance_sq<=(pix_dis*pix_dis)){
        return true;
    }
    else{
        return false;
    }
}

bool KinematicBody::handleOutOfBounds(){
    bool isOut = false;
    if(( x < -13) || ( x + collisionRect->getW() > -13 + LEVEL_WIDTH )){
        x -= lastVelX;
        lastVelX = 0;
        isOut = true;
    }
    if(( y < -13 ) || ( y + collisionRect->getH() > -13 + LEVEL_HEIGHT)){
        y -= lastVelY;
        lastVelY = 0;
        isOut = true;
    }
    collisionRect->shift(x,y);
    return isOut;
}

bool KinematicBody::handleCollision(RigidBody* rb)
{
    bool collisionOccured = false;
    if(collided(rb)){
        x -= lastVelX;
        y -= lastVelY;
        lastVelX = 0;
        lastVelY = 0;
        collisionOccured = true;
    }

    collisionRect->shift(x,y);
    return collisionOccured;
}

void KinematicBody::setPosVel(int pX, int pY, int pVelX, int pVelY){
    x = pX;
    y = pY;
    velX = pVelX;
    velY = pVelY;
}

void KinematicBody::setRotation(int deg){
    rotation = PI/180 * deg;
}
