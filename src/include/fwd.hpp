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
class Player;
class TileMap;
class Bullet;

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

enum GameModeType{
	HOME=0,
	PAUSE,
	RESUME,
	PLAY,
	QUIT
};