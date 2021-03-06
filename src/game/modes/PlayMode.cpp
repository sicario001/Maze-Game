#include "game/GameModes.hpp"
#include "game/Player.hpp"
#include "game/Throwable.hpp"
#include "game/LoadingScreen.hpp"
#include "game/GameMessage.hpp"

void PlayMode::spawnThrowable(int x, int y, int speed, double angle, int damage, ThrowableType type){
	// cout << x<< " " <<y<< " " <<speed << " " <<angle<<endl;
	if(damage>0){
		if (clientObj!=NULL){
			if ((clientObj->peer)!=NULL){
				clientObj->SendDataThrowablePosVel(clientObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)), angle, type);
			}
		}
		else{
			if ((serverObj->peer)!=NULL){
				// std::cout<<"in\n";
				serverObj->SendDataThrowablePosVel(serverObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)), angle, type);
			}
		}
		playerThrowables.push_back(Throwable(x,y,speed,angle,damage,pbTexture[type], type));
		player->playThrowableSound(type);
	}
	else{
		otherPlayerThrowables.push_back(Throwable(x,y,speed,angle,damage,pbTexture[type], type));
		otherPlayer->playThrowableSound(type);
	}
}

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE:
			{
				// if bomb was being planted or defused, cancel it
				if (!player->isDead){
					player->stopReloading();
					if (progressBar!=NULL){
						delete(progressBar);
						progressBar = NULL;
						if (playerObj==ATTACK){
							updateBombState(IDLE,false);
						}
						else{
							updateBombState(PLANTED,false);
						}
					}
					// don't allow player movement
					player->stopMovement();
				}
				openPauseMenu = true;
				break;
			}
			case SDLK_e:
			{
				if (LoadingComplete && !roundOver){
					if (!player->isDead){
						if (progressBar==NULL && ((bombState==IDLE && playerObj==ATTACK)||((bomb!=NULL) && (bombState == PLANTED) && (playerObj == DEFEND) && (player->inVicinity(bombLocation, 50))))){
							player->stopReloading();
							
							if (playerObj==ATTACK){
								progressBar = new ProgressBar(10000);
								updateBombState(PLANTING,false);
							}
							else{
								progressBar = new ProgressBar(5000);
								updateBombState(DEFUSING,false);
							}
							player->stopMovement();
						}
					}
				}
			}
        }
    }
	if (e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_e:
			{
				if (LoadingComplete && !roundOver){
					if (!player->isDead){
						if (progressBar!=NULL){
							delete(progressBar);
							progressBar = NULL;
							if (playerObj==ATTACK){
								updateBombState(IDLE,false);
							}
							else{
								updateBombState(PLANTED,false);
							}
						}
					}
				}
			}
		}
	}
	if (LoadingComplete && !roundOver){
		if (!player->isDead){
			player->handleEvent(e);
		}
	}

}

void PlayMode::handleThrowables(vector<Throwable> &th, Player* p, function<void(Throwable&)> onHit){
	for (Throwable& x : th)
	{
		x.move();
		tileMap->handleThrowables(&x);
		if (!p->isDead){
			if(x.handleCollision(p)){
				if(!x.collided){
					onHit(x);
				}
				x.onHit();
			}
		}
		if(x.isActive){
			x.render();
			// x.getCollisionRect()->render();
		}
	}

	th.erase(std::remove_if(th.begin(),th.end(),[](Throwable& x){
		bool erasable = (!x.isActive) || x.handleOutOfBounds();
		if(erasable){
			x.free();
		}
		return erasable;
	}),th.end());
}

