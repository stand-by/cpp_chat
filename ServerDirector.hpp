#ifndef SERVERDIRECTOR_HPP
#define SERVERDIRECTOR_HPP

#include "SocketClasses/ServerSocket.hpp"
#include "UserList.hpp"
#include "MessageStorage.hpp"

class ServerDirector {
private:
	ServerSocket listener;
	UserList userlist;
	MessageStorage msg_storage;
	string received_data;
public:
	ServerDirector(int port_to_listen);
	void receive_data();
};

#endif
