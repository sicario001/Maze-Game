#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "fwd.hpp"
#include "engine/net/ClientNet.hpp"
#include "engine/net/ServerNet.hpp"
#include "engine/audio/AudioMaster.hpp"
#include "game/GameModes.hpp"

enum GameModes{
	HOME_MODE = 0,
	PLAY_MODE = 1,
	PAUSE_MODE = 2
};

class GameEngine
{
    private:
        GameMode* gMode = NULL;
        PauseMode* pauseMode = NULL;
        HomeMode* homeMode = NULL;

        AudioSource* explosionSound = NULL;
        
    public:
        AudioMaster audioMaster;

        SDL_Rect* camera = NULL;

        PlayMode* playMode = NULL;
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
        bool gameStartMessage = false;
        bool gameEndMessage = false;

        void runLoop();

        void setGameMode( GameModeType a);
        void updateOtherPlayer(std::vector<int> &data);
        void addThrowableToVector(std::vector<int> &data);
        void damagePlayer(std::vector<int> &data);
        void updateMapfromServer(std::vector<int> &received_data);
        void checkRoundEnd();
        void resetListener(int x, int y);
};


