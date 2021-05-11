#pragma once

#include "engine/graphics/PhysicsObject.hpp"
#include "engine/audio/AudioSource.hpp"


const int PLAYER_SPRITE_SIZE = 71;
const int PLAYER_COLLIDER_SIZE = 45;

enum PlayerSpriteType{
	HITMAN,
	MAN_BLUE,
	MAN_BROWN,
	MAN_OLD,
	ROBOT,
	SOLDIER,
	SURVIVOR,
	WOMAN,
	ZOMBIE
};

class Player : public KinematicBody{
private:
    function <void(int x,int y, int speed, double angle, int damage, ThrowableType type)> shoot;
    int xMouse,yMouse;
    int health = 100;
    void resetRotation();

    AudioSource* walkingSounds;
    AudioSource* shootingSounds;
    AudioSource* reloadingSounds;
    AudioSource* slashingSounds;
    
    PlayerSpriteType playerType;
public:
    Player(int health, int x, int y, LTexture* pbt,SDL_Rect* pClip,function <void(int x,int y, int speed, double angle, int damage, ThrowableType type)> shootFunc, PlayerSpriteType type);
    Inventory* inventory = NULL;
    bool isReloading = false;
    int getHealth();
    ProgressBar* reloadBar = NULL;
    void damage(int x);
    // send position,velocity and rotation
    void sendUpdate(ClientNet* clientObj, ServerNet* serverObj);
    void handleEvent( SDL_Event& e);
    void resetCamera();

    void playSoundIfWalked(bool isListener);
    void playThrowableSound(ThrowableType t);

    void resetClip();
    void stopReloading();

    void release();
};