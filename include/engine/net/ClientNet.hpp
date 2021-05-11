#pragma once

#include <string.h>
#include <vector>
#include <iostream>

#include <enet/enet.h>
#include "CommonNet.hpp"

class ClientNet:public CommonNet{
    private:
        pthread_mutex_t mutex;

    public:
        ENetHost* client = NULL;
        ClientNet();
        int Init();
        int Connect(const char * host_name, int port_num);
        int Disconnect();
        void SendTileMapInitialized();
        void SendLoadingComplete();

};