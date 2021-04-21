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
	tileMap->render();
	
	//Render players
	player->render();
	otherPlayer->render();
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
}

PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	tileMap = new TileMap();
	player = new Player(100,160,180,gPlayerTexture);
	otherPlayer = new Player(100,100,100,gPlayerTexture);
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
			player = new Player(100,100,100,gPlayerTexture);
			otherPlayer = new Player(100,160,180,gPlayerTexture);
		}
		else{
			player = new Player(100,160,180,gPlayerTexture);
			otherPlayer = new Player(100,100,100,gPlayerTexture);
		}
		tileMap = new TileMap();
		isPaused = false;
	}
}
void PlayMode::ReInit(){
	gPlayerTexture = new LTexture();
	if (clientObj!=NULL){
		player = new Player(100,100,100,gPlayerTexture);
		otherPlayer = new Player(100,160,180,gPlayerTexture);
	}
	else{
		player = new Player(100,160,180,gPlayerTexture);
		otherPlayer = new Player(100,100,100,gPlayerTexture);
	}
	tileMap = new TileMap();
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

