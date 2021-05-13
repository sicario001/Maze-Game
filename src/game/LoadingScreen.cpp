#include "game/LoadingScreen.hpp"

LoadingScreen::LoadingScreen(){
    timer = new LTimer();
    timer->start();
}
LoadingScreen::~LoadingScreen(){
    delete(timer);
    gLoadTextTexture->free();
    timer = NULL;
}

void LoadingScreen::render(std::string text){
    background->renderBackground(NULL);
    loadText.str("");
    loadText<<text;
    gLoadTextTexture->loadFromRenderedText( loadText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
    gLoadTextTexture->render((SCREEN_WIDTH- gLoadTextTexture->getWidth())/2, 10, NULL, 1);
    loadIcon->render(SCREEN_WIDTH/2-25, 4*SCREEN_HEIGHT/5-25, NULL, 0.25, (double)((timer->getTicks())%1000)*(0.36));
}
void LoadingScreen::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    background = gEngine->textureStore->getSourceFor(TS_BG_LOADING);
    loadIcon = gEngine->textureStore->getSourceFor(TS_LOAD_ICON);
}