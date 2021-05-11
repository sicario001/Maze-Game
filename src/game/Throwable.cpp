#include "game/Throwable.hpp"

vector<int> THROWABLE_SPRITE_H = {BULLET_SPRITE_H, SLASH_SPRITE_H};
vector<int> THROWABLE_SPRITE_W = {BULLET_SPRITE_W, SLASH_SPRITE_W};
vector<int> THROWABLE_COLLIDER_H = {BULLET_COLLIDER_H, SLASH_COLLIDER_H};
vector<int> THROWABLE_COLLIDER_W = {BULLET_COLLIDER_W, SLASH_COLLIDER_W};
vector<int> ANIMATION_FRAMES = {6, 4};

Throwable::Throwable(int pX, int pY, int pSpeed, double rotation, int pDamage,LTexture* pTexture, ThrowableType type):KinematicBody(pX,pY,pSpeed * cos(rotation),pSpeed * sin(rotation),pSpeed,new CollisionRect(0,0,THROWABLE_COLLIDER_W[type],THROWABLE_COLLIDER_H[type],rotation,THROWABLE_SPRITE_W[type],THROWABLE_SPRITE_H[type]),pTexture,new SDL_Rect(), rotation){
    clip = new SDL_Rect();
    clip->x=0;
    clip->y=0;
    clip->w=THROWABLE_SPRITE_W[type];
    clip->h=THROWABLE_SPRITE_H[type];
    damage = pDamage;
    throwableType = type;
    if (throwableType==BULLET){
        numFramesEnd = 0;
    }
    else{
        numFramesEnd = 1;
    }
    hitSound = gEngine->audioMaster.loadWaveFile("media/audio/bullethit.wav");
    hitSound->setReferenceDistance(200);
    hitSound->setRollOffFactor(2);
}

void Throwable::move(){
    if (throwableType==BULLET){
        if (numFramesEnd > 0)
        {
            numFramesEnd++;
            clip->x = 64 + numFramesEnd/2 * 16;
            if(numFramesEnd==ANIMATION_FRAMES[throwableType]){
                isActive=false;
            }
        }
        else{
            KinematicBody::move();
        }
    }
    else if (throwableType==KNIFE_SLASH){
        if (numFramesEnd > 0)
        {
            numFramesEnd++;
            clip->x = numFramesEnd * 110;
            if(numFramesEnd==ANIMATION_FRAMES[throwableType]){
                isActive=false;
            }
        }
    }
}

void Throwable::onHit(){
    if (throwableType==BULLET){
        if (numFramesEnd == 0)
        {   
            hitSound->setPosition(x,y,0);
            hitSound->play();
            collided=true;
            x+=velX;
            y+=velY;
            velX = 0;
            velY = 0;
            numFramesEnd = 1;
            clip->x = 64;
        }
    }
    
}
void Throwable::release(){
    if(hitSound)
        hitSound->release();
}