#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <signal.h>

using namespace std;

// packet info
/*
struct in_addr{
  	in_addr_t s_addr;
}

struct sockaddr_in {
     uint8_t sin_len;
     sa_family_t   sin_family;
     in_port_t sin_port;
     struct  in_addr sin_addr;
     char    sin_zero[8];
};*/

/*int sock_id;

void signal_callback_handler(int signum) {
   //cout << "Caught signal " << signum << endl;
   // Terminate program
	int status;
	char str_request[100];
	std::sprintf(str_request, "3");
   	status = write(sock_id, str_request, strlen(str_request)+1);
	close(sock_id);
   	exit(signum);
}*/

string err_msg;

void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    printf("\nCannot be terminated using Ctrl+C \n");
	printf("please use 3.QUIT to abort\n\n");
	cout << err_msg;
    fflush(stdout);
}


void Server_Request_Procedure(int sockfd)
{
	int status;
	char str_buf[100];
	char str_request[100];
	int request;
	string req_buf;
	//sock_id = sockfd;
    signal(SIGINT, sigintHandler);

	while(1){
		
		cout << "What's your requirement? 1.DNS 2.QUERY 3.QUIT ";
		err_msg = "What's your requirement? 1.DNS 2.QUERY 3.QUIT ";
		cin >> req_buf;

		if((req_buf != "1") && (req_buf != "2") && (req_buf != "3")){
			cout << "please enter 1~3\n";
			continue;
		}	
		else{
			request = stoi(req_buf);
		}

		std::sprintf(str_request, "%d", request);
		status = write(sockfd, str_request, strlen(str_request)+1);
	    
		if(request == 1){
			printf("Input URL address :");
			err_msg = "Input URL address :";
			scanf("%s", str_buf);
			status = write(sockfd, str_buf, strlen(str_buf)+1);
			status = read(sockfd, str_buf, 100);	
			printf("address get from domain name: %s\n\n", str_buf);
		}
		else if(request == 2){
			printf("Input Student ID :");
			err_msg = "Input Student ID :";
			scanf("%s", str_buf);
			status = write(sockfd, str_buf, strlen(str_buf)+1);
			status = read(sockfd, str_buf, 100);	
			printf("Email get from server : %s\n\n", str_buf);
		}
		else if(request == 3){
			break;
		}
    }
}

int main (){	
	
	struct sockaddr_in server_addr;	
	int sockfd, status;	

    //setup the server address	
	server_addr.sin_family = PF_INET;	
	server_addr.sin_port = htons(5000);	
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	

	//connect to the server	
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("socket create failed\n");
	}		
	if(connect (sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) != 0){
		printf("connection with the server failed\n");
	}
    
	Server_Request_Procedure(sockfd);

	close(sockfd);
		
	return 0;
}
