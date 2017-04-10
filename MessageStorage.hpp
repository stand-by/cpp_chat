#ifndef MESSAGESTORAGE_HPP
#define MESSAGESTORAGE_HPP

#include "MessageThread.hpp"

class MessageStorage {
private:
	vector<MessageThread> threads;
	
public:
	//private?
	bool is_thread_connected(int id_to_check);
	//change read method when you will use buffer zone
	void connect_thread(int id);
	
	void push_message_to_thread(int thread_id, string username, string ip, string body);
	
	string get_buffered_messages(int thread_id);

	string get_buffered_messages_since(int thread_id, int msg_id);
};

#endif
