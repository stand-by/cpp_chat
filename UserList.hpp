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
	ifstream userlist_in;
	ofstream userlist_out;

public:
	UserList();
	~UserList();
};