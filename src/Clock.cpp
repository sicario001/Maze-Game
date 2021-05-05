#include "Clock.hpp"

Clock::Clock(){
    gTimeTextTexture = new LTexture();
    timer = new LTimer();
    background = new LTexture();
    running = false;

}
void Clock::reset(int totalTime){
    running = true;
    timer->stop();
    RoundTime = totalTime;
    start();
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
bool Clock::isRunning(){
    return running;
}
bool Clock::timeOver(){
    if (running){
        if ((int)timer->getTicks()>RoundTime){
            return true;
        }
    }
    return false;
}