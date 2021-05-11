#include "PhysicsObject.hpp"

vector<int> THROWABLE_SPRITE_H = {BULLET_SPRITE_H, SLASH_SPRITE_H};
vector<int> THROWABLE_SPRITE_W = {BULLET_SPRITE_W, SLASH_SPRITE_W};
vector<int> THROWABLE_COLLIDER_H = {BULLET_COLLIDER_H, SLASH_COLLIDER_H};
vector<int> THROWABLE_COLLIDER_W = {BULLET_COLLIDER_W, SLASH_COLLIDER_W};
vector<int> ANIMATION_FRAMES = {6, 4};

pair<int,int> CollisionRect::getVertex(int i){
    double pi =3.1415926535;
    double diag = sqrt(1.0*w*w+h*h)/2;
    // if(w==36){
    //     cout << w << " " << h << " " <<uw << " " << uh << endl;
    // }
    double dangle = atan2(h,w);
    switch (i)
    {
    case 0:
        dangle -= pi;
        break;
    case 1:
        dangle = - dangle;
        break;
    case 3:
        dangle = pi - dangle;
        break;
    
    default:
        break;
    }
    double x1 = 1.0*x+uw/2 + diag * cos(dangle + angle);
    double y1 = 1.0*y+uh/2 + diag * sin(dangle + angle);
    return {(int) x1,(int) y1};
}

bool CollisionRect::intersects(CollisionRect* collider){
    for (int x=0; x<2; x++)
    {
        CollisionRect* polygon = (x==0) ? this : collider;

        for (int i1=0; i1<4; i1++)
        {
            int   i2 = (i1 + 1) % 4;
            pair<int,int> p1 = polygon->getVertex(i1);
            pair<int,int> p2 = polygon->getVertex(i2);

            pair<int,int> normal(p2.second - p1.second, p1.first - p2.first);

            double minA = __DBL_MAX__;
            double maxA = - __DBL_MAX__;

            for (int i = 0; i< 4;i++)
            {
                pair<int,int> p = this->getVertex(i);
                double projected = normal.first * p.first + normal.second * p.second;

                if (projected < minA)
                    minA = projected;
                if (projected > maxA)
                    maxA = projected;
            }

            double minB = __DBL_MAX__;
            double maxB = - __DBL_MAX__;

            for (int i = 0; i< 4;i++)
            {
                pair<int,int> p = collider->getVertex(i);
                double projected = normal.first * p.first + normal.second * p.second;

                if (projected < minB)
                    minB = projected;
                if (projected > maxB)
                    maxB = projected;
            }

            if (maxA < minB || maxB < minA)
                return false;
        }
    }

    return true;
}

void CollisionRect::render(){
    if((x < gEngine->camera->x - TILE_SIZE) || (x > gEngine->camera->x + gEngine->camera->w + TILE_SIZE)){
        return;
    }
    if((y < gEngine->camera->y - TILE_SIZE) || (y > gEngine->camera->y + gEngine->camera->h + TILE_SIZE)){
        return;
    }
    for(int i =0;i<4;i++){
        pair<int,int> p1 = getVertex(i);
        pair<int,int> p2 = getVertex((i+1)%4);
        int x1 = (p1.first - gEngine->camera->x)*SCREEN_WIDTH/gEngine->camera->w;
        int y1 = (p1.second - gEngine->camera->y)*SCREEN_HEIGHT/gEngine->camera->h;
        int x2 = (p2.first - gEngine->camera->x)*SCREEN_WIDTH/gEngine->camera->w;
        int y2 = (p2.second - gEngine->camera->y)*SCREEN_HEIGHT/gEngine->camera->h;
        SDL_RenderDrawLine( gEngine->gRenderer, x1,y1,x2,y2 );
    }
}

