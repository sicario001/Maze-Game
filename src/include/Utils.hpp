#pragma once

#include <SDL2/SDL.h>

#include "fwd.hpp"
#include "Circle.hpp"
namespace utils
{
    bool checkCollision( Circle& a, Circle& b );

    bool checkCollision( Circle& a, SDL_Rect& b );

    double distanceSquared( int x1, int y1, int x2, int y2 );
    
}
