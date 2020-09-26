// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <omp.h>

#define MAXLINE 50 

// Driver code 
int main() { 
	omp_set_num_threads(2);
	int sockfd; 
	char msg[MAXLINE];
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	inet_aton("192.168.15.18" , &servaddr.sin_addr); 
	//servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(8080); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int n = 0; 

	socklen_t len;

	len = sizeof(cliaddr); //len is value/resuslt 

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			while(1){	//SEND
				scanf(" %[^\n]s", msg);
				printf("\n");
				sendto(sockfd, (const char *)msg, strlen(msg), 
					MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
					len); 
			}
		}

		#pragma omp section
		{
			while(1){ //READ
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
					&len); 
				buffer[n] = '\0'; 
				printf("Client : %s\n", buffer); 
				
			}
		}

		
	}

	close(sockfd);
	return 0; 
} 
