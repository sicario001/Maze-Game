#include "TileMap.hpp"


TileMap::TileMap(ClientNet* client, ServerNet* server){
    gTileSheet = new LTexture();
    pthread_mutex_init( &mutex, NULL);
    pthread_mutex_init( &mutex1, NULL);
    pthread_cond_init( &receiveMapSignal, NULL);
    if (client!=NULL){
        setNotReceived();
    }
    else{
        setReceived();
    }
    
    // generateTiles(client, server);
}
TileMap::~TileMap(){
    delete(gTileSheet);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&receiveMapSignal);

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
void TileMap::generateRandMap(){
    srand(time(NULL));
    initializeMap(15, 20);
    for (int i=0; i<15; i++){
        for (int j=0; j<20; j++){
            setMap(i, j, rand()%2);
        }
    }
    setMap(0, 0, 0);
    setMap(14, 19, 0);
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

void TileMap::renderLocal(Player* player){
    int x = player->getPosX();
    int y = player->getPosY();
    int ver_reg = y/240;
    int hor_reg = x/320;
    for(int i = 0; i < (int) floor.size();i++){
        int x_floor = floor[i].getPosX();
        int y_floor = floor[i].getPosY();
        int ver1 = y_floor/240;
        int ver2 = (y_floor+TILE_SIZE-1)/240;
        int hor1 = x_floor/320;
        int hor2 = (x_floor+TILE_SIZE-1)/320;
        if ((hor_reg==hor1||hor_reg==hor2) && (ver_reg==ver1||ver_reg==ver2)){
            SDL_Rect renderQuad = { 4*(x_floor-320*hor_reg), 4*(y_floor-240*ver_reg), 4*TILE_SIZE, 4*TILE_SIZE }; 
            floor[i].renderCustom(&renderQuad);
        } 
    }
    for(int i = 0; i < (int) walls.size();i++){
        int x_walls = walls[i].getPosX();
        int y_walls = walls[i].getPosY();
        int ver1 = y_walls/240;
        int ver2 = (y_walls+TILE_SIZE-1)/240;
        int hor1 = x_walls/320;
        int hor2 = (x_walls+TILE_SIZE-1)/320;
        if ((hor_reg==hor1||hor_reg==hor2) && (ver_reg==ver1||ver_reg==ver2)){
            SDL_Rect renderQuad = { 4*(x_walls-320*hor_reg), 4*(y_walls-240*ver_reg), 4*TILE_SIZE, 4*TILE_SIZE }; 
            walls[i].renderCustom(&renderQuad);
        } 
    }
    

}

void TileMap::handleCollisions(KinematicBody* body){
    for(int i = 0; i < (int) walls.size(); i++){
	    body->handleCollision(&walls[i]);
    }
}

void TileMap::generateTiles(ClientNet* client, ServerNet* server){
    
    if (server!=NULL){
        generateRandMap();
        // cout<<"\nWait for Connected\n";
        server->waitForConnection();
        
        vector<vector<bool>> map_clone;
        
        cloneMap(map_clone);
        
        server->SendMap(server->peer, map_clone);
        
    }
    int mapRows = (int) getMapSize().first;
    int mapCols = (int) getMapSize().second;
    for(int i=0;i< mapCols;i++){
        for(int j=0;j< mapRows;j++){
            SDL_Rect clip;
            clip.w = TILE_SIZE;
            clip.h = TILE_SIZE;
            if(getMap(j, i)==0){
                clip.x=0;
                clip.y=0;
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
                                    bitM+=bitVal*getMap(j+y, i+x);
                                }
                            }
                            bitVal*=2;
                        }
                    }
                }
                getTileClip(bitM,clip);
                CollisionRect* wallCollider = new CollisionRect(i*TILE_SIZE,j*TILE_SIZE,TILE_SIZE,TILE_SIZE);
                walls.push_back(RigidBody(i*TILE_SIZE,j*TILE_SIZE,wallCollider,gTileSheet ,&clip));
            }
        }
    }
}

bool TileMap::getReceived(){
    pthread_mutex_lock(&mutex);
    bool rec_val = received;
    pthread_mutex_unlock(&mutex);
    return rec_val;
}

void TileMap::setReceived(){
    pthread_mutex_lock(&mutex);
    received = true;
    pthread_cond_signal(&receiveMapSignal);
    pthread_mutex_unlock(&mutex);
}
void TileMap::setNotReceived(){
    pthread_mutex_lock(&mutex);
    received = false;
    pthread_mutex_unlock(&mutex);
}

void TileMap::waitToReceiveMap(){
    pthread_mutex_lock(&mutex);
    while (!received){
        pthread_cond_wait(&receiveMapSignal, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

bool TileMap::getMap(int i, int j){
    pthread_mutex_lock(&mutex1);
    bool val = map[i][j];
    pthread_mutex_unlock(&mutex1);
    return val;
}

void TileMap::setMap(int i, int j, bool val){
    pthread_mutex_lock(&mutex1);
    map[i][j] = val;
    pthread_mutex_unlock(&mutex1);
}
void TileMap::initializeMap(int i, int j){
    pthread_mutex_lock(&mutex1);
    // std::cout<<"in"<<endl;
    // cout<<TILE_SIZE<<" "<<received<<endl;
    map = vector<vector<bool>>(i, vector<bool>(j));
    // std::cout<<"out"<<endl;
    pthread_mutex_unlock(&mutex1);
}
pair<int, int> TileMap::getMapSize(){
    pthread_mutex_lock(&mutex1);
    int size_1 = map.size();
    int size_2 = map[0].size();
    pthread_mutex_unlock(&mutex1);
    return {size_1, size_2};
}
void TileMap::cloneMap(vector<vector<bool>> &vec_to_clone){
    pthread_mutex_lock(&mutex1);
    vec_to_clone = vector<vector<bool>>(map.size(), vector<bool>(map[0].size()));
    for (int i=0; i<(int)map.size(); i++){
        for (int j=0; j<(int)map[0].size(); j++){
            vec_to_clone[i][j] = map[i][j];
        }
    }
    pthread_mutex_unlock(&mutex1);
}