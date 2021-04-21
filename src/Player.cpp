#include "PhysicsObject.hpp"

bool CollisionRect::intersects(CollisionRect* collider){
    return SDL_HasIntersection(&rect,&(collider->rect));
}

void CollisionRect::render(){
    SDL_RenderDrawRect( gEngine->gRenderer, &rect );
}

Entity::Entity(int pX,int pY, LTexture* pTexture, SDL_Rect* pClip){
    x = pX;
    y = pY;
    texture = pTexture;
    if(pClip!=NULL){
        clip = new SDL_Rect();
        *clip = *pClip;
    }
}

void Entity::render(){
    texture->render(x,y,clip);
}

RigidBody::RigidBody(int pX,int pY, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* pClip):Entity(pX,pY,pTexture,pClip){
    collisionRect = pCollisionRect;
}

bool RigidBody::collided(RigidBody* rb){
    return collisionRect->intersects(rb->collisionRect);
}

KinematicBody::KinematicBody(int pX, int pY, int pSpeedX, int pSpeedY,int pSpeed, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* pClip):RigidBody(pX,pY,pCollisionRect,pTexture,pClip){
    x = pX;
    y = pY;
    velX = pSpeedX;
    velY = pSpeedY;
    speed = pSpeed;
}

void KinematicBody::move()
{
    //Move the body left or right
    x += velX;

    //Move the body up or down
    y += velY;

    lastVelX = velX;
    lastVelY = velY;

    collisionRect->shift(x,y);
}

void KinematicBody::handleOutOfBounds(int scrWidth,int scrHeight){
    if(( x < 0 ) || ( x + collisionRect->getW() > SCREEN_WIDTH )){
        x -= lastVelX;
        lastVelX = 0;
    }
    if(( y < 0 ) || ( y + collisionRect->getH()> SCREEN_HEIGHT)){
        y -= lastVelY;
        lastVelY = 0;
    }

    collisionRect->shift(x,y);
}

void KinematicBody::handleCollision(RigidBody* rb)
{
    if(collided(rb)){
        x -= lastVelX;
        y -= lastVelY;
        lastVelX = 0;
        lastVelY = 0;
    }

    collisionRect->shift(x,y);
}

Character::Character(int health, int pX,int pY, LTexture* pTexture, SDL_Rect* clip):KinematicBody(pX,pY,0,0,5, new CollisionRect(0,0,SQUARE_SIZE,SQUARE_SIZE),pTexture, clip){
    this->health=health;
}

Player::Player(int health, int pX,int pY, LTexture* pTexture): Character(health,pX,pY,pTexture,NULL){}


void Player::handleEvent(SDL_Event &e)
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: velY -= speed; break;
            case SDLK_DOWN: velY += speed; break;
            case SDLK_LEFT: velX -= speed; break;
            case SDLK_RIGHT: velX += speed; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: velY += speed; break;
            case SDLK_DOWN: velY -= speed; break;
            case SDLK_LEFT: velX += speed; break;
            case SDLK_RIGHT: velX -= speed; break;
        }
    }
}