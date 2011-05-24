/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "client.h"
#include "render.h"
#include "world.h"
#include "quit.h"
#include "network.h"
#include "random_number_generator.h"

#include <ctime>

using namespace std;
using namespace RakNet;

Client::Client(){
    //Start the keystates variable as NULL.
    keystates=NULL;

    timer_cursor.start();
    cursor=false;
    cursor_opacity=10;

    vector_clients.clear();

    name="";

    text_limit=29;

    for(int i=0;i<text_limit;i++){
        text_log[i]="";
    }
    //text_log[0]=">: Type /client to connect to a server, or /server to start as a server.";
    //text_log[1]=">: For more options, type /help.";
    text_log[text_limit-2]=">: Type /client to connect to a server, or /server to start as a server.";
    text_log[text_limit-1]=">: For more options, type /help.";

    option_fullscreen=false;
    option_text_color=COLOR_GREEN;
    option_dev=false;
}

Client::~Client(){
    //Free the music.
    Mix_FreeMusic(music);
    music=NULL;
}

void Client::load_data(){
    unsigned int temp_i=random_range(0,99999);
    ss.clear();ss.str("");ss<<"User-";ss<<temp_i;msg=ss.str();
    name=msg;

    //If any music is playing, fade it out and then stop it.
    while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()){
        SDL_Delay(100);
    }
    Mix_HaltMusic();

    //Free the music.
    Mix_FreeMusic(music);
    music=NULL;

    //Load the background music. The music that is loaded will later be determined by which level is being loaded.
    music=Mix_LoadMUS("data/music/1.ogg");

    //Begin playing the background music for this level.
    //Mix_FadeInMusic(music,-1,500);
}

void Client::handle_input(){
    //Check to see if the player has X'ed out of the game. If so, run quit_game().
    while(SDL_PollEvent(&event)){
        SDL_EnableUNICODE(SDL_ENABLE);
        string_input_chat.handle_events();
        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;
            case SDL_MOUSEBUTTONDOWN:
                int mouse_x=0;
                int mouse_y=0;
                if(event.button.button==SDL_BUTTON_LEFT){
                    mouse_x=event.button.x;
                    mouse_y=event.button.y;
                }
                break;
        }
    }

    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);

    //If the player hits the screenshot key, take a screenshot.
    if(keystates[SDLK_F5]){
        main_window.screenshot();

        //Once the screenshot key has been hit, the player must release it for it to function again.
        keystates[SDLK_F5]=NULL;
    }

    int mouse_x=0;
    int mouse_y=0;

    if(SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
    }
}

void Client::handle_events(int frame_rate,double ms_per_frame){
    now=time(NULL);
    tm_now=localtime(&now);
    strftime(buff,sizeof buff,"   %A, %B %d, %Y - %H:%M:%S",tm_now);

    //Set the window caption.
    msg="D_O Chat - ";
    msg+=name;
    msg+=buff;
    msg+=" FPS: ";
    string temp="";
    ss.clear();ss.str("");ss<<frame_rate;temp=ss.str();
    msg+=temp;
    SDL_WM_SetCaption(msg.c_str(),msg.c_str());

    if(timer_cursor.get_ticks()>=50){
        if(cursor_opacity==10){
            cursor=false;
        }
        else if(cursor_opacity==0){
            cursor=true;
        }

        if(cursor && cursor_opacity<10){
            cursor_opacity+=1;
        }
        else if(!cursor && cursor_opacity>0){
            cursor_opacity-=1;
        }

        timer_cursor.start();
    }
}

void Client::render(int frame_rate,float ms_per_frame){
    //Clear the screen, filling it with the clear color set in Window.init().
    glClear(GL_COLOR_BUFFER_BIT);

    //render_texture(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,background_static,WHITE,1.0);

    //render_texture((main_window.SCREEN_WIDTH-800)/2,(main_window.SCREEN_HEIGHT-600)/2,800,600,text_back,WHITE,1.0);

    for(int i=0;i<text_limit;i++){
        if(text_log[i]!=""){
            font.show(55,20+i*22,text_log[i],option_text_color,1.0);
        }
    }

    int chat_input_height=690;
    /**for(int i=text_limit-1;i>-1;i--){
        if(text_log[i]==""){
            chat_input_height=20+i*22;
        }
    }*/

    font.show(55,chat_input_height,">: "+string_input_chat.str1,option_text_color,1.0);
    font.show(55,chat_input_height+20,"   "+string_input_chat.str2,option_text_color,1.0);
    font.show(55,chat_input_height+40,"   "+string_input_chat.str3,option_text_color,1.0);

    if(string_input_chat.str1.length()<70){
        font.show(88+11*string_input_chat.str1.length(),chat_input_height,"\x7F",option_text_color,cursor_opacity*0.1);
    }
    else if(string_input_chat.str1.length()==70 && string_input_chat.str2.length()<70){
        font.show(88+11*string_input_chat.str2.length(),chat_input_height+20,"\x7F",option_text_color,cursor_opacity*0.1);
    }
    else if(string_input_chat.str1.length()==70 && string_input_chat.str2.length()==70){
        font.show(88+11*string_input_chat.str3.length(),chat_input_height+40,"\x7F",option_text_color,cursor_opacity*0.1);
    }

    if(option_dev){
        //Display the framerate and milliseconds per frame.
        ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;msg=ss.str();
        font.show(624,0,msg,option_text_color,1.0);
        ss.clear();ss.str("");ss<<"MS Per Frame: ";ss<<ms_per_frame;msg=ss.str();
        font.show(624,30,msg,option_text_color,1.0);
    }

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}

