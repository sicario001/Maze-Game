#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "fwd.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"
#include "GameEngine.hpp"
#include "PhysicsObject.hpp"
#include "HealthBar.hpp"
#include "TileMap.hpp"
#include "ServerNet.hpp"
#include "ClientNet.hpp"


extern GameEngine* gEngine;


class GameMode{
	public:
		virtual void eventHandler(SDL_Event &e){}
		virtual void update(){}
		virtual void enterMode(){}


};

class HomeMode :public GameMode{
	private:
		SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
		LTexture* gButtonSpriteSheetTexture;
		LButton* gButtons[ 2 ]; 
		bool loadMediaHome();
	public:
		HomeMode();
		void eventHandler(SDL_Event& e);
		void update();
		void enterMode();
};

class PauseMode :public GameMode{
	private:
		bool loadMediaPauseMenu();
		SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
		LTexture* gButtonSpriteSheetTexture;
		LButton* gButtons[ 2 ]; 
	public:
		PauseMode();
		void eventHandler(SDL_Event& e);
		void update();
		void enterMode();
};

class PlayMode :public GameMode{
	private:
		bool openPauseMenu = false;
		Player* player = NULL;
		
		bool isPaused;
		LTexture* gPlayerTexture;
		ClientNet* clientObj = NULL;
		ServerNet* serverObj = NULL;

		void initPlayers();
		pthread_mutex_t mutex;
    	pthread_cond_t initTileMapSignal;
		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		// change positions of objects and render
		void update();

		void getPlayerClip(int i,SDL_Rect &clip);
	public:
		Player* otherPlayer = NULL;
		HealthBar* healthBar = NULL;
		TileMap* tileMap = NULL;
		bool tileMapInit = false;
		PlayMode();
		PlayMode(bool flag, ClientNet* clientObj, ServerNet* serverObj);

		void ReInit();
		void enterMode();
		void Pause();
		void unPause();
		void Reset();
		void freePlayMode();
		void initTileMap();
		bool isInitTileMap();
		void deInitTileMap();
		void waitForInitTileMap();
};