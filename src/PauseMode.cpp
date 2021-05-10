#include "GameModes.hpp"

PauseMode::PauseMode(){
	gButtonSpriteSheetTexture = new LTexture();
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
	if( !gButtonSpriteSheetTexture->loadFromFile( "media/texture/sprites/pause_menu.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set buttons in corners
		gButtons[ 0 ]->setPosition( (SCREEN_WIDTH-BUTTON_WIDTH)/2, (SCREEN_HEIGHT/2- BUTTON_HEIGHT) );
		gButtons[ 1 ]->setPosition( (SCREEN_WIDTH-BUTTON_WIDTH)/2, SCREEN_HEIGHT/2 );
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