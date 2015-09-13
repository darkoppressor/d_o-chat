/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef client_data_h
#define client_data_h

#include "Raknet/Source/RakPeerInterface.h"
#include "Raknet/Source/BitStream.h"

#include <string>

//All information about clients is held in a vector of client_data.
//This includes the server itself, as well.
struct client_data{
    RakNet::RakNetGUID id;
    RakNet::SystemAddress address;
    std::string name;
    bool connected;
};

#endif
