#pragma once

#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/graphics/LTexture.hpp"
#include "game/LTimer.hpp"

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