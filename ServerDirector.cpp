#include "ServerDirector.hpp"

ServerDirector::ServerDirector(int port_to_listen): listener(port_to_listen) {
	userlist.read_json();
}

string ServerDirector::wait_request_at_socket(ServerSocket& acceptor) {
	listener.acceptConnection(acceptor);
	
	stringstream data_stream;
	acceptor >> data_stream;

	return data_stream.str();
}

void ServerDirector::handle_request() {
	ServerSocket acceptor;
	json request = json::parse(wait_request_at_socket(acceptor));	

	string command = request["command"].get<string>();
	string username = request["username"].get<string>();
	string password = request["password"].get<string>();

	if(command=="login") {
		if(!userlist.check_username(username)) {
			userlist.add_user(username, password);
			acceptor << get_info_response();

		} else if(userlist.check_username_password(username,password)) {
			acceptor << get_info_response();

		} else {
			acceptor << get_error_response("incorrect password for this user");
		}

	} else if(command=="get_thread") {
		int thread_id = request["thread"].get<int>();
		
		if(userlist.check_username_password(username,password)) {
			userlist.add_thread(username, thread_id);
			acceptor << get_messages_response(thread_id, -1);
		} else {
			acceptor << get_error_response("authentication problem");
		}

	} else if(command=="refresh") {
		int thread_id = request["thread"].get<int>();
		int msg_id = request["id"].get<int>();

		if(userlist.check_username_password(username, password) && userlist.check_thread(username, thread_id)) {
			acceptor << get_messages_response(thread_id, msg_id);
		} else {
			acceptor << get_error_response("authentication problem");
		}
		
	} else if(false) {

	} else {
		acceptor << get_error_response("some unexpected shit happened");
	}
	//do not forget to create and connect threads
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