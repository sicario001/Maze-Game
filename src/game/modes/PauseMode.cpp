#include "game/GameModes.hpp"

PauseMode::PauseMode(){
	gButtonSpriteSheetTexture = new LTexture();
	background = new LTexture();
	//Set sprites
	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	{
		gSpriteClips[ i ].x = 0;
		gSpriteClips[ i ].y = i * 200;
		gSpriteClips[ i ].w = BUTTON_WIDTH;
		gSpriteClips[ i ].h = BUTTON_HEIGHT;
	}
	gButtons[ 0 ] = new LButton(BUTTON_PRIM,gButtonSpriteSheetTexture,gSpriteClips);
	gButtons[ 1 ] = new LButton(BUTTON_ALT,gButtonSpriteSheetTexture,gSpriteClips);
}

bool PauseMode::loadMediaPauseMenu(){
	bool success = true;
	gButtons[0]->loadAudio();
	gButtons[1]->loadAudio();
	if( !gButtonSpriteSheetTexture->loadFromFile( "media/texture/sprites/pause_menu_new.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	if( !background->loadFromFile( "media/texture/pause_menu_background.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	else
	{
		//Set buttons in corners
		gButtons[ 0 ]->setPosition( SCREEN_WIDTH-BUTTON_WIDTH-20, 10 );
		gButtons[ 1 ]->setPosition( SCREEN_WIDTH-BUTTON_WIDTH-20, BUTTON_HEIGHT+10 );
	}
	return success;
}

void PauseMode::eventHandler(SDL_Event& e){
	//Handle button events
	for( int i = 0; i < TOTAL_BUTTONS; ++i )
	{
		gButtons[ i ]->handleEvent( &e );
	}
}

void PauseMode::update(){
	// std::cout << "update pause media" << std::endl;
	//Render buttons
	background->renderBackground(NULL);
	for( int i = 0; i < TOTAL_BUTTONS; ++i )
	{
		gButtons[ i ]->render();
	}
	//Select mode
	if (gButtons[0]->getMode()){
		gButtons[0]->setMode(0);
		gEngine->setGameMode(RESUME);
	}
	else if (gButtons[1]->getMode()){
		gButtons[1]->setMode(0);
		if (gEngine->serverObj!=NULL){
			gEngine->serverObj->sendDisconnectRequest();
		}
		gEngine->setGameMode(HOME);
	}
}

void PauseMode::enterMode(){
	if( !loadMediaPauseMenu() )
	{
		printf( "Failed to load media!\n" );
	}
	else{
		std::cout << "loaded pause media" << std::endl;
	}
}