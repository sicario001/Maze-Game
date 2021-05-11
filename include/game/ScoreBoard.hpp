#pragma once

#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/graphics/LTexture.hpp"

class ScoreBoard{
    private:
        int PlayerScore = 0;
        int OtherPlayerScore = 0;
		LTexture* gScoreTextTexture = NULL;
		std::stringstream scoreText;
        LTexture* background = NULL;
		TTF_Font *gFont = NULL;

    public:
        ScoreBoard();
        ~ScoreBoard();
        void reset();
        void render();
        void loadMedia();
        int getPlayerScore();
        int getOtherPlayerScore();
        void incPlayerScore();
        void incOtherPlayerScore();

};