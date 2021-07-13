#include "udp_server.h"

#define PORT 8082
#define KP 10
#define DATA_MULTIPLIER 100 


int main()
{
    uint8_t UDP_BUFFER_IN[19];
    uint8_t UDP_DATA_OUT[19];
    int16_t data_vector[9];
    float can_frame[9];
    int j =0;
    connect_socket();

    while(1)

    {
        
        set_num_signal(5);
        receive_message(UDP_BUFFER_IN);
        /*converte o buffer para um int16. O dado chega Kint vezes maior que o real.*/
        for(j = 0; j<9;j++)
        {
            can_frame[j] = get_int_16(UDP_BUFFER_IN,j+1);
            data_vector[j] = (int16_t)can_frame[j];
        }
        
        //data[0] = get_int_16(UDP_BUFFER_IN,1);
        //data_vector[0] = (int16_t)data[0];
        /*Passagem dos int16's para um buffer que será enviado pelo UDP*/
        fill_message(UDP_DATA_OUT,data_vector);
        send_message(UDP_DATA_OUT);
        
      
    }
    
    close_socket();
    return 0; 
}