#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include <tuple>
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>
using json = nlohmann::json;

string get_password();
bool login(string ip, string username, string password);
json get_thread(int thread_id, string ip, string username, string password);
tuple<int,string> parse_json_with_messages(json data);

int main(int argc, char const *argv[]) {
    system("clear");
    if(argc <= 1) {
      cout << "you should specify server ip as argument (for instance ./client 192.168.0.1)" << endl;
      return -1;
    }
    string ip = argv[1];
    string username; 
    string password; 

    //handle error inputs and responses with status 'error'

    cout << "username > ";
    cin >> username;
    cout << "password > ";
    password = get_password();

    if(!login(ip, username, password)) return -1;
    cout << "[*] " << "logged in or registered new" << endl;

    int thread_id;
    cout << "choose thread to join > ";
    cin >> thread_id;
    cout << thread_id << endl;

    system("clear");
    int last_msg_id = -1;
    string msg_stream;
    json threads = get_thread(thread_id, ip, username, password)["body"];
    tie(last_msg_id, msg_stream) = parse_json_with_messages(threads);

    return 0;
}

tuple<int,string> parse_json_with_messages(json data) {
    int last_msg_id = -1;
    stringstream stream;
    for(json::iterator it = data.begin(); it != data.end(); ++it) {
        stream << (*it)["datetime"]  << " [" << (*it)["username"] << "] " << "> " << (*it)["body"] << endl;
        last_msg_id = (*it)["id"];
    }
    return make_tuple(last_msg_id,stream.str());
}

json get_thread(int thread_id, string ip, string username, string password) {
    ClientSocket socket(ip, 8888);

    json req;
    req["username"] = username;
    req["password"] = password;
    req["command"] = "get_thread";
    req["thread"] = thread_id;
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    return resp;
}

string get_password() {
    string password;
    termios oldt;

    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cin >> password;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    return password;
}

bool login(string ip, string username, string password) {
    ClientSocket socket(ip, 8888);

    json req;
    req["username"] = username;
    req["password"] = password;
    req["command"] = "login";
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());

    if(resp["response"] == "ok") {
        for(json::iterator it = resp["body"]["threads_id"].begin(); it != resp["body"]["threads_id"].end(); ++it) {
            string str = to_string((*it).get<int>());
            cout << "Users for thread #" << str << ": ";// << resp["body"]["threads"][str] << endl;
            for(json::iterator jt = resp["body"]["threads"][str].begin(); jt != resp["body"]["threads"][str].end(); ++jt)
                cout << (*jt).get<string>() << ", ";
            cout << endl;
        }
        return true;
    } else {
        cout << "[*] " << resp["body"].get<string>() << endl;
        return false;
    }
}