#include "ClientNet.hpp"

ClientNet::ClientNet(){
    pthread_mutex_init( &mutex, NULL);
    setNotConnected();
    ClientNet::Init();
}
int ClientNet::Init(){
    if(enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet!\n");
        return 0;
    }
    atexit(enet_deinitialize);

    client = enet_host_create(NULL, 1, 1, 0, 0);

    if(client == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
        return 0;
    }
    return 1;
}

int ClientNet::Connect(const char * host_name, int port_num){
    enet_address_set_host(&address, host_name);
    address.port = port_num;

    peer = enet_host_connect(client, &address, 1, 0);
    if(peer == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection!\n");
        return 0;
    }

    if(enet_host_service(client, &event, 5000) > 0 &&
	    event.type == ENET_EVENT_TYPE_CONNECT)
    {
        // enet_host_flush(client); 
        std::cout<<"Connection to "<<host_name<<":"<<port_num<<" succeeded.\n";
        
        setConnected();
        return 1;
    }
    else
    {
        enet_peer_reset(peer);
        std::cout<<"Connection to "<<host_name<<":"<<port_num<<" failed.\n";
        return 0;
    }
}

int ClientNet::Disconnect(){
    setNotConnected();
    enet_peer_disconnect(peer, 0);
    while(enet_host_service(client, &event, 100)>0){
        switch(event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("Disconnection succeeded.");
                break;
            default:
                break;
        }
    }  
    return 1;
}
void ClientNet::SendDataPosVel(ENetPeer* peer, int x, int y, int velX, int velY){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "0|%d|%d|%d|%d", x, y, velX, velY);
    SendPacket(peer, send_data);
}

void ClientNet::SendPacket(ENetPeer* peer, const char* data)
{
    // Create the packet using enet_packet_create and the data we want to send
    // We are using the flag ENET_PACKET_FLAG_RELIABLE that acts a bit like TCP.
    // That is to say, it'll make sure the packet makes it to the destination.
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);

    // Finally, send the packet to the peer on channel 0!
    enet_peer_send(peer, 0, packet);
}

std::vector<int> ClientNet::Parsedata(char* data){
    // std::cout<<data<<"\n";
    int data_type;
    sscanf(data, "%d|", &data_type);
    switch(data_type){
        case 0:
        {
            int x, y, velX, velY;
            sscanf(data, "0|%d|%d|%d|%d", &x, &y, &velX, &velY);
            return{0, x, y, velX, velY};
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
        default:
            return {};
    }
}

bool ClientNet::isConnected(){
    pthread_mutex_lock(&mutex);
    bool conn_val = connected;
    pthread_mutex_unlock(&mutex);
    return conn_val;
}


void ClientNet::setConnected(){
    pthread_mutex_lock(&mutex);
    connected = true;
    pthread_mutex_unlock(&mutex);
}
void ClientNet::setNotConnected(){
    pthread_mutex_lock(&mutex);
    connected = false;
    pthread_mutex_unlock(&mutex);
}
