#include "GameEngine.hpp"

GameEngine::GameEngine(int c_or_s){
	pauseMode = new PauseMode();
	homeMode = new HomeMode() ;
	gMode = homeMode;
	currMode = HOME_MODE;
	if (c_or_s){
		clientObj = new ClientNet();
	}
	else{
		serverObj = new ServerNet();
	}
	// cout<<"in\n";
	playMode = new PlayMode(true, clientObj, serverObj);
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
	homeMode->enterMode();
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

			//Handle input for the player
			gMode->eventHandler(e);
		}

		//Clear screen
		SDL_SetRenderDrawColor( gEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gEngine->gRenderer );

		gMode->update();
		// cout<<currMode<<"\n";
		//Update screen
		SDL_RenderPresent( gEngine->gRenderer );
		// if (currMode == 1)cout<<"in\n";
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
void GameEngine::updateOtherPlayer(std::vector<int> &data){
	if (currMode==PLAY_MODE || currMode==PAUSE_MODE){
		playMode->otherPlayer->setPosVel(data[1], data[2], data[3], data[4]);
	}
}
void GameEngine::updateMapfromServer(vector<int> &map_vec){
	
	playMode->tileMap->initializeMap(15, 20);
	
    for (int i=1; i<(int)map_vec.size(); i++){
        if (map_vec[i]==1){
            playMode->tileMap->setMap((i-1)/20, (i-1)%20, true);
        }
        else{
            playMode->tileMap->setMap((i-1)/20, (i-1)%20, false);
        }
    }
    // std::cout<<"all good"<<endl;
    playMode->tileMap->setReceived();
}


void GameEngine::setGameMode(GameModeType a){
	switch (a)
	{
		case QUIT:
			quit_program=true;
			break;
		case PLAY:
			gMode = playMode;
			currMode = PLAY_MODE;
			
			playMode->Reset();
			if (clientObj!=NULL){
				clientObj->Connect("127.0.0.1", 7777);
			}
			playMode->enterMode();
			break;
		case RESUME:
			gMode = playMode;
			currMode = PLAY_MODE;
			playMode->enterMode();
			break;
		case PAUSE:
			gMode = pauseMode;
			currMode = PAUSE_MODE;
			pauseMode->enterMode();
			break;
		case HOME:
			playMode->freePlayMode();
			gMode = homeMode;
			currMode = HOME_MODE;
			homeMode->enterMode();
			if (clientObj!=NULL){
				if (clientObj->connected){
					clientObj->Disconnect();
				}
			}
			break;
		default:
		
			break;
	}
}