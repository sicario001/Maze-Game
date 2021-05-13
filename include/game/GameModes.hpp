#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>

#include "fwd.hpp"
#include "engine/audio/AudioSource.hpp"
#include "engine/graphics/LButton.hpp"
#include "engine/graphics/LTexture.hpp"
#include "engine/graphics/PhysicsObject.hpp"
#include "engine/graphics/ProgressBar.hpp"
#include "engine/net/ServerNet.hpp"
#include "engine/net/ClientNet.hpp"
#include "engine/GameEngine.hpp"
#include "game/Clock.hpp"
#include "game/HealthBar.hpp"
#include "game/Inventory.hpp"
#include "game/TileMap.hpp"
#include "game/ScoreBoard.hpp"

extern GameEngine* gEngine;

using namespace std;
class GameMode{
	public:
		virtual void eventHandler(SDL_Event &e){}
		virtual void update(bool render = true){}
		virtual void enterMode(){}


};

class HomeMode :public GameMode{
	private:
		SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
		LTexture* background;
		LTexture* gButtonSpriteSheetTexture = NULL;
		LButton* gButtons[ 2 ] = {NULL,NULL}; 
		bool loadMediaHome();
	public:
		HomeMode();
		void eventHandler(SDL_Event& e);
		void update(bool render = true);
		void enterMode();
};

class PauseMode :public GameMode{
	private:
		bool loadMediaPauseMenu();
		SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
		LTexture* gButtonSpriteSheetTexture = NULL;
		LTexture* background = NULL;
		LButton* gButtons[ 2 ] = {NULL,NULL}; 
	public:
		PauseMode();
		void eventHandler(SDL_Event& e);
		void update(bool render = true);
		void enterMode();
};

class PlayMode :public GameMode{
	private:
		bool openPauseMenu = false;
		bool isPaused;
		
		AudioSource* bombBeepSound = NULL;
		
		LTexture* gPlayerTexture = NULL;    
		vector<LTexture*> pbTexture = {NULL, NULL};
		ClientNet* clientObj = NULL;
		ServerNet* serverObj = NULL;

		vector<Throwable> playerThrowables;
		vector<Throwable> otherPlayerThrowables;
		
		void initPlayers();
		pthread_mutex_t mutex;
    	pthread_cond_t initTileMapSignal;
		bool loadMediaPlay();
		void eventHandler(SDL_Event& e);
		// change positions of objects and render
		

		void getPlayerClip(int i,SDL_Rect &clip);
		void initBombAudio();
	public:

		void update(bool render = true);


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
		void updateBombState(BombState state,bool ext = true);
		ScoreBoard* scoreBoard;
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
		void handleThrowables(vector<Throwable> &th, Player* p, function<void(Throwable&)> onHit);
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
		void StartNewRound();
		void InitRound();
		void updatePlayerToDead(int player_type);
		int currentRoundNum = 0;
		int totalRoundsInHalf = 2;
		bool swapSides = false;
		bool tileMapInit = false;
		bool LoadingComplete = false;
		bool ClientMapInitialized = false;
		bool mapSent = false;
		bool tileMapInitSent = false;
		bool roundStart = false;
		bool roundOver = false;
		bool canReturnHome = false;
		bool roundEndMessageInit = false;
		bool gameHalfMessageInit = false;
		bool gameEndMessageInit = false;

};