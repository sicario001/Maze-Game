#include "engine/net/ServerNet.hpp"


ServerNet::ServerNet(){
    pthread_mutex_init( &mutex, NULL);
    pthread_mutex_init( &mutex1, NULL);
    pthread_cond_init( &connectionSignal, NULL);
    
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

void ServerNet::SendMap(ENetPeer* peer, std::vector<std::vector<bool>> &map){
    char send_data[1024] = {'\0'};
    int total_len = map.size()*map[0].size();
    sprintf(send_data, "1|%d|", total_len);
    for (auto x: map){
        for (bool y:x){
            if (y){
                strcat(send_data, "1");
            }
            else{
                strcat(send_data, "0");
            }
        }
    }
    SendPacket(peer, send_data);
}

int ServerNet::Destroy(){
    enet_host_destroy(server);
    running = false;
    return 1;
}


bool ServerNet::isRunning(){
    pthread_mutex_lock(&mutex1);
    bool run_val = running;
    pthread_mutex_unlock(&mutex1);
    return run_val;
}


void ServerNet::setRunning(){
    pthread_mutex_lock(&mutex1);
    running = true;
    pthread_mutex_unlock(&mutex1);
}
void ServerNet::setNotRunning(){
    pthread_mutex_lock(&mutex1);
    running = false;
    pthread_mutex_unlock(&mutex1);
}

void ServerNet::waitForConnection(){
    pthread_mutex_lock(&mutex);
    while (!connected){
        pthread_cond_wait(&connectionSignal, &mutex);
    }
    pthread_mutex_unlock(&mutex);

}

void ServerNet::SendRoundEndSignal(PlayerObj winner){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "6|%d", winner);
    SendPacket(peer, send_data);
}

void ServerNet::sendDisconnectRequest(){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "8");
    SendPacket(peer, send_data);
}

void ServerNet::sendPlayerDead(int type){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "9|%d", type);
    SendPacket(peer, send_data);
}