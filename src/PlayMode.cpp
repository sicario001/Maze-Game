#include "GameModes.hpp"

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN && e.key.repeat==0)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
        }
    }
	dot->handleEvent(e);
}

void PlayMode::update(){
	if (openPauseMenu){
		openPauseMenu = false;
		Pause();
		gEngine->setGameMode(PAUSE);
		return;
	}
	//Move the dot and check collision
	dot->move( wall, otherDot->getCollider() );
	//Render wall
	SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	SDL_RenderDrawRect( gEngine->gRenderer, &wall );
	
	//Render dots
	dot->render(*gDotTexture);
	otherDot->render(*gDotTexture);
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture->loadFromFile( "media/texture/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	return success;
}
void PlayMode::freePlayMode(){
	gDotTexture->free();
}

PlayMode::PlayMode(){
	dot = new Dot();
	otherDot =new Dot();
	gDotTexture = new LTexture();
};

PlayMode::PlayMode(bool flag){
	if (!flag){
		PlayMode();
	}
	else{
		dot = new Dot(Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2 );
		otherDot = new Dot( SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 );
		gDotTexture = new LTexture();
		wall.x = 300;
		wall.y = 40;
		wall.w = 40;
		wall.h = 400;
		isPaused = false;
	}
}
void PlayMode::ReInit(){
	dot = new Dot(Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2 );
	otherDot =new Dot( SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 );
	gDotTexture = new LTexture();
	loadMediaPlay();
	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400; 
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

