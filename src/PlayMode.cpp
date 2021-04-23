#include "GameModes.hpp"

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN && e.key.repeat==0)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
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
	//Move the players
	player->move();
	otherPlayer->move();

	//check collision
	player->handleOutOfBounds(SCREEN_WIDTH,SCREEN_HEIGHT);
	tileMap->handleCollisions(player);
	player->handleCollision(otherPlayer);
	

	otherPlayer->handleOutOfBounds(SCREEN_WIDTH,SCREEN_HEIGHT);
	tileMap->handleCollisions(otherPlayer);
	

	player->sendUpdate(clientObj,serverObj);
	//Render wall
	// SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	tileMap->renderLocal(player);
	
	
	//Render players
	player->renderLocal();
	otherPlayer->renderLocal(player);

}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;

	//Load player texture
	if( !gPlayerTexture->loadFromFile( "media/texture/square.bmp" ) )
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
	return success;
}
void PlayMode::freePlayMode(){
	gPlayerTexture->free();
	delete (player);
	delete (otherPlayer);
	delete (tileMap);
	player = NULL;
	otherPlayer = NULL;
	tileMap = NULL;
}

PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	// tileMap = new TileMap(clientObj, serverObj);
	player = new Player(100,10,10,gPlayerTexture);
	otherPlayer = new Player(100,1260,940,gPlayerTexture);

};

PlayMode::PlayMode(bool flag, ClientNet* client, ServerNet* server){
	if (!flag){
		PlayMode();
	}
	else{
		// std::cout<<"PlayMode Initialized\n";
		clientObj = client;
		serverObj = server;
		gPlayerTexture = new LTexture();
		if (clientObj!=NULL){
			player = new Player(100,10,10,gPlayerTexture);
			otherPlayer = new Player(100,1260,940,gPlayerTexture);
		}
		else{
			player = new Player(100,1260,940,gPlayerTexture);
			otherPlayer = new Player(100,10,10,gPlayerTexture);
		}
		isPaused = false;
		pthread_mutex_init( &mutex, NULL);
    	pthread_cond_init( &initTileMapSignal, NULL);
	}
}
void PlayMode::ReInit(){
	
	gPlayerTexture = new LTexture();
	if (clientObj!=NULL){
		player = new Player(100,10,10,gPlayerTexture);
		otherPlayer = new Player(100,1260,940,gPlayerTexture);
	}
	else{
		player = new Player(100,1260,940,gPlayerTexture);
		otherPlayer = new Player(100,10,10,gPlayerTexture);
	}
	deInitTileMap();
	
	tileMap = new TileMap(clientObj, serverObj);
	// cout<<"in\n";
	tileMap->waitToReceiveMap();
	// cout<<"in\n";
	tileMap->generateTiles(clientObj, serverObj);

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