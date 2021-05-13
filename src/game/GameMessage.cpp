#include "game/GameMessage.hpp"

GameMessage::GameMessage(){
    timer = new LTimer();
    active = false;

}
GameMessage::~GameMessage(){
    if(explosionSound)
        explosionSound->free();
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
            messageText<<"  |  DRAW";
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
        explosionSound = gEngine->audioStore->getSourceFor(AS_EXPLOSION_SOUND);
    }
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    background_ct = gEngine->textureStore->getSourceFor(TS_BG_CT_WIN);
    background_t = gEngine->textureStore->getSourceFor(TS_BG_T_WIN);
    background = gEngine->textureStore->getSourceFor(TS_BG_END);
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