void PlayMode::StartNewRound(){
	currentRoundNum++;
	if (currentRoundNum> 2*totalRoundsInHalf){
		canReturnHome = true;
		return;
	}
	if (currentRoundNum == totalRoundsInHalf+1){
		swapSides = true;
		
	}
	InitRound();
}
void PlayMode::InitRound(){
	roundOver = false;
	roundEndMessageInit = false;
	bombState = IDLE;
	bombLocation = {-1, -1};
	bombBeepSound->rewind();
	player->free();
	otherPlayer->free();

	delete (player);
	delete (otherPlayer);
	delete (bomb);
	if (progressBar!=NULL){
		delete(progressBar);
	}
	progressBar = NULL;

	initPlayers();
	healthBar = new HealthBar();
	bomb = NULL; 
	clock->reset(RoundTime+RoundEndMessageDuration);
	player->inventory->loadMediaInventory();
	
	playerThrowables.clear();
	otherPlayerThrowables.clear();
	
	gEngine->stopbgm();
}
void PlayMode::update(bool render){
	
	if (openPauseMenu){
		openPauseMenu = false;
		Pause();
		gEngine->setGameMode(PAUSE);
		return;
	}
	if (roundOver){
		if (gameMessage->isActive()){
			if (render){
				gameMessage->render();
			}
			return;
		}
		else{
			if (currentRoundNum==totalRoundsInHalf && !gameHalfMessageInit){
				gameMessage->resetMessage("SWAPPING SIDES", 2000, SWAP_SIDES,false);
				gameHalfMessageInit = true;
				return;
			}
			if (currentRoundNum==2*totalRoundsInHalf && !gameEndMessageInit){
				int typeMessage;
				if (scoreBoard->getPlayerScore()>scoreBoard->getOtherPlayerScore()){
					typeMessage = WIN;
				}
				else if (scoreBoard->getPlayerScore()<scoreBoard->getOtherPlayerScore()){
					typeMessage = LOOSE;
				}
				else{
					typeMessage = DRAW;
				}
				gameMessage->resetMessage("GAME OVER", 2000, typeMessage,false);
				gameEndMessageInit = true;
				return;
			}
			// for disconnection in between the game
			if (currentRoundNum==-1){
				canReturnHome = true;
				return;
			}
			StartNewRound();
			return;
		}
		
	}
	if (LoadingComplete){
		loadingScreen->connecting_to_server = false;
		if(serverObj!=NULL && serverObj->peer!=NULL){
			pingStatus->updatePingStatus(serverObj->getPing());
		}
		if(clientObj!=NULL && clientObj->peer!=NULL){
			pingStatus->updatePingStatus(clientObj->getPing());
		}

		// handle movement of otherPlayer
		//Move the players
		player->move();
		otherPlayer->move();
		player->sendUpdate(clientObj,serverObj);

		//check collision
		player->handleOutOfBounds();
		if (!player->isDead){
			tileMap->handleCollisions(player);
		}
		if (!otherPlayer->isDead){
			player->handleCollision(otherPlayer);
		}
		

		otherPlayer->handleOutOfBounds();
		if (!otherPlayer->isDead){
			tileMap->handleCollisions(otherPlayer);
		}
		if (!player->isDead){
			otherPlayer->handleCollision(player);
		}

		player->resetCamera();

		
		otherPlayer->resetClip();
		if (render){
			//Render walls and tiles
			tileMap->render();

			//Render bomb
			if (bomb!=NULL){
				bomb->render();
			}
			
			//render other player
			otherPlayer->render();
		}

		
		
		// render bullets
		handleThrowables(playerThrowables,otherPlayer,[this](Throwable& x){
			if (clientObj!=NULL){
				if ((clientObj->peer)!=NULL){
					clientObj->SendHit(clientObj->peer, x.damage, currentRoundNum);
					otherPlayer->damage(x.damage);
				}
			}
			else{
				if ((serverObj->peer)!=NULL){
					serverObj->SendHit(serverObj->peer, x.damage, currentRoundNum);
					otherPlayer->damage(x.damage);
				}
			}
		});
		
		//Render player
		player->resetClip();
		if (render){
			player->render();
		}
		
		// render bullets
		handleThrowables(otherPlayerThrowables,player,[](Throwable& x){});
		
		healthBar->setHealth(player->getHealth());

		if (render){
			healthBar->render();

			clock->render();
			scoreBoard->render();
			pingStatus->render();
			player->inventory->render();
		}

		if (player->reloadBar!=NULL){
			if (player->isReloading){
				if (player->reloadBar->isComplete()){
					delete(player->reloadBar);
					player->reloadBar = NULL;
					player->stopReloading();
					player->inventory->reload();
				}
				else{
					if (render){
						player->reloadBar->render();
					}
				}
			}
			else{
				delete(player->reloadBar);
				player->reloadBar = NULL;
				player->stopReloading();
			}
		}

		if (progressBar!=NULL){
			if (progressBar->isComplete()){
				delete(progressBar);
				progressBar = NULL;
				if (playerObj==ATTACK){
					updateBombState(PLANTED,false);
				}
				else{
					updateBombState(DEFUSED,false);
				}
			}
			else{
				if (render){
					progressBar->render();
				}
			}
		}

		player->playSoundIfWalked(true);
		otherPlayer->playSoundIfWalked(false);
	}
	else{
		if (serverObj!=NULL){
			if (!serverObj->isConnected()){
				
				if (render)
					loadingScreen->render("Waiting for Client to Connect");			
			}
			else{
				if (render)
					loadingScreen->render("Waiting for Client to receive tile map");
			}
			if (serverObj->isConnected() && ClientMapInitialized && !mapSent){
				tileMap->sendMapToClient(serverObj);
				mapSent = true;
			}
		}
		else{
			if (!clientObj->isConnected()){
				loadingScreen->connecting_to_server = true;
				if (render)
					loadingScreen->render("Establishing connection with the server");
			}
			else{
				loadingScreen->connecting_to_server = false;
				if (render)
					loadingScreen->render("Waiting to receive tile map");
			}
			if (clientObj->isConnected() && isInitTileMap() && !tileMapInitSent){
				clientObj->SendTileMapInitialized();
				tileMapInitSent = true;
			}
		}
	}

	player->updateFireLock();
}

