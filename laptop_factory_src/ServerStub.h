#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "MultiPurposeServerSocket.h"
#include "MultiPurposeClientSocket.h"

#include "Messages.h"

class ServerStub {
private:
	std::unique_ptr<MultiPurposeServerSocket> server_socket;
    MultiPurposeClientSocket client_socket;
public:
	ServerStub();
	void Init(std::unique_ptr<MultiPurposeServerSocket> socket);
	CustomerRequest ReceiveCustomerRequest();
	int ShipLaptop(LaptopInfo info);
    int ReturnRecord(CustomerRecord info);

    HandShaking RecieveIdentification();
    int InitToBackup(std::string ip, int port);

    int SendIdentification(HandShaking id);

    int SendReplicationRequest(ReplicationRequest request);

    ReplicationRequest ReceiveReplicationRequest();

    void SendReplicationResponse();
};

#endif // end of #ifndef __SERVER_STUB_H__
