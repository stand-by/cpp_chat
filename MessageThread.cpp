#include "MessageThread.hpp"

 const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

MessageThread::MessageThread(int id, int amount): 
JsonWrapper("msg_threads/"+std::to_string(id)+".json"),
thread_id(id), amount_messages_to_hold(amount), amount_messages_shifted(0)
{}

int MessageThread::get_thread_id() {
	return thread_id;
}

void MessageThread::read_json() {
	JsonWrapper::read_json();

	if(json_file.size() <= amount_messages_to_hold) return;

	json temp = json::array();
	json::iterator it = json_file.begin();
	for (it = it + (json_file.size()-amount_messages_to_hold); it != json_file.end(); ++it)
 		temp.push_back(*it);
 	clear_json();
 	json_file = temp;
}

void MessageThread::read_whole_json() {
	JsonWrapper::read_json();
}

string MessageThread::get_whole_jsonbuffer() {
	return json_file.dump();
}

string MessageThread::get_jsonbuffer_start_from(int msg_id) {
	json temp = json::array();
	for (json::iterator it = json_file.begin(); it != json_file.end(); ++it) {
		if((*it)["id"].get<int>() > msg_id) {
			temp.push_back(*it);
		}
	}
	return temp.dump();
}

int MessageThread::get_amount_messages_jsonbuffer(){
	return json_file.size();
}

void MessageThread::push_message(string username, string ip, string body) {
	json msg_json = json({});
	//cout << "lol" << endl;
	msg_json["id"] = (*(json_file.end()-1))["id"].get<int>()+1;
	//cout << "lol" << endl;
	//cout << msg_json << endl;
	msg_json["ip"] = ip;
	msg_json["username"] = username;
	msg_json["datetime"] = currentDateTime();
	msg_json["body"] = body;

	json_file.push_back(msg_json);
}