void Client::receive_client_list(Packet *packet){
    MessageID type_id;
    MessageID use_timestamp;
    Time time_stamp;

    int send_size=0;
    client_data temp_data;
    RakString rstring;
    vector_clients.clear();

    BitStream bitstream(packet->data,packet->length,false);

    bitstream.Read(type_id);
    bitstream.Read(use_timestamp);
    bitstream.Read(time_stamp);
    bitstream.Read(send_size);

    for(int i=0;i<send_size;i++){
        bitstream.Read(temp_data.id);
        bitstream.Read(temp_data.address);
        bitstream.Read(rstring);
        temp_data.name=rstring;
        bitstream.Read(temp_data.connected);
        vector_clients.push_back(temp_data);
    }
}

void Client::receive_name(Packet *packet){
    MessageID type_id;
    MessageID use_timestamp;
    Time time_stamp;

    RakString rstring;
    string temp_name;
    bool connection;
    RakNetGUID id;

    BitStream bitstream(packet->data,packet->length,false);

    bitstream.Read(type_id);
    bitstream.Read(use_timestamp);
    bitstream.Read(time_stamp);
    bitstream.Read(rstring);
    temp_name=rstring;
    bitstream.Read(connection);
    bitstream.Read(id);

    //Find the client in the clients vector with this id.
    for(int i=0;i<client.vector_clients.size();i++){
        if(client.vector_clients.at(i).id==id){
            //Set the name for the client in its clients vector element.
            client.vector_clients.at(i).name=temp_name;

            //If this is a connection, and not merely a name change.
            if(connection){
                //Let everyone know that this client has connected.
                ss.clear();ss.str("");ss<<">: ";ss<<temp_name;ss<<" has connected from ";ss<<client.vector_clients.at(i).address.ToString(true);ss<<".";msg=ss.str();

                //Print the message on this instance's screen.
                for(int i=0;i<client.text_limit;i++){
                    if(i==client.text_limit-1){
                        client.text_log[i]=msg;
                    }
                    else{
                        client.text_log[i]=client.text_log[i+1];
                    }
                }

                BitStream send_bitstream;

                send_bitstream.Write((MessageID)ID_CHAT_MESSAGE);
                send_bitstream.Write((MessageID)ID_TIMESTAMP);
                send_bitstream.Write(GetTime());
                rstring=(msg.c_str());
                send_bitstream.Write(rstring);

                //Send the message to everyone.
                network.peer->Send(&send_bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,true);
            }

            send_clients_list();
        }
    }
}

void Client::receive_chat_message(Packet *packet){
    MessageID type_id;
    MessageID use_timestamp;
    Time time_stamp;

    RakString rstring;
    string message;

    BitStream bitstream(packet->data,packet->length,false);

    bitstream.Read(type_id);
    bitstream.Read(use_timestamp);
    bitstream.Read(time_stamp);
    bitstream.Read(rstring);
    message=rstring;

    //Print the message on this instance's screen.
    for(int i=0;i<client.text_limit;i++){
        if(i==client.text_limit-1){
            client.text_log[i]=message;
        }
        else{
            client.text_log[i]=client.text_log[i+1];
        }
    }
}

void Client::server_receive_chat_message(Packet *packet){
    MessageID type_id;
    MessageID use_timestamp;
    Time time_stamp;

    RakString rstring;
    string message;

    BitStream bitstream(packet->data,packet->length,false);

    bitstream.Read(type_id);
    bitstream.Read(use_timestamp);
    bitstream.Read(time_stamp);
    bitstream.Read(rstring);
    message=rstring;

    //Print the message on this instance's screen.
    for(int i=0;i<client.text_limit;i++){
        if(i==client.text_limit-1){
            client.text_log[i]=message;
        }
        else{
            client.text_log[i]=client.text_log[i+1];
        }
    }

    //Send the message to everyone else except for the sender.
    network.peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,true);
}
