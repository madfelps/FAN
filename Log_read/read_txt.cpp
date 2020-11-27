#include "lerCAN.h"
using namespace std;

int main()
{

string auxstr;
int wordCounter = 0; //contador para thread 2
int flag = 0;
int frame1_can[10];
int frame2_can[6];

ofstream file_CAN_processed("CAN_processed.txt"); //txt processado a ser preenchido
ifstream file_CAN("can.txt");                     //txt da CAN

    while (file_CAN >> auxstr)
    {
        if(auxstr == "can1")
        {
             
            
        }
        else if(auxstr == "[4]") 
        {
            flag = 6;
            
            frame2_can[wordCounter] = stoi(auxstr, nullptr, 16);
        
        cout << frame2_can[wordCounter] << " ";
        wordCounter++;
        
        if (wordCounter == flag)
        {
            wordCounter = 0; //finaliza uma linha do txt
            cout << endl;
        }
        }
        else if(auxstr == "[8]") 
        {
            flag = 10;
           
            frame1_can[wordCounter] = stoi(auxstr, nullptr, 16);
        
        cout << frame1_can[wordCounter] << " ";
        wordCounter++;
        
        if (wordCounter == flag)
        {
            wordCounter = 0; //finaliza uma linha do txt
            cout << endl;
        }
        }
        }

    file_CAN.close();
    file_CAN_processed.close();

    return 0;
}