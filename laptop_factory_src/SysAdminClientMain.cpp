#include <vector>
#include <iostream>

#include "SysAdminClientStub.h"

//
// Created by Egbantan Babatunde on 12/3/21.
//
int main(int argc, char *argv[]) {

    std::string ip;
    int port;
    int num_server;

    ip = argv[1];
    port = atoi(argv[2]);
    num_server = atoi(argv[3]);

    std::vector<ServerInfo> serverPeer;

    int offset = 4;

    for(int i = 0; i < num_server; i++){
        int peer_unique_id = atoi(argv[offset++]);
        std::string peer_ip = argv[offset++];
        int peer_port = atoi(argv[offset++]);

        serverPeer.emplace_back(peer_unique_id,  peer_port, peer_ip);
    }

    SysAdminClientStub stub;

    if (!stub.Init(ip, port)) {
        std::cout << "Server " << ip << " failed to connect" << std::endl;
        return 0;
    }

    AdminRequest request;

    for(auto v: serverPeer){

        request.SetRequest(1, v);
        stub.Send(request);

    }

    stub.CloseStream();

    return 1;
}