void PlayMode::sendBombState(){
	if (clientObj!=NULL){
		clientObj->SendBombState(bombState);
	}
	else{
		serverObj->SendBombState(bombState);
	}
}
void PlayMode::sendBombLocation(){
	if (clientObj!=NULL){
		clientObj->SendBombLocation(bombLocation);
	}
	else{
		serverObj->SendBombLocation(bombLocation);
	}
}

void PlayMode::bombPlanted(std::pair<int, int> location){
	cout << "planted" << endl;
	bombLocation = location;
	clock->reset(60000);
	bomb = new Entity((location.first)+(bombTexture->getWidth())/2, (location.second)+(bombTexture->getHeight())/2, bombTexture, NULL);
	updateBombState(PLANTED);
}

void PlayMode::bombDefused(){

}

void PlayMode::updateBombState(BombState state, bool ext){
	bombState = state;

	if(!ext){
		switch (state)
		{
			case PLANTING:
			case DEFUSING:
				player->stopMovement();
				break;
			
			default:
				player->allowMovement();
				break;
		}
		sendBombState();
	}
	switch (state)
	{
		case PLANTED:
			bombBeepSound->rewind();
			bombBeepSound->setPosition(bombLocation.first,bombLocation.second,0);
			bombBeepSound->play(true);
			if(!ext){
				if (bombLocation.first == -1){
					bombPlanted({player->getPosX(), player->getPosY()});
					sendBombLocation();
				}
			}
			break;
		case DEFUSED:
			bombBeepSound->rewind();
			break;
		default:
			break;
	}
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;
	//Load player bullet texture
	
	if( !pbTexture[0]->loadFromFile( "media/texture/bullet.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	if( !pbTexture[1]->loadFromFile( "media/texture/slashEffect.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	//Load player texture
	if( !gPlayerTexture->loadFromFile( "media/texture/spritesheet.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	if( !bombTexture->loadFromFile( "media/texture/c4.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	//Load wall texture
	if( !tileMap->loadTexture() )
	{
		printf( "Failed to load tile sheet texture!\n" );
		success = false;
	}
	clock->loadMediaClock();
	loadingScreen->loadMedia();
	gameMessage->loadMedia();
	scoreBoard->loadMedia();
	pingStatus->loadMedia();
	player->inventory->loadMediaInventory();
	gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 40);
	return success;
}
void PlayMode::freePlayMode(){
	tileMapInit = false;
	LoadingComplete = false;
	ClientMapInitialized = false;
	mapSent = false;
	tileMapInitSent = false;
	roundOver = false;
	canReturnHome = false;
	roundEndMessageInit = false;
	gameHalfMessageInit = false;
	gameEndMessageInit = false;
	swapSides = false;

	gPlayerTexture->free();
	delete(gPlayerTexture);
	gPlayerTexture = NULL;
	for (LTexture* x:pbTexture){
		x->free();
		delete(x);
		x = NULL;
	}
	bombTexture->free();
	bombBeepSound->free();

	delete(bombTexture);
	delete(bombBeepSound);

	bombTexture = NULL;
	bombBeepSound = NULL;

	delete (player);
	delete (otherPlayer);
	delete (tileMap);
	delete (bomb);
	delete (loadingScreen);
	delete (gameMessage);
	delete (scoreBoard);
	delete (pingStatus);
	if (progressBar!=NULL){
		delete (progressBar);
	}

	progressBar = NULL;
	loadingScreen = NULL;
	scoreBoard = NULL;
	pingStatus = NULL;
	gameMessage = NULL;
	bomb = NULL;
	player = NULL;
	otherPlayer = NULL;
	tileMap = NULL;
}

void PlayMode::getPlayerClip(int i,SDL_Rect &clip){
	clip.h= PLAYER_SPRITE_SIZE;
	clip.w= PLAYER_SPRITE_SIZE;
	clip.x=0;
	clip.y = (2 +6*i)*PLAYER_SPRITE_SIZE;
}

void PlayMode::initPlayers(){
	auto sf = [this](int x, int y, int speed, double angle, int damage, ThrowableType type){
		spawnThrowable(x,y,speed,angle,damage, type);
	};
	SDL_Rect clip1,clip2;
	int server_start_pos_x = 0;
	int server_start_pos_y = 0;
	int client_start_pos_x = 2500;
	int client_start_pos_y = 1860;

	if ((clientObj!=NULL)^swapSides){
		getPlayerClip(SURVIVOR,clip1);
		getPlayerClip(SOLDIER,clip2);
		player = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip1,sf, SURVIVOR);
		otherPlayer = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip2,sf, SOLDIER, true);
		playerObj = ATTACK;
	}
	else{
		getPlayerClip(SOLDIER,clip1);
		getPlayerClip(SURVIVOR,clip2);
		player = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip1,sf, SOLDIER);
		otherPlayer = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip2,sf, SURVIVOR, true);
		playerObj = DEFEND;
	}
	player->isDead = false;
	otherPlayer->isDead = false;
}
PlayMode::PlayMode(){}

