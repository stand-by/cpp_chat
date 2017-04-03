#include <iostream>
#include <fstream>
#include <string>
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

public:
	UserList();
	~UserList();
	void read_json();
	//method for debug purposes
	void print_json();
};