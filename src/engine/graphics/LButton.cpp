#include "engine/graphics/LButton.hpp"

LButton::LButton(){}

LButton::LButton(ButtonType type, LTexture* spriteSheet,SDL_Rect (&spriteClips)[BUTTON_SPRITE_TOTAL])
{
	mPosition.x = 0;
	mPosition.y = 0;
	button_type = type;
	modeSelected = 0;
	gButtonSpriteSheetTexture = spriteSheet;
	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	{
		gSpriteClips[i] = spriteClips[i];
		// std::cout << gSpriteClips[i].h << std::endl;
	}
	
	if (type==BUTTON_PRIM){
		mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	}
	else{
		mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_ALT;
	}	
}

void LButton::loadAudio(){
	hoverSound = gEngine->audioMaster.loadWaveFile("media/audio/ui_click1.wav");
	clickSound = gEngine->audioMaster.loadWaveFile("media/audio/ui_click2.wav");
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}
// modify
void LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			if (button_type==BUTTON_PRIM){
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
			}
			else{
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_ALT;
			}	
			wasInsideBefore = false;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				if (button_type==BUTTON_PRIM){
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				}
				else{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION_ALT;
				}	
				if(!wasInsideBefore){
					hoverSound->rewind();
					hoverSound->play();
				}				
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				if (button_type==BUTTON_PRIM){
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				}
				else{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN_ALT;
				}	
				clickSound->rewind();
				clickSound->play();
				break;
				
				case SDL_MOUSEBUTTONUP:
				if (button_type==BUTTON_PRIM){
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					modeSelected = 1;
				}
				else{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP_ALT;
					modeSelected = 1;
				}	
				break;
			}
			wasInsideBefore = true;
		}
	}
}
	
void LButton::render()
{
	// std::cout << mCurrentSprite << std::endl;
	// std::cout << gSpriteClips[ mCurrentSprite ].x << gSpriteClips[ mCurrentSprite ].y << gSpriteClips[ mCurrentSprite ].w << gSpriteClips[ mCurrentSprite ].h << std::endl;
	//Show current button sprite
	gButtonSpriteSheetTexture->render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

int LButton::getMode(){
	return modeSelected;
}
void  LButton::setMode(int a){
	modeSelected = a;
}