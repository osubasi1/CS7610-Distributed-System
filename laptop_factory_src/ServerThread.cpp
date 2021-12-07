#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"
#include "Messages.h"


LaptopInfo LaptopFactory::
CreateLaptop(CustomerRequest order, int engineer_id) {

	return PerformAdminstrative(order, engineer_id);;
}

LaptopInfo LaptopFactory::
PerformAdminstrative(CustomerRequest order, int engineer_id) {
	LaptopInfo laptop;
	laptop.CopyOrder(order);
	laptop.SetEngineerId(engineer_id);
    laptop.SetExpertId(-1);

	std::promise<LaptopInfo> prom;
	std::future<LaptopInfo> fut = prom.get_future();

	std::unique_ptr<ExpertRequest> req = 
		std::unique_ptr<ExpertRequest>(new ExpertRequest);
	req->laptop = laptop;
	req->prom = std::move(prom);

	erq_lock.lock();
	erq.push(std::move(req));
	erq_cv.notify_one();
	erq_lock.unlock();

	laptop = fut.get();
	return laptop;
}

void LaptopFactory::
EngineerThread(std::unique_ptr<MultiPurposeServerSocket> socket, int id) {


	ServerStub stub;

	stub.Init(std::move(socket));
    HandShaking role = stub.RecieveIdentification();
    if(role.message == 0){
        PrimaryServerRole(std::move(stub), id);
    }else if(role.message == 1){
        is_backup = true;
        IdleFactoryAdminRole(std::move(stub));
    }else{
        std::cout<<"Error: invalid role"<<std::endl;

    }

}

void LaptopFactory::PrimaryServerRole(ServerStub stub, int id){
    int engineer_id = id;

    LaptopInfo laptop;
    CustomerRecord record;
    CustomerRequest request;
    int request_type;
    while (true) {
        request = stub.ReceiveCustomerRequest();
        if (!request.IsValid()) {
            break;
        }
        request_type = request.GetRequestType();
        switch (request_type) {

            case 1:

                //become leader
                if(GetPrimaryId() != GetFactoryId()){
                    SetPrimaryId(GetFactoryId());
                    ConnectToBackups();
                    if(is_backup){
                        is_switched = true;
                        is_backup = false;
                    }
                }

                laptop = CreateLaptop(request, engineer_id);
                stub.ShipLaptop(laptop);

                break;
            case 2:
                record = ReadRecord(request);
                stub.ReturnRecord(record);
                break;
            case 3:
                record = DeleteRecord(request);
                stub.ReturnRecord(record);
                break;
            default:
                std::cout << "Undefined laptop type: "
                          << request_type << std::endl;

        }
    }
}

void LaptopFactory::IdleFactoryAdminRole(ServerStub stub){

    while(true){
        ReplicationRequest request = stub.ReceiveReplicationRequest();
        if (request.factory_id == -1) {
            SetPrimaryId(-1);
            break;
        }

        if(GetPrimaryId() != request.factory_id){
            SetPrimaryId(request.factory_id);
        }
        smr_log.push_back(request.op);
        last_index++;

        if(smr_log.size() >= request.committed_index && request.committed_index != -1){
            std::unique_lock<std::mutex> cl(customer_record_lock, std::defer_lock);
            cl.lock();

            MapOp op = smr_log[request.committed_index];
            customer_record[op.arg1] = op.arg2;

            committed_index++;
            cl.unlock();
        }

        stub.SendReplicationResponse();
    }
}
void LaptopFactory::ProductionAdminThread(int id) {
    std::unique_lock<std::mutex> cl(customer_record_lock, std::defer_lock);

    while (true) {
        cl.lock();

		if (erq.empty()) {
			erq_cv.wait(cl, [this]{ return !erq.empty(); });
		}

		auto req = std::move(erq.front());
		erq.pop();

        MapOp log = {1, req->laptop.GetCustomerId(), req->laptop.GetOrderNumber()};
        smr_log.push_back(log);

        if(is_switched){
            committed_index = last_index;
            customer_record[smr_log[last_index].arg1] = smr_log[last_index].arg2;
            is_switched = false;
        }else{

        }

        last_index++;

        if(Replicate(log)){
            customer_record[smr_log[last_index].arg1] = smr_log[last_index].arg2;
            committed_index = last_index;
        }

        cl.unlock();
        req->laptop.SetExpertId(id);
		req->prom.set_value(req->laptop);	
	}
}

