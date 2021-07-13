/**
 * Authors: Gaspar Henrique Martins, Felipe Madureira,Me.Henrique Borges Garcia
 * Description:this code (for Windows) sends and receives numeric datas between client and server using UDP protocol.This is the server code.
 * For more details, visit:https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code
 * Compile with: gcc -o executable main.c -lws2_32
 * */
/**/



#ifndef UDP_SERVER
#define UDP_SERVER


#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <stdint.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <omp.h>


#define PORT 8082 //PORTA DO SOQUETE
#define MAXLINE 18 //TAMANHO MÁXIMO EM BYTES DA MENSAGEM 

/*Union to get the data byte*/
typedef union{
  int16_t number;
  struct {
    uint8_t byte[2];
  }bytes;

} INT16UNION_t;

/*Set the number of signals that will be transmitted*/
void set_num_signal(int num_signal);

/*Connect socket with IP and PORT defined by ip and port, respectively.
Example: 
connect_socket("127.0.0.1",8080);
*/
void connect_socket(void);

/*Receive the message buffer sended by client.
Example:
uint8_t UDP_BUFFER_IN[MAXLINE+1];
receive_message(UDP_BUFFER_IN);
*/
void receive_message( uint8_t  buffer_[MAXLINE+1]);

/*Returns a 16-bit integer from a uint8_t buffer of MAXLINE bytes . Specify which signal
 (2 bytes) from the buffer will be returned. Example:
uint8_t BUFFER[MAXLINE+1];
int value;
value = toInt16(BUFFER,1);
*/
int16_t get_int_16(uint8_t buf[MAXLINE+1],u_int signal); 

/*Fill the message to be sent by the socket.
Example:
int16_t data_vector[MAXLINE/2] = {1,2,3,4,5,6,7,8,9};
fill_message(UDP_DATA_OUT,data_vector);
*/
void fill_message(uint8_t msg[MAXLINE+1],   int16_t num_[MAXLINE/2]);

/*Send a message by socket.
Example:
receive_message(UDP_BUFFER_IN);
send_message(UDP_DATA_IN);
*/
void send_message(uint8_t message_[MAXLINE+1]);

/*Close socket*/
void close_socket(void);

#endif
