#include <cstring>
#include <iostream>
#include <utility>

#include <arpa/inet.h>
#include "Messages.h"

CustomerRequest::CustomerRequest() {
	customer_id = -1;
	order_number = -1;
    request_type = -1;
}

void CustomerRequest::SetRequest(int id, int number, int type) {
	customer_id = id;
	order_number = number;
    request_type = type;
}

int CustomerRequest::GetCustomerId() { return customer_id; }
int CustomerRequest::GetOrderNumber() { return order_number; }
int CustomerRequest::GetRequestType() { return request_type; }

int CustomerRequest::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type);
}

void CustomerRequest::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_laptop_type = htonl(request_type);
	int offset = 0;
	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_laptop_type, sizeof(net_laptop_type));
}

void CustomerRequest::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_laptop_type;
	int offset = 0;
	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
    request_type = ntohl(net_laptop_type);
}

bool CustomerRequest::IsValid() {
	return (customer_id != -1);
}

void CustomerRequest::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << std::endl;
}

LaptopInfo::LaptopInfo() {
	customer_id = -1;
	order_number = -1;
    request_type = -1;
	engineer_id = -1;
    admin_id = -1;
}

void LaptopInfo::SetInfo(int id, int number, int type, int engid, int expid) {
	customer_id = id;
	order_number = number;
    request_type = type;
	engineer_id = engid;
    admin_id = expid;
}

void LaptopInfo::CopyOrder(CustomerRequest order) {
	customer_id = order.GetCustomerId();
	order_number = order.GetOrderNumber();
    request_type = order.GetRequestType();
}
void LaptopInfo::SetEngineerId(int id) { engineer_id = id; }
void LaptopInfo::SetExpertId(int id) { admin_id = id; }

int LaptopInfo::GetCustomerId() { return customer_id; }
int LaptopInfo::GetOrderNumber() { return order_number; }
int LaptopInfo::GetLaptopType() { return request_type; }
int LaptopInfo::GetEngineerId() { return engineer_id; }
int LaptopInfo::GetExpertId() { return admin_id; }

int LaptopInfo::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
		+ sizeof(engineer_id) + sizeof(admin_id);
}

void LaptopInfo::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_laptop_type = htonl(request_type);
	int net_engineer_id = htonl(engineer_id);
	int net_expert_id = htonl(admin_id);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_laptop_type, sizeof(net_laptop_type));
	offset += sizeof(net_laptop_type);
	memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(buffer + offset, &net_expert_id, sizeof(net_expert_id));

}

void LaptopInfo::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_laptop_type;
	int net_engineer_id;
	int net_expert_id;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));
	offset += sizeof(net_laptop_type);
	memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(&net_expert_id, buffer + offset, sizeof(net_expert_id));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
    request_type = ntohl(net_laptop_type);
	engineer_id = ntohl(net_engineer_id);
    admin_id = ntohl(net_expert_id);
}

bool LaptopInfo::IsValid() {
	return (customer_id != -1);
}

void LaptopInfo::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << " ";
	std::cout << "engid " << engineer_id << " ";
	std::cout << "expid " << admin_id << std::endl;
}

void LaptopInfo::operator=(const ServerClientInterface &in) {
	customer_id = in.info.customer_id;
	order_number = in.info.order_number;
	request_type = in.info.request_type;
	engineer_id = in.info.engineer_id;
	admin_id = in.info.admin_id;

}

CustomerRecord::CustomerRecord() {
    customer_id = -1;
    last_order = -1;
}

int CustomerRecord::Size() {
    return sizeof(customer_id) + sizeof(last_order);
}

void CustomerRecord::SetRecord(int id, int order) {
	customer_id = id;
	last_order = order;
}
int CustomerRecord::GetLastOrder() {
	return last_order;
}
int CustomerRecord::GetCustomerId(){
	return customer_id;
}

void CustomerRecord::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_last_order = htonl(last_order);
    int offset = 0;

    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_last_order, sizeof(net_last_order));
}

void CustomerRecord::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_last_order;
    int offset = 0;

    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));


    customer_id = ntohl(net_customer_id);
    last_order = ntohl(net_last_order);

}

CustomerRecord::CustomerRecord(int id, int order_no) {
    customer_id = id;
    last_order = order_no;
}

void CustomerRecord::operator=(const ServerClientInterface &info) {
	customer_id = info.record.customer_id;
	last_order = info.record.last_order;

}


HandShaking::HandShaking() {
	response = -1;
	message = -1;
}

void HandShaking::Marshal(char *buffer) {
	int net_message = htonl(message);
	int net_response = htonl(response);
	int offset = 0;

	memcpy(buffer + offset, &net_message, sizeof(net_message));
	offset += sizeof(net_message);
	memcpy(buffer + offset, &net_response, sizeof(net_response));
}

void HandShaking::Unmarshal(char *buffer) {
	int net_message;
	int net_response;
	int offset = 0;

	memcpy(&net_message, buffer + offset, sizeof(net_message));
	offset += sizeof(net_message);
	memcpy(&net_response, buffer + offset, sizeof(net_response));


	message = ntohl(net_message);
	response = ntohl(net_response);
}

int HandShaking::Size() {
	return sizeof(message) + sizeof(response);
}

ReplicationRequest::ReplicationRequest() {
	factory_id = -1;
	committed_index = -1;
	last_index = -1;
}

