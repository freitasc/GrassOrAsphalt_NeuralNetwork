#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

int percorre(char *binario){
	int valor = 0;
	for (int i = 0; i < 9 ; i++){
	if(binario[i] == '1') valor = valor + pow(2,8-i) ;
	}

	char guardavalor;
	guardavalor = binario[8];
	binario[8] = binario [7];
	binario[7] = binario [6];
	binario[6] = binario [5];
	binario[5] = binario [4];
	binario[4] = binario [3];
	binario[3] = binario [2];
	binario[2] = binario [1];
	binario[1] = binario [0];
	binario[0] = guardavalor;

	return valor;
}

int retornavalor(char *binario){

	int valor = 999;
	int recebe;
	for (int i = 0; i < 9; i++){

		recebe = percorre(binario);
		if (recebe < valor) valor = recebe;
	}

	return valor;
}

int *ilbp(char *caminho, double * vetor){

	int **mat;

	mat = (int**)malloc(1025*sizeof(int *));

	for(int i = 0; i < 1025; i++){
		mat[i] = (int*)malloc(1025*sizeof(int));
	}

	FILE *fp;

	fp = fopen(caminho, "r");


	for(int j=0; j < 1025; j++){
		for(int i=0; i < 1025; i++){
			fscanf(fp, "%d;", &mat[i][j]);
		}
	}

	double media;
	char binario[9];
	int vetorilbp[512] = {0};
	for(int i=0; i < 1025; i++){
		for(int j=0; j < 1025; j++){
			if(i != 0 && i != 1024 && j!= 0 &&  j != 1024){

				media = (mat[i][j]+mat[i][j+1]+mat[i][j-1]+mat[i+1][j]+mat[i+1][j+1]+mat[i+1][j-1]+mat[i-1][j]+mat[i-1][j+1]+mat[i-1][j-1])*1.0/9;



				if(media >= mat[i-1][j-1]) binario[0] = '1';
				else binario[0] = '0';
				if(media >= mat[i-1][j]) binario[1] = '1';
				else binario[1] = '0';
				if(media >= mat[i-1][j+1]) binario[2] = '1';
				else binario[2] = '0';
				if(media >= mat[i][j-1]) binario[3] = '1';
				else binario[3] = '0';
				if(media >= mat[i][j]) binario[4] = '1';
				else binario[4] = '0';
				if(media >= mat[i][j+1]) binario[5] = '1';
				else binario[5] = '0';
				if(media >= mat[i+1][j-1]) binario[6] = '1';
				else binario[6] = '0';
				if(media >= mat[i+1][j]) binario[7] = '1';
				else binario[7] = '0';
				if(media >= mat[i+1][j+1]) binario[8] = '1';
				else binario[8] = '0';

				int value;
				value = retornavalor(binario);
				vetorilbp[value]++;

			}
		}
	}


	for(int l = 0; l < 1025; l++){
		free(mat[l]);
	}

	for(int i = 0; i < 512; i++){
		vetor[i] = vetorilbp[i];
	}

}

