#include "ClientStub.h"

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	int result = socket.Init(ip, port);

    if(result){
        HandShaking id(0, -1);
        SendIdentification(id);
    }
    return  result;
}

LaptopInfo ClientStub::Order(CustomerRequest request) {
	LaptopInfo info;
	char buffer[32];
	int size;
	request.Marshal(buffer);
	size = request.Size();
	if (socket.Send(buffer, size, 0)) {
		size = info.Size();
		if (socket.Recv(buffer, size, 0)) {
			info.Unmarshal(buffer);
		} 
	}
	return info;
}

CustomerRecord ClientStub::ReadRecord(CustomerRequest request) {
    CustomerRecord info;
    char buffer[32];
    int size;
    request.Marshal(buffer);
    size = request.Size();
    if (socket.Send(buffer, size, 0)) {
        size = info.Size();
        if (socket.Recv(buffer, size, 0)) {
            info.Unmarshal(buffer);
        }
    }


    return info;
}

void ClientStub::SendIdentification(HandShaking id) {
    char buffer[32];
    int size = id.Size();

    id.Marshal(buffer);
    socket.Send(buffer, size, 0);

}

std::string ClientStub::ReceiveCache(CustomerRequest request, int size) {

    char buffer1[32];
    int size1;
    request.Marshal(buffer1);
    size1 = request.Size();
    std::string received;

    if (socket.Send(buffer1, size1, 0)) {
        received = socket.ReceiveString();
    }
    return received;
}
