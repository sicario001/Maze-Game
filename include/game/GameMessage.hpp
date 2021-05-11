#pragma once

#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/graphics/LTexture.hpp"
#include "game/LTimer.hpp"
class GameMessage{
    private:
        LTexture* messageTexture = NULL;
        LTimer* timer = NULL;
		std::stringstream messageText;
        LTexture* background_ct = NULL;
        LTexture* background_t = NULL;
        LTexture* background = NULL;
        TTF_Font *gFont = NULL;
        Uint32 totalTime;
        bool active = false;
        int background_type = 0;

    public:
        GameMessage();
        ~GameMessage();
        void render();
        void loadMedia();
        void resetMessage(std::string message, Uint32 duration, int type);
        bool isActive();
};
