/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 2;
bool quit_program = false;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT_PLAY = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION_PLAY= 1,
	BUTTON_SPRITE_MOUSE_DOWN_PLAY = 2,
	BUTTON_SPRITE_MOUSE_UP_PLAY = 3,
	BUTTON_SPRITE_MOUSE_OUT_EXIT = 4,
	BUTTON_SPRITE_MOUSE_OVER_MOTION_EXIT= 5,
	BUTTON_SPRITE_MOUSE_DOWN_EXIT = 6,
	BUTTON_SPRITE_MOUSE_UP_EXIT = 7,
	BUTTON_SPRITE_TOTAL = 8
};
enum LButtonSprite_pm
{
	BUTTON_SPRITE_MOUSE_OUT_RESUME = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION_RESUME= 1,
	BUTTON_SPRITE_MOUSE_DOWN_RESUME = 2,
	BUTTON_SPRITE_MOUSE_UP_RESUME = 3,
	BUTTON_SPRITE_MOUSE_OUT_HOME = 4,
	BUTTON_SPRITE_MOUSE_OVER_MOTION_HOME= 5,
	BUTTON_SPRITE_MOUSE_DOWN_HOME = 6,
	BUTTON_SPRITE_MOUSE_UP_HOME = 7,
	BUTTON_SPRITE_TOTAL_PM = 8
};

enum ButtonType
{
	BUTTON_PLAY = 0,
	BUTTON_EXIT = 1,
	BUTTON_RESUME = 2,
	BUTTON_HOME = 3

};
//A circle stucture
struct Circle
{
	int x, y;
	int r;
};

class LButton
{
	public:
		//Initializes internal variables
		LButton();
		LButton(ButtonType button_type, int type);

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();
		int getMode();
		void setMode(int a);

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
		LButtonSprite_pm mCurrentSprite_pm;
		int sprite_type; 
		ButtonType button_type;
		int modeSelected;
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 1;

		//Initializes the variables
		Dot( int x, int y );

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move( SDL_Rect& square, Circle& circle );

		//Shows the dot on the screen
		void render();

		//Gets collision circle
		Circle& getCollider();

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
		
		//Dot's collision circle
		Circle mCollider;

		//Moves the collision circle relative to the dot's offset
		void shiftColliders();
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMediaPlay();

bool loadMediaHome();


//Frees media and shuts down SDL
void close();

//Circle/Circle collision detector
bool checkCollision( Circle& a, Circle& b );

//Circle/Box collision detector
bool checkCollision( Circle& a, SDL_Rect& b );

//Calculates distance squared between two points
double distanceSquared( int x1, int y1, int x2, int y2 );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;

SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[] = {LButton(BUTTON_PLAY, 0), LButton(BUTTON_EXIT, 0)}; 

SDL_Rect gSpriteClips_pm[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture_pm;

//Buttons objects
LButton gButtons_pm[] = {LButton(BUTTON_RESUME, 1), LButton(BUTTON_HOME, 1)}; 

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Dot::Dot( int x, int y )
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision circle size
	mCollider.r = DOT_WIDTH / 2;

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
	if( ( mPosX - mCollider.r < 0 ) || ( mPosX + mCollider.r > SCREEN_WIDTH ) || checkCollision( mCollider, square ) || checkCollision( mCollider, circle ) )
    {
        //Move back
        mPosX -= mVelX;
		shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
	shiftColliders();

    //If the dot collided or went too far up or down
    if( ( mPosY - mCollider.r < 0 ) || ( mPosY + mCollider.r > SCREEN_HEIGHT ) || checkCollision( mCollider, square ) || checkCollision( mCollider, circle ) )
    {
        //Move back
        mPosY -= mVelY;
		shiftColliders();
    }
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( mPosX - mCollider.r, mPosY - mCollider.r );
}

Circle& Dot::getCollider()
{
	return mCollider;
}

void Dot::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}
LButton::LButton(){}

LButton::LButton(ButtonType type, int sprite)
{
	mPosition.x = 0;
	mPosition.y = 0;
	modeSelected = 0;
	button_type = type;
	sprite_type = sprite;
	if (sprite_type==0){
		if (type==BUTTON_PLAY){
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_PLAY;
		}
		else{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_EXIT;
		}
	}
	else{
		if (type==BUTTON_RESUME){
			mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OUT_RESUME;
		}
		else{
			mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OUT_HOME;
		}
	}
	
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
			if (sprite_type==0){
				if (button_type==BUTTON_PLAY){
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_PLAY;
				}
				else{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_EXIT;
				}
			}
			else{
				if (button_type == BUTTON_RESUME){
					mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OUT_RESUME;
				}
				else{
					mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OUT_HOME;
				}

			}
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				if (sprite_type==0){
					if (button_type==BUTTON_PLAY){
						mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION_PLAY;
					}
					else{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION_EXIT;
					}
				}
				else{
					if (button_type == BUTTON_RESUME){
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OVER_MOTION_RESUME;
					}
					else{
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_OVER_MOTION_HOME;
					}

				}
				
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				if (sprite_type==0){
					if (button_type==BUTTON_PLAY){
						mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN_PLAY;
					}
					else{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN_EXIT;
					}
				}
				else{
					if (button_type == BUTTON_RESUME){
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_DOWN_RESUME;
					}
					else{
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_DOWN_HOME;
					}

				}
				break;
				
				case SDL_MOUSEBUTTONUP:
				if (sprite_type == 0){
					if (button_type==BUTTON_PLAY){
						mCurrentSprite = BUTTON_SPRITE_MOUSE_UP_PLAY;
						modeSelected = 1;
					}
					else{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_UP_EXIT;
						modeSelected = 1;
					}
				}
				else{
					if (button_type==BUTTON_RESUME){
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_UP_RESUME;
						modeSelected = 1;
					}
					else{
						mCurrentSprite_pm = BUTTON_SPRITE_MOUSE_UP_HOME;
						modeSelected = 1;
					}
				}
				break;
			}
		}
	}
}
	
