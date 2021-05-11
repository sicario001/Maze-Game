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
#include "Inventory.hpp"
#include "TileMap.hpp"
#include "loadingScreen.hpp"
#include "gameMessage.hpp"
#include "ServerNet.hpp"
#include "ClientNet.hpp"
#include "Clock.hpp"
#include "AudioSource.hpp"
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
		LTexture* background;
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
		
		AudioSource* bombBeepSound;
		
		LTexture* gPlayerTexture;    
		vector<LTexture*> pbTexture = {NULL, NULL};
		ClientNet* clientObj = NULL;
		ServerNet* serverObj = NULL;

		vector<Throwable> playerThrowables;
		vector<Throwable> otherPlayerThrowables;
		void update();
		void initPlayers();
		pthread_mutex_t mutex;
    	pthread_cond_t initTileMapSignal;
		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		// change positions of objects and render
		

		void getPlayerClip(int i,SDL_Rect &clip);
		void initBombAudio();
	public:

		
		void updateInPauseMode();


		Player* player = NULL;
		PlayerObj playerObj;
		PlayerObj roundWinner;
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
		LoadingScreen* loadingScreen = NULL;
		GameMessage* gameMessage = NULL;
		Player* otherPlayer = NULL;
		HealthBar* healthBar = NULL;
		ProgressBar* progressBar = NULL;
		TileMap* tileMap = NULL;
		LTexture* messageTextTexture = NULL;
		TTF_Font *gFont = NULL;
		std::stringstream messageText;
		

		PlayMode();
		PlayMode(bool flag, ClientNet* clientObj, ServerNet* serverObj);
		

		void spawnThrowable(int x, int y, int speed, double angle, int damage, ThrowableType type);
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
		void setWinner(int x);
		bool tileMapInit = false;
		bool LoadingComplete = false;
		bool ClientMapInitialized = false;
		bool mapSent = false;
		bool tileMapInitSent = false;
		bool roundStart = false;
		bool roundOver = false;
		bool canReturnHome = false;
		bool roundEndMessageInit = false;

};