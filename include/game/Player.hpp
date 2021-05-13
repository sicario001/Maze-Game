#pragma once

#include "engine/graphics/PhysicsObject.hpp"
#include "engine/audio/AudioSource.hpp"

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

enum PlayerStanceType{
    PISTOL_EQUIPPED,
    EMPTY_HANDED,
    RIFLE_EQUIPPED,
    RELOADING,
    OTHER_GUN_EQUIPPED,
    STANDING
};

class Player : public KinematicBody{
private:
    int dirX;
    int dirY;

    function <void(int x,int y, int speed, double angle, int damage, ThrowableType type)> spawnFunc;

    void shoot(ThrowableType t);

    int fireLock = 0;
    int xMouse,yMouse;
    int health = 100;
    void resetRotation();

    AudioSource* walkingSounds;
    AudioSource* shootingSounds;
    AudioSource* reloadingSounds;
    AudioSource* slashingSounds;
    void resetAudioSourcePosition();
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
    void updateFireLock();
    void free();
};