CustomerRecord LaptopFactory::ReadRecord(CustomerRequest request) {
    if(customer_record.find(request.GetCustomerId()) == customer_record.end()){
        return  {};
    }
    int last_order_no = customer_record[request.GetCustomerId()];
    return {request.GetCustomerId(), last_order_no};
}

int LaptopFactory::GetFactoryId() {
    return factory_id;
}

int LaptopFactory::GetPrimaryId() {
    return primary_id;
}

void LaptopFactory::SetPrimaryId(int id) {
    primary_id = id;
}

bool LaptopFactory::Replicate(MapOp op) {
    std::unique_lock<std::mutex> maplock(map_record_lock, std::defer_lock);

    maplock.lock();
    for(int i= 0; i < zombies_peer_stubs.size(); i++){
        ServerInfo peer = peer_map[zombies_peer_stubs[i]];

        ServerStub* stub = new ServerStub();
        int response = stub->InitToBackup(peer.peer_ip, peer.port_no);
        if(response && (UpdateBackup(stub) == 1)){
            peer_stubs.push_back(stub);
            zombies_peer_stubs[i] = nullptr;
            peer_map.emplace(stub, peer);
           }

       }

    maplock.unlock();

    maplock.lock();
    for(int i = 0; i < zombies_peer_stubs.size(); i++){

        if(zombies_peer_stubs[i] == nullptr){
            zombies_peer_stubs.erase(zombies_peer_stubs.begin()+i);
        }
    }
    maplock.unlock();


    bool result = true;

    for(auto & peer_stub : peer_stubs){
        if(peer_stub) {
                ReplicationRequest request(GetFactoryId(),committed_index,last_index, op);

                int response = peer_stub->SendReplicationRequest(request);
                if(response == 0){
                    result &= response;
                }else if(response == -1){
                    maplock.lock();
                    zombies_peer_stubs.push_back(peer_stub);
                    maplock.unlock();
                    peer_stub = nullptr;
                }
            }

    }

    maplock.lock();
    for(int i = 0; i < peer_stubs.size(); i++){

        if(peer_stubs[i] == nullptr){
            peer_stubs.erase(peer_stubs.begin()+i);
        }
    }
    maplock.unlock();



    return result;
}

void LaptopFactory::ConnectToBackups() {
    std::unique_lock<std::mutex> maplock(map_record_lock, std::defer_lock);

    for(ServerInfo& peer: peer_info){
        ServerStub* stub = new ServerStub();
        if(stub->InitToBackup(peer.peer_ip, peer.port_no) == 0){
            stub = nullptr;
        }
        if(stub){

            maplock.lock();
            peer_map.emplace(stub, peer);
            peer_stubs.push_back(stub);
            maplock.unlock();
        }


    }

}

void LaptopFactory::SetPeerInfo(std::vector<ServerInfo>& info) {
    peer_info = info;
}

int LaptopFactory::UpdateBackup(ServerStub *pStub) {

    for(int i = 0; i < committed_index; i++){
        MapOp op = smr_log[i];
        ReplicationRequest request(GetFactoryId(),i,i, op);

        int response = pStub->SendReplicationRequest(request);
        if(response == 0){
            return 0;
        }

    }

    return 1;
}

CustomerRecord LaptopFactory::DeleteRecord(CustomerRequest request) {
    if(customer_record.find(request.GetCustomerId()) == customer_record.end()){
        return  {};
    }
    int last_order_no = customer_record[request.GetCustomerId()];
    customer_record.erase(request.GetCustomerId());
    return {request.GetCustomerId(), last_order_no};
}

