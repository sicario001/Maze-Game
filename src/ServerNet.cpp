#include "ServerNet.hpp"


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

void ServerNet::SendDataPosVelDeg(ENetPeer* peer, int x, int y, int velX, int velY, int deg){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "0|%d|%d|%d|%d|%d", x, y, velX, velY,deg);
    SendPacket(peer, send_data);
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

void ServerNet::SendDataThrowablePosVel(ENetPeer* peer, int x, int y, int velX, int velY, double angle, int type){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "2|%d|%d|%d|%d|%d|%f", x, y, velX, velY, type, angle);
    SendPacket(peer, send_data);
}

void ServerNet::SendHit(ENetPeer* peer, int damage){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "3|%d", damage);
    SendPacket(peer, send_data);
}

void ServerNet::SendBombState(int state){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "4|%d", state);
    SendPacket(peer, send_data);
}
void ServerNet::SendBombLocation(std::pair <int, int> location){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "5|%d|%d", location.first, location.second);
    SendPacket(peer, send_data);
}
void ServerNet::SendRoundEndSignal(PlayerObj winner){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "6|%d", winner);
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
    int data_type;
    sscanf(data, "%d|", &data_type);
    switch(data_type){
        case 0:
        {
            int x, y, velX, velY, deg;
            sscanf(data, "0|%d|%d|%d|%d|%d", &x, &y, &velX, &velY, &deg);
            return{0, x, y, velX, velY, deg};
        }            
        case 1:
        {
            char map_arr[1024];
            int total_len;
            sscanf(data, "1|%d|%s", &total_len, map_arr);
            std::vector<int> ret_vec(total_len+1);
            ret_vec[0] = 1;
            for (int i=1; i<total_len+1; i++){
                ret_vec[i] = (map_arr[i-1]=='1' ? 1:0);
            }
            return ret_vec;
        }
        case 2:
        {
            int x, y, velX, velY, type;
            double angle;
            sscanf(data, "2|%d|%d|%d|%d|%d|%lf", &x, &y, &velX, &velY, &type, &angle);
            return{2, x, y, velX, velY, type, (int)(angle*1e8)};
        }
        case 3:
        {
            int damage;
            sscanf(data, "3|%d", &damage);
            return{3, damage};
        }
        case 4:
        {
            int state;
            sscanf(data, "4|%d", &state);
            return {4, state};
        }
        case 5:
        {
            int x, y;
            sscanf(data, "5|%d|%d", &x, &y);
            return {5, x, y};   
        }
        case 6:
        {
            return {6};
        }
        case 7:
        {
            return {7};
        }
        default:
            return {};
    }
}

int ServerNet::Destroy(){
    enet_host_destroy(server);
    running = false;
    return 1;
}

bool ServerNet::isConnected(){
    pthread_mutex_lock(&mutex);
    bool conn_val = connected;
    pthread_mutex_unlock(&mutex);
    return conn_val;
}


void ServerNet::setConnected(){
    pthread_mutex_lock(&mutex);
    connected = true;
    pthread_cond_signal(&connectionSignal);
    pthread_mutex_unlock(&mutex);
}
void ServerNet::setNotConnected(){
    pthread_mutex_lock(&mutex);
    connected = false;
    pthread_mutex_unlock(&mutex);
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