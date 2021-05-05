#include "GameModes.hpp"

void PlayMode::spawnBullet(int x, int y, int speed, double angle, int damage){
	// cout << x<< " " <<y<< " " <<speed << " " <<angle<<endl;
	if(damage>0){
		if (clientObj!=NULL){
			if ((clientObj->peer)!=NULL){
				clientObj->SendDataBulletPosVel(clientObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)));
			}
		}
		else{
			if ((serverObj->peer)!=NULL){
				// std::cout<<"in\n";
				serverObj->SendDataBulletPosVel(serverObj->peer, x, y, (int)(speed * cos(angle)), (int)(speed * sin(angle)));
			}
		}
		playerBullets.push_back(Bullet(x,y,speed,angle,damage,pbTexture));
	}
	else{
		otherPlayerBullets.push_back(Bullet(x,y,speed,angle,damage,pbTexture));
	}
}

void PlayMode::eventHandler(SDL_Event& e){
	if( e.type == SDL_KEYDOWN && e.key.repeat==0)
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_ESCAPE: openPauseMenu = true; break;
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
	// cout<<"in\n";
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
	
	otherPlayer->render();

	// render bullets
	for (Bullet& x : playerBullets)
	{
		x.move();
		tileMap->handleBullets(&x);
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
	
	playerBullets.erase(std::remove_if(playerBullets.begin(),playerBullets.end(),[](Bullet& x){
		return (!x.isActive) || x.handleOutOfBounds();
	}),playerBullets.end());

	//Render players
	player->render();

	for (Bullet& x : otherPlayerBullets)
	{
		x.move();
		tileMap->handleBullets(&x);
		if(x.handleCollision(player)){
			x.onHit();
		}
		if(x.isActive){
			x.render();
			// x.getCollisionRect()->render();
		}
	}
	
	otherPlayerBullets.erase(std::remove_if(otherPlayerBullets.begin(),otherPlayerBullets.end(),[](Bullet& x){
		return (!x.isActive) || x.handleOutOfBounds();
	}),otherPlayerBullets.end());
}

bool PlayMode::loadMediaPlay()
{
	//Loading success flag
	bool success = true;
	//Load player bullet texture
	if( !pbTexture->loadFromFile( "media/texture/bullet.png" ) )
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
	//Load wall texture
	if( !tileMap->loadTexture() )
	{
		printf( "Failed to load tile sheet texture!\n" );
		success = false;
	}
	return success;
}
void PlayMode::freePlayMode(){
	gPlayerTexture->free();
	pbTexture->free();
	delete (player);
	delete (otherPlayer);
	delete (tileMap);
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
	auto sf = [this](int x, int y, int speed, double angle, int damage){
		spawnBullet(x,y,speed,angle,damage);
	};
	SDL_Rect clip1,clip2;
	int server_start_pos_x = 0;
	int server_start_pos_y = 0;
	int client_start_pos_x = 2500;
	int client_start_pos_y = 1860;
	if (clientObj!=NULL){
		getPlayerClip(SURVIVOR,clip1);
		getPlayerClip(SOLDIER,clip2);
		player = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip1,sf);
		otherPlayer = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip2,sf);
	}
	else{
		getPlayerClip(SOLDIER,clip1);
		getPlayerClip(SURVIVOR,clip2);
		player = new Player(100,server_start_pos_x,server_start_pos_y,gPlayerTexture,&clip1,sf);
		otherPlayer = new Player(100,client_start_pos_x,client_start_pos_y,gPlayerTexture,&clip2,sf);
	}
}
PlayMode::PlayMode(){
	gPlayerTexture = new LTexture();
	pbTexture = new LTexture();
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
		gPlayerTexture = new LTexture();
		pbTexture = new LTexture();
		initPlayers();
		isPaused = false;
		pthread_mutex_init( &mutex, NULL);
    	pthread_cond_init( &initTileMapSignal, NULL);
	}
}
void PlayMode::ReInit(){
	
	gPlayerTexture = new LTexture();
	pbTexture = new LTexture();
	initPlayers();
	deInitTileMap();
	
	tileMap = new TileMap(clientObj, serverObj);
	// cout<<"in\n";
	tileMap->waitToReceiveMap();
	// cout<<"in\n";
	tileMap->generateTiles(clientObj, serverObj);

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