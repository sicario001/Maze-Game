#include "engine/net/CommonNet.hpp"

void CommonNet::SendDataPosVelDeg(ENetPeer* peer, int x, int y, int velX, int velY, int deg){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "0|%d|%d|%d|%d|%d", x, y, velX, velY, deg);
    SendPacket(peer, send_data);
}

void CommonNet::SendDataThrowablePosVel(ENetPeer* peer, int x, int y, int velX, int velY, double angle, int type){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "2|%d|%d|%d|%d|%d|%f", x, y, velX, velY, type, angle);
    SendPacket(peer, send_data);
}

void CommonNet::SendHit(ENetPeer* peer, int damage, int currentRoundNum){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "3|%d|%d", damage, currentRoundNum);
    SendPacket(peer, send_data);
}
void CommonNet::SendBombState(int state){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "4|%d", state);
    SendPacket(peer, send_data);
}
void CommonNet::SendBombLocation(std::pair <int, int> location){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "5|%d|%d", location.first, location.second);
    SendPacket(peer, send_data);
}


void CommonNet::SendPacket(ENetPeer* peer, const char* data)
{
    // Create the packet using enet_packet_create and the data we want to send
    // We are using the flag ENET_PACKET_FLAG_RELIABLE that acts a bit like TCP.
    // That is to say, it'll make sure the packet makes it to the destination.
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);

    // Finally, send the packet to the peer on channel 0!
    enet_peer_send(peer, 0, packet);
}

std::vector<int> CommonNet::Parsedata(char* data){
    // std::cout<<data<<"\n";
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
            int damage, currRoundNum;
            sscanf(data, "3|%d|%d", &damage, &currRoundNum);
            return{3, damage, currRoundNum};
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
            int x;
            sscanf(data, "6|%d", &x);
            return {6, x};
        }
        case 7:
        {
            return {7};
        }
        case 8:
        {
            return {8};
        }
        case 9:
        {
            int x;
            sscanf(data, "9|%d", &x);
            return {9, x};
        }
        default:
            return {};
    }
}

bool CommonNet::isConnected(){
    pthread_mutex_lock(&mutex);
    bool conn_val = connected;
    pthread_mutex_unlock(&mutex);
    return conn_val;
}


void CommonNet::setConnected(){
    pthread_mutex_lock(&mutex);
    connected = true;
    pthread_mutex_unlock(&mutex);
}
void CommonNet::setNotConnected(){
    pthread_mutex_lock(&mutex);
    connected = false;
    pthread_mutex_unlock(&mutex);
}
