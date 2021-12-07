//
// Created by Egbantan Babatunde on 11/14/21.
//

#ifndef DISTRUBUTEDFINALCODE_LOADBALANCERTHREAD_H
#define DISTRUBUTEDFINALCODE_LOADBALANCERTHREAD_H




#include "LoadBalancerRing.h"
#include "LoadBalancerStub.h"

#include "LoadBalancerStub.h"
#include "MultiPurposeServerSocket.h"
#include "Messages.h"
#include "MultiPurposeClientSocket.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <iterator>


class LoadBalancerWorker {
private:

    std::map<int, MultiPurposeClientSocket*> ServersStubsMap;
    LoadBalancerRing ring;
    LoadBalancerRing old_ring;
    int algorithm;
    std::map<int,int> randServerMap;

    void Migrate();
    std::vector<int> GetRandAssignedServer(int id);
    void SendIdentification(int id);
    ServerClientInterface SendToServer(CustomerRequest request, ServerClientInterfaceOp operation, int server);
    void CustomerThread(LoadBalancerStub &&stub);
    void SysAdminThread(LoadBalancerStub &&stub);

    //@TODO create a wrapper class for map that use FIFO(easiest)/LIFO/LRU(most complicated) to control
    //size
    MaxLRUCache::Cache cache;
public:
    LoadBalancerWorker();

    void BalancerThread(std::unique_ptr<MultiPurposeServerSocket> socket);
    void InitRing(const std::vector<ServerInfo>& vector1,  LoadBalancerRing ring);
    void SetAlgorithm(int algo);






    void SetCacheSize(int size);

};


#endif //DISTRUBUTEDFINALCODE_LOADBALANCERTHREAD_H
