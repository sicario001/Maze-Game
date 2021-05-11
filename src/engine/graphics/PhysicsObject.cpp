#include "engine/graphics/PhysicsObject.hpp"

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
