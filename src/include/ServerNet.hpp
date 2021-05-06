#pragma once

#include <iostream>
#include <string.h>
#include <enet/enet.h>
#include <vector>

class ServerNet{
    private:
        
        pthread_mutex_t mutex;
        pthread_mutex_t mutex1;
        pthread_cond_t connectionSignal;

    public:
        ENetEvent event;
        ENetAddress address;
        ENetHost* server = NULL;
        ENetPeer* peer = NULL;
        bool running = false;
        bool connected = false;
        ServerNet();
        int Init();
        int Destroy();
        void SendPacket(ENetPeer* peer, const char* data);
        std::vector<int> Parsedata(int id, char* data);
        void SendDataPosVelDeg(ENetPeer* peer, int x, int y, int velX, int velY, int deg);
        void SendDataThrowablePosVel(ENetPeer* peer, int x, int y, int velX, int velY, double angle, int type);
        void SendMap(ENetPeer* peer, std::vector<std::vector<bool>> &map);
        void SendHit(ENetPeer* peer, int damage);
        void SendBombState(int state);
        void SendBombLocation(std::pair<int, int> location);


        bool isRunning();
        bool isConnected();
        void setRunning();
        void setConnected();
        void setNotConnected();
        void setNotRunning();
        void waitForConnection();
        

};
void* RunLoop(void* param);