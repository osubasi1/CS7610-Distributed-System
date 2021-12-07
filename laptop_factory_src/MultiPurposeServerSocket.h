#ifndef __SERVERSOCKET_H__
#define __SERVERSOCKET_H__

#include <memory>

#include "Socket.h"

class MultiPurposeServerSocket: public Socket {
public:
	MultiPurposeServerSocket() {}
	~MultiPurposeServerSocket() {}

	MultiPurposeServerSocket(int fd, bool nagle_on = NAGLE_ON);

	bool Init(int port);
    std::unique_ptr<MultiPurposeServerSocket> Accept();
};


#endif // end of #ifndef __SERVERSOCKET_H__
