#ifndef CLIENTDIRECTOR_HPP
#define CLIENTDIRECTOR_HPP

#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include <algorithm>
#include <sstream>
#include <tuple>

using json = nlohmann::json;

class ClientDirector {
private:
	string msg_stream;
	string server_ip;
	string username;
	string password;
	int last_msg_id;
	int session_id;
	int thread_id;

	json request_thread(int id);
	string parse_json_with_messages(json data);
public:
	ClientDirector(string ip, string user, string pass);
	bool login();
	bool display_thread(int id);
	void send_message(string msg);
	void refresh_messages();
	void print_msg_stream();
};

#endif
