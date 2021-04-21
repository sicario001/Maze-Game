#include "ServerNet.hpp"


ServerNet::ServerNet(){
    running = false;
    connected = false;
    ServerNet::Init();
}
int ServerNet::Init(){
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return 0;
    }
    atexit (enet_deinitialize);

    address.host = ENET_HOST_ANY; // This allows
    /* Bind the server to port 7777. */
    address.port = 7777;



    server = enet_host_create (&address	/* the address to bind the server host to */,
                    32	/* allow up to 32 clients and/or outgoing connections */,
                    1	/* allow up to 1 channel to be used, 0. */,
                    0	/* assume any amount of incoming bandwidth */,
                    0	/* assume any amount of outgoing bandwidth */);

    if (server == NULL)
    {
        printf("An error occurred while trying to create an ENet server host.");
        return 0;
    }
    running = true;
    return 1;
}

void ServerNet::SendDataPosVel(ENetPeer* peer, int x, int y, int velX, int velY){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "%d|%d|%d|%d", x, y, velX, velY);
    SendPacket(peer, send_data);
}

void ServerNet::SendPacket(ENetPeer* peer, const char* data)
{
    // Create the packet using enet_packet_create and the data we want to send
    // We are using the flag ENET_PACKET_FLAG_RELIABLE that acts a bit like TCP.
    // That is to say, it'll make sure the packet makes it to the destination.
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);

    // Finally, send the packet to the peer on channel 0!
    enet_peer_send(peer, 0, packet);
}

std::vector<int> ServerNet::Parsedata(int id, char* data){
    //std::cout<<"PARSE: "<<data<<"\n";

    int x, y, velX, velY;
    sscanf(data, "%d|%d|%d|%d", &x, &y, &velX, &velY);
    return {x, y, velX, velY};
}

int ServerNet::Destroy(){
    enet_host_destroy(server);
    running = false;
    return 1;
}

