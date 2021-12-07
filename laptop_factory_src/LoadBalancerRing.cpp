//
// Created by Egbantan Babatunde on 11/28/21.
//

#include "LoadBalancerRing.h"


std::vector<int> LoadBalancerRing::GetNodes(int customer_id) {

    data_count = std::max(customer_id, data_count);


    std::string customer_id_str = std::to_string(customer_id);
    std::size_t partition =  (   (std::size_t)(std::hash<std::string>{}(customer_id_str)) % PARTITION_MAX) ;

    std::vector<std::size_t> node_ids = {partition2node[partition]};
    std::vector<int> zones = {server_nodes[node_ids[0]].second};
    for(int i = 0; i < REPLICAS; i++){

        while(find(node_ids.begin(), node_ids.end(),partition2node[partition]) != node_ids.end() &&
        find(zones.begin(), zones.end(), server_nodes[partition2node[partition]].second) != zones.end()){
            partition++;
            if(partition >= partition2node.size()){
                partition = 0;
            }
        }

        node_ids.push_back(partition2node[partition]);
        zones.push_back(server_nodes[node_ids.back()].second);


    }
    std::vector<int> result;
    for(auto v: node_ids){
        result.push_back(server_nodes[v].first);
    }
    return result;

}

void LoadBalancerRing::createNodes() {


    while(server_nodes.size() < NODE_COUNT){
        int zone = 0;
        while(zone < ZONE_COUNT && server_nodes.size() < NODE_COUNT){
            size_t node_id = server_nodes.size();
            server_nodes[node_id] = {node_id, zone};
            zone++;
        }
    }

}


LoadBalancerRing::LoadBalancerRing(size_t node_count, int number_of_replicas) {

    NODE_COUNT = node_count;

    createNodes();

    //build ring
    std::vector<std::size_t> part2node;
    for(size_t partition = 0; partition < (size_t)pow(PARTITION_POWER, 2); partition++){
        part2node.push_back(partition % server_nodes.size());
    }

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(part2node), std::end(part2node), rng);


    //init
    partition2node = part2node;
    REPLICAS = number_of_replicas;
    int partitions_size = 1;
    while(pow(partitions_size, 2) < part2node.size()){

        partitions_size++;
    }

}

int LoadBalancerRing::GetNumberofReplicas() {
    return REPLICAS;
}

int LoadBalancerRing::GetDataCount() {
    return data_count;
}

size_t LoadBalancerRing::GetServersCount() {
    return server_nodes.size();
}


void LoadBalancerRing::InvalidateNode(int i) {
    server_nodes.erase(i);
}
