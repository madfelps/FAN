


#include "udp_server.h"
/*Variaveis globais*/
int sockfd;
int len, n,N;
int cont_message = 0;
struct sockaddr_in servaddr,my_addr; 
/*Link with ws2_32.lib*/
#pragma comment(lib, "Ws2_32.lib")


void connect_socket(void)
{
	
	 /*Initialization for to use winsock*/
	 WSADATA wsaData;   
     WSAStartup(MAKEWORD(2,2),&wsaData);

	 /*Creating socket file descriptor*/
	 printf("Creating socket file descriptor...\n");
	 if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET ) 
	 { 
	 	perror("socket creation failed"); 
	 	exit(EXIT_FAILURE); 
	 } 
	
	 memset(&servaddr, 0, sizeof(servaddr)); 
	 memset(&my_addr, 0, sizeof(my_addr)); 
	
	 /*Fill server information*/
	 my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	 servaddr.sin_family = AF_INET; // IPv4 
	 servaddr.sin_port = htons(PORT); 

	 /*Bind socket with server address. */
	 printf("Binding socket with server address...\n");
	 if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	 { 
	 	perror("Bind failed"); 
	 	exit(EXIT_FAILURE); 
	 } 
	 else printf("Bind successfully finished!\n Socket successfully created and ready for communication..\n");
	
}
void set_num_signal(int num_signal)
{
	N = 2*num_signal;
}
 void receive_message(uint8_t buffer_[MAXLINE+1])
{
	uint8_t buffer[MAXLINE+1]; 
	
	int j = 0;
	/* Server waits for message from Client */
	if(cont_message == 0) printf("Waiting Client message...\n");
	
	len = sizeof(my_addr); 
	n = recvfrom(sockfd, buffer, sizeof(buffer), 0, ( struct sockaddr *) &my_addr, &len); 
	buffer[n] = '\0';
	cont_message = 1;
	
/*Passing the buffer to an n-byte array*/
	int i = 0;
    for(i = 0; i<n;i++)
    {
        buffer_[i] = buffer[i];
    }
	buffer_[n] = '\0';
}


int16_t get_int_16(uint8_t buf[19],u_int signal)
{
	u_int byte_pos_1,byte_pos_2;
	byte_pos_1 = 2*signal - 2;
	byte_pos_2 = byte_pos_1+1;
    u_int d_pos = 0;
	u_int iterator = 0;
    INT16UNION_t int_16;
	/*Pass the 2 bytes of the buffer determined by the desired positions to a 16-bit number*/
    for (iterator =  byte_pos_1;iterator < (byte_pos_2 + 1) ;iterator++)
	{
        int_16.bytes.byte[d_pos] = buf[iterator];
		d_pos++;
    } 
    return int_16.number;
}


void fill_message(uint8_t msg[19], int16_t num_[18] )
{
	int num_iterator = 0;
	INT16UNION_t int_1[9];
	
	for(num_iterator = 0; num_iterator < (N/2);num_iterator++)
	{
		int_1[num_iterator].number = num_[num_iterator];
	}
	/*Pass to the output buffer msg the pairs of bytes of each signal of the array num_*/
	/*Verificar se colocar um for não deixa mais lento*/
	//for(num_iterator = 0; num_iterator<MAXLINE;num_iterator++)
	msg[0] = int_1[0].bytes.byte[0];
	msg[1] = int_1[0].bytes.byte[1];
	msg[2] = int_1[1].bytes.byte[0];
	msg[3] = int_1[1].bytes.byte[1];
	msg[4] = int_1[2].bytes.byte[0];
	msg[5] = int_1[2].bytes.byte[1];
	msg[6] = int_1[3].bytes.byte[0];
	msg[7] = int_1[3].bytes.byte[1];
	msg[8] = int_1[4].bytes.byte[0];
	msg[9] = int_1[4].bytes.byte[1];
	msg[10] = int_1[5].bytes.byte[0];
	msg[11] = int_1[5].bytes.byte[1];
	msg[12] = int_1[6].bytes.byte[0];
	msg[13] = int_1[6].bytes.byte[1];
	msg[14] = int_1[7].bytes.byte[0];
	msg[15] = int_1[7].bytes.byte[1];
	msg[16] = int_1[8].bytes.byte[0];
	msg[17] = int_1[8].bytes.byte[1];	
	msg[N] = '\0';
	
}

void send_message(uint8_t message_[19])
{
	//message_[N] = '\0';
	int send_result;
	/*If you want to send string, use char array and strlen() instead of sizeof()*/
	send_result = sendto(sockfd, message_, N, 
	0, (const struct sockaddr *) &my_addr, len);
	
	
}




void close_socket(void)
{
	close(sockfd);
}