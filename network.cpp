/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "network.h"
#include "world.h"
#include "client_data.h"

using namespace std;
using namespace RakNet;

Network::Network(){
    MAX_CLIENTS=32;
    SERVER_IP="192.168.1.102";
    SERVER_PORT=60000;
    CLIENT_PORT=0;
    SERVER_ID=UNASSIGNED_RAKNET_GUID;
    this_instance_server=false;
    network_initialized=false;

    //Get a RakPeerInterface and attach it to peer.
    peer=RakPeerInterface::GetInstance();
}

Network::~Network(){
    //
    peer->Shutdown(100,0);

    //
    RakPeerInterface::DestroyInstance(peer);

    //
    peer=NULL;
}

void Network::initialize(bool check_server){
    //The network has now been initialized.
    network_initialized=true;

    //If we are starting server mode.
    if(check_server){
        update_text_log("Starting in server mode.",2);

        //Get this instance's IP address and store it as the SERVER_IP.
        SERVER_IP=peer->GetLocalIP(0);

        //Start the server.
        peer->Startup(MAX_CLIENTS,&SocketDescriptor(SERVER_PORT,0),1);

        //Set the maxiximum number of allowed incoming connections.
        peer->SetMaximumIncomingConnections(MAX_CLIENTS);

        //
        peer->SetOccasionalPing(true);

        //
        peer->SetUnreliableTimeout(1000);

        //This instance is the server.
        this_instance_server=true;

        //Create a new client_data, store this instance's information in it, and add it to the clients vector.
        //This vector is only used by the server.
        //The first element in this array will always be the server's information.
        client_data temp_data;
        temp_data.id=peer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
        temp_data.address=peer->GetSystemAddressFromGuid(peer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS));
        temp_data.name=client.name;
        temp_data.connected=true;
        client.vector_clients.push_back(temp_data);
    }

    //If we are starting client mode.
    else{
        update_text_log("Starting in client mode.",2);

        //Start the client.
        peer->Startup(1,&SocketDescriptor(),1);

        ///Connect to the server. This will not automatically happen forever.
        ///Eventually I will remove this, and only the connect command will connect.
        ///peer->Connect(SERVER_IP.c_str(),SERVER_PORT,0,0);
    }
}

void Network::receive(){
    if(network_initialized){
        client_data temp_data;
        for(packet=peer->Receive();packet;peer->DeallocatePacket(packet),packet=peer->Receive()){
            switch(packet->data[0]){
                /**case ID_REMOTE_NEW_INCOMING_CONNECTION:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //If the client's id matches the id of this packet, then this client is already in the vector.
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Let the server know that this client has reconnected.
                                client.vector_clients.at(i).connected=true;
                                msg=client.vector_clients.at(i).name;
                                msg+=" has reconnected.";
                                update_text_log(msg.c_str(),0);
                                break;
                            }
                            //If this is the last iteration, then the client is a new one.
                            if(i==client.vector_clients.size()-1){
                                temp_data.id=packet->guid;
                                temp_data.address=packet->systemAddress;
                                temp_data.name="";
                                temp_data.connected=true;
                                client.vector_clients.push_back(temp_data);
                            }
                        }
                    }
                    break;*/
                case ID_NEW_INCOMING_CONNECTION:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //If the client's id matches the id of this packet, then this client is already in the vector.
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Let the server know that this client has reconnected.
                                client.vector_clients.at(i).connected=true;
                                ///msg=client.vector_clients.at(i).name;
                                ///msg+=" has reconnected.";
                                ///update_text_log(msg.c_str(),0);
                                break;
                            }
                            //If this is the last iteration, then the client is a new one.
                            if(i==client.vector_clients.size()-1){
                                temp_data.id=packet->guid;
                                temp_data.address=packet->systemAddress;
                                temp_data.name="";
                                temp_data.connected=true;
                                client.vector_clients.push_back(temp_data);
                            }
                        }
                    }
                    break;
                case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //When the client's id matches the id of this packet, we have our man!
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Announce to everyone that this client has disconnected.
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" has disconnected.";msg=ss.str();
                                update_text_log(msg.c_str(),0);
                                //Let the server know that this client is no longer connected.
                                client.vector_clients.at(i).connected=false;
                            }
                        }
                        send_clients_list();
                    }
                    break;
                case ID_DISCONNECTION_NOTIFICATION:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //When the client's id matches the id of this packet, we have our man!
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Announce to everyone that this client has disconnected.
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" has disconnected.";msg=ss.str();
                                update_text_log(msg.c_str(),0);
                                //Let the server know that this client is no longer connected.
                                client.vector_clients.at(i).connected=false;
                            }
                        }
                        send_clients_list();
                    }
                    else{
                        update_text_log("Disconnected from server.",2);
                    }
                    break;
                case ID_REMOTE_CONNECTION_LOST:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //When the client's id matches the id of this packet, we have our man!
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Announce to everyone that this client has been dropped.
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" has dropped.";msg=ss.str();
                                update_text_log(msg.c_str(),0);
                                //Let the server know that this client is no longer connected.
                                client.vector_clients.at(i).connected=false;
                            }
                        }
                        send_clients_list();
                    }
                    break;
                case ID_CONNECTION_LOST:
                    if(this_instance_server){
                        //Check all of the clients.
                        for(int i=0;i<client.vector_clients.size();i++){
                            //When the client's id matches the id of this packet, we have our man!
                            if(client.vector_clients.at(i).id==packet->guid){
                                //Announce to everyone that this client has been dropped.
                                ss.clear();ss.str("");ss<<client.vector_clients.at(i).name;ss<<" has dropped.";msg=ss.str();
                                update_text_log(msg.c_str(),0);
                                //Let the server know that this client is no longer connected.
                                client.vector_clients.at(i).connected=false;
                            }
                        }
                        send_clients_list();
                    }
                    else{
                        update_text_log("Lost connection to server.",2);
                    }
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    SERVER_ID=packet->guid;
                    submit_name(true);
                    ss.clear();ss.str("");ss<<"Success! Connected to server at ";ss<<packet->systemAddress.ToString(true);ss<<".";msg=ss.str();
                    update_text_log(msg.c_str(),2);
                    break;
                case ID_CONNECTION_ATTEMPT_FAILED:
                    update_text_log("Unable to connect to server.",2);
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    update_text_log("The server is full.",2);
                    break;
                case ID_ALREADY_CONNECTED:
                    update_text_log("Already connected to that server.",2);
                    break;
                ///I guess this has been removed in RakNet 4.0?
                /**case ID_MODIFIED_PACKET:
                    ss.clear();ss.str("");ss<<"A packet from ";ss<<packet->systemAddress.ToString(true);ss<<" has been tampered with.";msg=ss.str();
                    update_text_log(msg.c_str(),0);
                    update_text_log("Cheater alert!",0);
                    break;*/
                case ID_CLIENT_LIST:
                    client.receive_client_list(packet);
                    break;
                case ID_SUBMIT_NAME:
                    client.receive_name(packet);
                    break;
                case ID_CHAT_MESSAGE:
                    if(network.this_instance_server){
                        client.server_receive_chat_message(packet);
                    }
                    else{
                        client.receive_chat_message(packet);
                    }
                    break;
                default:
                    ss.clear();ss.str("");ss<<"Message with identifier ";ss<<packet->data[0];ss<<" has arrived.";msg=ss.str();
                    update_text_log(msg.c_str(),2);
                    update_text_log("This should not happen. Please notify Kevin.",2);
                    break;
            }
        }
    }
}

