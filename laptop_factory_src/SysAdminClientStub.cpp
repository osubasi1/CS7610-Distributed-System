//
// Created by Egbantan Babatunde on 12/3/21.
//

#include "SysAdminClientStub.h"

bool SysAdminClientStub::Init(std::string ip, int port) {
    int result = socket.Init(ip, port);
    if(result){
        HandShaking id(1, -1);
        SendIdentification(id);
    }
    return  result;
}

bool SysAdminClientStub::Send(AdminRequest info) {
    char buffer[64];
    int size;
    info.Marshal(buffer);
    size = info.Size();
    if (socket.Send(buffer, size, 0)) {
        return true;
    }

    return false;
}

bool SysAdminClientStub::CloseStream() {
    AdminRequest request;
    request.SetRequest(2,ServerInfo());
    char buffer[64];
    int size;

    request.Marshal(buffer);
    size = request.Size();
    if (socket.Send(buffer, size, 0)) {
        return true;
    }

    return false;
}

void SysAdminClientStub::SendIdentification(HandShaking shaking) {
    char buffer[32];
    int size = shaking.Size();

    shaking.Marshal(buffer);
    socket.Send(buffer, size, 0);
}
