#include "UserList.hpp"

UserList::UserList(): JsonWrapper("userlist.txt") {}

void UserList::add_user(string username, string password) {
	if(!check_username(username)) {
		json new_user = json({});
		new_user["username"] = username;
		new_user["password"] = password;
		new_user["threads"] = json::array();

		json_file.push_back(new_user);
	}
}

void UserList::add_thread(string username, int thread) {
	if(check_username(username) && !check_thread(username, thread)) {
		for (json::iterator it = json_file.begin(); it != json_file.end(); ++it) {
  			if((*it)["username"]==username) {
				(*it)["threads"].push_back(thread);
				break;
  			}
		}
	}
}

bool UserList::check_username(string username) {
	bool is_exist = false;

	for(json::iterator it = json_file.begin(); it != json_file.end(); ++it) {
  		if((*it)["username"]==username) {
  			is_exist = true;
  			break;
		}
  	}

  	return is_exist;
}

bool UserList::check_thread(string username, int thread) {
	bool is_exist = false;

	for(json::iterator it = json_file.begin(); it != json_file.end(); ++it) {
  		if((*it)["username"]==username) {
  			vector<int> threads = (*it)["threads"].get<std::vector<int> >();
  			if(std::find(threads.begin(), threads.end(), thread) != threads.end()) {
  				is_exist = true;
  				break;
  			}
		}
  	}

  	return is_exist;
}

bool UserList::check_username_password(string username, string password) {
	for(json::iterator it = json_file.begin(); it != json_file.end(); ++it)
  		if((*it)["username"]==username && (*it)["password"]==password) 
  			return true; 
  	return false;
}

json UserList::get_threads_info() {
	set<int> unique_threads;
	for(json::iterator it = json_file.begin(); it != json_file.end(); ++it)
		for(json::iterator jt = (*it)["threads"].begin(); jt != (*it)["threads"].end(); ++jt)
			unique_threads.insert((*jt).get<int>());

	json threads = json::object();
	for(set<int>::iterator it = unique_threads.begin(); it != unique_threads.end(); ++it)
		threads[to_string(*it)] = json::array();

	for(json::iterator it = json_file.begin(); it != json_file.end(); ++it) {
		for(json::iterator jt = (*it)["threads"].begin(); jt != (*it)["threads"].end(); ++jt)
			threads[to_string((*jt).get<int>())].push_back((*it)["username"]);
	}

	json wrapper = json::object();
	json threads_id(unique_threads);
	wrapper["threads_id"] = threads_id;
	wrapper["threads"] = threads;

	return wrapper;
}
