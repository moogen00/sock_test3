#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/un.h>

// 11111111111111
// 3333333333333
#define TEST

#ifndef TEST
char *socket_path = "\0hidden";
#else
char *socket_path = "/tmp/telematics_service_tcp_server";
#endif

int main (int argc, char *argv[]) {
	int cSocket;
	struct sockaddr_un addr;
	//char messages[1000] = "--------------------------------------------------------------";
	char messages[1000];
	char serverreply[2000];

	if((cSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("socket error");
	}
	printf("Socket create");

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

	if(connect(cSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0 ) {
		perror("connection error");
		exit(EXIT_FAILURE);
	}

#ifndef TEST
	if(recv(cSocket, serverreply, 2000, 0) < 0) {
		printf("recv failed");
	}
#endif

	printf("Connected\n");


	while(1) {
#if 1//ndef TEST
		printf("enter");
		scanf("%s", messages);
		// 한번 send 하면 해당 내용이 전달된다.
		// 긴 data 를 보내려면 protocol 재정의 필요.
		// data length 에 대한 정보를 같이 보내야된다.
		if(send (cSocket, messages, strlen(messages), 0) <0) {
			printf("sending fail");
			exit(EXIT_FAILURE);
		}

		sleep(3);
#endif

#if 0//ndef TEST
		if(recv(cSocket, serverreply, 2000, 0) < 0) {
			printf("recv failed");
			break;
		}

		printf("serv message\n");
		printf("serv message =  %s\n", serverreply);
		//puts(serverreply);
		sleep(3);
#endif

	}
	close(cSocket);
	return 0;
}

