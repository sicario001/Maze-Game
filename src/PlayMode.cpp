#include "GameModes.hpp"

void PlayMode::spawnThrowable(int x, int y, int speed, double angle, int damage, ThrowableType type){
	// cout << x<< " " <<y<< " " <<speed << " " <<angle<<endl;
	if(damage>0){
		if (clientObj!=NULL){
			if ((clientObj->peer)!=NULL){
				clientObj->SendDataThrowablePosVel(clientObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)), angle, type);
			}
		}
		else{
			if ((serverObj->peer)!=NULL){
				// std::cout<<"in\n";
				serverObj->SendDataThrowablePosVel(serverObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)), angle, type);
			}
		}
		playerThrowables.push_back(Throwable(x,y,speed,angle,damage,pbTexture[type], type));
	}
	else{
		otherPlayerThrowables.push_back(Throwable(x,y,speed,angle,damage,pbTexture[type], type));
	}
}

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
			case SDLK_e:
			{
				if (progressBar==NULL && ((bombState==IDLE && playerObj==ATTACK)||((bomb!=NULL) && (bombState == PLANTED) && (playerObj == DEFEND) && (player->inVicinity(bombLocation, 50))))){
					player->isReloading = false;
					progressBar = new ProgressBar(10000);
					if (playerObj==ATTACK){
						bombState = PLANTING;
						player->stopMovement();
						sendBombState();
					}
					else{
						bombState = DEFUSING;
						player->stopMovement();
						sendBombState();
					}
					player->stopMovement();
				}
			}
        }
    }
	if (e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_e:
			{
				if (progressBar!=NULL){
					delete(progressBar);
					progressBar = NULL;
					if (playerObj==ATTACK){
						bombState = IDLE;
						player->allowMovement();
						sendBombState();
					}
					else{
						bombState = PLANTED;
						player->allowMovement();
						sendBombState();
					}
				}
			}
		}
	}
	player->handleEvent(e);

}

void PlayMode::update(){
	
	if (openPauseMenu){
		openPauseMenu = false;
		Pause();
		gEngine->setGameMode(PAUSE);
		return;
	}
	
	// handle movement of otherPlayer
	if (playerObj!=ATTACK){
		if (bombState==PLANTING){
			otherPlayer->stopMovement();
		}
		else{
			otherPlayer->allowMovement();
		}
	}
	else{
		if (bombState==DEFUSING){
			otherPlayer->stopMovement();
		}
		else{
			otherPlayer->allowMovement();
		}
	}
	//Move the players
	player->move();
	otherPlayer->move();
	player->sendUpdate(clientObj,serverObj);

	//check collision
	player->handleOutOfBounds();
	tileMap->handleCollisions(player);
	player->handleCollision(otherPlayer);
	

	otherPlayer->handleOutOfBounds();
	tileMap->handleCollisions(otherPlayer);
	otherPlayer->handleCollision(player);

	player->resetCamera();

	
	//Render wall
	// SDL_SetRenderDrawColor( gEngine->gRenderer, 0x00, 0x00, 0x00, 0xFF );		
	tileMap->render();
	// player->getCollisionRect()->render();
	// otherPlayer->getCollisionRect()->render();

	if (bomb!=NULL){
		bomb->render();
	}
	
	otherPlayer->render();
	
	// render bullets
	for (Throwable& x : playerThrowables)
	{
		x.move();
		tileMap->handleThrowables(&x);
		if(x.handleCollision(otherPlayer)){
			// send hit if bullet was moving
			if(!x.collided){
				if (clientObj!=NULL){
					if ((clientObj->peer)!=NULL){
						clientObj->SendHit(clientObj->peer, x.damage);
					}
				}
				else{
					if ((serverObj->peer)!=NULL){
						serverObj->SendHit(serverObj->peer, x.damage);
					}
				}
			}
			x.onHit();
		}
		if(x.isActive){
			x.render();
			// x.getCollisionRect()->render();
		}
	}
	
	playerThrowables.erase(std::remove_if(playerThrowables.begin(),playerThrowables.end(),[](Throwable& x){
		return (!x.isActive) || x.handleOutOfBounds();
	}),playerThrowables.end());
	
	//Render players
	player->resetClip();
	player->render();
	for (Throwable& x : otherPlayerThrowables)
	{
		x.move();
		tileMap->handleThrowables(&x);
		if(x.handleCollision(player)){
			x.onHit();
		}
		if(x.isActive){
			x.render();
			// x.getCollisionRect()->render();
		}
	}
	
	otherPlayerThrowables.erase(std::remove_if(otherPlayerThrowables.begin(),otherPlayerThrowables.end(),[](Throwable& x){
		return (!x.isActive) || x.handleOutOfBounds();
	}),otherPlayerThrowables.end());
	healthBar->setHealth(player->getHealth());
	healthBar->render();
	clock->render();
	
	player->inventory->render();
	if (player->reloadBar!=NULL){
		if (player->isReloading){
			if (player->reloadBar->isComplete()){
				delete(player->reloadBar);
				player->reloadBar = NULL;
				player->isReloading = false;
				player->inventory->reload();
			}
			else{
				player->reloadBar->render();
			}
		}
		else{
			delete(player->reloadBar);
			player->reloadBar = NULL;
			player->isReloading = false;

		}
	}
	if (progressBar!=NULL){
		if (progressBar->isComplete()){
			delete(progressBar);
			progressBar = NULL;
			if (playerObj==ATTACK){
				bombState = PLANTED;
				player->allowMovement();
				bombPlanted({player->getPosX(), player->getPosY()});
				sendBombState();
				sendBombLocation();
			}
			else{
				bombState = DEFUSED;
				player->allowMovement();
				sendBombState();
			}
		}
		else{
			progressBar->render();
		}
	}
}

