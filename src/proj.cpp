/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <iostream>
#include "GameEngine.hpp"
#include "Dot.hpp"
#include "Circle.hpp"
#include "LButton.hpp"
#include "LTexture.hpp"

GameEngine* gEngine;

int main( int argc, char* args[] )
{
	gEngine = new GameEngine();
	std::cout << "Engine loaded" << std::endl;
	//Start up SDL and create window
	if( !gEngine->init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		std::cout << "start loop" << std::endl;
		gEngine->runLoop();
	}
	return 0;
}