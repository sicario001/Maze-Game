#include "game/TileMap.hpp"
#include "game/Throwable.hpp"

TileMap::TileMap(ClientNet* client, ServerNet* server){
    gTileSheet = new LTexture();
    pthread_mutex_init( &mutex1, NULL);
    
    // generateTiles(client, server);
}
TileMap::~TileMap(){
    delete(gTileSheet);
    pthread_mutex_destroy(&mutex1);
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
void dfs(int x, int y, vector<vector<bool>> &m){
    m[x][y] = true;
    for (int i=-1; i<2; i++){
        for (int j=-1; j<2; j++){
            if ((i!=0 || j!=0) && (i==0 || j==0) && ((x+i)>=0 && (x+i)<(int)m.size() && (y+j)>=0 && (y+j)<(int)m[0].size())){
                if (!m[x+i][y+j]){
                    dfs(x+i, y+j, m);
                }
            }
        }
    }
}
bool checkConnected(vector<vector<bool>> &m){
    int cnt = 0;
    for (int i=0; i<(int)m.size(); i++){
        for (int j=0; j<(int)m[0].size(); j++){
            if (!m[i][j]){
                cnt++;
                dfs(i, j, m);
            }
        }
    }
    if (cnt<=1){
        return true;
    }
    return false;
    
}
void TileMap::generateMap1(double density){
    srand(time(NULL));
    int numRows = LEVEL_HEIGHT/TILE_SIZE;
    int numCols = LEVEL_WIDTH/TILE_SIZE;
    initializeMap(numRows, numCols);
    vector<vector<bool>> bad_points(numRows, vector<bool>(numCols, false));
    int numWalls = density*(numRows*numCols);
    int s1_x = 0;
    int s1_y = 0;
    int s2_x = numRows-1;
    int s2_y = numCols-1;
    while (numWalls>0){
        int x = rand()%numRows;
        int y = rand()%numCols;
        if (getMap(x, y) || bad_points[x][y] || (x==s1_x && y==s1_y) || (x==s2_x && y ==s2_y)){
            continue;
        }
        vector<vector<bool>> m;
        cloneMap(m);
        m[x][y] = true;
        if (!checkConnected(m)){
            bad_points[x][y] = true;
            continue;
        }
        setMap(x, y, true);
        numWalls--;
    }
    setMap(0, 0, 0);
    setMap(numRows-1, numCols-1, 0);
}



void TileMap::generateMap(){
    srand(time(NULL));
    int numRows = LEVEL_HEIGHT/TILE_SIZE;
    int numCols = LEVEL_WIDTH/TILE_SIZE;
    initializeMap(numRows, numCols);
    for (int i=0; i<numRows; i++){
        for (int j=0; j<numCols; j++){
            setMap(i, j, rand()%2);
        }
    }
    setMap(0, 0, 0);
    setMap(numRows-1, numCols-1, 0);
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
        // walls[i].getCollisionRect()->render();
    }
}


void TileMap::handleCollisions(KinematicBody* body){
    for(int i = 0; i < (int) walls.size(); i++){
	    body->handleCollision(&walls[i]);
    }
}

void TileMap::handleThrowables(Throwable* x){
    for(int i = 0; i < (int) walls.size(); i++){
        if(x->handleCollision(&walls[i])){
            x->onHit();
            break;
        }
    }
}
void TileMap::sendMapToClient(ServerNet* server){
    vector<vector<bool>> map_clone;
    cloneMap(map_clone);
    server->SendMap(server->peer, map_clone);
}

void TileMap::generateTiles(ServerNet* server){
    
    if (server!=NULL){
        generateMap1(0.5);
    }
    int mapRows = (int) getMapSize().first;
    int mapCols = (int) getMapSize().second;
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
                                    bitM+=bitVal*getMap(j+y, i+x);
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