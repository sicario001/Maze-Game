#pragma once
#include <SDL2/SDL_image.h>
#include "LTexture.hpp"
extern GameEngine* gEngine;
class HealthBar{
    private:
        int health;
        int damage;
        int x, y;
        SDL_Rect* healthBar;
        SDL_Rect* damageBar;
        SDL_Rect* BarBoundary;
        void UpdateBar();

    public:
        
        HealthBar();
        ~HealthBar();
        void setHealth(int h);
        int getHealth();
        int getDamage();
        void render();

};