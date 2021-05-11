#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include <string>
#include <sstream>

class LoadingScreen{
    private:
		LTexture* gLoadTextTexture = NULL;
		std::stringstream loadText;
        LTexture* loadIcon = NULL;
        LTexture* background = NULL;
		TTF_Font *gFont = NULL;
        LTimer* timer = NULL;

    public:
        LoadingScreen();
        ~LoadingScreen();
        void render(std::string text);
        void loadMedia();
};