#include "UserList.hpp"

UserList::UserList(): userlist(json({})), filename("userlist.txt") {};

UserList::~UserList() {};

void UserList::read_json() {
	ifstream input(filename);
	input >> userlist;
	if(input.is_open()) input.close();
}

void UserList::write_json() {
	ofstream output(filename);
	output << setw(4) << userlist << endl;
	if(output.is_open()) output.close();
}

void UserList::clear_json() { 
	if (!userlist.empty())
		userlist.clear(); 
}

void UserList::print_json() {
	cout << userlist << endl;
}