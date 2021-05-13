#pragma once

#include <string.h>
#include <vector>
#include <iostream>

#include <enet/enet.h>

#include "fwd.hpp"
#include "CommonNet.hpp"

class ServerNet:public CommonNet{
    private:
        
        pthread_mutex_t mutex;
        pthread_mutex_t mutex1;
        pthread_cond_t connectionSignal;

    public:
        ENetHost* server = NULL;
        bool running = false;
        ServerNet();
        int Init();
        int Destroy();

        bool isRunning();
        void setRunning();
        void setNotRunning();
        void waitForConnection();
        void sendDisconnectRequest();
        void SendRoundEndSignal(PlayerObj winner);
        void SendMap(ENetPeer* peer, std::vector<std::vector<bool>> &map);
};