int glcm(char *caminho, double *vetor){

	FILE *fp;

	fp = fopen(caminho, "r");

	int **mat;

	mat = (int**)malloc(1025*sizeof(int *));

	for(int i = 0; i < 1025; i++){
		mat[i] = (int*)malloc(1025*sizeof(int));
	}

	for(int i=0; i < 1025; i++){
		for(int j=0; j < 1025; j++){
			fscanf(fp, "%d;", &mat[i][j]);
		}
	}


	//norte
	int **n;
	n = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	n[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			n[i][j]=0;
		}
	}

	for(int i = 1; i < 1025;i++){
		for(int j = 0; j < 1025;j++){
			n[mat[i][j]][mat[i-1][j]]++;
		}
	}

	double contraste = 0;
	double energia = 0;
	double homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (n[i][j]*1.0 / 65536))  ;
			energia = energia + ((n[i][j]*1.0 / 65536)*(n[i][j]*1.0 / 65536));
			homo = homo + ((n[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}

	vetor[512] = contraste;
	vetor[513] = energia;
	vetor[514] = homo;



	//nordeste
	int **nl;
	nl = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	nl[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			nl[i][j]=0;
		}
	}

	for(int i = 1; i < 1025;i++){
		for(int j = 0; j < 1024;j++){
			nl[mat[i][j]][mat[i-1][j+1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (nl[i][j]*1.0 / 65536))  ;
			energia = energia + ((nl[i][j]*1.0 / 65536)*(nl[i][j]*1.0 / 65536));
			homo = homo + ((nl[i][j]*1.0 / (65536))/(1 + fabs(i - j)));
		}
	}

	vetor[515] = contraste;
	vetor[516] = energia;
	vetor[517] = homo;


	//leste
	int **l;
	l = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	l[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			l[i][j]=0;
		}
	}

	for(int i = 0; i < 1025;i++){
		for(int j = 0; j < 1024;j++){
			l[mat[i][j]][mat[i][j+1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (l[i][j]*1.0 / 65536))  ;
			energia = energia + ((l[i][j]*1.0 / 65536)*(l[i][j]*1.0 / 65536));
			homo = homo + ((l[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[518] = contraste;
	vetor[519] = energia;
	vetor[520] = homo;

	//sudeste
	int **sl;
	sl = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	sl[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			sl[i][j]=0;
		}
	}

	for(int i = 0; i < 1024;i++){
		for(int j = 0; j < 1024;j++){
			sl[mat[i][j]][mat[i+1][j+1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 1; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (sl[i][j]*1.0 / 65536))  ;
			energia = energia + ((sl[i][j]*1.0 / 65536)*(sl[i][j]*1.0 / 65536));
			homo = homo + ((sl[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[521] = contraste;
	vetor[522] = energia;
	vetor[523] = homo;

	//sul
	int **s;
	s = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	s[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			s[i][j]=0;
		}
	}

	for(int i = 0; i < 1024;i++){
		for(int j = 0; j < 1025;j++){
			s[mat[i][j]][mat[i+1][j]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (s[i][j]*1.0 / 65536))  ;
			energia = energia + ((s[i][j]*1.0 / 65536)*(s[i][j]*1.0 / 65536));
			homo = homo + ((s[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[524] = contraste;
	vetor[525] = energia;
	vetor[526] = homo;

	//sudoeste
	int **so;
	so = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	so[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			so[i][j]=0;
		}
	}

	for(int i = 0; i < 1024;i++){
		for(int j = 1; j < 1025;j++){
			so[mat[i][j]][mat[i+1][j-1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2)) * (so[i][j]*1.0 / 65536))  ;
			energia = energia + ((so[i][j]*1.0 / 65536)*(so[i][j]*1.0 / 65536));
			homo = homo + ((so[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[527] = contraste;
	vetor[528] = energia;
	vetor[529] = homo;

	//oeste
	int **o;
	o = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	o[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			o[i][j]=0;
		}
	}

	for(int i = 0; i < 1025;i++){
		for(int j = 1; j < 1025;j++){
			o[mat[i][j]][mat[i][j-1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( (i-j)*(i-j)) * (o[i][j]*1.0 / 65536))  ;
			energia = energia + ((o[i][j]*1.0 / 65536)*(o[i][j]*1.0 / 65536));
			homo = homo + ((o[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[530] = contraste;
	vetor[531] = energia;
	vetor[532] = homo;

	//noroeste
	int **no;
	no = (int**)malloc(256*sizeof(int *));
	for(int i = 0; i < 256; i++)	no[i] = (int*)malloc(256*sizeof(int));

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			no[i][j]=0;
		}
	}

	for(int i = 1; i < 1025;i++){
		for(int j = 1; j < 1025;j++){
			no[mat[i][j]][mat[i-1][j-1]]++;
		}
	}

	contraste = 0;
	energia = 0;
	homo = 0;

	for(int i = 0; i < 256;i++){
		for(int j = 0; j < 256;j++){
			contraste = contraste + (fabs( pow(i - j ,2.0)) * (no[i][j]*1.0 / 65536))  ;
			energia = energia + ((no[i][j]*1.0 / 65536)*(no[i][j]*1.0 / 65536));
			homo = homo + ((no[i][j]*1.0 / (256*256))/(1 + fabs(i - j)));
		}
	}
	vetor[533] = contraste;
	vetor[534] = energia;
	vetor[535] = homo;

	// desmalloca tudo
	for(int i = 0; i < 256; i++) free(n[i]);
	for(int i = 0; i < 256; i++) free(nl[i]);
	for(int i = 0; i < 256; i++) free(l[i]);
	for(int i = 0; i < 256; i++) free(sl[i]);
	for(int i = 0; i < 256; i++) free(s[i]);
	for(int i = 0; i < 256; i++) free(so[i]);
	for(int i = 0; i < 256; i++) free(o[i]);
	for(int i = 0; i < 256; i++) free(no[i]);

	for(int l = 0; l < 1025; l++){
		free(mat[l]);
	}

}


int main(int argc, char *argv[]){

    int *p;
    double vetoroculta[536];
	int r=0, n=0;
	int qO;
	qO = atoi(argv[0]);
	char caminho[50];
	double vetor[536] = {0};
	int total[50];
	int treinamento[25], teste[25];
    int acerto = 0, frejeicao = 0, faceitacao=0;
    int randomico;
    double vetorbE[536], vetorwE[536];
    double vetorbO[qO], vetorwO[qO];
    double bS, wS;
    double entradaparaoculta[536], ocultaparasaida[qO];
    double vetorAtivadoOculta[qO];
    double Noculta[qO];
    double Nsaida;
    double resultado;
    double salvaN;
    double erro;
    double vetorerrosA[25], vetorerrosG[25];
    double learningrate = 0.1;
    double gradienteSaida, gradienteOculta[qO], gradienteEntrada[536];
    double soma=0;
    double MSE=10000;
    double MSEaux=0;
    int contaepoca=0;

    //randomizando as imagens

    srand(time(NULL));
    //cria um array com todos os numeros de 1 a 50
    for(int i=0;i<50;i++){
        total[i] = i+1;
    }
    //randomiza 25 numeros do treinamento
    for(int i=0;i<25;i++){
        r = rand()%50;
        if(r==0){
            r++;
        }
        for(int j=0;j<25;j++){
            if(r == treinamento[j]){
                r++;
                j=0;
            }
        }
        treinamento[i] = r;
    }
    //salva os 25 restantes para serem de teste
    for(int i=0;i<25;i++){
        for(int j=0;j<50;j++){
            if(treinamento[i] == total[j]){
                total[j]=0;
            }
        }
    }
    for(int i=0;i<50;i++){
        if(total[i] != 0){
            teste[n] = total[i];
            n++;
        }
    }

    for(int i=0;i<535;i++){
        vetorwE[i] = (double)rand()/RAND_MAX*2.0-1.0;
        vetorbE[i] = (double)rand()/RAND_MAX*2.0-1.0;
    }

    for(int i=0;i<qO;i++){
        vetorwO[i] = (double)rand()/RAND_MAX*2.0-1.0;
        vetorbO[i] = (double)rand()/RAND_MAX*2.0-1.0;
    }

    wS= (double)rand()/RAND_MAX*2.0-1.0;
    bS= (double)rand()/RAND_MAX*2.0-1.0;


    for(int j=0;j<qO-1;j++){
            Noculta[j]=0;
    }
    Nsaida=0;

while(MSE>0.2 || contaepoca==1000){
//inicializa os treinamentos de asphalt e depois de grass
    printf(" Fazendo os treinamentos com Asphalt e Grass\n\n");
	for (int i = 0; i < 25  ; i++){
		if (treinamento[i] <= 9) snprintf(caminho, 50, "DataSet/asphalt/asphalt_0%d.txt", treinamento[i]);
		else snprintf(caminho, 50, "DataSet/asphalt/asphalt_%d.txt", treinamento[i]);


		printf("%s\n", caminho);

		ilbp(caminho, vetor);
		glcm(caminho, vetor);

		for(int j=0;j<535;j++){
		    salvaN=(vetor[j]*vetorwE[j])+vetorbE[j];
            entradaparaoculta[j] = 1/1+exp(-salvaN); //calcula as saidas dos neuronios da camada de entrada
		}
		for(int j=0;j<qO-1;j++){
		    for(int k=0;k<535;k++){
		        Noculta[j]=(entradaparaoculta[k]*vetorwO[j])+Noculta[j];
		    }
		    Noculta[j]=Noculta[j]+vetorbO[j];
            ocultaparasaida[j] = 1/1+exp(-Noculta[j]); //calcula as saidas dos neuronios da camada oculta
		}
		for(int j=0;j<qO;j++){
            Nsaida = (ocultaparasaida[j]*wS)+Nsaida;
		}
		Nsaida = Nsaida + bS;
		resultado = 1/1+exp(-Nsaida); //calcula a saida final do neuronio de saida

		erro = 0 - resultado; //calcula o erro, target de asfalto = 0
		vetorerrosA[i]=erro;

		//backpropagation
		gradienteSaida =(1/1+exp(-resultado))*erro;
		randomico = rand()%7;
		wS = wS+learningrate*ocultaparasaida[randomico]*gradienteSaida; //calcula o Gradiente, W e B da camada de saida
		bS = bS+learningrate*gradienteSaida;

		for(int j=0;j<qO-1;j++){
            gradienteOculta[j] =(1/1+exp(-ocultaparasaida[j]))*gradienteSaida*wS; //usa o Gradiente da camada de saida pra descobrir o Gradiente da camada oculta
		}
        for(int j=0;j<qO-1;j++){
            randomico = rand()%535;
            vetorwO[j] = vetorwO[j]+learningrate*entradaparaoculta[randomico]*gradienteOculta[j]; //calcula o W e o B de cada neuronio da camada oculta usando o gradiente local
            vetorbO[j] = vetorbO[j]+learningrate*gradienteOculta[j];
        }

        for(int j=0;j<qO-1;j++){
            soma = soma + (gradienteOculta[qO]*vetorwO[qO]);
        }

        for(int j=0;j<535;j++){
            gradienteEntrada[j] =(1/1+exp(-entradaparaoculta[j]))*(soma); //calcula o gradiente da camada de entrada fazendo a soma iniciando em 0
		}
        for(int j=0;j<535;j++){
            vetorwO[j] = vetorwO[j]+learningrate*vetor[j]*gradienteEntrada[j];
            vetorbO[j] = vetorbO[j]+learningrate*gradienteEntrada[j]; //calcula W e B de cada neuronio da camada de entrada usando o gradiente
        }




        for(int j=0;j<qO;j++){
            Noculta[j]=0;
        }
        Nsaida=0; //reseta variaveis usadas em somas
        soma=0;




        if (treinamento[i] <= 9) snprintf(caminho, 50, "DataSet/grass/grass_0%d.txt", treinamento[i]);
		else snprintf(caminho, 50, "DataSet/grass/grass_%d.txt", treinamento[i]);

		printf("%s\n", caminho);

		ilbp(caminho, vetor);
		glcm(caminho, vetor);

		for(int j=0;j<535;j++){
		    salvaN=(vetor[j]*vetorwE[j])+vetorbE[j];
            entradaparaoculta[j] = 1/1+exp(-salvaN); //calcula as saidas dos neuronios da camada de entrada
		}
		for(int j=0;j<qO-1;j++){
		    for(int k=0;k<535;k++){
		        Noculta[j]=(entradaparaoculta[k]*vetorwO[j])+Noculta[j];
		    }
		    Noculta[j]=Noculta[j]+vetorbO[j];
            ocultaparasaida[j] = 1/1+exp(-Noculta[j]); //calcula a saida final do neuronio de saida
		}
		for(int j=0;j<qO;j++){
            Nsaida = (ocultaparasaida[j]*wS)+Nsaida;
		}
		Nsaida = Nsaida + bS;
		resultado = 1/1+exp(-Nsaida); //calcula a saida final do neuronio de saida

		erro = 1 - resultado; //calcula o erro, target de grama = 1
		vetorerrosG[i]=erro;

		//backpropagation
		gradienteSaida =(1/1+exp(-resultado))*erro;
		randomico = rand()%7;
		wS = wS+learningrate*ocultaparasaida[randomico]*gradienteSaida; //calcula o Gradiente, W e B da camada de saida
		bS = bS+learningrate*gradienteSaida;

		for(int j=0;j<qO-1;j++){
            gradienteOculta[j] =(1/1+exp(-ocultaparasaida[j]))*gradienteSaida*wS; //usa o Gradiente da camada de saida pra descobrir o Gradiente da camada oculta
		}
        for(int j=0;j<qO-1;j++){
            randomico = rand()%535;
            vetorwO[j] = vetorwO[j]+learningrate*entradaparaoculta[randomico]*gradienteOculta[j]; //calcula o W e o B de cada neuronio da camada oculta usando o gradiente local
            vetorbO[j] = vetorbO[j]+learningrate*gradienteOculta[j];
        }

        for(int j=0;j<qO-1;j++){
            soma = soma + (gradienteOculta[qO]*vetorwO[qO]);
        }

        for(int j=0;j<535;j++){
            gradienteEntrada[j] =(1/1+exp(-entradaparaoculta[j]))*(soma); //calcula o gradiente da camada de entrada fazendo a soma iniciando em 0
		}
        for(int j=0;j<535;j++){
            vetorwO[j] = vetorwO[j]+learningrate*vetor[j]*gradienteEntrada[j];
            vetorbO[j] = vetorbO[j]+learningrate*gradienteEntrada[j]; //calcula W e B de cada neuronio da camada de entrada usando o gradiente
        }




        for(int j=0;j<qO;j++){
            Noculta[j]=0;
        }
        Nsaida=0; //reseta variaveis usadas em somas
        soma=0;
	}

	for(int j=0;j<24;j++){
        MSEaux=MSEaux+(vetorerrosA[j]*vetorerrosA[j]+vetorerrosG[j]*vetorerrosG[j]);
	}
	MSE=MSEaux*0.02; //calcula o MSE
	contaepoca++;
	printf("\n\nFIM DE EPOCA\n\n");
}

printf("\n\nFIM DO TREINO\n\n");

//inicializa o teste de asphalt
    printf("\n Inicializando os testes com Asphalt e Grass\n\n");
    for (int i = 0; i < 25  ; i++){
		if (teste[i] <= 9) snprintf(caminho, 50, "DataSet/asphalt/asphalt_0%d.txt", teste[i]);
		else snprintf(caminho, 50, "DataSet/asphalt/asphalt_%d.txt", teste[i]);

		printf("%s\n", caminho);

		ilbp(caminho, vetor);
		glcm(caminho, vetor);

		for(int j=0;j<535;j++){
		    salvaN=(vetor[j]*vetorwE[j])+vetorbE[j];
            entradaparaoculta[j] = 1/1+exp(-salvaN); //calcula as saidas da camada de entrada
		}
		for(int j=0;j<qO-1;j++){
		    for(int k=0;k<535;k++){
		        Noculta[j]=(entradaparaoculta[k]*vetorwO[j])+Noculta[j];
		    }
		    Noculta[j]=Noculta[j]+vetorbO[j];
            ocultaparasaida[j] = 1/1+exp(-Noculta[j]); //calcula as saidas da camada oculta
		}
		for(int j=0;j<qO;j++){
            Nsaida = (ocultaparasaida[j]*wS)+Nsaida;
		}
		Nsaida = Nsaida + bS;
		resultado = 1/1+exp(-Nsaida); //calcula a saida da camada de saida

		if(resultado<=0.5){
            acerto++;
		}
		else{
            faceitacao++; //marca acerto ou falsa aceitaco
		}



		for(int j=0;j<qO;j++){
            Noculta[j]=0;
        }
        Nsaida=0;
        soma=0;



		if (teste[i] <= 9) snprintf(caminho, 50, "DataSet/grass/grass_0%d.txt", teste[i]);
		else snprintf(caminho, 50, "DataSet/grass/grass_%d.txt", teste[i]);

		printf("%s\n", caminho);

		ilbp(caminho, vetor);
		glcm(caminho, vetor);

		for(int j=0;j<535;j++){
		    salvaN=(vetor[j]*vetorwE[j])+vetorbE[j];
            entradaparaoculta[j] = 1/1+exp(-salvaN); //calcula as saidas da camada de saida
		}
		for(int j=0;j<qO-1;j++){
		    for(int k=0;k<535;k++){
		        Noculta[j]=(entradaparaoculta[k]*vetorwO[j])+Noculta[j];
		    }
		    Noculta[j]=Noculta[j]+vetorbO[j];
            ocultaparasaida[j] = 1/1+exp(-Noculta[j]);  //calcula as saidas da camada oculta
		}
		for(int j=0;j<qO;j++){
            Nsaida = (ocultaparasaida[j]*wS)+Nsaida;
		}
		Nsaida = Nsaida + bS;
		resultado = 1/1+exp(-Nsaida); //calcula a saida da camada de saida

		if(resultado>0.5){
            acerto++;
		}
		else{
            frejeicao++; //marca acerto ou falsa rejeicao
		}


		for(int j=0;j<qO;j++){
            Noculta[j]=0;
        }
        Nsaida=0;
        soma=0;

    }


    printf("\n\nTaxa de acerto: %d%%\n", acerto*100/50);
    printf("Taxa de falsa aceitacao: %d%%\n", faceitacao*100/50);
    printf("Taxa de falsa rejeicao: %d%%\n", frejeicao*100/50); //printa as saidas

    return 0;
}
