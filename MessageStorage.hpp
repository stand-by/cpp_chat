#include "MessageThread.hpp"

class MessageStorage {
private:
	vector<MessageThread> threads;
public:
	bool is_thread_connected(int id_to_check);
	//change read method when you will use buffer zone
	void connect_thread(int id);

	void push_message_to_thread(int thread_id, string msg_json);
};
