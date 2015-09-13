/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "world.h"
#include "render.h"

using namespace std;

//Create the main window.
Game_Window main_window;

Random_Number_Generator rng;

//
Client client;

//
Network network;

//We use this to poll for events when getting input.
SDL_Event event;

//The game's bitmap font.
BitmapFont font;

//String stream used to easily render ints and stuff as strings.
stringstream ss("");

//ss passes its information into this string for output.
string msg="";

string_input string_input_chat;

Image image;

//All of the world variables are defined here.

//Here we define the sound files.
///Mix_Chunk *sound_shot=NULL;

//This function is called at the beginning of the game.
void load_world(){
    //Generate the font.
    font.build_font("data/images/bitmap_font.png",4096,16,11,24);

    //Load the textures.
    image.load_images_global();

    //Load the sound effects.
    ///sound_shot=Mix_LoadWAV("data/sounds/shot.wav");
}

//This is called in quit_game(). It frees all of the memory allocated in load_world().
void unload_world(){
    image.current_texture=0;

    //Unload the fonts.
    font.unload_font();

    //Delete the surfaces and textures:
    image.unload_images_global();

    //Free the sound effects.
    ///Mix_FreeChunk(sound_shot);
}
