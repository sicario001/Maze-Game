#include "engine/graphics/PhysicsObject.hpp"
#include "game/Player.hpp"
#include "game/Throwable.hpp"

Player::Player(int pHealth, int pX,int pY, LTexture* pTexture,SDL_Rect* pClip,function <void(int x,int y, int speed, double angle, int damage, ThrowableType type)> sf, PlayerSpriteType type): KinematicBody(pX,pY,0,0,5,new CollisionRect(0,0,PLAYER_COLLIDER_SIZE,PLAYER_COLLIDER_SIZE,0,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE),pTexture,pClip){
    playerType = type;
    spawnFunc = sf;
    health = pHealth;
    inventory = new Inventory();

    walkingSounds = gEngine->audioStore->getSourceFor(AS_WALKING_SOUND);
    walkingSounds->setReferenceDistance(200);
    walkingSounds->setRollOffFactor(0.7);

    shootingSounds = gEngine->audioStore->getSourceFor(AS_SHOOTING_SOUND);
    shootingSounds->setReferenceDistance(200);
    shootingSounds->setRollOffFactor(2);

    slashingSounds = gEngine->audioStore->getSourceFor(AS_SLASHING_SOUND);
    slashingSounds->setReferenceDistance(200);
    slashingSounds->setRollOffFactor(2);

    reloadingSounds = gEngine->audioStore->getSourceFor(AS_RELOADING_SOUND);
    reloadingSounds->setReferenceDistance(200);
    reloadingSounds->setRollOffFactor(2);

    dirX=0;
    dirY=0;
}

void Player::damage(int d){
    if (!isDead){
        health -= d;
        cout <<"Health "<< health << endl;
    }
}

void Player::resetAudioSourcePosition(){
    reloadingSounds->setPosition(x,y,0);
    shootingSounds->setPosition(x,y,0);
    slashingSounds->setPosition(x,y,0);
    walkingSounds->setPosition(x,y,0);
}

void Player::resetRotation(){
    // angle formed right shoulder to pointer = angle of texture
    double cx = 0;
    double cy = 44.0 - PLAYER_SPRITE_SIZE/2;
    double d = sqrt(cx*cx+cy*cy);
    double a = atan2(cy,cx);
    cx = x + PLAYER_SPRITE_SIZE/2 + d * cos(a+rotation);
    cy = y + PLAYER_SPRITE_SIZE/2 + d * sin(a+rotation);
    pair<int,int> cOnCamera = getPosOnCamera({cx,cy});
    rotation = atan2(yMouse-cOnCamera.second,xMouse-cOnCamera.first);
}

void Player::updateFireLock(){
    if(fireLock>0){
        fireLock--;
    }
}

void Player::shoot(ThrowableType t){
    if (fireLock==0)
    {
        int damage = inventory->getDamage();
        fireLock = inventory->getFireLockDuration();
        stopReloading();
        // spawn at tip
        double cx = 62.0 - PLAYER_SPRITE_SIZE/2;
        double cy = 44.0 - PLAYER_SPRITE_SIZE/2;
        double d = sqrt(cx*cx+cy*cy);
        double a = atan2(cy,cx);
        cx = x + PLAYER_SPRITE_SIZE/2 + d * cos(a+rotation);
        cy = y + PLAYER_SPRITE_SIZE/2 + d * sin(a+rotation);
        switch (t)
        {
        case BULLET:
            cx -= BULLET_SPRITE_W/2;
            cy -= BULLET_SPRITE_H/2;
            spawnFunc(cx,cy,15,rotation,damage, BULLET);
            inventory->useBullet();
            break;
        
        case KNIFE_SLASH:
            cx -= SLASH_SPRITE_W/2;
            cy -= SLASH_SPRITE_H/2;
            spawnFunc(cx,cy,0,rotation,damage, KNIFE_SLASH);
            break;
        default:
            break;
        }
    }
}

void Player::handleEvent(SDL_Event &e)
{
    if (canMove){
        if(e.type == SDL_MOUSEMOTION){   
            SDL_GetMouseState(&xMouse,&yMouse);
        }
        
        // left click to shoot
        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat==0 && inventory->getCurrWeapon()>0 && !(inventory->isEmptyMag())){
            shoot(BULLET);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat==0 && inventory->getCurrWeapon()==KNIFE){
            shoot(KNIFE_SLASH);
        }
    }
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: dirY = -1; break;
            case SDLK_s: dirY = 1; break;
            case SDLK_a: dirX = -1; break;
            case SDLK_d: dirX = 1; break;
            case SDLK_1: if(canMove) {
                inventory->changeWeapon(KNIFE); 
                stopReloading();
            } break;
            case SDLK_2: if(canMove) {
                inventory->changeWeapon(RIFLE);
                stopReloading();
            } break;
            case SDLK_3: if(canMove) {
                inventory->changeWeapon(PISTOL); 
                stopReloading();
            } break;
            // case SDLK_4: if(canMove) {
            //     inventory->changeWeapon(GRENADE); 
            //     stopReloading();
            // } break;
            // case SDLK_5: if(canMove) {
            //     inventory->changeWeapon(FLASH); 
            //     stopReloading();
            // } break;
            case SDLK_r: {
                if (canMove){
                    if (isReloading==false && inventory->canReload()){
                        reloadingSounds->rewind();
                        reloadingSounds->play();
                        isReloading =true;
                        int reloadDuration = inventory->getReloadDuration();
                        reloadBar = new ProgressBar(reloadDuration, 0, 0, 255);
                    }
                }
            }
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: 
            case SDLK_s: dirY = 0; break;
            case SDLK_a: 
            case SDLK_d: dirX = 0; break;
        }
    }

    if (dirX==0 && dirY==0)
    {   
        velX=0;
        velY=0;
    }
    else{
        double angle = atan2(dirY,dirX);
        velX = (int)(1.0*speed*cos(angle));
        velY = (int)(1.0*speed*sin(angle));
    }

    resetRotation();
}

