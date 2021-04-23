#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "GameModes.hpp"
#include "ClientNet.hpp"
#include "ServerNet.hpp"

enum GameModes{
	HOME_MODE = 0,
	PLAY_MODE = 1,
	PAUSE_MODE = 2
};

class GameEngine
{
    private:
        GameMode* gMode;
        PauseMode* pauseMode;
        HomeMode* homeMode;

        

    public:
        SDL_Rect* camera;

        PlayMode* playMode;
    	GameModes currMode;
        ClientNet* clientObj = NULL;
        ServerNet* serverObj = NULL;
        GameEngine(int c_or_s);
        bool quit_program = false;
        bool init();

        //The window we'll be rendering to
        SDL_Window* gWindow = NULL;

        //The window renderer
        SDL_Renderer* gRenderer = NULL;

        void runLoop();

        void setGameMode( GameModeType a);
        void updateOtherPlayer(std::vector<int> &data);
        void updateMapfromServer(std::vector<int> &received_data);
};

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

const int CAMERA_WIDTH = 480;
const int CAMERA_HEIGHT = 270;

const int LEVEL_WIDTH = 2560;
const int LEVEL_HEIGHT = 1920;

const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 2;