void update_text_log(const char *message,unsigned char message_type){
    //Format the message according to its type.

    //System message or local message.
    if(message_type==0 || message_type==2){
        string temp_message=">: ";
        temp_message+=message;
        message=temp_message.c_str();
    }
    //Chat message.
    if(message_type==1){
        string temp_message=client.name+": ";
        temp_message+=message;
        message=temp_message.c_str();
    }

    //Print the message on this instance's screen.
    for(int i=0;i<client.text_limit;i++){
        if(i==client.text_limit-1){
            client.text_log[i]=message;
        }
        else{
            client.text_log[i]=client.text_log[i+1];
        }
    }

    //System message or chat message.
    if(message_type==0 || message_type==1){
        RakNet::BitStream bitstream;
        RakNet::RakString rstring;

        bitstream.Write((MessageID)ID_CHAT_MESSAGE);
        bitstream.Write((MessageID)ID_TIMESTAMP);
        bitstream.Write(RakNet::GetTime());
        rstring=(message);
        bitstream.Write(rstring);

        //If a client.
        if(!network.this_instance_server){
            //Send the message to the server only.
            network.peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,network.peer->GetSystemAddressFromGuid(network.SERVER_ID),false);
        }
        //If the server.
        else{
            //Send the message to everyone.
            network.peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,UNASSIGNED_SYSTEM_ADDRESS,true);
        }
    }
}

void submit_name(bool connection){
    //A client calls submit_name() locally when it successfully connects to the server.
    //If submit_name() was called by a client.
    if(!network.this_instance_server){
        RakNet::BitStream bitstream;
        RakNet::RakString rstring;

        bitstream.Write((MessageID)ID_SUBMIT_NAME);
        bitstream.Write((MessageID)ID_TIMESTAMP);
        bitstream.Write(RakNet::GetTime());
        rstring=(client.name.c_str());
        bitstream.Write(rstring);
        bitstream.Write(connection);
        bitstream.Write(network.peer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS));

        network.peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,network.peer->GetSystemAddressFromGuid(network.SERVER_ID),false);
    }
}

void send_clients_list(){
    if(network.this_instance_server){
        //Send the up-to-date clients list to everyone else.
        int send_size=client.vector_clients.size();

        client_data temp_data;
        RakNet::RakString rstring;

        RakNet::BitStream bitstream;

        bitstream.Write((MessageID)ID_CLIENT_LIST);
        bitstream.Write((MessageID)ID_TIMESTAMP);
        bitstream.Write(RakNet::GetTime());
        bitstream.Write(send_size);

        for(int i=0;i<send_size;i++){
            bitstream.Write(client.vector_clients.at(i).id);
            bitstream.Write(client.vector_clients.at(i).address);
            rstring=(client.vector_clients.at(i).name.c_str());
            bitstream.Write(rstring);
            bitstream.Write(client.vector_clients.at(i).connected);
        }

        network.peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,0,UNASSIGNED_SYSTEM_ADDRESS,true);
    }
}
