#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include <string>
#include <sstream>

class Clock{
    private:
        bool running;
        LTimer* timer = NULL;
		LTexture* gTimeTextTexture = NULL;
		std::stringstream timeText;
        LTexture* background = NULL;
		TTF_Font *gFont = NULL;

    public:
        Clock();
        ~Clock();
        void start();
        int RoundTime;
        void reset(int totalTime);
        int getTime();
        void render();
        void loadMediaClock();
        bool timeOver();
        bool isRunning();

};