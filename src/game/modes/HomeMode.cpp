#include "game/GameModes.hpp"

HomeMode::HomeMode(){
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

bool HomeMode::loadMediaHome(){
	std::cout << "load media home" << std::endl;
	bool success = true;
	gButtons[0]->loadAudio();
	gButtons[1]->loadAudio();
	if( !gButtonSpriteSheetTexture->loadFromFile( "media/texture/sprites/menu_new.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set buttons in corners
		gButtons[ 0 ]->setPosition( 0, 0 );
		gButtons[ 1 ]->setPosition( 0, BUTTON_HEIGHT +10);
	}
	if (!background->loadFromFile("media/texture/background.png")){
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	return success;
}

void HomeMode::update(bool render){
	
	background->renderBackground(NULL);
	//Render buttons
	for( int i = 0; i < TOTAL_BUTTONS; ++i )
	{
		gButtons[ i ]->render();
	}
	//Select mode
	if (gButtons[0]->getMode()){
		gButtons[0]->setMode(0);
		gEngine->setGameMode(PLAY);
	}
	else if (gButtons[1]->getMode()){
		gButtons[1]->setMode(0);
		gEngine->setGameMode(QUIT);
	}
}

void HomeMode::eventHandler(SDL_Event &e){
	//Handle button events
	for( int i = 0; i < TOTAL_BUTTONS; ++i )
	{
		gButtons[ i ]->handleEvent( &e );
	}
}

void HomeMode::enterMode(){
	if( !loadMediaHome() )
	{
		printf( "Failed to load media!\n" );
	}
}