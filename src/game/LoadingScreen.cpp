#include "game/LoadingScreen.hpp"

LoadingScreen::LoadingScreen(){
    gLoadTextTexture = new LTexture();
    loadIcon = new LTexture();
    timer = new LTimer();
    timer->start();
}
LoadingScreen::~LoadingScreen(){
    delete(timer);
    loadIcon->free();
    gLoadTextTexture->free();
    timer = NULL;
}

void LoadingScreen::render(std::string text){
    loadText.str("");
    loadText<<text;
    gLoadTextTexture->loadFromRenderedText( loadText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
    gLoadTextTexture->render(SCREEN_WIDTH/4, 10, NULL, 1);
    loadIcon->render(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2-100, NULL, 1.0, (double)((timer->getTicks())%1000)*(0.36));
}
void LoadingScreen::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !loadIcon->loadFromFile( "media/texture/loadIcon.png" ) )
	{
		printf( "Failed to load loadIcon texture!\n" );
	}
}