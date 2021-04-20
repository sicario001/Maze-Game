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
	//Move the dot
	player->move();

	//check collision
	player->handleOutOfBounds(SCREEN_WIDTH,SCREEN_HEIGHT);
	player->handleCollision(wall);

	//Render wall
	SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	wall->render();
	
	//Render player
	player->render();
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture->loadFromFile( "media/texture/square.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	//Load wall texture
	if( !gWallTexture->loadFromFile( "media/texture/wall.bmp" ) )
	{
		printf( "Failed to load wall texture!\n" );
		success = false;
	}
	return success;
}
void PlayMode::freePlayMode(){
	gDotTexture->free();
}

PlayMode::PlayMode(){
	gDotTexture = new LTexture();
	gWallTexture = new LTexture();
	player = new Player(100,20,20,gDotTexture);
};

PlayMode::PlayMode(bool flag){
	if (!flag){
		PlayMode();
	}
	else{
		gDotTexture = new LTexture();
		gWallTexture = new LTexture();
		player = new Player(100,20,20,gDotTexture);
		wall = new RigidBody(300,40,gWallTexture,new CollisionRect(300,40,WALL_W,WALL_H));
		isPaused = false;
	}
}
void PlayMode::ReInit(){
	gDotTexture = new LTexture();
	player = new Player(100,20,20,gDotTexture);
	wall = new RigidBody(300,40,gWallTexture,new CollisionRect(300,40,40,400));
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

