#include "engine/net/ClientNet.hpp"

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


void ClientNet::SendTileMapInitialized(){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "6");
    SendPacket(peer, send_data);
}
void ClientNet::SendLoadingComplete(){
    char send_data[1024] = {'\0'};
    sprintf(send_data, "7");
    SendPacket(peer, send_data);
}
