#pragma once

#include <iostream>
#include <string.h>
#include <enet/enet.h>
#include <vector>

class ClientNet{
    private:
        


    public:
        ENetHost* client = NULL;
        ENetAddress address;
        ENetEvent event;
        ENetPeer* peer = NULL;
        bool connected;
        ClientNet();
        int Init();
        int Connect(const char * host_name, int port_num);
        int Disconnect();
        void SendPacket(ENetPeer* peer, const char* data);
        std::vector<int> Parsedata(char* data);
        void SendDataPosVel(ENetPeer* peer, int x, int y, int velX, int velY);

};
void* ReceiveLoop(void* param);