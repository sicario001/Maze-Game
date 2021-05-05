#include "GameModes.hpp"

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
			case SDLK_e:
			{
				if (progressBar==NULL && ((bombState==IDLE && playerObj==ATTACK)||((bomb!=NULL) && (bombState == PLANTED) && (playerObj == DEFEND) && (player->inVicinity(bombLocation, 50))))){
					progressBar = new ProgressBar(10000);
					if (playerObj==ATTACK){
						bombState = PLANTING;
						player->stopMovement();
						sendBombState();
					}
					else{
						bombState = DEFUSING;
						player->stopMovement();
						sendBombState();
					}
					player->stopMovement();
				}
			}
        }
    }
	if (e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_e:
			{
				if (progressBar!=NULL){
					delete(progressBar);
					progressBar = NULL;
					if (playerObj==ATTACK){
						bombState = IDLE;
						player->allowMovement();
						sendBombState();
					}
					else{
						bombState = PLANTED;
						player->allowMovement();
						sendBombState();
					}
				}
			}
		}
	}
	player->handleEvent(e);

}

void PlayMode::update(){
	if (openPauseMenu){
		openPauseMenu = false;
		Pause();
		gEngine->setGameMode(PAUSE);
		return;
	}
	// cout<<"in\n";
	
	// handle movement of otherPlayer
	if (playerObj!=ATTACK){
		if (bombState==PLANTING){
			otherPlayer->stopMovement();
		}
		else{
			otherPlayer->allowMovement();
		}
	}
	else{
		if (bombState==DEFUSING){
			otherPlayer->stopMovement();
		}
		else{
			otherPlayer->allowMovement();
		}
	}
	//Move the players
	player->move();
	otherPlayer->move();
	player->sendUpdate(clientObj,serverObj);

	//check collision
	player->handleOutOfBounds();
	tileMap->handleCollisions(player);
	player->handleCollision(otherPlayer);
	

	otherPlayer->handleOutOfBounds();
	tileMap->handleCollisions(otherPlayer);
	otherPlayer->handleCollision(player);

	player->resetCamera();

	
	//Render wall
	// SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	tileMap->render();

	if (bomb!=NULL){
		bomb->render();
	}
	
	//Render players
	player->render();
	otherPlayer->render();
	healthBar->setHealth(player->getHealth());
	healthBar->render();
	clock->render();
	if (progressBar!=NULL){
		if (progressBar->isComplete()){
			delete(progressBar);
			progressBar = NULL;
			if (playerObj==ATTACK){
				bombState = PLANTED;
				player->allowMovement();
				bombPlanted({player->getPosX(), player->getPosY()});
				sendBombState();
				sendBombLocation();
			}
			else{
				bombState = DEFUSED;
				player->allowMovement();
				sendBombState();
			}
		}
		else{
			progressBar->render();
		}
	}


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
	bombLocation = location;
	clock->reset(60000);
	bomb = new Entity((location.first)+(bombTexture->getWidth())/2, (location.second)+(bombTexture->getHeight())/2, bombTexture, NULL);

}
void PlayMode::bombDefused(){

}
void PlayMode::updateBombState(int state){
	if (state==0){
		bombState = IDLE;
	}
	else if (state==1){
		bombState = PLANTING;
	}
	else if (state==2){
		bombState = PLANTED;
	}
	else if (state==3){
		bombState = DEFUSING;
	}
	else{
		bombState = DEFUSED;
		bombDefused();
	}
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;

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
	gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 40);
	return success;
}
void PlayMode::freePlayMode(){
	gPlayerTexture->free();
	bombTexture->free();
	delete (player);
	delete (otherPlayer);
	delete (tileMap);
	delete (bomb);
	bomb = NULL;
	player = NULL;
	otherPlayer = NULL;
	tileMap = NULL;
}

void PlayMode::getPlayerClip(int i,SDL_Rect &clip){
	clip.h= PLAYER_SPRITE_H;
	clip.w= PLAYER_SPRITE_W;
	clip.x=0;
	clip.y = (3 +6*i)*PLAYER_SPRITE_H;
}

void PlayMode::initPlayers(){
	SDL_Rect clip1,clip2;
	int server_start_pos_x = 0;
	int server_start_pos_y = 0;
	int client_start_pos_x = 2500;
	int client_start_pos_y = 1860;
	if (clientObj!=NULL){
		getPlayerClip(SURVIVOR,clip1);
		getPlayerClip(SOLDIER,clip2);
		player = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip1);
		otherPlayer = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip2);
		playerObj = ATTACK;
	}
	else{
		getPlayerClip(SOLDIER,clip1);
		getPlayerClip(SURVIVOR,clip2);
		player = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip1);
		otherPlayer = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip2);
		playerObj = DEFEND;
	}
}
PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	bombTexture = new LTexture(0.1);
	healthBar = new HealthBar();
	initPlayers();
};

PlayMode::PlayMode(bool flag, ClientNet* client, ServerNet* server){
	if (!flag){
		PlayMode();
	}
	else{
		// std::cout<<"PlayMode Initialized\n";
		clientObj = client;
		serverObj = server;
		bombState = IDLE;
		gPlayerTexture = new LTexture();
		bombTexture = new LTexture(0.1);
		healthBar = new HealthBar();
		clock = new Clock();
		initPlayers();
		isPaused = false;
		pthread_mutex_init( &mutex, NULL);
    	pthread_cond_init( &initTileMapSignal, NULL);
	}
}
void PlayMode::ReInit(){
	bombState = IDLE;
	gPlayerTexture = new LTexture();
	bombTexture = new LTexture(0.1);
	initPlayers();
	player->allowMovement();
	deInitTileMap();
	
	tileMap = new TileMap(clientObj, serverObj);
	// cout<<"in\n";
	tileMap->waitToReceiveMap();
	// cout<<"in\n";
	tileMap->generateTiles(clientObj, serverObj);
	clock->reset(RoundTime);
	// cout<<"in play\n";
	loadMediaPlay();
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