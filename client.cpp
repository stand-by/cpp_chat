#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>
using json = nlohmann::json;

string get_password();

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

    /*
    json credentials;
    credentials["username"] = argv[1];
    credentials["password"] = argv[2];
    credentials["command"] = "login";
    req["body"] = "oh shit, maaan!";
    req["thread"] = 1;
    req["id"] = 3;

    ClientSocket cli("127.0.0.1", 8888);
    cli << req.dump();

    stringstream stream;
    cli >> stream;
    cout << stream.str() << endl;
    */
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