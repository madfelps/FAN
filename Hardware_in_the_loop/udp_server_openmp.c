
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <windows.h>
#include <winsock.h>

#define PORT	 8080 
#define MAXLINE 1024 


// Driver code 
int main() { 


	int sockfd; 
	char buffer[MAXLINE]; 
	char message[MAXLINE] ; 
	struct sockaddr_in servaddr, cliaddr,my_addr; 
	
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(2, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	my_addr.sin_addr.s_addr = inet_addr("10.0.0.106");
	servaddr.sin_family = 2; // IPv4 
	servaddr.sin_port = htons(PORT); 

	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len, n;
			
			printf("\nServer:");
			//scanf(" %[^\n]s", message);
			fgets(message, MAXLINE, stdin); 
			
			sendto(sockfd, (const char *)message, strlen(message), 
				MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
					len); 
			
			len = sizeof(cliaddr); //len is value/resuslt 
			n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
						MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
						&len); 
			buffer[n] = '\0'; 

			printf("\nClient : %s\n", buffer);
			
			
	
	return 0; 
}