#include "ServerStub.h"
#include <cstring>

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<MultiPurposeServerSocket> socket) {
	this->server_socket = std::move(socket);
}

CustomerRequest ServerStub::ReceiveCustomerRequest() {
	char buffer[32];
	CustomerRequest request;
	if (server_socket->Recv(buffer, request.Size(), 0)) {
		request.Unmarshal(buffer);
	}
	return request;
}

int ServerStub::ShipLaptop(LaptopInfo info) {
	char buffer[32];
	info.Marshal(buffer);
	return server_socket->Send(buffer, info.Size(), 0);
}

int ServerStub::ReturnRecord(CustomerRecord info) {
    char buffer[32];
    info.Marshal(buffer);
    return server_socket->Send(buffer, info.Size(), 0);
}

HandShaking ServerStub::RecieveIdentification() {
    char buffer[24];
    HandShaking id;
    if (server_socket->Recv(buffer, id.Size(), 0)) {
        id.Unmarshal(buffer);
    }
    return id;
}


ReplicationRequest ServerStub::ReceiveReplicationRequest() {
    char buffer[128];
    ReplicationRequest request;
    int size = request.Size();

    if(server_socket->Recv(buffer, size, 0)){
        request.Unmarshal(buffer);
    }
    return request;
}

void ServerStub::SendReplicationResponse() {
    char buffer[32];
    int response = 1;
    int size = sizeof(response);

    memcpy(buffer, &response, sizeof(response));
    server_socket->Send(buffer, size, 0);
}

int ServerStub::InitToBackup(std::string ip, int port) {

    int result = client_socket.Init(ip, port);

    if(result){
        HandShaking id(1, -1);
        result = SendIdentification(id);
    }
    return  result;
}

int ServerStub::SendIdentification(HandShaking id) {
    char buffer[32];
    int size = id.Size();

    id.Marshal(buffer);
    if(client_socket.Send(buffer, size, 0)){
        return  1;
    }else{
        return 0;
    }
}

int ServerStub::SendReplicationRequest(ReplicationRequest request) {
    char buffer[128];
    int size = request.Size();

    request.Marshal(buffer);

    int response = 0;
    if(client_socket.Send(buffer, size, 0)){

        size = sizeof(response);
        if(client_socket.Recv(buffer, size, 0)){
            memcpy(&response, buffer, sizeof(response));
        }else{
            return -1;
        }
    }else{
        return -1;
    }
    return response;
}


