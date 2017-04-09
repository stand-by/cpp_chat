#include "MessageStorage.hpp"

bool MessageStorage::is_thread_connected(int id_to_check) {
	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == id_to_check) return true;
	return false;
}

void MessageStorage::connect_thread(int id) {
	if(!is_thread_connected(id)) {
		MessageThread t(id,3);
		t.read_whole_json();
		threads.push_back(t);
	}
}

void MessageStorage::push_message_to_thread(int thread_id, string msg_json) {
	connect_thread(thread_id);

	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == thread_id) threads[i].push_message(msg_json);
}
