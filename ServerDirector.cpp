#include "ServerDirector.hpp"

ServerDirector::ServerDirector(int port_to_listen): listener(port_to_listen) {}

string ServerDirector::wait_request_at_socket(ServerSocket& acceptor) {
	listener.acceptConnection(acceptor);
	
	stringstream data_stream;
	acceptor >> data_stream;

	return data_stream.str();
}

void ServerDirector::handle_request() {
	userlist.read_json();
	ServerSocket acceptor;
	string request = wait_request_at_socket(acceptor);	

	//error test
	string response = get_info_response();
	acceptor << response;
}

string ServerDirector::get_error_response(string body) {
	json error_response;
	error_response["response"] = "error";
	error_response["body"] = body;	
	return error_response.dump();
}

string ServerDirector::get_info_response() {
	json info;
	info["response"] = "ok";
	info["threads"] = userlist.get_threads_info();
	return info.dump();
}
