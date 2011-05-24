/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef network_h
#define network_h

#include "client_data.h"

#include "RakNet/Source/RakPeerInterface.h"
#include "RakNet/Source/MessageIdentifiers.h"
#include "RakNet/Source/BitStream.h"
#include "RakNet/Source/GetTime.h"
#include "RakNet/Source/RakString.h"

#include <vector>

class Network{
    private:

    public:

    //
    Network();

    //
    ~Network();

    //Called when the player chooses to be either the server or a client.
    //check_server should be true if server, false if client.
    void initialize(bool check_server);

    //Check for received packets and handle them accordingly.
    void receive();

    //The maximum number of clients.
    //Used by the server only.
    //Note: The server does not count in this number, even though it also acts as a client.
    //Therefore, a value of 10 means that 11 total people can be chatting/playing together.
    unsigned short MAX_CLIENTS;

    //The IP address for the server.
    //Used by clients to connect.
    std::string SERVER_IP;

    //The port that the server runs on.
    //Hardcoded.
    unsigned short SERVER_PORT;

    //The port for the client to use.
    //Setting it to 0 allows the client to use some available port.
    unsigned short CLIENT_PORT;

    //The id of the server, used by clients.
    //When a client successfully connects, SERVER_ID is changed to the id of the server.
    RakNet::RakNetGUID SERVER_ID;

    //True if this instance of the game is the server.
    bool this_instance_server;

    //False until initialize() has been run.
    bool network_initialized;

    //Used as our interface with RakPeer.
    RakNet::RakPeerInterface *peer;

    //Used to collect and handle received packets.
    RakNet::Packet *packet;
};

//message_type:
//0 = System message.
//1 = Chat message.
//2 = Local message.
extern void update_text_log(const char *message,unsigned char message_type);

//When a client connects to the server, they submit their name using submit_name().
//name is the name of the client.
//raknet_guid is the id of the client.
//Since the client has already connected, the server has stored the client's other information in the client_data vector.
//submit_name(), called on the server, then finds the corresponding client in the vector and sets the proper name.
extern void submit_name(bool connection);

//
extern void send_clients_list();

//Custom packet identifiers.
enum{
    ID_D_O_CHAT_MSG_1=ID_USER_PACKET_ENUM,
    ID_CLIENT_LIST,
	ID_SUBMIT_NAME,
	ID_CHAT_MESSAGE,
};

#endif
