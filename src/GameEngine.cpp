#include "GameEngine.hpp"

GameEngine::GameEngine(){
	playMode = new PlayMode(true);
	pauseMode = new PauseMode();
	homeMode = new HomeMode() ;
	gMode = homeMode;
}

bool GameEngine::init()
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

void GameEngine::runLoop(){
	SDL_Event e;
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
			gMode->eventHandler(e);
		}

		//Clear screen
		SDL_SetRenderDrawColor( gEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gEngine->gRenderer );

		gMode->update();

		//Update screen
		SDL_RenderPresent( gEngine->gRenderer );
	}
	//Free loaded images
	playMode->freePlayMode();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer );
	SDL_DestroyWindow(gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void GameEngine::setGameMode(GameModeType a){
	switch (a)
	{
		case QUIT:
			quit_program=true;
			break;
		case PLAY:
			gMode = playMode;
			playMode->Reset();
			playMode->enterMode();
			break;
		case RESUME:
			gMode = playMode;
			playMode->enterMode();
			break;
		case PAUSE:
			gMode = pauseMode;
			pauseMode->enterMode();
			break;
		case HOME:
			gMode = homeMode;
			homeMode->enterMode();
			break;
		default:
		
			break;
	}
}