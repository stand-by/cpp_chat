#ifndef JSONWRAPPER_HPP
#define JSONWRAPPER_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "../nlohmann_json/json.hpp"

using namespace std;
using namespace nlohmann;

class JsonWrapper {
protected:
	json json_file;
	string filename;

public:
	JsonWrapper(string name);
	~JsonWrapper();
	void read_json();
	void write_json();
	void clear_json();
	void print_json();
};

#endif