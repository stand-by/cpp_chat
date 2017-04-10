#ifndef MESSAGETHREAD_HPP
#define MESSAGETHREAD_HPP

#include "JsonWrapper.hpp"

class MessageThread: public JsonWrapper {
private:
	int thread_id;
	int amount_messages_to_hold;
	int amount_messages_shifted;
public:
	MessageThread(int id, int amount);
	
	int get_thread_id();
	//need to implement appropriate write method(to append), because we will lose data
	void read_json();
	void read_whole_json();

	string get_whole_jsonbuffer();
	string get_jsonbuffer_start_from(int msg_id);
	int get_amount_messages_jsonbuffer();

	//validate id of pushed msg and check if it has necessary fields before inserting
	//and shift first message to keep amount_messages_to_hold right
	void push_message(string username, string ip, string body);
};

#endif
