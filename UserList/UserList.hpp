#ifndef USERLIST_HPP
#define USERLIST_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "../JsonWrapper/JsonWrapper.hpp"

class UserList: public JsonWrapper {
public:
	UserList();
	void add_user(string username, string password);
	void add_thread(string username, int thread);
	bool check_username(string username);
	bool check_thread(string username, int thread);
	bool check_username_password(string username, string password);
	json get_threads_info();
};

#endif