void PlayMode::sendBombState(){
	if (clientObj!=NULL){
		clientObj->SendBombState(bombState);
	}
	else{
		serverObj->SendBombState(bombState);
	}
}
void PlayMode::sendBombLocation(){
	if (clientObj!=NULL){
		clientObj->SendBombLocation(bombLocation);
	}
	else{
		serverObj->SendBombLocation(bombLocation);
	}
}

void PlayMode::bombPlanted(std::pair<int, int> location){
	cout << "planted" << endl;
	bombLocation = location;
	clock->reset(60000);
	bomb = new Entity((location.first)+(bombTexture->getWidth())/2, (location.second)+(bombTexture->getHeight())/2, bombTexture, NULL);
}
void PlayMode::bombDefused(){

}
void PlayMode::updateBombState(int state){
	if (state==0){
		bombState = IDLE;
	}
	else if (state==1){
		bombState = PLANTING;
	}
	else if (state==2){
		bombState = PLANTED;
	}
	else if (state==3){
		bombState = DEFUSING;
	}
	else{
		bombState = DEFUSED;
		bombDefused();
	}
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;
	//Load player bullet texture
	
	if( !pbTexture[0]->loadFromFile( "media/texture/bullet.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	if( !pbTexture[1]->loadFromFile( "media/texture/slashEffect.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	//Load player texture
	if( !gPlayerTexture->loadFromFile( "media/texture/spritesheet.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	if( !bombTexture->loadFromFile( "media/texture/c4.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}
	//Load wall texture
	if( !tileMap->loadTexture() )
	{
		printf( "Failed to load tile sheet texture!\n" );
		success = false;
	}
	clock->loadMediaClock();
	player->inventory->loadMediaInventory();
	gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 40);
	return success;
}
void PlayMode::freePlayMode(){
	gPlayerTexture->free();
	for (LTexture* x:pbTexture){
		x->free();
	}
	bombTexture->free();
	delete (player);
	delete (otherPlayer);
	delete (tileMap);
	delete (bomb);
	bomb = NULL;
	player = NULL;
	otherPlayer = NULL;
	tileMap = NULL;
}

void PlayMode::getPlayerClip(int i,SDL_Rect &clip){
	clip.h= PLAYER_SPRITE_SIZE;
	clip.w= PLAYER_SPRITE_SIZE;
	clip.x=0;
	clip.y = (2 +6*i)*PLAYER_SPRITE_SIZE;
}

void PlayMode::initPlayers(){
	auto sf = [this](int x, int y, int speed, double angle, int damage, ThrowableType type){
		spawnThrowable(x,y,speed,angle,damage, type);
	};
	SDL_Rect clip1,clip2;
	int server_start_pos_x = 0;
	int server_start_pos_y = 0;
	int client_start_pos_x = 2500;
	int client_start_pos_y = 1860;
	if (clientObj!=NULL){
		getPlayerClip(SURVIVOR,clip1);
		getPlayerClip(SOLDIER,clip2);
		player = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip1,sf, SURVIVOR);
		otherPlayer = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip2,sf, SOLDIER);
		playerObj = ATTACK;
	}
	else{
		getPlayerClip(SOLDIER,clip1);
		getPlayerClip(SURVIVOR,clip2);
		player = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip1,sf, SOLDIER);
		otherPlayer = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip2,sf, SURVIVOR);
		playerObj = DEFEND;
	}
}
PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	for (int i=0; i<(int)pbTexture.size(); i++){
		pbTexture[i] = new LTexture();
	}
	bombTexture = new LTexture(0.1);
	healthBar = new HealthBar();
	initPlayers();
};

