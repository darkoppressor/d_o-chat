/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef world_h
#define world_h

#include <string>
#include <sstream>

#include "font.h"
#include "client.h"
#include "game_window.h"
#include "network.h"
#include "string_input.h"
#include "image.h"
#include "random_number_generator.h"

extern Game_Window main_window;

extern Random_Number_Generator rng;

extern Client client;

extern Network network;

/**const int SPECIAL_RAINBOW=0;
const int SPECIAL_GRAY=-1;
const int SPECIAL_WHITE=-2;
const int SPECIAL_BROWN=-3;
const int SPECIAL_YELLOW=-4;
const int SPECIAL_ORANGE=-5;
const int SPECIAL_RED=-6;
const int SPECIAL_PINK=-7;
const int SPECIAL_PURPLE=-8;
const int SPECIAL_BLUE=-9;
const int SPECIAL_GREEN=-10;
const int SPECIAL_WARM=-11;
const int SPECIAL_COOL=-12;

const int BLACK=1;

const int GRAY=2;
const int PLATINUM=3;
const int SILVER=4;
const int COOLGRAY=5;

const int WHITE=6;
const int GHOSTWHITE=7;
const int AFWHITE=8;

const int BROWN=9;
const int AUBURN=10;
const int BRONZE=11;
const int COPPER=12;
const int SAND=13;

const int YELLOW=14;
const int SCHOOLBUS=15;
const int GOLD=16;

const int ORANGE=17;
const int PUMPKIN=18;
const int AMBER=19;

const int RED=20;
const int FIREENGINE=21;
const int FLAME=22;

const int PINK=23;
const int ROSE=24;
const int SHOCKINGPINK=25;
const int ULTRAPINK=26;

const int PURPLE=27;
const int VIOLET=28;
const int INDIGO=29;

const int BLUE=30;
const int SKYBLUE=31;
const int UNBLUE=32;

const int GREEN=33;
const int JUNGLE=34;
const int SPRING=35;*/

extern SDL_Event event;

extern BitmapFont font;

extern std::stringstream ss;
extern std::string msg;

extern string_input string_input_chat;

extern Image image;

///extern Mix_Chunk *sound_shot;

void load_world();

void unload_world();

#endif
