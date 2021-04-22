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
	player->sendUpdate(clientObj,serverObj);

	//check collision
	player->handleOutOfBounds(SCREEN_WIDTH,SCREEN_HEIGHT);
	tileMap->handleCollisions(player);
	player->handleCollision(otherPlayer);

	otherPlayer->handleOutOfBounds(SCREEN_WIDTH,SCREEN_HEIGHT);
	tileMap->handleCollisions(otherPlayer);
	otherPlayer->handleCollision(player);

	//Render wall
	// SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	tileMap->render();
	
	//Render players
	player->render();
	// uncomment if you want to see collision area
	// player->getCollisionRect()->render();
	otherPlayer->render();
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

void PlayMode::getPlayerClip(int i,SDL_Rect &clip){
	clip.h= PLAYER_SPRITE_H;
	clip.w= PLAYER_SPRITE_W;
	clip.x=0;
	clip.y = (3 +6*i)*PLAYER_SPRITE_H;
}

void PlayMode::initPlayers(){
	SDL_Rect clip1,clip2;
	int server_start_pos_x = 130;
	int server_start_pos_y = 180;
	int client_start_pos_x = 100;
	int client_start_pos_y = 70;
	if (clientObj!=NULL){
		getPlayerClip(SURVIVOR,clip1);
		getPlayerClip(SOLDIER,clip2);
		player = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip1);
		otherPlayer = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip2);
	}
	else{
		getPlayerClip(SOLDIER,clip1);
		getPlayerClip(SURVIVOR,clip2);
		player = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip1);
		otherPlayer = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip2);
	}
}
PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	tileMap = new TileMap();
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
		gPlayerTexture = new LTexture();
		initPlayers();
		tileMap = new TileMap();
		isPaused = false;
	}
}
void PlayMode::ReInit(){
	gPlayerTexture = new LTexture();
	initPlayers();
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

