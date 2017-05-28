#ifndef SERVERDIRECTOR_HPP
#define SERVERDIRECTOR_HPP

#include "SocketClasses/ServerSocket.hpp"
#include "UserList.hpp"
#include "MessageStorage.hpp"
#include <algorithm>
#include <sstream>
#include <tuple>
#include <map>

class ServerDirector {
private:
	ServerSocket listener;
	UserList userlist;
	MessageStorage msg_storage;
	int session_index;
	map< int,tuple<string, string> > session_map;
public:
	ServerDirector(int port_to_listen);
	void handle_request();
	int create_session_id(string username, string password);
	bool check_session_id_existance(int id);
	tuple<string,string> get_credentials_for_session_id(int id);
	string wait_request_at_socket(ServerSocket& acceptor);
	string get_error_response(string body);
	string get_success_response(string body = "");
	string get_info_response(int session_id);
	string get_messages_response(int thread, int id_since);
	void write_threads();
	void write_userlist();
};

#endif
