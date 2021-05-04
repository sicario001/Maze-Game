#include "Clock.hpp"

Clock::Clock(int totalTime){
    gTimeTextTexture = new LTexture();
    timer = new LTimer();
    background = new LTexture();
    RoundTime =  totalTime;

}
void Clock::render(){
    timeText.str( "" );
	timeText << ( (int)(RoundTime - timer->getTicks()) / 1000 );
	gTimeTextTexture->loadFromRenderedText( timeText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
	gTimeTextTexture->render(SCREEN_WIDTH/2, 10, NULL, 1);
    background->render(SCREEN_WIDTH/2-50, 15, NULL, 0.75);
}

void Clock::start(){
    timer->start();
}
int Clock::getTime(){
    return (timer->getTicks());
}
void Clock::loadMediaClock(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !background->loadFromFile( "media/texture/clock.png" ) )
	{
		printf( "Failed to load clock texture!\n" );
	}

}