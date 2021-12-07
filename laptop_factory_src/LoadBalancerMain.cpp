//
// Created by Egbantan Babatunde on 11/14/21.
//
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include "LoadBalancerThread.h"

int main(int argc, char *argv[]) {

    MultiPurposeServerSocket socket;
    LoadBalancerWorker balancer;
    std::vector<ServerInfo> primaryServers;
    std::unique_ptr<MultiPurposeServerSocket> new_socket;






    int port = atoi(argv[1]);;
    int number_of_servers = atoi(argv[2]);
    int cache_size = atoi(argv[3]);
    int number_of_replicas = atoi(argv[4]);
    int  algorithm = atoi(argv[5]); //1 is consistent , 0 is rand
    if(number_of_servers <= number_of_replicas)
        return 0;

    LoadBalancerRing ring = LoadBalancerRing(number_of_servers, number_of_replicas);

    int offset = 6;

    balancer.SetCacheSize(cache_size);



    for(int i = 0; i < number_of_servers; i++){

        int peer_unique_id = atoi(argv[offset++]);
        std::string peer_ip = argv[offset++];
        int peer_port = atoi(argv[offset++]);

        primaryServers.emplace_back(peer_unique_id,  peer_port, peer_ip);
    }

    if (!socket.Init(port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }

    balancer.SetAlgorithm(algorithm);
    balancer.InitRing(primaryServers, ring);

    std::vector<std::thread> thread_vector;

    while ((new_socket = socket.Accept())) {
        std::thread worker_thread(&LoadBalancerWorker::BalancerThread,
                                  &balancer, std::move(new_socket));
        thread_vector.push_back(std::move(worker_thread));
    }
    return 0;
}