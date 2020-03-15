// Função: Processar determinado dado para seguir o padrão do AngleVelocity, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
float AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleVelocityValue = NegativeValuesTwoBytes(AngleVelocityValue);
	AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
}