#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include <string>
#include <sstream>

class GameMessage{
    private:
        LTexture* messageTexture = NULL;
        LTimer* timer = NULL;
		std::stringstream messageText;
        TTF_Font *gFont = NULL;
        Uint32 totalTime;
        bool active = false;

    public:
        GameMessage();
        ~GameMessage();
        void render();
        void loadMedia();
        void resetMessage(std::string message, Uint32 duration);
        bool isActive();
};