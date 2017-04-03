#include "UserList.hpp"

UserList::UserList(): userlist(json::array()), filename("userlist.txt") {};

UserList::~UserList() {};

void UserList::read_json() {
	ifstream input;
	input.open(filename);

	if(input.is_open()) {
		input >> userlist;
		input.close();
	}
}

void UserList::write_json() {
	ofstream output(filename);

	output << setw(4) << userlist << endl;

	if(output.is_open()) 
		output.close();
}

void UserList::clear_json() { 
	if (!userlist.empty())
		userlist.clear(); 
}

void UserList::print_json() {
	cout << userlist << endl;
}

void UserList::add_user(string username, string password) {
	if(!check_existance(username)) {
		json new_user = json({});
		new_user["username"] = username;
		new_user["password"] = password;
		new_user["threads"] = json::array();

		userlist.push_back(new_user);
	}
}

void UserList::add_thread(string username, int thread) {
	if(check_existance(username) && !check_thread(username, thread)) {
		for (json::iterator it = userlist.begin(); it != userlist.end(); ++it) {
  			if((*it)["username"]==username) 
				(*it)["threads"].push_back(thread);
		}
	}
}

bool UserList::check_existance(string username) {
	bool is_exist = false;

	for (json::iterator it = userlist.begin(); it != userlist.end(); ++it) {
  		if((*it)["username"]==username) {
  			is_exist = true;
  			break;
		}
  	}

  	return is_exist;
}

bool UserList::check_thread(string username, int thread) {
	bool is_exist = false;

	for (json::iterator it = userlist.begin(); it != userlist.end(); ++it) {
  		if((*it)["username"]==username) {
  			vector<int> threads =  (*it)["threads"].get<std::vector<int> >();
  			if(std::find(threads.begin(), threads.end(), thread) != threads.end()) {
  				is_exist = true;
  				break;
  			}
		}
  	}

  	return is_exist;
}