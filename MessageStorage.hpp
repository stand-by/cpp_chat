#include "MessageThread.hpp"

class MessageStorage {
private:
	vector<MessageThread> threads;
	
public:
	//private?
	bool is_thread_connected(int id_to_check);
	//change read method when you will use buffer zone
	void connect_thread(int id);
	
	//private?
	void push_message_to_thread(int thread_id, string msg_json);
	
	void recieve_message(int thread_id, string username, string ip, string body);

	//string get_whole_buffer

	//string get_buffer_since
};
