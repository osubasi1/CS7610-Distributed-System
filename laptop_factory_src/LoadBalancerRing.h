//
// Created by Egbantan Babatunde on 11/28/21.
//

#ifndef DISTRUBUTEDFINALCODE_LOADBALANCERRING_H
#define DISTRUBUTEDFINALCODE_LOADBALANCERRING_H
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <map>
#include <random>
#include <algorithm>

class LoadBalancerRing {


private:
    std::map<int, std::pair<int, int>> server_nodes;
    std::vector< std::size_t> partition2node;

    size_t REPLICAS;
    size_t PARTITION_POWER = 16;
    size_t PARTITION_MAX = pow(PARTITION_POWER - 1,2);
    size_t NODE_COUNT;
    int ZONE_COUNT = 16;
    int data_count = -1;
    void createNodes();
    int dead_servers;


public:
    LoadBalancerRing(){};
    LoadBalancerRing(size_t node_count, int number_of_replicas);
    std::vector<int> GetNodes(int customer_id);

    int GetNumberofReplicas();

    int GetDataCount();

    size_t GetServersCount();

    void InvalidateNode(int i);

    size_t GetAllServersCount();
};


#endif //DISTRUBUTEDFINALCODE_LOADBALANCERRING_H
