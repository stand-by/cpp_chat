#include "ServerDirector.hpp" 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

ServerDirector director(8888);

void save_handler(int s){
	printf("Saving stuff... %d\n",s);
	director.write_threads();
	director.write_userlist();
	exit(1); 
}

int main() { 
	struct sigaction sigint_handler;

	sigint_handler.sa_handler = save_handler;
	sigemptyset(&sigint_handler.sa_mask);
	sigint_handler.sa_flags = 0;

	sigaction(SIGINT, &sigint_handler, NULL);

	for(;;) director.handle_request(); 
}