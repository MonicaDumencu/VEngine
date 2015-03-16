#pragma once
/*
* This header file contains all the controlling keys for our project
* Author: Kan
*/
#include "Keyboard.h"

//player behavior
#define PLAYER_JUMP				KEYBOARD_G
#define PLAYER_TURN_LEFT		KEYBOARD_J
#define PLAYER_TURN_RIGHT		KEYBOARD_L
#define PLAYER_MOVE_FOWARD		KEYBOARD_K
#define PLAYER_MOVE_BACKWARD	KEYBOARD_O
#define PLAYER_MOVE_M			KEYBOARD_M
#define PLAYER_MOVE_N			KEYBOARD_N
#define PLAYER_DEBUG			KEYBOARD_T
#define PLAYER_FLY              KEYBOARD_Y
//camera behavior
#define CAMERA_FOWARD			KEYBOARD_W
#define CAMERA_BACKWARD			KEYBOARD_S
#define CAMERA_LEFT				KEYBOARD_A
#define CAMERA_RIGHT			KEYBOARD_D
#define CAMERA_UP				KEYBOARD_SHIFT
#define CAMERA_DOWN				KEYBOARD_SPACE
#define CAMERA_SPEED_UP			KEYBOARD_PRIOR	//this is pageup key
#define CAMERA_SPEED_DOWN		KEYBOARD_NEXT	//this is pagedown key

//cube robot related
//don't know what they mean but...
#define IS_RUM					KEYBOARD_I
#define IS_NOT_RUM				KEYBOARD_K

//HUD related
#define TOGGLE_DEBUG_MODE		KEYBOARD_1
#define TOGGLE_PAUSE_SCREEN		KEYBOARD_P
#define MENU_UP					KEYBOARD_UP
#define MENU_DOWN				KEYBOARD_DOWN
#define MENU_ENTER				KEYBOARD_E

//Window related
#define WINDOW_CLOSE			KEYBOARD_ESCAPE

//Terrain related
#define TERRAIN_RESTART			KEYBOARD_R

//spawning related
#define BUFF_PLAYER				KEYBOARD_UP
#define DEBUFF_PLAYER			KEYBOARD_DOWN

//render related
#define APPLY_BLUR				KEYBOARD_B
#define APPLY_FOG				KEYBOARD_F

//particle related
#define ToggleRain				KEYBOARD_Z
#define ToggleSnow				KEYBOARD_X
