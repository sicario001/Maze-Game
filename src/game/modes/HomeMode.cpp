#include "game/GameModes.hpp"

HomeMode::HomeMode(){
	//Set sprites
	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	{
		gSpriteClips[ i ].x = 0;
		gSpriteClips[ i ].y = i * 200;
		gSpriteClips[ i ].w = BUTTON_WIDTH;
		gSpriteClips[ i ].h = BUTTON_HEIGHT;
	}
}

bool HomeMode::loadMediaHome(){
	std::cout << "load media home" << std::endl;
	bool success = true;

	gButtonSpriteSheetTexture = gEngine->textureStore->getSourceFor(TS_MENU);

	gButtons[ 0 ] = new LButton(BUTTON_PRIM,gButtonSpriteSheetTexture,gSpriteClips);
	gButtons[ 1 ] = new LButton(BUTTON_ALT,gButtonSpriteSheetTexture,gSpriteClips);
	gButtons[0]->loadAudio();
	gButtons[1]->loadAudio();
	gButtons[ 0 ]->setPosition( 0, 0 );
	gButtons[ 1 ]->setPosition( 0, BUTTON_HEIGHT +10);
	
	background = gEngine->textureStore->getSourceFor(TS_BG);
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