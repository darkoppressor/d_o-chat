/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef client_h
#define client_h

#include "timer.h"
#include "client_data.h"

#include <string>
#include <vector>

#include <SDL_Mixer.h>

class Client{
    private:
    //We will use this to hold the keystates, which will be given to us by SDL.
    uint8_t *keystates;

    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];

    //Background music will be loaded into this variable.
    Mix_Music *music;

    //
    Timer timer_cursor;
    bool cursor;
    int cursor_opacity;

    public:
    //The constructor takes starting coordinates.
    Client();

    //
    ~Client();

    //
    void load_data();

    //Get input.
    void handle_input();

    //Handle events.
    //For now, this simply handles the blinking cursor and window captions.
    void handle_events(int frame_rate,double ms_per_frame);

    //Render everything.
    void render(int frame_rate,float ms_per_frame);

    void receive_client_list(RakNet::Packet *packet);

    void receive_chat_message(RakNet::Packet *packet);

    ///Will be a server function.
    void receive_name(RakNet::Packet *packet);

    ///Will be a server function.
    void server_receive_chat_message(RakNet::Packet *packet);

    unsigned short text_limit;

    std::string text_log[29];

    bool option_fullscreen;

    char option_text_color;

    bool option_dev;

    //Each instance of RakNet that connects (including the server itself) gets its own spot in this vector.
    //If a client disconnects and then reconnects, he will have the same id and thus the server can recognize him.
    //However, if a client closes the program and then re-runs it and connects, he will have a different id and thus
    //be treated like a new person.
    std::vector<client_data> vector_clients;

    std::string name;
};

#endif
