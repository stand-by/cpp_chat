#include "MessageThread.hpp"

MessageThread::MessageThread(int id, int amount): 
JsonWrapper("msg_threads/"+std::to_string(id)+".json"),
thread_id(id), amount_messages_to_hold(amount), amount_messages_shifted(0)
{}

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
