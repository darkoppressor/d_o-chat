/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "string_input.h"
#include "world.h"
#include "quit.h"

#include "RakNet/Source/RakPeer.h"

#include <ctime>

using namespace std;
using namespace RakNet;

string_input::string_input(){
    str1="";
    str2="";
    str3="";
}

string_input::~string_input(){
}

void string_input::handle_events(){
    if(event.type==SDL_KEYDOWN){
        if(str1.length()<70){/**If the string is less than the maximum size.*/
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str1+=(char)event.key.keysym.unicode;
            }
        }
        else if(str2.length()<70){/**If the string is less than the maximum size.*/
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str2+=(char)event.key.keysym.unicode;
            }
        }
        else if(str3.length()<70){/**If the string is less than the maximum size.*/
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str3+=(char)event.key.keysym.unicode;
            }
        }

        if(event.key.keysym.sym==SDLK_BACKSPACE && str3.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            str3.erase(str3.length()-1);/**Remove one character from the end of the string.*/
        }
        else if(event.key.keysym.sym==SDLK_BACKSPACE && str2.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            str2.erase(str2.length()-1);/**Remove one character from the end of the string.*/
        }
        else if(event.key.keysym.sym==SDLK_BACKSPACE && str1.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            str1.erase(str1.length()-1);/**Remove one character from the end of the string.*/
        }

        if(event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER){/**If the player hits enter.*/
            if(str1[0]=='/'){
                str1.erase(0,1);
                string str_command=str1+str2+str3;
                if(str_command=="client" || str_command=="c"){
                    if(!network.network_initialized){
                        network.initialize(false);
                    }
                    else{
                        if(network.this_instance_server){
                            update_text_log("Server mode already initialized.",2);
                        }
                        else{
                            update_text_log("Client mode already initialized.",2);
                        }
                    }
                }

                else if(str_command=="server" || str_command=="s"){
                    if(!network.network_initialized){
                        network.initialize(true);
                    }
                    else{
                        if(network.this_instance_server){
                            update_text_log("Server mode already initialized.",2);
                        }
                        else{
                            update_text_log("Client mode already initialized.",2);
                        }
                    }
                }

                //
                else if(str_command=="start"){
                    //If the network is initialized.
                    if(network.network_initialized){
                        //If we are the server.
                        if(network.this_instance_server){
                            //If the server is inactive.
                            if(!network.peer->IsActive()){
                                //Start the server.
                                network.peer->Startup(network.MAX_CLIENTS,&SocketDescriptor(network.SERVER_PORT,0),1);
                                update_text_log("Starting server.",2);
                            }
                            //If the server is active.
                            else{
                                update_text_log("Server already started.",2);
                            }
                        }
                        //If we are a client.
                        else{
                            update_text_log("Cannot start the server, you are a client!",2);
                        }
                    }
                    //If the network has not been initialized yet.
                    else{
                        update_text_log("Network not initialized.",2);
                    }
                }

                //
                else if(str_command=="connect" || str_command=="con"){
                    //If the network is initialized.
                    if(network.network_initialized){
                        //If we are the server.
                        if(network.this_instance_server){
                            update_text_log("No need to connect, you are the server!",2);
                        }
                        //If we are a client.
                        else{
                            //If we are not connected to the server.
                            if(network.peer->GetConnectionState(network.SERVER_ID)!=IS_CONNECTED){
                                //Connect to the server.
                                network.peer->Connect(network.SERVER_IP.c_str(),network.SERVER_PORT,0,0);
                                ss.clear();ss.str("");ss<<"Attempting to connect to ";ss<<network.SERVER_IP;ss<<":";ss<<network.SERVER_PORT;ss<<".";msg=ss.str();
                                update_text_log(msg.c_str(),2);
                            }
                            //If we are already connected to the server.
                            else{
                                ss.clear();ss.str("");ss<<"Already connected to ";ss<<network.SERVER_IP;ss<<":";ss<<network.SERVER_PORT;ss<<".";msg=ss.str();
                                update_text_log(msg.c_str(),2);
                            }
                        }
                    }
                    //If the network has not been initialized yet.
                    else{
                        update_text_log("Network not initialized.",2);
                    }
                }

                //
                else if(str_command=="stop"){
                    //If the network is initialized.
                    if(network.network_initialized){
                        //If we are the server.
                        if(network.this_instance_server){
                            //If the server is active.
                            if(network.peer->IsActive()){
                                //Shutdown the server.
                                network.peer->Shutdown(100);
                                update_text_log("Shutting down server.",2);
                            }
                            //If the server is inactive.
                            else{
                                update_text_log("Server is already off.",2);
                            }
                        }
                        //If we are a client.
                        else{
                            update_text_log("Cannot stop the server, you are a client!",2);
                        }
                    }
                    //If the network has not been initialized yet.
                    else{
                        update_text_log("Network not initialized.",2);
                    }
                }

                //
                else if(str_command=="disconnect" || str_command=="dis"){
                    //If the network is initialized.
                    if(network.network_initialized){
                        //If we are the server.
                        if(network.this_instance_server){
                            update_text_log("Cannot disconnect, you are the server!",2);
                        }
                        //If we are a client.
                        else{
                            //If we are connected to the server.
                            if(network.peer->GetConnectionState(network.SERVER_ID)){
                                //Disconnect from the server.
                                network.peer->CloseConnection(network.peer->GetSystemAddressFromGuid(network.SERVER_ID),true);
                            }
                            //If we are not connected to the server.
                            else{
                                update_text_log("Not connected to any server.",2);
                            }
                        }
                    }
                    //If the network has not been initialized yet.
                    else{
                        update_text_log("Network not initialized.",2);
                    }
                }

                else if(str_command=="status" || str_command=="stats" || str_command=="stat"){
                    //Determine the date and time.
                    time_t now;
                    struct tm *tm_now;
                    char buff[BUFSIZ];
                    now=time(NULL);
                    tm_now=localtime(&now);
                    strftime(buff,sizeof buff,"%A, %B %d, %Y - %H:%M:%S",tm_now);

                    update_text_log("------------------------------------Stats---------------------------------------",2);

                    if(network.network_initialized){
                        if(network.this_instance_server){
                            update_text_log("Network mode: Server",2);
                        }
                        else{
                            update_text_log("Network mode: Client",2);
                        }
                    }
                    else{
                        update_text_log("Network mode: Not initialized.",2);
                    }

                    msg="Name: ";
                    msg+=client.name;
                    update_text_log(msg.c_str(),2);

                    if(network.network_initialized){
                        msg="IP address: ";
                        msg+=network.peer->GetSystemAddressFromGuid(network.peer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS)).ToString(true);
                        update_text_log(msg.c_str(),2);

                        if(network.this_instance_server){
                            int number_of_clients=0;
                            //Check all of the clients.
                            for(int i=0;i<client.vector_clients.size();i++){
                                //
                                if(client.vector_clients.at(i).connected==true){
                                    number_of_clients++;
                                }
                            }
                            ss.clear();ss.str("");ss<<"Connected clients: ";ss<<number_of_clients;msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                        else{
                            msg="Server IP address: ";
                            msg+=network.SERVER_IP;
                            msg+=":";
                            ss.clear();ss.str("");ss<<network.SERVER_PORT;msg+=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                    }
                    msg=buff;
                    update_text_log(msg.c_str(),2);
                }

                else if(str_command.rfind("ip",0,2)!=string::npos && str_command.rfind("ip",0,2)==0){
                    if(!network.this_instance_server){
                        str_command.erase(0,2);
                        //If the user typed the ip command without a parameter.
                        if(str_command.size()==0){
                            //Display the current server ip.
                            ss.clear();ss.str("");ss<<"Current server IP: ";ss<<network.SERVER_IP;ss<<".";msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                        //If the user typed the ip command with a parameter.
                        else{
                            str_command.erase(0,1);
                            network.SERVER_IP=str_command;
                            ss.clear();ss.str("");ss<<"Server IP changed to ";ss<<network.SERVER_IP;ss<<".";msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                    }
                    else{
                        update_text_log("The server doesn't need to specify an IP address.",2);
                    }
                }

                else if(str_command.rfind("whois",0,5)!=string::npos && str_command.rfind("whois",0,5)==0){
                    str_command.erase(0,6);
                    if(client.vector_clients.size()>0){
                        bool user_found=false;
                        for(int i=0;i<client.vector_clients.size();i++){
                            if(client.vector_clients.at(i).name==str_command && client.vector_clients.at(i).connected){
                                user_found=true;
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" is ";ss<<client.vector_clients.at(i).address.ToString(true);msg=ss.str();
                                update_text_log(msg.c_str(),2);
                            }
                        }
                        if(!user_found){
                            ss.clear();ss.str("");ss<<"No user by the name ";ss<<str_command;ss<<" is currently connected.";msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                    }
                    else if(client.vector_clients.size()==0){
                        update_text_log("No connection(s).",2);
                    }
                }

                else if(str_command.rfind("whowas",0,6)!=string::npos && str_command.rfind("whowas",0,6)==0){
                    str_command.erase(0,7);
                    if(client.vector_clients.size()>0){
                        bool user_found=false;
                        for(int i=0;i<client.vector_clients.size();i++){
                            if(client.vector_clients.at(i).name==str_command && !client.vector_clients.at(i).connected){
                                user_found=true;
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" was ";ss<<client.vector_clients.at(i).address.ToString(true);msg=ss.str();
                                update_text_log(msg.c_str(),2);
                            }
                            else if(client.vector_clients.at(i).name==str_command && client.vector_clients.at(i).connected){
                                user_found=true;
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" is still connected from ";ss<<client.vector_clients.at(i).address.ToString(true);ss<<".";msg=ss.str();
                                update_text_log(msg.c_str(),2);
                            }
                        }
                        if(!user_found){
                            ss.clear();ss.str("");ss<<"No user by the name ";ss<<str_command;ss<<" was ever connected.";msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                    }
                    else if(client.vector_clients.size()==0){
                        update_text_log("No connection(s).",2);
                    }
                }

                else if(str_command.rfind("name",0,4)!=string::npos && str_command.rfind("name",0,4)==0){
                    str_command.erase(0,4);
                    //If there are no other characters, tell the player what his name is.
                    if(str_command.size()==0){
                        ss.clear();ss.str("");ss<<"You are currently known as ";ss<<client.name;msg=ss.str();
                        update_text_log(msg.c_str(),2);
                    }
                    //If the character after name is a space, proceed to set the name.
                    else if(str_command.rfind(" ",0,1)!=string::npos && str_command.rfind(" ",0,1)==0){
                        str_command.erase(0,1);
                        //If there is only one space, tell the player what his name is.
                        if(str_command.size()==0){
                            ss.clear();ss.str("");ss<<"You are currently known as ";ss<<client.name;msg=ss.str();
                            update_text_log(msg.c_str(),2);
                        }
                        //
                        else if(str_command.size()<=10){
                            //Store current name in temp.
                            string temp_name=client.name;
                            //Set name to new name.
                            client.name=str_command;
                            if(!network.this_instance_server){
                                submit_name(false);
                            }
                            else{
                                client.vector_clients.at(0).name=client.name;
                                send_clients_list();
                            }
                            ss.clear();ss.str("");ss<<temp_name;ss<<" is now known as ";ss<<client.name;msg=ss.str();
                            update_text_log(msg.c_str(),0);
                        }
                        //
                        else{
                            update_text_log("Name too long! 10 characters maximum.",2);
                        }
                    }
                    //If the character after name is not a space.
                    else{
                        update_text_log("Unrecognized command.",2);
                    }
                }

                else if(str_command.rfind("list",0,4)!=string::npos && str_command.rfind("list",0,4)==0){
                    str_command.erase(0,5);
                    if(str_command=="clients" || str_command=="client" || str_command=="cl"){
                        if(client.vector_clients.size()>0){
                            update_text_log(" Clients List:",2);
                            for(int i=0;i<client.vector_clients.size();i++){
                                if(client.vector_clients.at(i).connected){
                                    ss.clear();ss.str("");ss<<"  ";ss<<client.vector_clients.at(i).name;ss<<" - ";ss<<client.vector_clients.at(i).address.ToString(true);msg=ss.str();
                                    update_text_log(msg.c_str(),2);
                                    ss.clear();ss.str("");ss<<"   ID: ";ss<<client.vector_clients.at(i).id.ToString();msg=ss.str();
                                    update_text_log(msg.c_str(),2);
                                }
                            }
                        }
                        else if(client.vector_clients.size()==0){
                            update_text_log("No connection(s).",2);
                        }
                    }
                    else if(str_command=="colors" || str_command=="color" || str_command=="co"){
                        update_text_log(" Available Text Colors:",2);
                        update_text_log("  gray, platinum, silver, cool gray,",2);
                        update_text_log("  white, ghost white, anti-flash white,",2);
                        update_text_log("  brown, auburn, bronze, copper, sand,",2);
                        update_text_log("  yellow, school bus, gold,",2);
                        update_text_log("  orange, pumpkin, amber,",2);
                        update_text_log("  red, fire engine, flame,",2);
                        update_text_log("  pink, rose, shocking pink, ultra pink,",2);
                        update_text_log("  purple, violet, indigo,",2);
                        update_text_log("  blue, sky blue, UN blue,",2);
                        update_text_log("  green, jungle, spring,",2);
                        update_text_log("  special gray, special white, special brown, special yellow,",2);
                        update_text_log("  special orange, special red, special pink, special purple,",2);
                        update_text_log("  special blue, special green, special warm, special cool,",2);
                        update_text_log("  rainbow",2);
                    }
                    else{
                        update_text_log("Invalid list type.",2);
                    }
                }

                else if(str_command=="dev"){
                    client.option_dev=!client.option_dev;
                    if(client.option_dev){
                        update_text_log("Dev mode on.",2);
                    }
                    else{
                        update_text_log("Dev mode off.",2);
                    }
                }

                else if(str_command=="clear" || str_command=="cl"){
                    for(int i=0;i<client.text_limit;i++){
                        client.text_log[i]="";
                    }
                }

                else if(str_command=="help" || str_command=="h"){
                    update_text_log("-------------------------------------Help---------------------------------------",2);
                    update_text_log("If you are a client, type '/ip <IP address of server>' to set the server IP.",2);
                    update_text_log("Then, type '/connect' or '/con' to connect to the server.",2);
                    update_text_log("You can also type '/ip' with no parameter to get the current server IP address.",2);
                    update_text_log("For a full list of commands, type '/commands'.",2);
                }

                else if(str_command=="commands"){
                    update_text_log("-----------------------------------Commands-------------------------------------",2);
                    update_text_log("/client",2);
                    update_text_log("/server",2);
                    update_text_log("/start",2);
                    update_text_log("/stop",2);
                    update_text_log("/connect",2);
                    update_text_log("/disconnect",2);
                    update_text_log("/status",2);
                    update_text_log("/ip",2);
                    update_text_log("/whois",2);
                    update_text_log("/whowas",2);
                    update_text_log("/name",2);
                    update_text_log("/list",2);
                    update_text_log("/clear",2);
                    update_text_log("/help",2);
                    update_text_log("/about",2);
                    update_text_log("/exit",2);
                    update_text_log("/fullscreen",2);
                    update_text_log("/color",2);
                }

                else if(str_command=="about" || str_command=="a"){
                    update_text_log("--------------------------------------------------------------------------------",2);
                    update_text_log("                                    D_O Chat                                    ",2);
                    update_text_log("                            Created by: Kevin Wells",2);
                    update_text_log("                               Version: 1.0 Beta",2);
                    update_text_log("                            Built on: May 24, 2011",2);
                    update_text_log("--------------------------------------------------------------------------------",2);
                }

                else if(str_command=="quit" || str_command=="exit"){
                    quit_game();
                }

                else if(str_command=="toggle fullscreen" || str_command=="togglefullscreen" || str_command=="toggle fs" || str_command=="togglefs" || str_command=="fullscreen" || str_command=="fs"){
                    client.option_fullscreen=!client.option_fullscreen;
                    main_window.toggle_fullscreen();
                    if(client.option_fullscreen){
                        update_text_log("Window switched to fullscreen mode.",2);
                    }
                    else{
                        update_text_log("Window switched to windowed mode.",2);
                    }
                }

                else if(str_command.rfind("color",0,5)!=string::npos && str_command.rfind("color",0,5)==0){
                    str_command.erase(0,6);
                    if(str_command=="red"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_RED){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_RED;
                    }
                    else if(str_command=="green"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_GREEN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_GREEN;
                    }
                    else if(str_command=="white"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_WHITE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_WHITE;
                    }
                    else if(str_command=="yellow"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_YELLOW){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_YELLOW;
                    }
                    else if(str_command=="blue"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_BLUE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_BLUE;
                    }
                    else if(str_command=="gray" || str_command=="grey"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_GRAY){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_GRAY;
                    }
                    else if(str_command=="purple"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_PURPLE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_PURPLE;
                    }
                    else if(str_command=="orange"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_ORANGE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_ORANGE;
                    }
                    else if(str_command=="violet"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_VIOLET){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_VIOLET;
                    }
                    else if(str_command=="indigo"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_INDIGO){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_INDIGO;
                    }
                    else if(str_command=="sky blue" || str_command=="skyblue" || str_command=="sky" || str_command=="baby blue" || str_command=="babyblue" || str_command=="angel blue" || str_command=="angelblue"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SKY_BLUE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SKY_BLUE;
                    }
                    else if(str_command=="un blue" || str_command=="UN blue" || str_command=="unblue" || str_command=="UNblue" || str_command=="robin egg" || str_command=="robinegg" || str_command=="robins egg" || str_command=="robinsegg"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_UN_BLUE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_UN_BLUE;
                    }
                    /**else if(str_command=="school bus" || str_command=="bus" || str_command=="schoolbus"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SCHOOLBUS){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SCHOOLBUS;
                    }*/
                    else if(str_command=="gold"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_GOLD){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_GOLD;
                    }
                    else if(str_command=="pumpkin"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_PUMPKIN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_PUMPKIN;
                    }
                    /**else if(str_command=="amber"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_AMBER){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_AMBER;
                    }*/
                    /**else if(str_command=="fire engine" || str_command=="fireengine" || str_command=="fire truck" || str_command=="firetruck"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_FIREENGINE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_FIREENGINE;
                    }*/
                    else if(str_command=="flame" || str_command=="fire"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_FLAME){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_FLAME;
                    }
                    else if(str_command=="spring" || str_command=="spring green" || str_command=="springgreen"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SPRING){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SPRING;
                    }
                    else if(str_command=="jungle" || str_command=="jungle green" || str_command=="junglegreen"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_JUNGLE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_JUNGLE;
                    }
                    else if(str_command=="pink"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_PINK){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_PINK;
                    }
                    else if(str_command=="rose"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_ROSE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_ROSE;
                    }
                    else if(str_command=="shocking pink" || str_command=="shockingpink" || str_command=="neon pink" || str_command=="neonpink" || str_command=="hot pink" || str_command=="hotpink" || str_command=="kinky pink" || str_command=="kinkypink"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SHOCKING_PINK){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SHOCKING_PINK;
                    }
                    /**else if(str_command=="ultra pink" || str_command=="ultrapink"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_ULTRAPINK){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_ULTRAPINK;
                    }*/
                    /**else if(str_command=="ghost white" || str_command=="ghostwhite" || str_command=="ghost"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_GHOSTWHITE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_GHOSTWHITE;
                    }*/
                    /**else if(str_command=="antiflash white" || str_command=="antiflashwhite" || str_command=="anti-flash white" || str_command=="anti-flashwhite" || str_command=="af white" || str_command=="afwhite" || str_command=="AF white" || str_command=="AFwhite"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_AFWHITE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_AFWHITE;
                    }*/
                    /**else if(str_command=="platinum"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_PLATINUM){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_PLATINUM;
                    }*/
                    else if(str_command=="silver"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SILVER){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SILVER;
                    }
                    /**else if(str_command=="cool gray" || str_command=="coolgray" || str_command=="cool grey" || str_command=="coolgrey"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_COOLGRAY){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_COOLGRAY;
                    }*/
                    else if(str_command=="brown"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_BROWN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_BROWN;
                    }
                    /**else if(str_command=="auburn"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_AUBURN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_AUBURN;
                    }*/
                    else if(str_command=="bronze"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_BRONZE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_BRONZE;
                    }
                    else if(str_command=="copper"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_COPPER){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_COPPER;
                    }
                    else if(str_command=="sand" || str_command=="sand brown" || str_command=="sandbrown" || str_command=="sandy brown" || str_command=="sandybrown"){
                        msg="Text color is ";
                        if(client.option_text_color!=COLOR_SANDY_BROWN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=COLOR_SANDY_BROWN;
                    }
                    /**else if(str_command=="special rainbow" || str_command=="specialrainbow" || str_command=="rainbow" || str_command=="s rainbow" || str_command=="srainbow"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_RAINBOW){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_RAINBOW;
                    }
                    else if(str_command=="special blue" || str_command=="specialblue" || str_command=="s blue" || str_command=="sblue"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_BLUE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_BLUE;
                    }
                    else if(str_command=="special red" || str_command=="specialred" || str_command=="s red" || str_command=="sred"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_RED){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_RED;
                    }
                    else if(str_command=="special yellow" || str_command=="specialyellow" || str_command=="s yellow" || str_command=="syellow"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_YELLOW){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_YELLOW;
                    }
                    else if(str_command=="special gray" || str_command=="specialgray" || str_command=="s gray" || str_command=="sgray" || str_command=="special grey" || str_command=="specialgrey" || str_command=="s grey" || str_command=="sgrey"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_GRAY){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_GRAY;
                    }
                    else if(str_command=="special white" || str_command=="specialwhite" || str_command=="s white" || str_command=="swhite"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_WHITE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_WHITE;
                    }
                    else if(str_command=="special brown" || str_command=="specialbrown" || str_command=="s brown" || str_command=="sbrown"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_BROWN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_BROWN;
                    }
                    else if(str_command=="special orange" || str_command=="specialorange" || str_command=="s orange" || str_command=="sorange"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_ORANGE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_ORANGE;
                    }
                    else if(str_command=="special pink" || str_command=="specialpink" || str_command=="s pink" || str_command=="spink"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_PINK){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_PINK;
                    }
                    else if(str_command=="special purple" || str_command=="specialpurple" || str_command=="s purple" || str_command=="spurple"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_PURPLE){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_PURPLE;
                    }
                    else if(str_command=="special green" || str_command=="specialgreen" || str_command=="s green" || str_command=="sgreen"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_GREEN){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_GREEN;
                    }
                    else if(str_command=="special warm" || str_command=="specialwarm" || str_command=="s warm" || str_command=="swarm" || str_command=="warm"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_WARM){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_WARM;
                    }
                    else if(str_command=="special cool" || str_command=="specialcool" || str_command=="s cool" || str_command=="scool" || str_command=="cool"){
                        msg="Text color is ";
                        if(client.option_text_color!=SPECIAL_COOL){
                            msg+="now ";
                        }
                        else{
                            msg+="already ";
                        }
                        msg+=str_command;
                        msg+=".";
                        update_text_log(msg.c_str(),2);
                        client.option_text_color=SPECIAL_COOL;
                    }*/
                    else{
                        update_text_log("Color not recognized.",2);
                    }
                }

                else{
                    update_text_log("Unrecognized command.",2);
                }

                str1.clear();
                str2.clear();
                str3.clear();
            }

            else{
                if(str1.find_first_not_of(' ')!=string::npos){
                    update_text_log(str1.c_str(),1);
                }
                else{
                    str1.clear();
                    update_text_log(str1.c_str(),1);
                }

                if(str2.length()>0 && str2.find_first_not_of(' ')!=string::npos){
                    update_text_log(str2.c_str(),1);
                }
                else if(str2.length()>0){
                    str2.clear();
                    update_text_log(str2.c_str(),1);
                }

                if(str3.length()>0 && str3.find_first_not_of(' ')!=string::npos){
                    update_text_log(str3.c_str(),1);
                }
                else if(str3.length()>0){
                    str3.clear();
                    update_text_log(str3.c_str(),1);
                }

                str1.clear();
                str2.clear();
                str3.clear();
            }

            //Make sure that enter is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);/**Get keystates.*/
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }
    }
}
