#pragma once
#include <vector>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "PhysicsObject.hpp"

using namespace std;

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
    const int TILE_SIZE = 64;
    vector<vector<bool>> map;

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
    TileMap();
    ~TileMap();
    bool loadTexture();

    void render();
    // check for collision of body with any wall
    void handleCollisions(KinematicBody* body);
};