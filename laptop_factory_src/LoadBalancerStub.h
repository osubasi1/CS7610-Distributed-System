//
// Created by Egbantan Babatunde on 11/14/21.
//


#ifndef DISTRUBUTEDFINALCODE_LOADBALANCERSTUB_H
#define DISTRUBUTEDFINALCODE_LOADBALANCERSTUB_H


#include <memory>



#include <map>



#include <string>
#include <cstring>

#include "MultiPurposeServerSocket.h"
#include "MultiPurposeClientSocket.h"
#include "Messages.h"
#include "MaxLRUCache.h"


class LoadBalancerStub {
private:
    std::unique_ptr<MultiPurposeServerSocket> server_socket;
public:
    void Init(std::unique_ptr<MultiPurposeServerSocket> socket);

    CustomerRequest ReceiveCustomerRequest();

    int Ship(ServerClientInterface info, ServerClientInterfaceOp operation);



    HandShaking RecieveIdentification();

    AdminRequest ReceiveAdminRequest();




    int ShipCacheToClient(std::string cacheToSend);

};


#endif //DISTRUBUTEDFINALCODE_LOADBALANCERSTUB_H
