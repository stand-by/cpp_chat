#include "ClientDirector.hpp"

ClientDirector::ClientDirector(string ip, string user, string pass) {
	server_ip = ip;
	username = user;
	password = pass;
	last_msg_id = -1;
    session_id = -1;
    thread_id = -1;
    msg_stream = "";
}

bool ClientDirector::login() {
    ClientSocket socket(server_ip, 8888);

    json req;
    req["username"] = username;
    req["password"] = password;
    req["command"] = "login";
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    if(resp["response"] == "ok") {
        for(json::iterator it = resp["body"]["threads_id"].begin(); it != resp["body"]["threads_id"].end(); ++it) {
            string str = to_string((*it).get<int>());
            cout << "Users for thread #" << str << ": ";// << resp["body"]["threads"][str] << endl;
            for(json::iterator jt = resp["body"]["threads"][str].begin(); jt != resp["body"]["threads"][str].end(); ++jt)
                cout << (*jt).get<string>() << ", ";
            cout << endl;
        }
        session_id = resp["session_id"].get<int>();
    	cout << "[*] " << "logged in or registered new" << endl;
        return true;
    } else {
        cout << "[*] " << resp["body"].get<string>() << endl;
        return false;
    }
}

string ClientDirector::parse_json_with_messages(json data) {
	data = data["body"];
    stringstream stream;
    for(json::iterator it = data.begin(); it != data.end(); ++it) {
        stream << (*it)["datetime"].get<string>()  << " [" << (*it)["username"].get<string>() << "] " << "> " << (*it)["body"].get<string>() << endl;
        last_msg_id = (*it)["id"];
    }
    return stream.str();
}

json ClientDirector::request_thread(int id) {
	ClientSocket socket(server_ip, 8888);

    json req;
    req["session_id"] = session_id;
    req["command"] = "get_thread";
    req["thread"] = id;
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    return resp;
}

bool ClientDirector::display_thread(int id) {
	json response = request_thread(id);

	if(response["response"]=="ok") {
		msg_stream = parse_json_with_messages(response);
		thread_id = id;
		return true;
	} else {
		cout << "[*] " << "The problem has occurred while displaying threads" << endl;
		cout << "[*] " << response["body"].get<string>() << endl;
		return false;
	}
}

void ClientDirector::send_message(string msg) {
	json req;
    req["session_id"] = session_id;
    req["command"] = "write";
    req["thread"] = thread_id;
    req["body"] = msg;

    ClientSocket socket(server_ip, 8888);
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    if(resp["response"]!="ok")
    	cout << "[*] " << "The problem has occurred while sending your message" << endl;
}

void ClientDirector::refresh_messages() {
	json req;
    req["session_id"] = session_id;
    req["command"] = "refresh";
    req["thread"] = thread_id;
    req["id"] = last_msg_id;

    ClientSocket socket(server_ip, 8888);
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    if(resp["response"]!="ok") {
    	cout << "[*] " << "The problem has occurred while receiving messages" << endl;
    } else {
	    msg_stream += parse_json_with_messages(resp);
	}
}

void ClientDirector::print_msg_stream() {
	cout << msg_stream;
}