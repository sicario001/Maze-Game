#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include "fwd.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"
#include "GameEngine.hpp"
#include "PhysicsObject.hpp"
#include "HealthBar.hpp"
#include "ProgressBar.hpp"
#include "TileMap.hpp"
#include "ServerNet.hpp"
#include "ClientNet.hpp"
#include "Clock.hpp"
#include <string>
#include <sstream>

extern GameEngine* gEngine;

using namespace std;
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
		bool isPaused;
		
		LTexture* gPlayerTexture;    
		LTexture* pbTexture;
		ClientNet* clientObj = NULL;
		ServerNet* serverObj = NULL;

		vector<Bullet> playerBullets;
		vector<Bullet> otherPlayerBullets;

		void initPlayers();
		pthread_mutex_t mutex;
    	pthread_cond_t initTileMapSignal;
		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		// change positions of objects and render
		void update();

		void getPlayerClip(int i,SDL_Rect &clip);
	public:
		Player* player = NULL;
		PlayerObj playerObj;
		BombState bombState;
		std::pair<int, int> bombLocation = {-1, -1};
		int RoundTime = 120000;
		void sendBombState();
		void sendBombLocation();
		void bombPlanted(std::pair<int, int> location);
		void bombDefused();
		void updateBombState(int state);
		LTexture* bombTexture = NULL;
		Entity* bomb = NULL;
		Clock* clock = NULL;
		Player* otherPlayer = NULL;
		HealthBar* healthBar = NULL;
		ProgressBar* progressBar = NULL;
		TileMap* tileMap = NULL;
		LTexture* messageTextTexture = NULL;
		TTF_Font *gFont = NULL;
		std::stringstream messageText;
		bool tileMapInit = false;
		PlayMode();
		PlayMode(bool flag, ClientNet* clientObj, ServerNet* serverObj);

		void spawnBullet(int x, int y, int speed, double angle, int damage);
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