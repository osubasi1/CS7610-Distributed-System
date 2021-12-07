//
// Created by Egbantan Babatunde on 12/3/21.
//

#ifndef DISTRUBUTEDFINALCODE_SYSADMINCLIENT_H
#define DISTRUBUTEDFINALCODE_SYSADMINCLIENT_H

#include <string>

#include "MultiPurposeClientSocket.h"
#include "Messages.h"

class SysAdminClientStub {
    MultiPurposeClientSocket socket;

public:
    SysAdminClientStub(){

    }

    bool Init(std::string ip, int port);

    bool Send(AdminRequest info);

    bool CloseStream();

    void SendIdentification(HandShaking shaking);
};


#endif //DISTRUBUTEDFINALCODE_SYSADMINCLIENT_H
