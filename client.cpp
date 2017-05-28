#include "nlohmann_json/json.hpp"
#include "SocketClasses/ClientSocket.hpp"
#include <tuple>
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>
using json = nlohmann::json;

string get_password();
int login(string ip, string username, string password);
json get_thread(int thread_id, string ip, int session_id);
tuple<int,string> parse_json_with_messages(json data);
void send_msg(int thread_id, string ip, string msg, int session_id);
json refresh_messages(int thread_id, string ip, int last_msg_id, int session_id);

int main(int argc, char const *argv[]) {
    system("clear");
    if(argc <= 1) {
      cout << "you should specify server ip as argument (for instance ./client 192.168.0.1)" << endl;
      return -1;
    }
    string ip = argv[1];
    string username; 
    string password;
    int session_id; 

    //handle error inputs and responses with status 'error'

    cout << "username > ";
    cin >> username;
    cout << "password > ";
    password = get_password();

    session_id = login(ip, username, password);
    if(session_id < 0) return -1;
    cout << "[*] " << "logged in or registered new" << endl;

    string str;
    int thread_id;
    cout << "choose thread to join > ";
    cin >> str;
    thread_id = std::stoi(str);
    cout << thread_id << endl;

    system("clear");
    int last_msg_id = -1;
    string msg_stream;
    json threads = get_thread(thread_id, ip, session_id)["body"];
    tie(last_msg_id, msg_stream) = parse_json_with_messages(threads);

    for(;;) {
        string msg;
        cout << username << " > ";
        getline(cin, msg);

        if(msg != "/update") send_msg(thread_id, ip, msg, session_id);
        json messages = refresh_messages(thread_id, ip, last_msg_id, session_id)["body"];
        string temp;
        tie(last_msg_id, temp) = parse_json_with_messages(messages);
        msg_stream += temp;

        system("clear");
        cout << msg_stream;
        msg.clear();
    }

    return 0;
}

json refresh_messages(int thread_id, string ip, int last_msg_id, int session_id) {
    json req;
    req["session_id"] = session_id;
    req["command"] = "refresh";
    req["thread"] = thread_id;
    req["id"] = last_msg_id;

    ClientSocket socket(ip, 8888);
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());
    return resp;
}

void send_msg(int thread_id, string ip, string msg, int session_id) {
    json req;
    req["session_id"] = session_id;
    req["command"] = "write";
    req["thread"] = thread_id;
    req["body"] = msg;

    ClientSocket socket(ip, 8888);
    socket << req.dump();

    stringstream response;
    socket >> response;
    json resp = json::parse(response.str());
    //cout << resp << endl;
}

tuple<int,string> parse_json_with_messages(json data) {
    int last_msg_id = -1;
    stringstream stream;
    for(json::iterator it = data.begin(); it != data.end(); ++it) {
        stream << (*it)["datetime"].get<string>()  << " [" << (*it)["username"].get<string>() << "] " << "> " << (*it)["body"].get<string>() << endl;
        last_msg_id = (*it)["id"];
    }
    return make_tuple(last_msg_id,stream.str());
}

json get_thread(int thread_id, string ip, int session_id) {
    ClientSocket socket(ip, 8888);

    json req;
    req["session_id"] = session_id;
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

int login(string ip, string username, string password) {
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
        return resp["session_id"].get<int>();
    } else {
        cout << "[*] " << resp["body"].get<string>() << endl;
        return -1;
    }
}