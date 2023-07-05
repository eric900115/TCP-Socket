#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <cstdio>
#include <netdb.h>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

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
};
*/
/*
int DNS(char* hostname, char* ip)
{
	struct hostent *lh = gethostbyname(hostname);

    if (lh)
        puts(inet_ntoa(*(struct in_addr *)lh->h_addr_list[0]));
    else
        herror("gethostbyname");

    return 0;
}*/

void handle_client(int streamfd)
{

}

int main (){	
	struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;	
	struct hostent *lh;
	int sockfd, streamfd, port, status; 
    socklen_t addr_size;	
	char str_buf[100];
	int process_id;		
	string id, email;
	bzero(&myaddr, sizeof(myaddr));	
	myaddr.sin_family = PF_INET;	
	myaddr.sin_port = htons(5000);	
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	
	sockfd = socket(PF_INET, SOCK_STREAM, 0);	
	if(sockfd == -1){
		printf("socket creation failed\n");
	}
	else{
		printf("socket creation success\n");
	}

	if(bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) != 0){
		printf("socket bind failed\n");
	}	
	else{
		printf("socket bind success\n");
	}

	if(listen(sockfd, 10) != 0){
		printf("listen failed\n");
	}
	else{
		printf("listen success\n");
	}

	addr_size = sizeof (client_addr);	

	while(1){
	    streamfd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);

		if(streamfd < 0){
			printf("server accept fail\n");
		}
		else{
			printf("client connect suceesfully\n");
		}

		string timeout_err_msg;
		int timeout_err_count;
        
		while(1){
			status = read(streamfd, str_buf, 100);	
			process_id = atoi(str_buf);

			cout << "str_buf " << str_buf << "\n";
			cout << "string from net: " << process_id << "\n";

			/*if(timeout_err_msg != str_buf){
				timeout_err_count = 0;
				timeout_err_msg = str_buf;
			}
			else{
				timeout_err_count++;
			}

			if(timeout_err_count == 2000){
				break;
			}*/

			if(process_id == 1){
				char ip_buf[100];
				char hostname[1000];
				status = read(streamfd, hostname, 100);
				lh = gethostbyname(hostname);
    			if (lh){
					strcpy(str_buf, inet_ntoa(*(struct in_addr *)lh->h_addr_list[0]));
        			printf("return ip address: %s\n", str_buf);
				}
				else{
					strcpy(str_buf, "No such URL");
					printf("URL does not exist\n");
        			herror("gethostbyname");
				}

				status = write(streamfd, str_buf, strlen(str_buf)+1);
			}
			else if(process_id == 2){
				string request_id;
				bool found = false;
				ifstream ifs("query.txt");
			
				status = read(streamfd, str_buf, 100);
				request_id = str_buf;

				while(ifs >> id >> email){
					if(id == request_id){
						strcpy(str_buf, email.c_str());
						found = true;
						break;
					}
				}

				if(found == false){
					email = "No such student ID";
					strcpy(str_buf, email.c_str());
				}

				printf("return message: %s\n", str_buf);

				status = write(streamfd, str_buf, strlen(str_buf)+1);
  				ifs.close();
			}
			else if(process_id == 3){
				printf("client quit\n");
				break;
			}
			else{
				continue;
			}
		}

		close(streamfd);
	}
	
    return 0;
}
