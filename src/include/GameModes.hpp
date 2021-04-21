#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "fwd.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"
#include "GameEngine.hpp"
#include "PhysicsObject.hpp"
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
		Player* player;
		TileMap* tileMap;
		bool isPaused;
		LTexture* gPlayerTexture;
		ClientNet* clientObj = NULL;
		ServerNet* serverObj = NULL;


		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		void update();
	public:
		Player* otherPlayer;
		PlayMode();
		PlayMode(bool flag, ClientNet* clientObj, ServerNet* serverObj);

		void ReInit();
		void enterMode();
		void Pause();
		void unPause();
		void Reset();
		void freePlayMode();
};