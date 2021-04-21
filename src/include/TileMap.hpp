#pragma once
#include <vector>
#include <SDL2/SDL_image.h>
#include "fwd.hpp"
#include "PhysicsObject.hpp"

using namespace std;

class TileMap
{
private:
    LTexture* gTileSheet;
    const int TILE_SIZE = 64;
    vector<vector<bool>> map;

    vector<Entity> floor;
    vector<RigidBody> walls;
    bool getWhenZero(int i,vector<int> zero, vector<int> one);
    void getTileClip(int bitmask, SDL_Rect& clip);
    void generateMap();
    void generateTiles();
public:
    TileMap();
    ~TileMap();
    bool loadTexture();

    void render();
    void handleCollisions(KinematicBody* body);
};