#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "fwd.hpp"
#include "LTexture.hpp"
#include "GameEngine.hpp"
#include "AudioSource.hpp"
extern GameEngine* gEngine;

class LButton
{
	public:
		//Initializes internal variables
		LButton();
		LButton(ButtonType button_type, LTexture* gButtonSpriteSheetTexture, SDL_Rect (&gSpriteClips)[BUTTON_SPRITE_TOTAL]);

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();
		int getMode();
		void setMode(int a);

		void loadAudio();
	private:
		LTexture* gButtonSpriteSheetTexture;

		SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
		//Top left position
		SDL_Point mPosition;
		int modeSelected;
		//Currently used global sprite
		LButtonSprite mCurrentSprite;
		int sprite_type; 
		ButtonType button_type;
		//Audio
		AudioSource* hoverSound;
		AudioSource* clickSound;
		bool wasInsideBefore = false;
};
