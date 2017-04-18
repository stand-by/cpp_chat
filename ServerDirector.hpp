#ifndef SERVERDIRECTOR_HPP
#define SERVERDIRECTOR_HPP

#include "SocketClasses/ServerSocket.hpp"
#include "UserList.hpp"
#include "MessageStorage.hpp"
#include <sstream>

class ServerDirector {
private:
	ServerSocket listener;
	UserList userlist;
	MessageStorage msg_storage;
public:
	ServerDirector(int port_to_listen);
	void handle_request();
	string wait_request_at_socket(ServerSocket& acceptor);
	string get_error_response(string body);
	string get_info_response();
};

#endif
