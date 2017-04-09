#include "JsonWrapper.hpp"

JsonWrapper::JsonWrapper(string file): json_file(json::array()), filename(file) {};

JsonWrapper::~JsonWrapper() { write_json(); }

void JsonWrapper::read_json() {
	ifstream input;
	input.open(filename);

	if(input.is_open()) {
		input >> json_file;
		input.close();
	}
}

void JsonWrapper::write_json() {
	ofstream output(filename);

	output << setw(4) << json_file << endl;
	if(output.is_open()) 
		output.close();
}

void JsonWrapper::clear_json() {
	if (!json_file.empty())
		json_file.clear(); 
}

void JsonWrapper::print_json() {
	cout << json_file << endl;
}