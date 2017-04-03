#include "UserList.hpp"

UserList::UserList(): userlist(json({})), filename("userlist.txt") {};

UserList::~UserList() {};

void UserList::read_json() {
	ifstream input(filename);
	input >> userlist;
	if(input.is_open()) input.close();
}

void UserList::print_json() {
	cout << userlist << endl;
}