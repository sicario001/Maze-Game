#pragma once

#include <iostream>
#include <string.h>
#include <enet/enet.h>
#include <vector>

class ServerNet{
    private:
        


    public:
        ENetEvent event;
        ENetAddress address;
        ENetHost* server = NULL;
        ENetPeer* peer = NULL;
        bool running;
        bool connected;
        ServerNet();
        int Init();
        int Destroy();
        void SendPacket(ENetPeer* peer, const char* data);
        std::vector<int> Parsedata(int id, char* data);
        void SendDataPosVel(ENetPeer* peer, int x, int y, int velX, int velY);
        

};
void* RunLoop(void* param);