#include "game/GameMessage.hpp"

GameMessage::GameMessage(){
    messageTexture = new LTexture();
    background_ct = new LTexture();
    background_t = new LTexture();
    background = new LTexture();
    timer = new LTimer();
    active = false;

}
GameMessage::~GameMessage(){
    if(explosionSound)
        explosionSound->free();
    messageTexture->free();
    background_ct->free();
    background_t->free();
    background->free();
    delete(timer);
    timer = NULL;

}
void GameMessage::render(){
    switch (background_type)
    {
    case T_WIN:
        background_t->renderBackground(NULL);
        break;
    case CT_WIN:
        background_ct->renderBackground(NULL);
        break;    
    default:
        background->renderBackground(NULL);
        break;
    }
    messageTexture->loadFromRenderedText( messageText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
    messageTexture->render((SCREEN_WIDTH -messageTexture->getWidth())/2, 10, NULL, 1);
    
}
void GameMessage::resetMessage(std::string message, Uint32 duration, int type, bool explosion){
    active = true;
    timer->stop();
    timer->start();
    totalTime = duration;
    messageText.str("");
    messageText<<message;
    switch (type)
    {
        case CT_WIN:
            messageText<<"  |  DEFENDERS WIN";
            break;
        case T_WIN:
            messageText<<"  |  ATTACKERS WIN";
            break;
        case DRAW_DISCONNECT:
            messageText<<"  |  DISCONNECTED";
            break;
        case WIN:
            messageText<<"  |  YOU WIN";
            break;
        case LOOSE:
            messageText<<"  |  YOU LOSE";
            break;
        case DRAW:
            messageText<<"  |  YOU LOSE";
            break;
        default:
            break;
    }
    background_type = type;
    if(explosion){
        explosionSound->rewind();
        explosionSound->play();
    }
    gEngine->startbgm();
}
void GameMessage::loadMedia(){
    if(explosionSound==NULL){
        explosionSound = gEngine->audioMaster.loadWaveFile("media/audio/explosion.wav");
    }
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !background_ct->loadFromFile( "media/texture/ct_win.png" ) )
	{
		printf( "Failed to load background texture!\n" );
	}
    if( !background_t->loadFromFile( "media/texture/t_win.png" ) )
	{
		printf( "Failed to load background texture!\n" );
	}
    if( !background->loadFromFile( "media/texture/background_end.png" ) )
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