#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include "ClientDirector.hpp" 
#include <tuple>
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>
using json = nlohmann::json;

string get_password();

int main(int argc, char const *argv[]) {
    system("clear");
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

    ClientDirector director(ip, username, password);

    if(director.login() == false) exit(0);

    string str;
    int thread_id;
    cout << "choose thread to join > ";
    cin >> str;
    thread_id = std::stoi(str);

    system("clear");
    if(director.display_thread(thread_id) == false) exit(1);
 
    for(;;) {
        string msg;
        cout << username << " > ";
        getline(cin, msg);
        if(cin.bad() || cin.eof()) {
            cout << "IO Error" << endl;
            exit(1);
        }
 
        if(msg != "/update") director.send_message(msg);
        director.refresh_messages();

        system("clear");
        msg.clear();
        director.print_msg_stream();
    }

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
