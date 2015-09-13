#include "update.h"
#include "world.h"
#include "quit.h"
#include "random_number_generator.h"

#include <SDL.h>

using namespace std;

//There is currently only one player. When I add multiplayer, I'll be using a client-server model, so each client will only be handling a lot of stuff for one player.
//I don't know exactly how that will work yet, so a lot of the following code might need to be changed later when I start implementing multiplayer.

//Gather input.
void input(){


    //
    client.handle_input();
}

//Handle events, such as collisions.
void events(){
}

//Handle animations.
void animation(){
}

//Render everything to the screen.
void render(int frame_rate,float ms_per_frame){
    //Clear the screen, filling it with the clear color set in Window.init().
    glClear(GL_COLOR_BUFFER_BIT);

    //Display the framerate and milliseconds per frame.
    ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;msg=ss.str();
    font.show(624,0,msg,RED);
    ss.clear();ss.str("");ss<<"MS Per Frame: ";ss<<ms_per_frame;msg=ss.str();
    font.show(624,30,msg,RED);

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
