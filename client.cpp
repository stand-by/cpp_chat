#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>
using json = nlohmann::json;

string get_password();
bool login(string ip, string username, string password);

int main(int argc, char const *argv[]) {
    if(argc <= 1) {
      cout << "you should specify server ip as argument (for instance ./client 192.168.0.1)" << endl;
      return -1;
    }
    string ip = argv[1];
    string username; 
    string password; 

    cout << "username > ";
    cin >> username;
    cout << "password > ";
    password = get_password();

    if(!login(ip, username, password)) return -1;
    cout << "[*] " << "logged in or registered new" << endl;

    return 0;
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
    ClientSocket cli(ip, 8888);

    json req;
    req["username"] = username;
    req["password"] = password;
    req["command"] = "login";
    cli << req.dump();

    stringstream response;
    cli >> response;
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