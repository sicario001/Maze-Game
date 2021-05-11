#include <iostream>
#include <pthread.h>

#include "engine/GameEngine.hpp"

void* RunLoop(void* param){
    GameEngine* gEngine = (GameEngine*) param;
    while(!(gEngine->quit_program))
    {
		if (gEngine->serverObj->isRunning()){
			ENetEvent event;
			while (enet_host_service (gEngine->serverObj->server, & event, 0) > 0)
			{
				switch (event.type)
				{
					case ENET_EVENT_TYPE_CONNECT:
					{   
						// enet_host_flush(serverObj->server);
						// cout<<"in_connect\n";
						
						gEngine->serverObj->peer = event.peer;
						printf ("A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port);
						gEngine->serverObj->setConnected();
						// cout<<"connected\n";
						break;
					}

					case ENET_EVENT_TYPE_RECEIVE:
						// enet_host_flush(serverObj->server);
					{
						std::vector<int> received_data = gEngine->serverObj->Parsedata((char*)event.packet->data); // Parse the receiving data.
						if (received_data[0]==0){
							gEngine->updateOtherPlayer(received_data);
						}
						else if(received_data[0]==2){
							gEngine->addThrowableToVector(received_data);
						}
						else if(received_data[0]==3){
							gEngine->damagePlayer(received_data);
						}
						else if (received_data[0]==4){
							gEngine->playMode->updateBombState((BombState)received_data[1]);
						}
						else if (received_data[0]==5){
							gEngine->playMode->bombPlanted({received_data[1], received_data[2]});
						}
						else if (received_data[0]==6){
							gEngine->playMode->ClientMapInitialized = true;
						}
						else if (received_data[0]==7){
							gEngine->playMode->LoadingComplete = true;
							gEngine->playMode->clock->reset(gEngine->playMode->RoundTime);
						}
						enet_packet_destroy(event.packet);
						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT:
						gEngine->serverObj->setNotConnected();
						printf ("%x:%u disconnected.\n",
						event.peer -> address.host,
						event.peer -> address.port);
						break;
						
					default:
						break;
				}
			}
		}
    }
	pthread_exit(NULL);
}

void* ReceiveLoop(void* param){
    GameEngine* gEngine = (GameEngine*) param;
  	while(!(gEngine->quit_program)){
		if (gEngine->clientObj->isConnected()){
			ENetEvent event;
			while(enet_host_service(gEngine->clientObj->client, &event, 0) > 0){
				switch(event.type){
					case ENET_EVENT_TYPE_RECEIVE:
					{
						// enet_host_flush(ClientObj->client);
						
						std::vector<int> received_data = gEngine->clientObj->Parsedata((char*)event.packet->data); // Parse the receiving data.
						if (received_data[0]==0){
							gEngine->updateOtherPlayer(received_data);
						}
						else if(received_data[0]==1){
							gEngine->updateMapfromServer(received_data);
							gEngine->playMode->tileMap->generateTiles(gEngine->serverObj);
							gEngine->playMode->LoadingComplete = true;
							gEngine->clientObj->SendLoadingComplete();
							gEngine->playMode->clock->reset(gEngine->playMode->RoundTime);
						}
						else if(received_data[0]==2){
							gEngine->addThrowableToVector(received_data);
						}
						else if(received_data[0]==3){
							gEngine->damagePlayer(received_data);
						}
						else if (received_data[0]==4){
							gEngine->playMode->updateBombState((BombState)received_data[1]);
						}
						else if (received_data[0]==5){
							gEngine->playMode->bombPlanted({received_data[1], received_data[2]});
						}
						enet_packet_destroy(event.packet);

						break;
					}
					default:
						break;
				}
			}
		}
  	}
	pthread_exit(NULL);
}

GameEngine* gEngine;

int main( int argc, char* args[] )
{
	// AudioMaster am;
	// am.init();
	
	// AudioSource *x = am.loadWaveFile("media/audio/walk.wav");
	// x->play();
	int c_or_s;
	std::cout<<"Enter 0 for server and 1 for client: ";
	std::cin>>c_or_s;
	gEngine = new GameEngine(c_or_s);
	std::cout << "Engine loaded" << std::endl;
	//Start up SDL and create window
	if( !gEngine->init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		// cout<<"in\n";
		pthread_t thread;
		if (gEngine->clientObj!=NULL){
  			pthread_create(&thread, NULL, ReceiveLoop, gEngine);
		}
		else{
			pthread_create(&thread, NULL, RunLoop, gEngine);
		}
		std::cout << "start loop" << std::endl;
		gEngine->runLoop();
	}
	// x->free();
	// am.free();
	return 0;
}