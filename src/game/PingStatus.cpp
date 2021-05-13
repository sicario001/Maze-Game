#include "game/PingStatus.hpp"

PingStatus::PingStatus(){
    reset();
}
PingStatus::~PingStatus(){
}
void PingStatus::reset(){
    ping = 0;
}
void PingStatus::render(){
    pingText.str( "" );
	pingText <<ping;
	gPingTextTexture->loadFromRenderedText( pingText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
	gPingTextTexture->render(300+SCREEN_WIDTH/2, 10, NULL, 1);
}
void PingStatus::updatePingStatus(int p){
    ping = p;
}
void::PingStatus::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
}