/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "quit.h"
#include "world.h"
#include "network.h"

#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

void quit_game(){
    //If any music is playing, fade it out and then stop it.
    while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()){
        SDL_Delay(100);
    }
    Mix_HaltMusic();

    //Disable unicode.
    SDL_EnableUNICODE(SDL_DISABLE);

    //This function frees any dynamically allocated memory that was loaded in load_world().
    unload_world();

    //Close the audio subsystem.
    Mix_CloseAudio();

    //Shutdown SDL.
    SDL_Quit();

    //Print a success message to stdout.txt.
    printf("Exited successfully!\n");

    //Tell the program to exit successfully.
    exit(EXIT_SUCCESS);
}
