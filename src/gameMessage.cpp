#include "gameMessage.hpp"

GameMessage::GameMessage(){
    messageTexture = new LTexture();
    timer = new LTimer();
    active = false;

}
GameMessage::~GameMessage(){
    messageTexture->free();
    delete(timer);
    timer = NULL;

}
void GameMessage::render(){
    messageTexture->loadFromRenderedText( messageText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
    messageTexture->render((SCREEN_WIDTH -messageTexture->getWidth())/2, 10, NULL, 1);
}
void GameMessage::resetMessage(std::string message, Uint32 duration){
    active = true;
    timer->stop();
    timer->start();
    totalTime = duration;
    messageText.str("");
    messageText<<message;

}
void GameMessage::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
}
bool GameMessage::isActive(){
    if (!active){
        return false;
    }
    if (timer->getTicks()<totalTime){
        return true;
    }
    else{
        active = false;
        return false;
    }
}