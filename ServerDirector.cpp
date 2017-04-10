#include "ServerDirector.hpp"

ServerDirector::ServerDirector(int port_to_listen): listener(port_to_listen) {}

void ServerDirector::receive_data() {
	ServerSocket acceptor;
	listener.acceptConnection(acceptor);
	
	string info;
	info.resize(1024);
	acceptor >> info;

	int data_size = json::parse(info)["size"].get<int>();

	acceptor << "ok";

	string data;
	data.resize(data_size);

	acceptor >> data;
	received_data = data;
}
