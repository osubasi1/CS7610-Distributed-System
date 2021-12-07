#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <map>
#include <vector>

#include "Messages.h"
#include "MultiPurposeServerSocket.h"
#include "ServerStub.h"

struct ExpertRequest {
	LaptopInfo laptop;
	std::promise<LaptopInfo> prom;
};






class LaptopFactory {
private:
	std::queue<std::unique_ptr<ExpertRequest>> erq;
	std::mutex erq_lock;
	std::condition_variable erq_cv;


    std::mutex customer_record_lock;
    std::mutex map_record_lock;

    std::map<int, int> customer_record;
    std::vector<MapOp> smr_log;
    std::vector<ServerInfo> peer_info;
    std::vector<ServerStub*> peer_stubs;
    std::vector<ServerStub*> zombies_peer_stubs;
    std::map<ServerStub*, ServerInfo> peer_map;

    int last_index = -1;
    int committed_index = -1;
    int primary_id = -1;
    int factory_id;

    bool is_switched = false;
    bool is_backup = false;

    LaptopInfo CreateLaptop(CustomerRequest order, int engineer_id);
    LaptopInfo PerformAdminstrative(CustomerRequest order, int engineer_id);
    CustomerRecord ReadRecord(CustomerRequest request);


    int GetFactoryId();

    int GetPrimaryId();

    void SetPrimaryId(int i);

    bool Replicate(MapOp op);

    void PrimaryServerRole(ServerStub stub, int id);

    void IdleFactoryAdminRole(ServerStub stub);

    void ConnectToBackups();


    int UpdateBackup(ServerStub *pStub);
public:

	void EngineerThread(std::unique_ptr<MultiPurposeServerSocket> socket, int id);
	void ProductionAdminThread(int id);
    void SetFactoryId(int id){
        factory_id = id;
    }
    void SetPeerInfo(std::vector<ServerInfo>& info);

    CustomerRecord DeleteRecord(CustomerRequest request);
};

#endif // end of #ifndef __SERVERTHREAD_H__

