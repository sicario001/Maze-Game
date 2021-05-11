#include "engine/graphics/PhysicsObject.hpp"

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
    pair<int,int> posOnCamera = getPosOnCamera({x,y});
    texture->render(posOnCamera.first,posOnCamera.second,clip,scale,rotation * 180 / PI);
}
