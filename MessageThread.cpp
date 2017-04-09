#include "MessageThread.hpp"

MessageThread::MessageThread(int id, int amount)
:thread_id(id), amount_messages_to_hold(amount), amount_messages_shifted(0) {
	filename = std::to_string(id)+".json";
}
