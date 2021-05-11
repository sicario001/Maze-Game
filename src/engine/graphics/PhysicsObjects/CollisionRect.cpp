#include "engine/graphics/PhysicsObject.hpp"

pair<int,int> CollisionRect::getVertex(int i){
    double diag = sqrt(1.0*w*w+h*h)/2;
    // if(w==36){
    //     cout << w << " " << h << " " <<uw << " " << uh << endl;
    // }
    double dangle = atan2(h,w);
    switch (i)
    {
    case 0:
        dangle -= PI;
        break;
    case 1:
        dangle = - dangle;
        break;
    case 3:
        dangle = PI - dangle;
        break;
    
    default:
        break;
    }
    double x1 = 1.0*x+uw/2 + diag * cos(dangle + angle);
    double y1 = 1.0*y+uh/2 + diag * sin(dangle + angle);
    return {(int) x1,(int) y1};
}

bool CollisionRect::intersects(CollisionRect* collider){
    for (int x=0; x<2; x++)
    {
        CollisionRect* polygon = (x==0) ? this : collider;

        for (int i1=0; i1<4; i1++)
        {
            int   i2 = (i1 + 1) % 4;
            pair<int,int> p1 = polygon->getVertex(i1);
            pair<int,int> p2 = polygon->getVertex(i2);

            pair<int,int> normal(p2.second - p1.second, p1.first - p2.first);

            double minA = __DBL_MAX__;
            double maxA = - __DBL_MAX__;

            for (int i = 0; i< 4;i++)
            {
                pair<int,int> p = this->getVertex(i);
                double projected = normal.first * p.first + normal.second * p.second;

                if (projected < minA)
                    minA = projected;
                if (projected > maxA)
                    maxA = projected;
            }

            double minB = __DBL_MAX__;
            double maxB = - __DBL_MAX__;

            for (int i = 0; i< 4;i++)
            {
                pair<int,int> p = collider->getVertex(i);
                double projected = normal.first * p.first + normal.second * p.second;

                if (projected < minB)
                    minB = projected;
                if (projected > maxB)
                    maxB = projected;
            }

            if (maxA < minB || maxB < minA)
                return false;
        }
    }

    return true;
}

void CollisionRect::render(){
    if((x < gEngine->camera->x - TILE_SIZE) || (x > gEngine->camera->x + gEngine->camera->w + TILE_SIZE)){
        return;
    }
    if((y < gEngine->camera->y - TILE_SIZE) || (y > gEngine->camera->y + gEngine->camera->h + TILE_SIZE)){
        return;
    }
    for(int i =0;i<4;i++){
        pair<int,int> p1 = getVertex(i);
        pair<int,int> p2 = getVertex((i+1)%4);
        p1 = getPosOnCamera(p1);
        p2 = getPosOnCamera(p2);
        SDL_RenderDrawLine( gEngine->gRenderer, p1.first,p1.second,p2.first,p2.second );
    }
}

void CollisionRect::shift(int px,int py){
    x = px;
    y = py;
}