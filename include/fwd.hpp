#pragma once
class GameEngine;
class GameMode;
class PlayMode;
class PauseMode;
class HomeMode;
class LButton;
class LTexture;

class Entity;
class RigidBody;
class KinematicBody;
class Throwable;
class Player;

class HealthBar;
class TileMap;
class Clock;
class LTimer;
class ProgressBar;
class Inventory;
class ScoreBoard;
class PingStatus;

class Throwable;
class LoadingScreen;
class GameMessage;

class AudioMaster;
class AudioSource;
class AudioStore;
class TextureStore;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_MOUSE_OUT_ALT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION_ALT,
	BUTTON_SPRITE_MOUSE_DOWN_ALT,
	BUTTON_SPRITE_MOUSE_UP_ALT,
	BUTTON_SPRITE_TOTAL 
};

enum ButtonType
{
	BUTTON_PRIM,
	BUTTON_ALT
};
enum PlayerObj{
	ATTACK = 0,
	DEFEND = 1
};
enum GameModeType{
	HOME=0,
	PAUSE,
	RESUME,
	PLAY,
	QUIT
};
enum BombState{
	IDLE = 0,
	PLANTING = 1,
	PLANTED = 2,
	DEFUSING = 3,
	DEFUSED = 4
};

enum WeaponType{
	KNIFE = 0,
	RIFLE = 1,
	PISTOL = 2,
	// SMOKE = 2,
	// FLASH = 3,
	// GRENADE = 4
	// BOMB = 5,
	// DEFUSE_KIT = 6
};
enum ThrowableType{
	BULLET = 0,
	KNIFE_SLASH = 1
};

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

const int CAMERA_WIDTH = 480;
const int CAMERA_HEIGHT = 270;

const int LEVEL_WIDTH = 2560;
const int LEVEL_HEIGHT = 1920;

const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 2;

const int PLAYER_SPRITE_SIZE = 71;
const int PLAYER_COLLIDER_SIZE = 45;

const int BULLET_SPRITE_H = 16;
const int BULLET_SPRITE_W = 16;
const int BULLET_COLLIDER_H = 8;
const int BULLET_COLLIDER_W = 14;

const int SLASH_SPRITE_H = 129;
const int SLASH_SPRITE_W = 110;
const int SLASH_COLLIDER_H = 20;
const int SLASH_COLLIDER_W = 20;

const int TILE_SIZE = 128;

const double PI = 3.1415926535;

