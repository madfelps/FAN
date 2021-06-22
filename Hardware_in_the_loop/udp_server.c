
/**
 * Authors: Gaspar Henrique Martins, Felipe Madureira,Me.Henrique Garcia
 * Description:this code sends and receives between client and server using UDP protocol.This is the server code.
 * For more details, visit:https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code
 * Compile with: gcc -o executable main.c -lws2_32
 * MinGW is necessary.
 * */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <windows.h>
#include <winsock.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define PORT 8082 //Socket port
#define MAXLINE 1024 //Maximum data size in bytes

/*Link with ws2_32.lib*/
#pragma comment(lib, "Ws2_32.lib")

int main() { 
	int16_t proc_buffer;
	uint8_t proc_byte[2];
	
	int sockfd; 
	
	
	struct sockaddr_in servaddr, cliaddr,my_addr; 

	/*Initialization for to use winsock*/
	WSADATA wsaData;   
    WSAStartup(MAKEWORD(2,2),&wsaData);

	/*Criando descritor de arquivo de socket*/
	printf("Creating socket file descriptor...\n");
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET ) 
	{ 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	/* Preenchendo informações do servidor */
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_port = htons(PORT); 

	
	/* Ligando o soquete com o endereço do servidor */
	printf("Binding socket with server address...\n");
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("Bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	else printf("Bind successfully finished!\n Socket successfully created and ready for communication..\n");
	
	char buffer[MAXLINE]; 
	char message[2] ; 
	int cont_message = 0;
	int len, n;
	int i = 0;

	while(1)
	{

	/* Server waits for message from Client */
	if(cont_message == 0) printf("Waiting Client message...\n");
	
	len = sizeof(cliaddr); //len é value/result 
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0';
	cont_message = 1;

	/*Buffer data processing*/
	for(i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] < 0 )  proc_byte[i] = buffer[i] + 256;
		else proc_byte[i] = buffer[i];
	}
	proc_buffer = 256*proc_byte[1] + proc_byte[0];
	if(proc_buffer > 32767)proc_buffer = proc_buffer - 65536;

	/*Data debug*/
	printf("Bytes = %d \n",n);
	printf("Buffer[0] = %d\n", buffer[0]); 
	printf("Buffer[1] = %d\n", buffer[1]); 
	printf("Byte_proc[0] = %d\n", proc_byte[0]); 
	printf("Byte_proc[1] = %d\n", proc_byte[1]); 
	printf("proc_buffer = %d\n",proc_buffer);
	
	/*Message construction*/
	/** Here, you can add the message in the terminal for any purpose.Caution with the data size,
	 * because maybe cause errors in the data processing.
	 * Example:
	 * printf("Server envia:");
	 * fgets(message, MAXLINE, stdin); 
	 * */
	message[0] = message[0]+1;
	/*Always complete the last char array element with terminator '\0'*/
	message[1] = '\0';
	/* Server sends message to Client */
	sendto(sockfd, (const char *)message, strlen(message), 
		0, (const struct sockaddr *) &cliaddr, 
			len); 
	printf("Message = %d\n",message[0]);
	/*Sleep for to limit the sending speed.*/
	Sleep(1000);
	}
	close(sockfd);
	return 0; 
}