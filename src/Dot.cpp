#include "Dot.hpp"

Dot::Dot(){
    mCollider = new Circle();
}

Dot::Dot( int x, int y )
{
    mCollider = new Circle();
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision circle size
	mCollider->r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

	//Move collider relative to the circle
	shiftColliders();
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move( SDL_Rect& square, Circle& circle )
{
    //Move the dot left or right
    mPosX += mVelX;
	shiftColliders();

    //If the dot collided or went too far to the left or right
	if( ( mPosX - mCollider->r < 0 ) || ( mPosX + mCollider->r > SCREEN_WIDTH ) || utils::checkCollision( *mCollider, square ) || utils::checkCollision( *mCollider, circle ) )
    {
        //Move back
        mPosX -= mVelX;
		shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
	shiftColliders();

    //If the dot collided or went too far up or down
    if( ( mPosY - mCollider->r < 0 ) || ( mPosY + mCollider->r > SCREEN_HEIGHT ) || utils::checkCollision( *mCollider, square ) || utils::checkCollision( *mCollider, circle ) )
    {
        //Move back
        mPosY -= mVelY;
		shiftColliders();
    }
}

void Dot::render(LTexture& gDotTexture )
{
    //Show the dot
	gDotTexture.render( mPosX - mCollider->r, mPosY - mCollider->r );
}

Circle& Dot::getCollider()
{
	return *mCollider;
}

void Dot::shiftColliders()
{
	//Align collider to center of dot
	mCollider->x = mPosX;
	mCollider->y = mPosY;
}