void Player::sendUpdate(ClientNet* clientObj, ServerNet* serverObj){
    if (clientObj!=NULL){
        if ((clientObj->peer)!=NULL){
            clientObj->SendDataPosVelDeg(clientObj->peer, x, y, velX, velY, (int)(rotation * 180 / PI));
        }
    }
    else{
        if ((serverObj->peer)!=NULL){
            // std::cout<<"in\n";
            serverObj->SendDataPosVelDeg(serverObj->peer, x, y, velX, velY, (int)(rotation * 180 / PI));
        }
    }
}
int Player::getHealth(){
    return health;

}
void Player::resetClip(){
    if (isDead){
        clip->y = (STANDING +6*playerType)*PLAYER_SPRITE_SIZE;
    }
    else if (canMove && inventory->getCurrWeapon()>0){
        if (isReloading){
            clip->y = (RELOADING +6*playerType)*PLAYER_SPRITE_SIZE;
        }
        else{
            if(inventory->getCurrWeapon()==RIFLE){
                clip->y = (RIFLE_EQUIPPED +6*playerType)*PLAYER_SPRITE_SIZE;
            }
            else if(inventory->getCurrWeapon()==PISTOL){
                clip->y = (PISTOL_EQUIPPED +6*playerType)*PLAYER_SPRITE_SIZE;
            }
        }
    }
    else{
        clip->y = (EMPTY_HANDED +6*playerType)*PLAYER_SPRITE_SIZE;
    }
}
void Player::resetCamera(){
    gEngine->camera->h = CAMERA_HEIGHT;
    gEngine->camera->w = CAMERA_WIDTH;
    gEngine->camera->x = x + PLAYER_SPRITE_SIZE/2 - CAMERA_WIDTH/2;
    gEngine->camera->y = y + PLAYER_SPRITE_SIZE/2 - CAMERA_HEIGHT/2;
    gEngine->camera->x = max(gEngine->camera->x,0);
    gEngine->camera->y = max(gEngine->camera->y,0);
    gEngine->camera->x = min(gEngine->camera->x,LEVEL_WIDTH-CAMERA_WIDTH);
    gEngine->camera->y = min(gEngine->camera->y,LEVEL_HEIGHT-CAMERA_HEIGHT);
}
void Player::playSoundIfWalked(bool isListener){
    resetAudioSourcePosition();
    if(lastVelX!=0 || lastVelY!=0){
        if(isListener){
    	    gEngine->resetListener(x,y);
        }
	    // cout << "walker at " <<x <<" " << y << endl;
        if(walkingSounds->getState() == AL_PAUSED){
            walkingSounds->play();
        }
        else if(walkingSounds->getState()!=AL_PLAYING){
	        cout << "played at " <<x <<" " << y << endl;
            walkingSounds->rewind();
            walkingSounds->play();
        }
    }
    else{
        if(walkingSounds->getState()==AL_PLAYING){
	        // cout << "played at " <<x <<" " << y << endl;
            walkingSounds->pause();
        }
    }
}

void Player::stopReloading(){
    isReloading=false;
    if(reloadingSounds->getState()==AL_PLAYING){
        reloadingSounds->rewind();
    }
}

void Player::free(){
    if(walkingSounds)
        walkingSounds->free();
    if(shootingSounds)
        shootingSounds->free();
    if(reloadingSounds)
        reloadingSounds->free();
    if(slashingSounds)
        slashingSounds->free();
}

void Player::playThrowableSound(ThrowableType type){
    switch (type)
    {
    case BULLET:
        shootingSounds->rewind();
        shootingSounds->play();
        break;
    case KNIFE_SLASH:
        slashingSounds->rewind();
        slashingSounds->play();
        break;
    default:
        break;
    }
}

void Player::stopMovement(){
    canMove = false;
    // modify this to set dirX and dirY to 0
    velX = 0;
    velY = 0;
    dirX = 0;
    dirY = 0;
    lastVelX = 0;
    lastVelY = 0;
}
void Player::allowMovement(){
    canMove = true;
}