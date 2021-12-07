//
// Created by Egbantan Babatunde on 11/14/21.
//
#include "LoadBalancerStub.h"

void LoadBalancerStub::Init(std::unique_ptr<MultiPurposeServerSocket> socket) {
    this->server_socket = std::move(socket);

}

CustomerRequest LoadBalancerStub::ReceiveCustomerRequest() {
    char buffer[32];
    CustomerRequest request;
    if (server_socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    return request;
}

int LoadBalancerStub::Ship(ServerClientInterface info,ServerClientInterfaceOp operation) {
    char buffer[32];

    if(operation == INFO){
        info.info.Marshal(buffer);
        return server_socket->Send(buffer, info.info.Size(), 0);
    }else{
        info.record.Marshal(buffer);
        return server_socket->Send(buffer, info.record.Size(), 0);
    }


}

HandShaking LoadBalancerStub::RecieveIdentification() {
    HandShaking id;

    char buffer[id.Size()];

    if (server_socket->Recv(buffer, id.Size(), 0)) {
        id.Unmarshal(buffer);
    }

    memset(buffer, 0, id.Size());
    return id;
}

AdminRequest LoadBalancerStub::ReceiveAdminRequest() {
    AdminRequest request;

    char buffer[64];
    if (server_socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    memset(buffer, 0, 64);
    return request;
}





int LoadBalancerStub::ShipCacheToClient(std::string data) {

    return server_socket->SendString(data);

}