void LButton::render()
{
	//Show current button sprite
	if (sprite_type==0){
		gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
	}
	else{
		gButtonSpriteSheetTexture_pm.render( mPosition.x, mPosition.y, &gSpriteClips_pm[ mCurrentSprite_pm ] );
	}
}
int LButton::getMode(){
	return modeSelected;
}
void  LButton::setMode(int a){
	modeSelected = a;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMediaPlay()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "media/texture/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	return success;
}

bool loadMediaPauseMenu(){
	bool success = true;
	if( !gButtonSpriteSheetTexture_pm.loadFromFile( "media/texture/sprites/pause_menu.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL_PM; ++i )
		{
			gSpriteClips_pm[ i ].x = 0;
			gSpriteClips_pm[ i ].y = i * 200;
			gSpriteClips_pm[ i ].w = BUTTON_WIDTH;
			gSpriteClips_pm[ i ].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons_pm[ 0 ].setPosition( (SCREEN_WIDTH-BUTTON_WIDTH)/2, (SCREEN_HEIGHT/2- BUTTON_HEIGHT) );
		gButtons_pm[ 1 ].setPosition( (SCREEN_WIDTH-BUTTON_WIDTH)/2, SCREEN_HEIGHT/2 );
	}
	return success;
}

bool loadMediaHome(){
	bool success = true;
	if( !gButtonSpriteSheetTexture.loadFromFile( "media/texture/sprites/menu.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
	}
	return success;
}
void close()
{
	//Free loaded images
	gDotTexture.free();
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( Circle& a, Circle& b )
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool checkCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a.x < b.x )
    {
        cX = b.x;
    }
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

double distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}
void pauseMenu();
void HomeMode();
void PlayMode();

bool openPauseMenu = false;

void resumePlayMode(){
	PlayMode();
}

void PauseMenu(){
	if( !loadMediaPauseMenu() )
	{
		printf( "Failed to load media!\n" );
	}
	else
	{	

		//Event handler
		SDL_Event e;

		//While application is running
		while( !quit_program )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit_program = true;
				}
				
				//Handle button events
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons_pm[ i ].handleEvent( &e );
				}
			}

			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			//Render buttons
			for( int i = 0; i < TOTAL_BUTTONS; ++i )
			{
				gButtons_pm[ i ].render();
			}

			//Update screen
			SDL_RenderPresent( gRenderer );

			//Select mode
			if (gButtons_pm[0].getMode()){
				gButtons_pm[0].setMode(0);
				resumePlayMode();
			}
			else if (gButtons_pm[1].getMode()){
				gButtons_pm[1].setMode(0);
				HomeMode();
			}
		}
		
	}
}


void playMode_eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN && e.key.repeat==0)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
        }
    }
}

void PlayMode(){
	if( !loadMediaPlay() ){
		printf( "Failed to load media!\n" );
	}
	else
	{	
		//Event handler
		SDL_Event e;

		//The dot that will be moving around on the screen
		Dot dot( Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2 );
		Dot otherDot( SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 );

		//Set the wall
		SDL_Rect wall;
		wall.x = 300;
		wall.y = 40;
		wall.w = 40;
		wall.h = 400;
		
		//While application is running
		while( !quit_program )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit_program = true;
				}

				//Handle input for the dot
				playMode_eventHandler(e);
				dot.handleEvent( e );
			}
			if (openPauseMenu){
				openPauseMenu = false;
				PauseMenu();
				return;
			}
			//Move the dot and check collision
			dot.move( wall, otherDot.getCollider() );

			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			//Render wall
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );		
			SDL_RenderDrawRect( gRenderer, &wall );
			
			//Render dots
			dot.render();
			otherDot.render();

			//Update screen
			SDL_RenderPresent( gRenderer );
		}
	}
}
void HomeMode(){
	if( !loadMediaHome() )
	{
		printf( "Failed to load media!\n" );
	}
	else
	{	
		

		//Event handler
		SDL_Event e;

		//While application is running
		while( !quit_program )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit_program = true;
				}
				
				//Handle button events
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].handleEvent( &e );
				}
			}

			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			//Render buttons
			for( int i = 0; i < TOTAL_BUTTONS; ++i )
			{
				gButtons[ i ].render();
			}

			//Update screen
			SDL_RenderPresent( gRenderer );

			//Select mode
			if (gButtons[0].getMode()){
				gButtons[0].setMode(0);
				PlayMode();
			}
			else if (gButtons[1].getMode()){
				gButtons[1].setMode(0);
				quit_program = true;
			}
		}
		
	}
	
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		HomeMode();
	}
	close();

	return 0;
}