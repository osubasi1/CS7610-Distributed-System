#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <string>

#include "MultiPurposeServerSocket.h"
#include "ServerThread.h"

int main(int argc, char *argv[]) {
	int port;
	int engineer_cnt = 0;

	int factory_id;
    int number_of_peer;

	MultiPurposeServerSocket socket;
	LaptopFactory factory;
	std::unique_ptr<MultiPurposeServerSocket> new_socket;

	std::vector<std::thread> thread_vector;
	
	if (argc < 3) {
		std::cout << "not enough arguments" << std::endl;
		std::cout << argv[0] << "[port #] [# experts]" << std::endl;
		return 0;
	}
	port = atoi(argv[1]);
	factory_id = atoi(argv[2]);
    factory.SetFactoryId(factory_id);

    number_of_peer = atoi(argv[3]);
    int offset = 4;
    //std::cout<<port<<factory_id<<number_of_peer<<std::endl;

    std::vector<ServerInfo> serverPeer;
    for(int i = 0; i < number_of_peer; i++){
        int peer_unique_id = atoi(argv[offset++]);
        std::string peer_ip = argv[offset++];
        int peer_port = atoi(argv[offset++]);

        serverPeer.emplace_back(peer_unique_id,  peer_port, peer_ip);
    }

    factory.SetPeerInfo(serverPeer);

    std::thread expert_thread(&LaptopFactory::ProductionAdminThread,
				&factory, engineer_cnt++);
    thread_vector.push_back(std::move(expert_thread));


	if (!socket.Init(port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	while ((new_socket = socket.Accept())) {

		std::thread engineer_thread(&LaptopFactory::EngineerThread,
				&factory, std::move(new_socket), 
				engineer_cnt++);
		thread_vector.push_back(std::move(engineer_thread));
	}
	return 0;
}
