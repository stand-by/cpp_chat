#include "JsonWrapper.hpp"

class MessageThread: public JsonWrapper {
private:
	int thread_id;
	int amount_messages_to_hold;
	int amount_messages_shifted;
public:
	MessageThread(int id, int amount);
	
	//need to implement appropriate write method(to append), because we will lose data
	void read_json();
	void read_whole_json();

	string get_whole_jsonbuffer();
	string get_jsonbuffer_start_from(int msg_id);
};