// Função: Processar determinado dado para seguir o padrão do Torque, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
float Torque::ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10;
	return TorqueValue;
}