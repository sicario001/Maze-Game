#pragma once

#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/graphics/LTexture.hpp"

class PingStatus{
    private:
        int ping;
		LTexture* gPingTextTexture = NULL;
		std::stringstream pingText;
        LTexture* background = NULL;
		TTF_Font *gFont = NULL;

    public:
        PingStatus();
        ~PingStatus();
        void reset();
        void render();
        void loadMedia();
        void updatePingStatus(int p);
};