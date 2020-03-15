// Função: Processar determinado dado para seguir o padrão do Angle, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
float Angle::ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleValue = NegativeValuesTwoBytes(AngleValue);
	AngleValue = AngleValue/10;
	return AngleValue;

		/*if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // TODO tratamento de erros (a desenvolver)
			throw 1;
		}*/
}