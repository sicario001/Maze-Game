#include "TileMap.hpp"

TileMap::TileMap(){
    gTileSheet = new LTexture();
    generateTiles();
}

bool TileMap::getWhenZero(int i,vector<int> zero, vector<int> one){
    for(auto x : zero){
        if((i & (1<<(x-1)))!=0){
            return false;
        }
    }
    for(auto x : one){
        if((i & (1<<(x-1)))==0){
            return false;
        }
    }
    return true;
}

void TileMap::getTileClip(int bitmask, SDL_Rect& clip){
    //4 sides
    if(getWhenZero(bitmask,{2,4,5,7},{})){
        //TLRB
        clip.x = 6*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }

    // 3 sides
    if(getWhenZero(bitmask,{2,4,5},{7})){
        //TLR_
        clip.x = 5*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{2,4,7},{5})){
        //TL_B
        clip.x = 6*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{2,5,7},{4})){
        //T_RB
        clip.x = 5*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,5,7},{2})){
        //_LRB
        clip.x = 6*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }

    // 2 sides
    if(getWhenZero(bitmask,{2,7},{4,5})){
        //TB
        clip.x = 2*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,5},{2,7})){
        //LR
        clip.x = 2*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }

    if(getWhenZero(bitmask,{2,4},{5,7,8})){
        //TL
        clip.x = 7*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{2,5},{4,7,6})){
        //TR
        clip.x = 8*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,7},{2,5,3})){
        //LB
        clip.x = 7*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5,7},{2,4,1})){
        //RB
        clip.x = 8*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }

    if(getWhenZero(bitmask,{2,4,8},{5,7})){
        //TL.
        clip.x = 0*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{2,5,6},{4,7})){
        //TR.
        clip.x = 1*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,7,3},{2,5})){
        //LB.
        clip.x = 0*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5,7,1},{2,4})){
        //RB.
        clip.x = 1*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }

    // One Side
    if(getWhenZero(bitmask,{2,6,8},{4,5,7})){
        //T..
        clip.x = 3*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,3,8},{2,5,7})){
        //TR.
        clip.x = 3*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5,1,6},{2,4,7})){
        //LB.
        clip.x = 4*TILE_SIZE;
        clip.y = 5*TILE_SIZE;
    }    
    if(getWhenZero(bitmask,{7,1,3},{2,4,5})){
        //RB.
        clip.x = 4*TILE_SIZE;
        clip.y = 4*TILE_SIZE;
    }

    if(getWhenZero(bitmask,{2,6},{4,5,7,8})){
        //T._
        clip.x = 4*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,3},{2,5,7,8})){
        //TR.
        clip.x = 2*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5,1},{2,4,7, 6})){
        //LB.
        clip.x = 3*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{7,1},{2,4,5,3})){
        //RB.
        clip.x = 4*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }

    if(getWhenZero(bitmask,{2,8},{4,5,7,6})){
        //T_.
        clip.x = 5*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4,8},{2,5,7,3})){
        //TR.
        clip.x = 2*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5,6},{2,4,7,1})){
        //LB.
        clip.x = 3*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{7,3},{2,4,5,1})){
        //RB.
        clip.x = 5*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }

    if(getWhenZero(bitmask,{2},{4,5,7,6,8})){
        //T__
        clip.x = 4*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{4},{2,5,7,3,8})){
        //TR.
        clip.x = 4*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{5},{2,4,7,1,6})){
        //LB.
        clip.x = 5*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{7},{2,4,5,1,3})){
        //RB.
        clip.x = 5*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }

    // 0 side
    if(getWhenZero(bitmask,{1,3,6,8},{2,4,5,7})){
        //RB.
        clip.x = 7*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,3,6},{2,4,5,7,8})){
        //RB.
        clip.x = 0*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,3,8},{2,4,5,7,6})){
        //RB.
        clip.x = 1*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,6,8},{2,4,5,7,3})){
        //RB.
        clip.x = 0*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{3,6,8},{2,4,5,7,1})){
        //RB.
        clip.x = 1*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,3},{2,4,5,7,6,8})){
        //RB.
        clip.x = 3*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,6},{2,4,5,7,3,8})){
        //RB.
        clip.x = 3*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1,8},{2,4,5,7,3,6})){
        //RB.
        clip.x = 6*TILE_SIZE;
        clip.y = 9*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{3,6},{2,4,5,7,1,8})){
        //RB.
        clip.x = 6*TILE_SIZE;
        clip.y = 8*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{3,8},{2,4,5,7,1,6})){
        //RB.
        clip.x = 2*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{6,8},{2,4,5,7,1,3})){
        //RB.
        clip.x = 2*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{1},{2,4,5,7,3,6,8})){
        //RB.
        clip.x = 1*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{3},{2,4,5,7,1,6,8})){
        //RB.
        clip.x = 0*TILE_SIZE;
        clip.y = 7*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{6},{2,4,5,7,1,3,8})){
        //RB.
        clip.x = 1*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{8},{2,4,5,7,1,3,6})){
        //RB.
        clip.x = 0*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
    if(getWhenZero(bitmask,{},{2,4,5,7,1,3,6,8})){
        //RB.
        clip.x = 6*TILE_SIZE;
        clip.y = 6*TILE_SIZE;
    }
}

