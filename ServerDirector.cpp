#include "ServerDirector.hpp"

ServerDirector::ServerDirector(int port_to_listen): listener(port_to_listen) {
	userlist.read_json();
	session_index = 0;
}

string ServerDirector::wait_request_at_socket(ServerSocket& acceptor) {
	listener.acceptConnection(acceptor);
	
	stringstream data_stream;
	acceptor >> data_stream;

	return data_stream.str();
}

int ServerDirector::create_session_id(string username, string password) {
	tuple<string,string> tpl = make_tuple(username,password);
	
	auto it = std::find_if(std::begin(session_map), std::end(session_map), 
		[&](const std::pair<int, tuple<string,string> > &pair) { return pair.second == tpl; });
	
	if(it != session_map.end()) {
		session_map.erase(it);
	}

	session_index++;
	session_map[session_index] = tpl;
	return session_index;
}

bool ServerDirector::check_session_id_existance(int id) {
	auto it = session_map.find(id);
	if(it != session_map.end()) return true;
	else return false;
}

tuple<string,string> ServerDirector::get_credentials_for_session_id(int id) {
	return session_map[id];
}

void ServerDirector::handle_request() {
	ServerSocket acceptor;
	json request = json::parse(wait_request_at_socket(acceptor));	
	cout << request << endl;

	string command, username, password; 
	try {
		command = request["command"].get<string>();
	} catch(...) {
		acceptor << get_error_response("server has received incorrect data while getting command");
		return;
	}
	//retrive username and pass by id

	if(command=="login") {
		try {
			string username = request["username"].get<string>(); 
			string password = request["password"].get<string>(); 
			int session_id;

			if(!userlist.check_username(username)) {
				userlist.add_user(username, password);
				session_id = create_session_id(username, password);
				acceptor << get_info_response(session_id);

			} else if(userlist.check_username_password(username,password)) {
				session_id = create_session_id(username, password);
				acceptor << get_info_response(session_id);

			} else {
				acceptor << get_error_response("incorrect password for this user");
			}
		} catch(...) {
			acceptor << get_error_response("server has received incorrect data while getting username or password");
		}

	} else if(command=="get_thread") {
		try {
			int thread_id = request["thread"].get<int>();
			int session_id = request["session_id"].get<int>();
			std::tie(username, password) = get_credentials_for_session_id(session_id);

			if(userlist.check_username_password(username,password)) {
				userlist.add_thread(username, thread_id);
				acceptor << get_messages_response(thread_id, -1);
			} else {
				acceptor << get_error_response("authentication problem");
			}
		} catch(...) {
			acceptor << get_error_response("server has received incorrect data while getting thread_id");
		}

	} else if(command=="refresh") {
		try {
			int thread_id = request["thread"].get<int>();
			int msg_id = request["id"].get<int>();
			int session_id = request["session_id"].get<int>();
			std::tie(username, password) = get_credentials_for_session_id(session_id);
	
			if(userlist.check_username_password(username, password) && userlist.check_thread(username, thread_id)) {
				acceptor << get_messages_response(thread_id, msg_id);
			} else {
				acceptor << get_error_response("authentication problem");
			}
		} catch(...) {
			acceptor << get_error_response("server has received incorrect data while getting thread_id or msg_id");
		}

	} else if(command=="write") {
		try {
			int thread_id = request["thread"].get<int>();
			string text = request["body"].get<string>();
			int session_id = request["session_id"].get<int>();
			std::tie(username, password) = get_credentials_for_session_id(session_id);
			
			if(userlist.check_username_password(username, password) && userlist.check_thread(username, thread_id)) {
				msg_storage.push_message_to_thread(thread_id, username, acceptor.getLastClientIP(), text);
				acceptor << get_success_response();
			} else {
				acceptor << get_error_response("authentication problem");
			}
		} catch(...) {
			acceptor << get_error_response("server has received incorrect data while getting data for writing massage");
		}	

	} else {
		acceptor << get_error_response("some unexpected shit happened");
	}
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

string ServerDirector::get_info_response(int session_id) {
	json info;
	info["response"] = "ok";
	info["body"] = userlist.get_threads_info();
	info["session_id"] = session_id;
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

void ServerDirector::write_threads() {
	msg_storage.save_threads();
}

void ServerDirector::write_userlist() {
	userlist.write_json();
}