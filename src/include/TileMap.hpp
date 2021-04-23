#pragma once
#include <vector>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "PhysicsObject.hpp"
#include "ServerNet.hpp"
#include "ClientNet.hpp"

using namespace std;

class TileMap
{
private:
    LTexture* gTileSheet;
    const int TILE_SIZE = 64;
    
    pthread_mutex_t mutex;
    pthread_mutex_t mutex1;
    pthread_cond_t receiveMapSignal;

    vector<Entity> floor;
    vector<RigidBody> walls;
    bool getWhenZero(int i,vector<int> zero, vector<int> one);
    void getTileClip(int bitmask, SDL_Rect& clip);
    void generateMap();
    
    void generateRandMap();
public:
    TileMap(ClientNet* client, ServerNet* server);
    ~TileMap();
    bool loadTexture();
    vector<vector<bool>> map;
    bool received = false;
    void render();
    void renderLocal(Player* player);
    void generateTiles(ClientNet* client, ServerNet* server);
    void handleCollisions(KinematicBody* body);
    void updateMapfromServer(vector<int> map_vec);
    bool getReceived();
    void setReceived();
    void setNotReceived();
    void waitToReceiveMap();
    bool getMap(int i, int j);
    void setMap(int i, int j, bool val);
    void initializeMap(int i, int j);
    pair<int, int> getMapSize();
    void cloneMap(vector<vector<bool>> &vec_to_clone);
};