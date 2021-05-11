#pragma once

#include <SDL2/SDL_image.h>

#include "engine/graphics/LTexture.hpp"

extern GameEngine* gEngine;
class ProgressBar{
    private:
        int totalTime;
        int timeElapsed;
        int x, y;
        Uint8 red, green, blue, alpha;
        SDL_Rect* progressBar;
        SDL_Rect* BarBoundary;
        LTimer* progressTimer;
        void UpdateBar();

    public:
        ProgressBar(int total,Uint8 r = 0, Uint8 g = 255, Uint8 b =0, Uint8 a =255, int x_pos = SCREEN_WIDTH/4, int y_pos = 3*SCREEN_HEIGHT/4);
        ~ProgressBar();
        void render();
        bool isComplete();

};