PlayMode::PlayMode(bool flag, ClientNet* client, ServerNet* server){
	if (!flag){
		PlayMode();
	}
	else{
		// std::cout<<"PlayMode Initialized\n";
		clientObj = client;
		serverObj = server;
		bombState = IDLE;
		gPlayerTexture = new LTexture();
		for (int i=0; i<(int)pbTexture.size(); i++){
			pbTexture[i] = new LTexture();
		}
		bombTexture = new LTexture(0.1);
		healthBar = new HealthBar();
		clock = new Clock();
		initPlayers();
		isPaused = false;
		pthread_mutex_init( &mutex, NULL);
    	pthread_cond_init( &initTileMapSignal, NULL);
	}
}
void PlayMode::ReInit(){
	bombState = IDLE;
	gPlayerTexture = new LTexture();
	for (int i=0; i<(int)pbTexture.size(); i++){
		pbTexture[i] = new LTexture();
	}
	bombTexture = new LTexture(0.1);
	initPlayers();
	player->allowMovement();
	deInitTileMap();
	
	tileMap = new TileMap(clientObj, serverObj);
	// cout<<"in\n";
	tileMap->waitToReceiveMap();
	// cout<<"in\n";
	tileMap->generateTiles(clientObj, serverObj);
	clock->reset(RoundTime);
	// cout<<"in play\n";
	
	loadMediaPlay();
	
	isPaused = false;
}
void PlayMode::enterMode(){
	if (isPaused){
		unPause();
	}
	else{
		ReInit();
	}
}

void PlayMode::Pause(){
	isPaused = true;
}
void PlayMode::unPause(){
	isPaused = false;
}
void PlayMode::Reset(){
	isPaused = false;
}

void PlayMode::initTileMap(){
	pthread_mutex_lock(&mutex);
    tileMapInit = true;
	pthread_cond_signal(&initTileMapSignal);
    pthread_mutex_unlock(&mutex);
}
bool PlayMode::isInitTileMap(){
	pthread_mutex_lock(&mutex);
    bool init_val = tileMapInit;
    pthread_mutex_unlock(&mutex);
    return init_val;
}
void PlayMode::deInitTileMap(){
	pthread_mutex_lock(&mutex);
    tileMapInit = true;
    pthread_mutex_unlock(&mutex);
}

void PlayMode::waitForInitTileMap(){
	pthread_mutex_lock(&mutex);
    while (!tileMapInit){
        pthread_cond_wait(&initTileMapSignal, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}