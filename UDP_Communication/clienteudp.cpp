// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <omp.h>

#define PORT	 8080 
#define MAXLINE 50 



// Driver code 
int main() { 
	omp_set_num_threads(2);
	int sockfd; 
	char msg[MAXLINE];
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(8080); 
	inet_aton("192.168.15.18" , &servaddr.sin_addr); 
	//servaddr.sin_addr.s_addr = INADDR_ANY;



	
	int n = 0; 
	socklen_t len = sizeof(servaddr);

	#pragma omp parallel sections
	{
		#pragma omp section //SEND
		{
			while(1){
				scanf(" %[^\n]s", buffer);
				printf("\n");
				sendto(sockfd, (const char *)buffer, strlen(buffer), 
					MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
					len); 
			}
		}

		#pragma omp section //READ
		{
			while(1){
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, (struct sockaddr *) &servaddr, 
					&len); 
				buffer[n] = '\0'; 
				printf("O valor de n eh: %d\n", n);
				printf("Server : %s\n", buffer); 
			}
		}
	}
	

	close(sockfd); 
	return 0; 
} 
