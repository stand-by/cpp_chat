#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann_json/json.hpp"

using namespace std;
using namespace nlohmann;
//TO-DO: remove iostream

class UserList {
private:
	json userlist;
	string filename;
	//TO-DO: it looks like we don't need members above
	//ifstream userlist_in;
	//ofstream userlist_out;

	//TO-DO: may be add some private method which returns username object out from whole list!!!
public:
	UserList();
	~UserList();
	void read_json();
	void write_json();
	void clear_json();
	//method for debug purposes
	void print_json();
	void add_user(string username, string password);
	void add_thread(string username, int thread);
	bool check_existance(string username);
	bool check_thread(string username, int thread);
};