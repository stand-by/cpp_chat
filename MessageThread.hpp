#include "JsonWrapper.hpp"

class MessageThread: public JsonWrapper {
private:
	int thread_id;
	int amount_messages_to_hold;
	int amount_messages_shifted;
public:
	MessageThread(int id, int amount);
};