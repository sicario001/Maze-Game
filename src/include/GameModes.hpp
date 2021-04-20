#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "fwd.hpp"
#include "PhysicsObject.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"
#include "GameEngine.hpp"

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
		RigidBody* wall;
		bool isPaused;
		LTexture* gDotTexture;
		LTexture* gWallTexture;


		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		void update();
	public:
		PlayMode();
		PlayMode(bool flag);

		void ReInit();
		void enterMode();
		void Pause();
		void unPause();
		void Reset();
		void freePlayMode();
};