#include <iostream>
#include <string>
#include <fstream>
#include <linux/can.h>


typedef struct can_frame CANframe;

void printCANframe(CANframe* frame)
{
	std::cout << "id: " << frame->can_id << std::endl;

	for(int i = 0; i < 8; i++)
	{
		std::cout << "[" << i << "]: " << (int)frame->data[i] << std::endl;
	}
}

int main()
{
	CANframe meuFrame;

	std::ifstream listaCAN("listaCAN.txt");
	std::string auxStr;

	bool endOfFile = false;

	int wordCounter = 0;

	while(listaCAN >> auxStr)
	{
		if(wordCounter == 0)
		{
			meuFrame.can_id = stoi(auxStr, 0, 16);
		}
		else
		{
			meuFrame.data[wordCounter-1] = stoi(auxStr, 0, 16);
		}

		wordCounter++;

		if(wordCounter == 9)
		{
			printCANframe(&meuFrame);
			wordCounter = 0;
		}
	}

}