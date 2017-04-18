#include "ServerDirector.hpp"

ServerDirector::ServerDirector(int port_to_listen): listener(port_to_listen) {}

string ServerDirector::wait_request_at_socket(ServerSocket& acceptor) {
	listener.acceptConnection(acceptor);
	
	stringstream data_stream;
	acceptor >> data_stream;

	return data_stream.str();
}

void ServerDirector::handle_request() {
	ServerSocket acceptor;
	string request = wait_request_at_socket(acceptor);	

	//echo
	string response = request;
	acceptor << response;
}