void TileMap::generateMap(){
    map = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1},
        {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

        };
}

bool TileMap::loadTexture(){
	//Load wall texture
	if( !gTileSheet->loadFromFile( "media/texture/tilesheet.png" ) )
	{
        return false;
	}
    return true;
}

void TileMap::render(){
    for(int i = 0; i < (int) floor.size();i++){
        floor[i].render();
    }
    for(int i = 0; i < (int) walls.size();i++){
        walls[i].render();
    }
}

void TileMap::handleCollisions(KinematicBody* body){
    for(int i = 0; i < (int) walls.size(); i++){
	    body->handleCollision(&walls[i]);
    }
}

void TileMap::generateTiles(){
    generateMap();
    int mapRows = (int) map.size();
    int mapCols = (int) map[0].size();
    for(int i=0;i< mapCols;i++){
        for(int j=0;j< mapRows;j++){
            SDL_Rect clip;
            clip.w = TILE_SIZE;
            clip.h = TILE_SIZE;
            if(map[j][i]==0){
                clip.x=(rand()%4)*TILE_SIZE;
                clip.y=0;
                switch (floor_type)
                {
                    case STONE:
                        clip.x += 6*TILE_SIZE;
                        break;
                    default:
                        break;
                }
                floor.push_back(Entity(i*TILE_SIZE,j*TILE_SIZE,gTileSheet ,&clip));
            } 
            else{
                int bitM = 0;
                int bitVal = 1;
                for(int y = -1; y <2; y++){
                    for(int x = -1; x <2; x++){
                        if((x!=0) || (y!=0)){
                            if((i+x>=0) && (i+x<mapCols)){
                                if((j+y>=0) && (j+y<mapRows)){
                                    bitM+=bitVal*map[j+y][i+x];
                                }
                            }
                            bitVal*=2;
                        }
                    }
                }
                CollisionRect* wallCollider = new CollisionRect(i*TILE_SIZE,j*TILE_SIZE,TILE_SIZE,TILE_SIZE);
                getTileClip(bitM,clip);
                switch (wall_type)
                {
                    case BROWN:
                        clip.x += 9*TILE_SIZE;
                        break;
                    case SILVER:
                        clip.x += 9*TILE_SIZE;
                        clip.y += 6*TILE_SIZE;
                        break;
                    case SILVER_FILLED:
                        clip.y += 6*TILE_SIZE;
                        break;
                    default:
                        break;
                }
                walls.push_back(RigidBody(i*TILE_SIZE,j*TILE_SIZE,wallCollider,gTileSheet ,&clip));
            }
        }
    }
}