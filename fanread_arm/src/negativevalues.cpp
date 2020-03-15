/* Como 2^16 = 65536, dividimos pela metade, e, valores abaixo de 65536/2 = 32768 são convencionados como positivos, enquanto valores acima de 32768 (até 65536) são negativos.  */
/* Para entender a conversão, ver anexo 1 da documentação desse projeto.  */

// Função: Processar determinado dado para verificar se é positivo ou negativo
// Parâmetros: Valor a ser verificado e processado.
// Retorno: Valor processado (com correta indicação se é positivo ou negativo).
float NegativeValues::NegativeValuesTwoBytes(float Value){
		if(Value >= 32768){
			Value = Value - 65536;
		}
		else{
		}
		return Value;
}