#ifndef SERVERDIRECTOR_HPP
#define SERVERDIRECTOR_HPP

#include "SocketClasses/ServerSocket.hpp"
#include "UserList.hpp"
#include "MessageStorage.hpp"

class ServerDirector {
private:
	ServerSocket listener;
	ServerSocket acceptor;
	UserList userlist;
	MessageStorage msg_storage;
public:
	ServerDirector(int port_to_listen);
};

#endif