PlayMode::PlayMode(bool flag, ClientNet* client, ServerNet* server){
	if (!flag){
		PlayMode();
	}
	else{
		// std::cout<<"PlayMode Initialized\n";
		clientObj = client;
		serverObj = server;
		pthread_mutex_init( &mutex, NULL);
    	pthread_cond_init( &initTileMapSignal, NULL);
	}
}
void PlayMode::initBombAudio(){
	if(bombBeepSound){
		bombBeepSound->free();
	}
	bombBeepSound = gEngine->audioStore->getSourceFor(AS_BOMB_BEEP_SOUND);
	bombBeepSound->setReferenceDistance(200);
	bombBeepSound->setRollOffFactor(1.5);
}

void PlayMode::ReInit(){
	if(player)
		player->free();
	if(otherPlayer)
		otherPlayer->free();

	bombState = IDLE;
	initBombAudio();

	gPlayerTexture = new LTexture();
	for (int i=0; i<(int)pbTexture.size(); i++){
		pbTexture[i] = new LTexture();
	}
	scoreBoard = new ScoreBoard();
	pingStatus = new PingStatus();
	bombTexture = new LTexture(0.1);
	tileMap = new TileMap(clientObj, serverObj);
	healthBar = new HealthBar();
	clock = new Clock();


	initPlayers();
	player->allowMovement();
	
	initTileMap();
	if (serverObj!=NULL){
		tileMap->generateTiles(serverObj);
	}
	// cout<<"in play\n";
	loadingScreen = new LoadingScreen();
	gameMessage = new GameMessage();
	loadMediaPlay();
	playerThrowables.clear();
	otherPlayerThrowables.clear();
	currentRoundNum = 1;
	isPaused = false;
}
void PlayMode::enterMode(){
	if (isPaused){
		unPause();
	}
	else{
		ReInit();
	}
}

void PlayMode::Pause(){
	isPaused = true;
}
void PlayMode::unPause(){
	player->allowMovement();
	isPaused = false;
}
void PlayMode::Reset(){
	isPaused = false;
}

void PlayMode::initTileMap(){
	pthread_mutex_lock(&mutex);
    tileMapInit = true;
	pthread_cond_signal(&initTileMapSignal);
    pthread_mutex_unlock(&mutex);
}
bool PlayMode::isInitTileMap(){
	pthread_mutex_lock(&mutex);
    bool init_val = tileMapInit;
    pthread_mutex_unlock(&mutex);
    return init_val;
}
void PlayMode::deInitTileMap(){
	pthread_mutex_lock(&mutex);
    tileMapInit = true;
    pthread_mutex_unlock(&mutex);
}

void PlayMode::waitForInitTileMap(){
	pthread_mutex_lock(&mutex);
    while (!tileMapInit){
        pthread_cond_wait(&initTileMapSignal, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void PlayMode::setWinner(int x){
	roundOver = true;
	if (x){
		roundWinner = DEFEND;
	}
	else{
		roundWinner = ATTACK;
	}
	if (playerObj==roundWinner){
		scoreBoard->incPlayerScore();
	}
	else{
		scoreBoard->incOtherPlayerScore();
	}
}

void PlayMode::updatePlayerToDead(int player_type){
	if (player_type){
		otherPlayer->isDead = true;
		otherPlayer->resetClip();
	}
	else{
		player->isDead = true;
		if (progressBar!=NULL){
			delete(progressBar);
			progressBar = NULL;
			if (playerObj==ATTACK){
				updateBombState(IDLE,false);
			}
			else{
				updateBombState(PLANTED,false);
			}
		}
		player->stopReloading();
		player->stopMovement();
		player->resetClip();
	}
}