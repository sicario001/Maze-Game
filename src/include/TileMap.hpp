#pragma once
#include <vector>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "PhysicsObject.hpp"
#include "ServerNet.hpp"
#include "ClientNet.hpp"

using namespace std;

const int TILE_SIZE = 128;

enum FLOOR_TYPE{
    GRASS,
    STONE
};

enum WALL_TYPE{
    ORANGE,
    BROWN,
    SILVER,
    SILVER_FILLED
};

class TileMap
{
private:
    LTexture* gTileSheet;
    
    pthread_mutex_t mutex;
    pthread_mutex_t mutex1;
    pthread_cond_t receiveMapSignal;

    // Different map styles
    int floor_type = GRASS;
    int wall_type = BROWN;

    vector<Entity> floor;
    vector<RigidBody> walls;

    // To get the position in source to clip from
    bool getWhenZero(int i,vector<int> zero, vector<int> one);
    void getTileClip(int bitmask, SDL_Rect& clip);

    // generate the vector of 0s and 1s denoting w=positions with walls
    void generateMap();
    // generate floors and walls
    void generateTiles();
public:
    TileMap(ClientNet* client, ServerNet* server);
    ~TileMap();
    bool loadTexture();
    vector<vector<bool>> map;
    bool received = false;
    void render();
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