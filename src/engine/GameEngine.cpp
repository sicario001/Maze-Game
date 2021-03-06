#include "engine/GameEngine.hpp"
#include "game/Player.hpp"
#include "game/GameMessage.hpp"

GameEngine::GameEngine(int c_or_s){
	camera = new SDL_Rect();
	camera->x = 0;
	camera->y = 0;
	camera->w = CAMERA_WIDTH;
	camera->h = CAMERA_HEIGHT;
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
	audioMaster = new AudioMaster();
	audioStore = new AudioStore();
}

bool GameEngine::init()
{
	audioMaster->init();
	audioStore->init();
	bgm = audioStore->getSourceFor(AS_BGM);
	startbgm();
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
		gWindow = SDL_CreateWindow( "Maze Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void GameEngine::checkRoundEnd(){
	if (serverObj!=NULL){
		if (playMode->canReturnHome){
			setGameMode(HOME);
			return;
		}
		if (playMode->roundEndMessageInit){
			return;
		}
		PlayerObj winner;
		if (playMode->playerObj == ATTACK){
			winner = DEFEND;
		}
		else{
			winner = ATTACK;
		}
		if(playMode->LoadingComplete && !serverObj->isConnected()){
			playMode->setWinner(0);
			playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, DRAW_DISCONNECT, false);
			playMode->currentRoundNum = -1;
			playMode->roundEndMessageInit = true;
			return;
		}
		if (currMode==PLAY_MODE || currMode==PAUSE_MODE){
			if (playMode->player->getHealth()<=0){
				if (playMode->playerObj==DEFEND || playMode->bombState==IDLE || playMode->bombState==PLANTING){
					playMode->setWinner(winner);
					serverObj->SendRoundEndSignal(winner);
					playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, winner,false);
					playMode->roundEndMessageInit = true;
				}
				else{
					if(!playMode->player->isDead){
						playMode->updatePlayerToDead(0);
						serverObj->sendPlayerDead(0);
					}
				}
			}
			else if (playMode->otherPlayer->getHealth()<=0){
				if (playMode->playerObj==ATTACK || playMode->bombState==IDLE || playMode->bombState==PLANTING){
					playMode->setWinner(playMode->playerObj);
					serverObj->SendRoundEndSignal(playMode->playerObj);
					playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, playMode->playerObj,false);
					playMode->roundEndMessageInit = true;
				}
				else{
					if(!playMode->otherPlayer->isDead){
						playMode->updatePlayerToDead(1);
						serverObj->sendPlayerDead(1);
					}
				}
			}
			if (playMode->clock->timeOver()){
				if (playMode->bombState == PLANTED || playMode->bombState == DEFUSING){
					playMode->setWinner(ATTACK);
					cout << "TIME OVER GAME END" << endl;
					serverObj->SendRoundEndSignal(ATTACK);
					playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, ATTACK, true);
					playMode->roundEndMessageInit = true;
				}	
				else{
					playMode->setWinner(DEFEND);
					serverObj->SendRoundEndSignal(DEFEND);
					playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, DEFEND, false);
					playMode->roundEndMessageInit = true;
				}
			}
			if (playMode->bombState==DEFUSED){
				playMode->setWinner(DEFEND);
				// cout<<"IN1\n";
				serverObj->SendRoundEndSignal(DEFEND);
				playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, DEFEND,false);
				playMode->roundEndMessageInit = true;
			}
			
		}
	}
	else{
		if (playMode->canReturnHome){
			setGameMode(HOME);
			return;
		}
		if (playMode->roundEndMessageInit){
			return;
		}
		if(playMode->LoadingComplete && !clientObj->isConnected()){
			playMode->setWinner(0);
			playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, DRAW_DISCONNECT, false);
			playMode->currentRoundNum = -1;
			playMode->roundEndMessageInit = true;
			return;
		}
		if (playMode->roundOver){
			playMode->gameMessage->resetMessage("ROUND OVER", playMode->RoundEndMessageDuration, playMode->roundWinner, playMode->clock->timeOver() && playMode->bombState == PLANTED);
			playMode->roundEndMessageInit = true;
		}
	}
}

void GameEngine::runLoop(){
	SDL_Event e;
	homeMode->enterMode();
	while( !quit_program )
	{
		//Handle events on queue
		checkRoundEnd();
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
		if (currMode==PAUSE_MODE){
			playMode->update(false);
		}
		SDL_SetRenderDrawColor( gEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gEngine->gRenderer );

		gMode->update();
		
		// cout<<currMode<<"\n";
		//Update screen
		SDL_RenderPresent( gEngine->gRenderer );
		// if (currMode == 1)cout<<"in\n";
	}
	//Free loaded images
	// playMode->freePlayMode();
	bgm->free();
	audioMaster->free();
	audioStore->free();
	//Destroy window	
	SDL_DestroyRenderer(gRenderer );
	SDL_DestroyWindow(gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}
void GameEngine::updateOtherPlayer(std::vector<int> &data){
	if (currMode==PLAY_MODE || currMode==PAUSE_MODE){
		playMode->otherPlayer->setPosVel(data[1], data[2], data[3], data[4]);
		playMode->otherPlayer->setRotation(data[5]);
	}
}

void GameEngine::addThrowableToVector(std::vector<int> &data){
	if (currMode==PLAY_MODE || currMode==PAUSE_MODE){
		ThrowableType type;
		if (data[5]==0){
			type=BULLET;
		}
		else{
			type = KNIFE_SLASH;
		}
		playMode->spawnThrowable(data[1],data[2],(int)sqrt(data[3]*data[3]+data[4]*data[4]),((double)data[6])/1e8,0, type);
	}
}

void GameEngine::damagePlayer(std::vector<int> &data){
	if (currMode==PLAY_MODE || currMode==PAUSE_MODE){
		if ((playMode->currentRoundNum) ==data[2]){
			playMode->player->damage(data[1]);
		}
	}
}

void GameEngine::updateMapfromServer(vector<int> &map_vec){
	
	playMode->tileMap->initializeMap(LEVEL_HEIGHT/TILE_SIZE, LEVEL_WIDTH/TILE_SIZE);
	
    for (int i=1; i<(int)map_vec.size(); i++){
        if (map_vec[i]==1){
            playMode->tileMap->setMap((i-1)/20, (i-1)%20, true);
        }
        else{
            playMode->tileMap->setMap((i-1)/20, (i-1)%20, false);
        }
    }
}

void GameEngine::resetListener(int x,int y){
	// cout << "listener at " <<x <<" " << y << endl;
	audioMaster->setListenerPosition(x,y,0);
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
			
			// if (clientObj!=NULL){
			// 	clientObj->Connect(server_address.c_str(), server_port);
			// }
			playMode->enterMode();
			break;
		case RESUME:
			stopbgm();
			gMode = playMode;
			currMode = PLAY_MODE;
			playMode->enterMode();
			break;
		case PAUSE:
			startbgm();
			gMode = pauseMode;
			currMode = PAUSE_MODE;
			pauseMode->enterMode();
			break;
		case HOME:
			startbgm();
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

void GameEngine::startbgm(){
	if(bgm->getState()!=AL_PLAYING){
		bgm->rewind();
		bgm->play(true);
	}
}

void GameEngine::stopbgm(){
	bgm->rewind();
}