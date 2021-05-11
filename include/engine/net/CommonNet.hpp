#pragma once

#include <string.h>
#include <vector>
#include <iostream>

#include <enet/enet.h>

class CommonNet{
    private:
        pthread_mutex_t mutex;

    public:
        ENetAddress address;
        ENetEvent event;
        ENetPeer* peer = NULL;
        bool connected;
        bool isConnected();
        void setConnected();
        void setNotConnected();
        void SendPacket(ENetPeer* peer, const char* data);
        std::vector<int> Parsedata(char* data);
        void SendDataPosVelDeg(ENetPeer* peer, int x, int y, int velX, int velY, int deg);
        void SendDataThrowablePosVel(ENetPeer* peer, int x, int y, int speedX, int speedY, double angle, int type);
        void SendHit(ENetPeer* peer, int damage);
        void SendBombState(int state);
        void SendBombLocation(std::pair<int, int> location);
};
void* ReceiveLoop(void* param);