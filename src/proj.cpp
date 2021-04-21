/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <iostream>
#include "GameEngine.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"
#include <pthread.h>


void* RunLoop(void* param){
    GameEngine* gEngine = (GameEngine*) param;
    while(!(gEngine->quit_program))
    {
		if (gEngine->serverObj->running){
			ENetEvent event;
			while (enet_host_service (gEngine->serverObj->server, & event, 0) > 0)
			{
				switch (event.type)
				{
					case ENET_EVENT_TYPE_CONNECT:
					{   
						// enet_host_flush(serverObj->server);
						gEngine->serverObj->connected = true;
						gEngine->serverObj->peer = event.peer;
						printf ("A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port);
						break;
					}

					case ENET_EVENT_TYPE_RECEIVE:
						// enet_host_flush(serverObj->server);
					{
						std::vector<int> received_data = gEngine->serverObj->Parsedata(0, (char*)event.packet->data); // Parse the receiving data.
						gEngine->updateOtherPlayer(received_data);
						enet_packet_destroy(event.packet);
						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT:
						gEngine->serverObj->connected = false;
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
		if (gEngine->clientObj->connected){
			ENetEvent event;
			while(enet_host_service(gEngine->clientObj->client, &event, 0) > 0){
				switch(event.type){
					case ENET_EVENT_TYPE_RECEIVE:
					{
						// enet_host_flush(ClientObj->client);

						std::vector<int> received_data = gEngine->clientObj->Parsedata((char*)event.packet->data); // Parse the receiving data.
						gEngine->updateOtherPlayer(received_data);
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
	return 0;
}