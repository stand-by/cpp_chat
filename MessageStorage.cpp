#include "MessageStorage.hpp"

bool MessageStorage::is_thread_connected(int id_to_check) {
	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == id_to_check) return true;
	return false;
}

void MessageStorage::connect_thread(int id) {
	if(!is_thread_connected(id)) {
		MessageThread t(id,10);
		t.read_whole_json();
		threads.push_back(t);
	}
}

void MessageStorage::push_message_to_thread(int thread_id, string username, string ip, string body) {
	connect_thread(thread_id);

	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == thread_id) threads[i].push_message(username, ip, body);
}

string MessageStorage::get_buffered_messages(int thread_id) {
	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == thread_id) return threads[i].get_whole_jsonbuffer();
}

string MessageStorage::get_buffered_messages_since(int thread_id, int msg_id) {
	for (int i = 0; i < threads.size(); i++) 
		if(threads[i].get_thread_id() == thread_id) return threads[i].get_jsonbuffer_start_from(msg_id);
}