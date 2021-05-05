#include "ProgressBar.hpp"

ProgressBar::ProgressBar(int total, int x_pos, int y_pos){
    x = x_pos;
    y = y_pos;
    progressBar = new SDL_Rect();
    BarBoundary = new SDL_Rect();
    progressTimer = new LTimer();
    progressTimer->start();
    totalTime = total;
    timeElapsed = 0;
    progressBar->x = x;
    progressBar->y = y;
    BarBoundary->x = x;
    BarBoundary->y = y;
    BarBoundary->w = SCREEN_WIDTH/2;
    BarBoundary->h = 20;
}
ProgressBar::~ProgressBar(){

}
void ProgressBar::render(){
    UpdateBar();

    SDL_SetRenderDrawColor(gEngine->gRenderer, 0, 255, 0, 255 );
    SDL_RenderFillRect(gEngine->gRenderer, progressBar);

    SDL_SetRenderDrawColor(gEngine->gRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gEngine->gRenderer, BarBoundary);

}
void ProgressBar::UpdateBar(){
    timeElapsed = progressTimer->getTicks();
    
    progressBar->w = ((timeElapsed*SCREEN_WIDTH)/(2*totalTime));
    progressBar->h = (20);

}
bool ProgressBar::isComplete(){
    if (timeElapsed>totalTime){
        return true;
    }
    return false;
}