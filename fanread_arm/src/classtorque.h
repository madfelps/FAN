/* Em grandezas compostas de dois bytes temos que o valor referido é a soma do valor do byte menos significativo (LSB) com o byte mais significativo (MSB) multiplicado por 256. (Ver Manual)  */

class Torque:public NegativeValues{
private:

public:
	static float ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);

};