ReplicationRequest::ReplicationRequest(int fac_id, int committed_in, int last_in, MapOp opcode) {
	factory_id = fac_id;
	committed_index = committed_in;
	last_index = last_in;
	op = opcode;
}

int ReplicationRequest::Size() {
	return sizeof(int) + sizeof(int)
			+ sizeof(int) + sizeof(MapOp);
}

void ReplicationRequest::Marshal(char *buffer) {
	int net_factory_id = htonl(factory_id);
	int net_committed_index = htonl(committed_index);
	int net_last_index = htonl(last_index);
	int net_opcode = htonl(op.opcode);
	int net_arg1 = htonl(op.arg1);
	int net_arg2 = htonl(op.arg2);

	int offset = 0;


	memcpy(buffer + offset, &net_factory_id, sizeof(net_factory_id));
	offset += sizeof(net_factory_id);
	memcpy(buffer + offset, &net_committed_index, sizeof(net_committed_index));
	offset += sizeof(net_committed_index);
	memcpy(buffer + offset, &net_last_index, sizeof(net_last_index));
	offset += sizeof(net_last_index);
	memcpy(buffer + offset, &net_opcode, sizeof(net_opcode));
	offset += sizeof(net_opcode);
	memcpy(buffer + offset, &net_arg1, sizeof(net_arg1));
	offset += sizeof(net_arg1);
	memcpy(buffer + offset, &net_arg2, sizeof(net_arg2));


}

void ReplicationRequest::Unmarshal(char *buffer) {
	int net_factory_id;
	int net_committed_index;
	int net_last_index;
	int net_opcode;
	int net_arg1;
	int net_arg2;

	int offset = 0;

	memcpy(&net_factory_id, buffer + offset, sizeof(net_factory_id));
	offset += sizeof(net_factory_id);
	memcpy(&net_committed_index, buffer + offset, sizeof(net_committed_index));
	offset += sizeof(net_committed_index);
	memcpy(&net_last_index, buffer + offset, sizeof(net_last_index));
	offset += sizeof(net_last_index);
	memcpy(&net_opcode, buffer + offset, sizeof(net_opcode));
	offset += sizeof(net_opcode);
	memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
	offset += sizeof(net_arg1);
	memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));

	factory_id = ntohl(net_factory_id);
	committed_index = ntohl(net_committed_index);
	last_index = ntohl(net_last_index);
	op.opcode = ntohl(net_opcode);
	op.arg1 =  ntohl(net_arg1);
	op.arg2 =  ntohl(net_arg2);
}

void ServerClientInterface::operator=(LaptopInfo in) {
	info = in;
}

int AdminRequest::GetRequestType() {
    return request_type;
}

int AdminRequest::Size() {
    return sizeof(request_type) + sizeof(s_info);
}

void AdminRequest::Unmarshal(char *buffer) {

    int net_request_type;
    int net_server_info_port;
    int net_server_info_id;
    struct in_addr net_server_info_server_ip;

    int offset = 0;

    memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));
    offset += sizeof(net_request_type);

    memcpy(&net_server_info_id, buffer + offset, sizeof(net_server_info_id));
    offset += sizeof(net_server_info_id);

    memcpy(&net_server_info_port, buffer + offset, sizeof(net_server_info_port));
    offset += sizeof(net_server_info_port);


    memcpy(&net_server_info_server_ip, buffer + offset, sizeof(net_server_info_server_ip));
    offset += sizeof(net_server_info_server_ip);

    request_type = ntohl(net_request_type);
    s_info.port_no = ntohl(net_server_info_port);
    s_info.peer_ip = inet_ntoa(net_server_info_server_ip);
    s_info.unique_id = ntohl(net_server_info_id);
    memset(buffer, 0, 64);

}

void AdminRequest::Marshal(char *buffer) {
    int net_request_type = htonl(request_type);
    int net_server_info_port = htonl(s_info.port_no);
    int net_server_info_id = htonl(s_info.unique_id);
    std::string net_server_info_address = s_info.peer_ip;
    int net_server_info_server_ip = htonl(inet_network(s_info.peer_ip.c_str()));


    int offset = 0;
    memset(buffer, 0, 64);

    memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
    offset += sizeof(net_request_type);

    memcpy(buffer + offset, &net_server_info_id, sizeof(net_server_info_id));
    offset += sizeof(net_server_info_id);

    memcpy(buffer + offset, &net_server_info_port, sizeof(net_server_info_port));
    offset += sizeof(net_server_info_port);

   memcpy(buffer + offset, &net_server_info_server_ip, sizeof(net_server_info_server_ip));

}

void AdminRequest::SetRequest(int i, const ServerInfo& in_info) {
    request_type = i;
    s_info = in_info;
}

ServerInfo AdminRequest::GetServerInfo() {
    return s_info;
}

AdminRequest::AdminRequest() {

    request_type = -1;
    s_info = ServerInfo();

}


ServerInfo::ServerInfo(int id, int port, std::string ip) {
    unique_id = id;
    port_no = port;
    peer_ip = std::move(ip);
}

ServerInfo &ServerInfo::operator=(const ServerInfo &info) {
    unique_id = info.unique_id;
    port_no = info.port_no;
    peer_ip = info.peer_ip;

    return *this;
}
