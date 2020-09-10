#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

int binario_para_decimal_string(char *bin) {
    int dec = 0;
    int i = 0;
    int tam = strlen(bin); 

    while(tam--) {
        if(bin[tam] == '0' || bin[tam] == '1') {
            dec = dec + pow(2, i++) * (bin[tam] - '0');
        }
    }
    
	return dec;
}

bool verificar_recorrencia(int *vetor, int num, int tamanho) {
	
	for(int i = 0; i < tamanho; i++) {
		if(vetor[i] == num) {
			return true;
		}
	}

	return false;
}

void printar_tabela_binario(int **tabela, int linhas, int colunas, int *vetor_binario, int tam_vetor_bin) {
	for(int i = 0; i < linhas; i++) {
		for(int j = 0; j < colunas; j++) {
			int num = tabela[i][j];

			int k;
			for(k = 0; num > 0; k++) {
				vetor_binario[k] = num % 2;
				num /= 2;
			}

			for(k = tam_vetor_bin - 1; k >= 0; k--) {
				printf("%d", vetor_binario[k]);
				vetor_binario[k] = 0;
			}
			printf("|");
		}
		printf("\n");
	}
}

int main() {
	int bits_endereco_virtual, tamanho_pagina, tamanho_processo, numero_paginas_do_processo, numero_quadros, endereco_virtual;
	double espaco_endereco_virtual, n_bits_D, n_bits_P;
	int *endereco_virtual_binario, *endereco_fisico_binario;
	char *endereco_fisico_p;

	int **matriz_mapeamento, *vetor_recorrencia, *vetor_binario;

	printf("Quantidade de bits do endereco virtual: ");
	scanf("%d", &bits_endereco_virtual);	//Número de bits para representar o endereço virtual
	printf("Tamanho da pagina (deve ser uma potencia de 2): ");
	scanf("%d", &tamanho_pagina);			//Tamanho total da página
	printf("Tamanho do processo: ");
	scanf("%d", &tamanho_processo);			//Tamanho do Processo

	numero_paginas_do_processo = tamanho_processo / tamanho_pagina;
	if(tamanho_processo % tamanho_pagina != 0) {
		numero_paginas_do_processo++;
	}

	espaco_endereco_virtual = pow(2, bits_endereco_virtual);
	numero_quadros = espaco_endereco_virtual / tamanho_pagina;

	printf("Numero de paginas: %d\n", numero_paginas_do_processo);
	printf("Numero de quadros: %d\n", numero_quadros);

	n_bits_D = log2(tamanho_pagina);	//Número de bits para representar o D
	n_bits_P = bits_endereco_virtual - n_bits_D;	//Número de bits para representar o P

	endereco_virtual_binario = (int *) malloc(bits_endereco_virtual * sizeof(int));		//Aloca memória para os binários virtual e físico
	endereco_fisico_binario = (int *) malloc(bits_endereco_virtual * sizeof(int));

	vetor_binario = (int *) calloc(n_bits_P, sizeof(int));			//Aloca memória para o vetor para impressão de binários da tabela

	endereco_fisico_p = (char *) malloc(n_bits_P * sizeof(char));	//Aloca memória para o P do endereço físico (string)

	vetor_recorrencia = (int *) malloc(numero_paginas_do_processo * sizeof(int));	//Aloca memória para o vetor que verifica a recorrência dos números random
	for(int i = 0; i < numero_paginas_do_processo; i++) {		//Seta o vetor de recorrência com valores negativos
		vetor_recorrencia[i] = -1;
	}

	matriz_mapeamento = (int **) malloc(numero_paginas_do_processo * sizeof(int *));	//Cria a matriz de mapeamento
	for(int i = 0; i < numero_paginas_do_processo; i++) {
		matriz_mapeamento[i] = (int *) malloc(2 * sizeof(int));
	}

	for(int i = 0; i < numero_paginas_do_processo; i++) {		//Setando a matriz de mapeamento com os valores aleatórios
		int random;

		do {
			random = rand() % numero_quadros;
		} while(verificar_recorrencia(vetor_recorrencia, random, numero_paginas_do_processo));

		matriz_mapeamento[i][0] = i;
		matriz_mapeamento[i][1] = random;
	}

	while(true) {
		printf("Endereco virtual: ");
		scanf("%d", &endereco_virtual);		//Lê o endereço virtual

		if(endereco_virtual < 0) {
			break;
		}

		printar_tabela_binario(matriz_mapeamento, numero_paginas_do_processo, 2, vetor_binario, n_bits_P);	//Printa a tabela de mapeamento

		int i = 0;
		if(endereco_virtual == 0) {				//Passa o endereço virtual para binário
			for(i = 0; i < bits_endereco_virtual; i++) {
				endereco_virtual_binario[i] = 0;
			}
		}
		else if(endereco_virtual == 1) {
			endereco_virtual_binario[0] = 1;
			for(i = 1; i < bits_endereco_virtual; i++) {
				endereco_virtual_binario[i] = 0;
			}
		}
		else {
			for(i = 0; endereco_virtual > 0; i++) {
				endereco_virtual_binario[i] = endereco_virtual % 2;
				endereco_virtual /= 2;
			}
		}

		int j;
		int k = 0;
		printf("Paginacao: %d %d %d\n", bits_endereco_virtual, n_bits_P, n_bits_D);
		for(j = bits_endereco_virtual - 1; j >= bits_endereco_virtual - n_bits_D - 1; j--) {	//Printa a parte P do binário do endereço virtual
			printf("%d", endereco_virtual_binario[j]);

			if(endereco_virtual_binario[j] == 1) {
				endereco_fisico_p[k] = '1';
			}
			else {
				endereco_fisico_p[k] = '0';
			}
			k++;
		}

		printf("\nDeslocamento: ");			//Printa a parte D do binário do endereço virtual
		for(j = j; j >= 0; j--) {
			printf("%d", endereco_virtual_binario[j]);
		}
		printf("\n");

		int p1 = binario_para_decimal_string(endereco_fisico_p);	//Passa a parte P do binário do endereço virtual para decimal
		int p2 = matriz_mapeamento[p1][1];		//Pega o respectivo número na tabela de mapeamento
		
		k = 0;														//Passando o D do endereço virtual para o D do endereço físico (são iguais)
		endereco_fisico_binario[k] = endereco_virtual_binario[k];
		k++;
		endereco_fisico_binario[k] = endereco_virtual_binario[k];
		k++;

		if(p2 == 0) {		//Passando o P do endereço fisico para o binário do endereço fisico completo (de trás pra frente)
			for(i = k; i < bits_endereco_virtual; i++) {
				endereco_fisico_binario[i] = 0;
			}
		}
		else if(p2 == 1) {
			endereco_fisico_binario[k] = 1;
			for(i = k + 1; i < bits_endereco_virtual; i++) {
				endereco_fisico_binario[i] = 0;
			}
		}
		else {
			for(i = k; p2 > 0; i++) {
				endereco_fisico_binario[i] = p2 % 2;
				p2 /= 2;
			}
		}

		printf("Endereco Fisico: ");	//Printando o endereço físico
		for(i = i - 1; i >= 0; i--) {
			printf("%d", endereco_fisico_binario[i]);
			endereco_fisico_binario[i] = 0;
		}

		printf("\n");
	}

	free(endereco_virtual_binario);
	free(endereco_fisico_binario);

	free(vetor_binario);

	free(endereco_fisico_p);

	free(vetor_recorrencia);

	for(int i = 0; i < numero_paginas_do_processo; i++) {
		free(matriz_mapeamento[i]);
	}
	free(matriz_mapeamento);

	return 0;
}