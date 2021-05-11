#pragma once

#include <SDL2/SDL_image.h>

#include "engine/graphics/LTexture.hpp"

extern GameEngine* gEngine;
class HealthBar{
    private:
        int health;
        int damage;
        int x, y;
        SDL_Rect* healthBar = NULL;
        SDL_Rect* damageBar = NULL;
        SDL_Rect* BarBoundary = NULL;
        void UpdateBar();

    public:
        
        HealthBar();
        ~HealthBar();
        void setHealth(int h);
        int getHealth();
        int getDamage();
        void render();

};