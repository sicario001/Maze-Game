#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "GameModes.hpp"
class GameEngine
{
    private:
        GameMode* gMode;
        PlayMode* playMode;
        PauseMode* pauseMode;
        HomeMode* homeMode;
        bool quit_program = false;
    public:
        GameEngine();
        bool init();
        //The window we'll be rendering to
        SDL_Window* gWindow = NULL;

        //The window renderer
        SDL_Renderer* gRenderer = NULL;

        void runLoop();

        void setGameMode( GameModeType a);
};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 2;
