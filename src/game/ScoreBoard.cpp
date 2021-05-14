#include "game/ScoreBoard.hpp"

ScoreBoard::ScoreBoard(){
    background = new LTexture();
    gScoreTextTexture = new LTexture();
    reset();
}
ScoreBoard::~ScoreBoard(){
    background->free();
    gScoreTextTexture->free();
}
void ScoreBoard::reset(){
    PlayerScore = 0;
    OtherPlayerScore = 0;
}
void ScoreBoard::render(){
    scoreText.str( "" );
	scoreText<<"Score: " <<PlayerScore<<" - "<<OtherPlayerScore;
	gScoreTextTexture->loadFromRenderedText( scoreText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
	gScoreTextTexture->render(100+SCREEN_WIDTH/2, 10, NULL, 1);
}
int ScoreBoard::getPlayerScore(){
    return PlayerScore;
}
int ScoreBoard::getOtherPlayerScore(){
    return OtherPlayerScore;
}
void ScoreBoard::incPlayerScore(){
    PlayerScore++;
}
void ScoreBoard::incOtherPlayerScore(){
    OtherPlayerScore++;
}
void::ScoreBoard::loadMedia(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
}