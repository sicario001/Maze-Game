#pragma once

#include <iostream>
#include <string.h>
#include <enet/enet.h>
#include <vector>

class ClientNet{
    private:
        pthread_mutex_t mutex;

    public:
        ENetHost* client = NULL;
        ENetAddress address;
        ENetEvent event;
        ENetPeer* peer = NULL;
        bool connected;
        bool isConnected();
        void setConnected();
        void setNotConnected();
        ClientNet();
        int Init();
        int Connect(const char * host_name, int port_num);
        int Disconnect();
        void SendPacket(ENetPeer* peer, const char* data);
        std::vector<int> Parsedata(char* data);
        void SendDataPosVelDeg(ENetPeer* peer, int x, int y, int velX, int velY, int deg);
        void SendDataThrowablePosVel(ENetPeer* peer, int x, int y, int speedX, int speedY, double angle, int type);
        void SendHit(ENetPeer* peer, int damage);
        void SendBombState(int state);
        void SendBombLocation(std::pair<int, int> location);
        void SendTileMapInitialized();
        void SendLoadingComplete();

};
void* ReceiveLoop(void* param);