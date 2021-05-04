#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include <string>
#include <sstream>

class Clock{
    private:
        int RoundTime;
        LTimer* timer = NULL;
		LTexture* gTimeTextTexture = NULL;
		std::stringstream timeText;
        LTexture* background = NULL;
		TTF_Font *gFont = NULL;

    public:
        Clock(int totalTime);
        ~Clock();
        void start();
        int getTime();
        void render();
        void loadMediaClock();


};