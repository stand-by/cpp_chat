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
	string response = get_success_response("lol");
	acceptor << response;
	//be sure that user belongs to thread when calling get_messages_response
}

string ServerDirector::get_success_response(string body) {
	json success_response;
	success_response["response"] = "ok";
	success_response["body"] = body;	
	return success_response.dump();
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
	info["body"] = userlist.get_threads_info();
	return info.dump();
}

string ServerDirector::get_messages_response(int thread, int id_since) {
	msg_storage.connect_thread(thread);

	json messages;
	messages["response"] = "ok";

	if(id_since < 0)  
		messages["body"] = json::parse(msg_storage.get_buffered_messages(thread));
	else 
		messages["body"] = json::parse(msg_storage.get_buffered_messages_since(thread, id_since));

	return messages.dump();
}