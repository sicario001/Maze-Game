#include "HealthBar.hpp"

HealthBar::HealthBar(){
    health = 100;
    damage = 0;
    x = 10;
    y = 10;
    healthBar = new SDL_Rect();
    damageBar = new SDL_Rect();
    BarBoundary = new SDL_Rect();
    healthBar->x = x;
    healthBar->y = y;
    damageBar->x = x;
    damageBar->y = y;
    BarBoundary->x = x;
    BarBoundary->y = y;
    BarBoundary->w = (health*5);
    BarBoundary->h = (20);
    UpdateBar();
}

void HealthBar::UpdateBar(){
    healthBar->w = (health*5);
    healthBar->h = (20);

    damageBar->w = ((health+damage)*5);
    damageBar->h = (20);
}
void HealthBar::setHealth(int h){
    damage = max(0, health - h);
    health = h;
    UpdateBar();

}
int HealthBar::getHealth(){
    return health;
}
int HealthBar::getDamage(){
    return damage;
}
void HealthBar::render(){
    SDL_SetRenderDrawColor(gEngine->gRenderer, 255, 0, 0, 255 );
    SDL_RenderFillRect(gEngine->gRenderer, damageBar);

    SDL_SetRenderDrawColor(gEngine->gRenderer, 0, 255, 0, 255 );
    SDL_RenderFillRect(gEngine->gRenderer, healthBar);

    SDL_SetRenderDrawColor(gEngine->gRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gEngine->gRenderer, BarBoundary);
}