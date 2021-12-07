#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>
#include <cstring>
#include <iostream>

#include "MultiPurposeClientSocket.h"
#include "Messages.h"

class ClientStub {
private:
	MultiPurposeClientSocket socket;
public:
	ClientStub();
	int Init(std::string ip, int port);
	LaptopInfo Order(CustomerRequest request);
    CustomerRecord ReadRecord(CustomerRequest request);

    void SendIdentification(HandShaking id);
	std::string ReceiveCache(CustomerRequest request, int size = 8192);
};


#endif // end of #ifndef __CLIENT_STUB_H__
