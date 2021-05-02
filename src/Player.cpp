#include "PhysicsObject.hpp"

bool CollisionRect::intersects(CollisionRect* collider){
    return SDL_HasIntersection(&rect,&(collider->rect));
}

void CollisionRect::render(){
    SDL_RenderDrawRect( gEngine->gRenderer, &rect );
}

void CollisionRect::shift(int x,int y){
    rect.x = x + PLAYER_SPRITE_W/2 - COLLIDER_SIZE/2;
    rect.y = y + PLAYER_SPRITE_H/2 - COLLIDER_SIZE/2;
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
Entity::~Entity(){
    texture = NULL;
    clip = NULL;
}
void Entity::render(){
    if((x < gEngine->camera->x - TILE_SIZE) || (x > gEngine->camera->x + gEngine->camera->w + TILE_SIZE)){
        return;
    }
    if((y < gEngine->camera->y - TILE_SIZE) || (y > gEngine->camera->y + gEngine->camera->h + TILE_SIZE)){
        return;
    }
    double scale = 1.0 * SCREEN_WIDTH / gEngine->camera->w;
    double xOnCamera = 1.0 * (x - gEngine->camera->x)/gEngine->camera->w*SCREEN_WIDTH;
    double yOnCamera = 1.0 * (y - gEngine->camera->y)/gEngine->camera->h*SCREEN_HEIGHT;
    texture->render((int)xOnCamera,(int)yOnCamera,clip,scale,rotation);
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

void KinematicBody::resetRotation(){
    if(lastVelX!=0 || lastVelY!=0){
        rotation = atan2(lastVelY,lastVelX) * 180/3.1415926535;
    }
}

void KinematicBody::move()
{
    //Move the body left or right
    x += velX;

    //Move the body up or down
    y += velY;

    lastVelX = velX;
    lastVelY = velY;

    resetRotation();
    collisionRect->shift(x,y);
}

void KinematicBody::handleOutOfBounds(){
    if(( x < 0 ) || ( x + collisionRect->getW() > LEVEL_WIDTH )){
        x -= lastVelX;
        lastVelX = 0;
    }
    if(( y < 0 ) || ( y + collisionRect->getH() > LEVEL_HEIGHT)){
        y -= lastVelY;
        lastVelY = 0;
    }

    resetRotation();
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

    resetRotation();
    collisionRect->shift(x,y);
}

void KinematicBody::setPosVel(int pX, int pY, int pVelX, int pVelY){
    x = pX;
    y = pY;
    velX = pVelX;
    velY = pVelY;
    resetRotation();
}

Player::Player(int health, int pX,int pY, LTexture* pTexture,SDL_Rect* pClip): KinematicBody(pX,pY,0,0,5,new CollisionRect(0,0,COLLIDER_SIZE,COLLIDER_SIZE),pTexture,pClip){}


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

void Player::sendUpdate(ClientNet* clientObj, ServerNet* serverObj){
    if (clientObj!=NULL){
        if ((clientObj->peer)!=NULL){
            clientObj->SendDataPosVel(clientObj->peer, x, y, lastVelX, lastVelY);
        }
    }
    else{
        if ((serverObj->peer)!=NULL){
            // std::cout<<"in\n";
            serverObj->SendDataPosVel(serverObj->peer, x, y, lastVelX, lastVelY);
        }
    }
}

void Player::resetCamera(){
    gEngine->camera->h = CAMERA_HEIGHT;
    gEngine->camera->w = CAMERA_WIDTH;
    gEngine->camera->x = x + PLAYER_SPRITE_W/2 - CAMERA_WIDTH/2;
    gEngine->camera->y = y + PLAYER_SPRITE_H/2 - CAMERA_HEIGHT/2;
    gEngine->camera->x = max(gEngine->camera->x,0);
    gEngine->camera->y = max(gEngine->camera->y,0);
    gEngine->camera->x = min(gEngine->camera->x,LEVEL_WIDTH-CAMERA_WIDTH);
    gEngine->camera->y = min(gEngine->camera->y,LEVEL_HEIGHT-CAMERA_HEIGHT);
}