void CollisionRect::shift(int px,int py){
    x = px;
    y = py;
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
void Entity::render(double pre_scale){
    if((x < gEngine->camera->x - TILE_SIZE) || (x > gEngine->camera->x + gEngine->camera->w + TILE_SIZE)){
        return;
    }
    if((y < gEngine->camera->y - TILE_SIZE) || (y > gEngine->camera->y + gEngine->camera->h + TILE_SIZE)){
        return;
    }
    double scale = 1.0 * pre_scale * SCREEN_WIDTH / gEngine->camera->w;
    int xOnCamera = (x - gEngine->camera->x)*SCREEN_WIDTH/gEngine->camera->w;
    int yOnCamera = (y - gEngine->camera->y)*SCREEN_HEIGHT/gEngine->camera->h;
    texture->render(xOnCamera,yOnCamera,clip,scale,rotation * 180 / 3.1415926535);
}

RigidBody::RigidBody(int pX,int pY, CollisionRect* pCollisionRect, LTexture* pTexture, SDL_Rect* pClip):Entity(pX,pY,pTexture,pClip){
    collisionRect = pCollisionRect;
}

bool RigidBody::collided(RigidBody* rb){
    return collisionRect->intersects(rb->collisionRect);
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
    rotation = 3.1415926535/180 * deg;
}

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

Player::Player(int pHealth, int pX,int pY, LTexture* pTexture,SDL_Rect* pClip,function <void(int x,int y, int speed, double angle, int damage, ThrowableType type)> sf, PlayerSpriteType type): KinematicBody(pX,pY,0,0,5,new CollisionRect(0,0,PLAYER_COLLIDER_SIZE,PLAYER_COLLIDER_SIZE,0,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE),pTexture,pClip){
    playerType = type;
    shoot = sf;
    health = pHealth;
    inventory = new Inventory();

    walkingSounds = gEngine->audioMaster.loadWaveFile("media/audio/walk.wav");
    walkingSounds->setReferenceDistance(200);
    walkingSounds->setRollOffFactor(0.7);

    shootingSounds = gEngine->audioMaster.loadWaveFile("media/audio/gunshot.wav");
    shootingSounds->setReferenceDistance(200);
    shootingSounds->setRollOffFactor(2);

    slashingSounds = gEngine->audioMaster.loadWaveFile("media/audio/slash.wav");
    slashingSounds->setReferenceDistance(200);
    slashingSounds->setRollOffFactor(2);

    reloadingSounds = gEngine->audioMaster.loadWaveFile("media/audio/reload.wav");
    reloadingSounds->setReferenceDistance(200);
    reloadingSounds->setRollOffFactor(2);
}

void Player::damage(int d){
    health -= d;
    cout <<"Health "<< health << endl;
}

void Player::resetRotation(){
    // angle formed right shoulder to pointer = angle of texture
    double cx = 0;
    double cy = 44.0 - PLAYER_SPRITE_SIZE/2;
    double d = sqrt(cx*cx+cy*cy);
    double a = atan2(cy,cx);
    cx = x + PLAYER_SPRITE_SIZE/2 + d * cos(a+rotation);
    cy = y + PLAYER_SPRITE_SIZE/2 + d * sin(a+rotation);
    cx = (cx - gEngine->camera->x)*SCREEN_WIDTH/gEngine->camera->w;
    cy = (cy - gEngine->camera->y)*SCREEN_HEIGHT/gEngine->camera->h;
    rotation = atan2(yMouse-cy,xMouse-cx);
}

void Player::handleEvent(SDL_Event &e)
{
    if (canMove){
        if(e.type == SDL_MOUSEMOTION)
        {   
            SDL_GetMouseState(&xMouse,&yMouse);
        }
        
        // left click to shoot
        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat==0 && inventory->getCurrWeapon()==GUN && !(inventory->isEmptyMag())){
            // spawn at tip
            // cout << "SHOOT!" << endl;
            stopReloading();
            double cx = 62.0 - PLAYER_SPRITE_SIZE/2;
            double cy = 44.0 - PLAYER_SPRITE_SIZE/2;
            double d = sqrt(cx*cx+cy*cy);
            double a = atan2(cy,cx);
            cx = x + PLAYER_SPRITE_SIZE/2 + d * cos(a+rotation) - BULLET_SPRITE_W/2;
            cy = y + PLAYER_SPRITE_SIZE/2 + d * sin(a+rotation) - BULLET_SPRITE_H/2;
            shoot(cx,cy,15,rotation,10, BULLET);
            shootingSounds->rewind();
            shootingSounds->play();
            inventory->useBullet();
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat==0 && inventory->getCurrWeapon()==KNIFE){
            stopReloading();
            double cx = 62.0 - PLAYER_SPRITE_SIZE/2;
            double cy = 44.0 - PLAYER_SPRITE_SIZE/2;
            double d = sqrt(cx*cx+cy*cy);
            double a = atan2(cy,cx);
            cx = x + PLAYER_SPRITE_SIZE/2 + d * cos(a+rotation) - SLASH_SPRITE_W/2;
            cy = y + PLAYER_SPRITE_SIZE/2 + d * sin(a+rotation) - SLASH_SPRITE_H/2;
            shoot(cx,cy,0,rotation,5, KNIFE_SLASH);
            slashingSounds->rewind();
            slashingSounds->play();
        }
    }
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: velY -= speed; break;
            case SDLK_s: velY += speed; break;
            case SDLK_a: velX -= speed; break;
            case SDLK_d: velX += speed; break;
            case SDLK_1: if(canMove) {
                inventory->changeWeapon(KNIFE); 
                stopReloading();
            } break;
            case SDLK_2: if(canMove) {inventory->changeWeapon(GUN);} break;
            case SDLK_3: if(canMove) {
                inventory->changeWeapon(SMOKE); 
            stopReloading();
            } break;
            case SDLK_4: if(canMove) {
                inventory->changeWeapon(GRENADE); 
                stopReloading();
            } break;
            case SDLK_5: if(canMove) {
                inventory->changeWeapon(FLASH); 
                stopReloading();
            } break;
            case SDLK_r: {
                if (canMove){
                    if (isReloading==false && inventory->canReload()){
                        reloadingSounds->rewind();
                        reloadingSounds->play();
                        isReloading =true;
                        reloadBar = new ProgressBar(10000, 0, 0, 255);
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
            case SDLK_w: velY += speed; break;
            case SDLK_s: velY -= speed; break;
            case SDLK_a: velX += speed; break;
            case SDLK_d: velX -= speed; break;
        }
    }
    resetRotation();
}

void Player::sendUpdate(ClientNet* clientObj, ServerNet* serverObj){
    if (clientObj!=NULL){
        if ((clientObj->peer)!=NULL){
            clientObj->SendDataPosVelDeg(clientObj->peer, x, y, velX, velY, (int)(rotation * 180 / 3.1415926535));
        }
    }
    else{
        if ((serverObj->peer)!=NULL){
            // std::cout<<"in\n";
            serverObj->SendDataPosVelDeg(serverObj->peer, x, y, velX, velY, (int)(rotation * 180 / 3.1415926535));
        }
    }
}
int Player::getHealth(){
    return health;

}
void Player::resetClip(){
    if (canMove && inventory->getCurrWeapon()==GUN){
        if (isReloading){
            clip->y = (3 +6*playerType)*PLAYER_SPRITE_SIZE;
        }
        else{
            clip->y = (2 +6*playerType)*PLAYER_SPRITE_SIZE;
        }
    }
    else{
        clip->y = (5 +6*playerType)*PLAYER_SPRITE_SIZE;
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
    if(lastVelX!=0 || lastVelY!=0){
        if(isListener){
    	    gEngine->resetListener(x,y);
        }
        reloadingSounds->setPosition(x,y,0);
        shootingSounds->setPosition(x,y,0);
        slashingSounds->setPosition(x,y,0);
        walkingSounds->setPosition(x,y,0);
	    // cout << "walker at " <<x <<" " << y << endl;
        if(walkingSounds->getState() == AL_PAUSED){
            walkingSounds->play();
        }
        else if(walkingSounds->getState()!=AL_PLAYING){
	        // cout << "played at " <<x <<" " << y << endl;
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

void Player::release(){
    walkingSounds->release();
    shootingSounds->release();
    reloadingSounds->release();
    slashingSounds->release();
}