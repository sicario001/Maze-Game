#pragma once
#include <SDL2/SDL_image.h>
#include "LTexture.hpp"
extern GameEngine* gEngine;
class ProgressBar{
    private:
        int totalTime;
        int timeElapsed;
        int x, y;
        SDL_Rect* progressBar;
        SDL_Rect* BarBoundary;
        LTimer* progressTimer;
        void UpdateBar();

    public:
        ProgressBar(int total,int x_pos = SCREEN_WIDTH/4, int y_pos = 3*SCREEN_HEIGHT/4);
        ~ProgressBar();
        void render();
        bool isComplete();

};