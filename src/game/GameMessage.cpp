#include "game/GameMessage.hpp"

GameMessage::GameMessage(){
    messageTexture = new LTexture();
    background_ct = new LTexture();
    background_t = new LTexture();
    timer = new LTimer();
    active = false;

}
GameMessage::~GameMessage(){
    messageTexture->free();
    background_ct->free();
    background_t->free();
    delete(timer);
    timer = NULL;

}
void GameMessage::render(){
    if (background_type){
        background_ct->renderBackground(NULL);
    }
    else{
        background_t->renderBackground(NULL);
    }
    messageTexture->loadFromRenderedText( messageText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
    messageTexture->render((SCREEN_WIDTH -messageTexture->getWidth())/2, 10, NULL, 1);
    
}
void GameMessage::resetMessage(std::string message, Uint32 duration, int type){
    active = true;
    timer->stop();
    timer->start();
    totalTime = duration;
    messageText.str("");
    messageText<<message;
    if (type==1){
        messageText<<"  |  DEFENDERS WIN";
    }
    else if (type==0){
        messageText<<"  |  ATTACKERS WIN";
    }
    background_type = type;

}
void GameMessage::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !background_ct->loadFromFile( "media/texture/ct_win.png" ) )
	{
		printf( "Failed to load background texture!\n" );
	}
    if( !background_t->loadFromFile( "media/texture/t_win.png" ) )
	{
		printf( "Failed to load background texture!\n" );
	}
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