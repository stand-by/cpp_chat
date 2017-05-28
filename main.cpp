#include "ServerDirector.hpp" 
int main() { for(ServerDirector director(8888);;) director.handle_request(); }