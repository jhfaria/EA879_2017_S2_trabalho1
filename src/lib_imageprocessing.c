#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "imageprocessing.h"
#include <pthread.h>
#include <FreeImage.h>

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo)
{
	FIBITMAP *bitmapIn;
	int x, y;
	RGBQUAD color;
	imagem I;

	bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

	if (bitmapIn == 0)
	{
		printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
	}
	else
	{
	    printf("Arquivo lido corretamente!\n");
	}

	x = FreeImage_GetWidth(bitmapIn);
	y = FreeImage_GetHeight(bitmapIn);

	I.width = x;
	I.height = y;

	I.r = malloc(sizeof(float) * x * y);
	I.g = malloc(sizeof(float) * x * y);
	I.b = malloc(sizeof(float) * x * y);

	for (int i=0; i<x; i++)
	{
	    for (int j=0; j <y; j++)
	    {
		    int idx;
	    	FreeImage_GetPixelColor(bitmapIn, i, j, &color);

	        idx = i + (j*x);

	        I.r[idx] = color.rgbRed;
	        I.g[idx] = color.rgbGreen;
	        I.b[idx] = color.rgbBlue;
	    }
	}

	return I;
}

void liberar_imagem(imagem *I)
{
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I)
{
 	FIBITMAP *bitmapOut;
  	RGBQUAD color;

  	printf("Salvando imagem %d por %d...\n", I->width, I->height);

  	bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   	for (int i=0; i<I->width; i++)
   	{
    	for (int j=0; j<I->height; j++)
    	{
      		int idx;


      		idx = i + (j*I->width);
      		color.rgbRed = I->r[idx];
      		color.rgbGreen = I->g[idx];
      		color.rgbBlue = I->b[idx];

      		FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    	}
  	}

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

/* função para alterar o brilho da imagem */
void altera_brilho(imagem *I, float valor_ganho)
{
	// contagem do tempo de execucao
	struct timeval t1, t2;
	double deltat;
	//checa o tempo no inicio
	gettimeofday(&t1, NULL);

	/* laço para alcançar todos os pixels da imagem */
	for (int i=0; i<I->width; i++)
	{
     	for (int j=0; j<I->height; j++)
     	{
	      	int idx;

	      	idx = i + (j*I->width);

		    /* para a parte vermelha do pixel */
			if ((I->r[idx] * valor_ganho) <= 255) {I->r[idx] = (I->r[idx] * valor_ganho);}
		    else {I->r[idx] = 255;}

		    /* para a parte verde do pixel */
			if ((I->g[idx] * valor_ganho) <= 255) {I->g[idx] = (I->g[idx] * valor_ganho);}
		    else {I->g[idx] = 255;}

		    /* para a parte azul do pixel */
			if ((I->b[idx] * valor_ganho) <= 255) {I->b[idx] = (I->b[idx] * valor_ganho);}
		    else {I->b[idx] = 255;}
	   	}
  	}
		//checa o tempo no final
		gettimeofday(&t2, NULL);
		//calcula o tempo transcorrido
		deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		//imprime o tempo de execucao
		printf("Tempo de execucao %f ms\n", deltat );

	return;
}
/* função que imprime o valor maximo dos pixels da imagem */
void valor_maximo(imagem *I)
{
	float valor_maximo = 0;

	/* laço para alcançar todos os pixels da imagem */
	for (int i=0; i<I->width; i++)
	{
     	for (int j=0; j<I->height; j++)
     	{
	      	int idx;

	      	int valor_aux;

	      	idx = i + (j*I->width);

	      	valor_aux = sqrt(pow(I->r[idx], 2.0) + pow(I->g[idx], 2.0) + pow(I->b[idx], 2.0));

	      	if (valor_aux > valor_maximo) {valor_maximo = valor_aux;}
    	}
  	}

  	/* imprime o valor máximo */
  	printf("%f\n", valor_maximo);

	return;
}

void * multMatrixthread(void * parameterTh)
 {

	  //recupera o argumento da thread
		strThread * parameterThread;
		parameterThread = (strThread *)parameterTh;
		// recupera o thread id
		long tid = parameterThread->t;
		//recupera o ganho
		float valor_ganho = parameterThread->ganho;
		//diz pra main que pode criar outra thread, ja que essa aqui já pegou tudo
		parameterThread->flag = 1;

		//job eh o numero de trabalhos que a thread deve fazer
		int jobs;
		//se for a ultima e o numer de trabalhos nao for multiplo do numero de threads, a ultma faz o que sobra
		if (tid == (NUM_THREADS - 1))
				jobs = parameterThread->width*parameterThread->height/NUM_THREADS + (parameterThread->width*parameterThread->height)%NUM_THREADS;
		else
			  jobs = parameterThread->width*parameterThread->height/NUM_THREADS;

		// trabalhos das outras thread que a thread deve pular
		int jobsdone =  parameterThread->width*parameterThread->height/NUM_THREADS * tid;


		//executa o seu lote de trabalho
		for (int i = 0; i < jobs; i++) {

				/* para a parte vermelha do pixel */
				if ((parameterThread->r[i+jobsdone] * valor_ganho) <= 255)
						parameterThread->r[i+jobsdone] = (parameterThread->r[i+jobsdone] * valor_ganho);
				else
						parameterThread->r[i+jobsdone] = 255;

				/* para a parte verde do pixel */
				if ((parameterThread->g[i+jobsdone] * valor_ganho) <= 255)
						parameterThread->g[i+jobsdone] = (parameterThread->g[i+jobsdone] * valor_ganho);
				else
						parameterThread->g[i+jobsdone] = 255;

				/* para a parte azul do pixel */
				if ((parameterThread->b[i+jobsdone] * valor_ganho) <= 255)
					parameterThread->b[i+jobsdone] = (parameterThread->b[i+jobsdone] * valor_ganho);
				else
						parameterThread->b[i+jobsdone] = 255;

			}
		//avisa a main que acabou o trabalho
		parameterThread->flagend++;

		return NULL;
 }

void newThreads(imagem *I, float valor_ganho){

		// contagem do tempo de execucao
		struct timeval t1, t2;
		double deltat;
		//checa o tempo no inicio
		gettimeofday(&t1, NULL);

		//cria NUM_THREADS threads
		pthread_t threads[NUM_THREADS];
		int rc;
		strThread parameterThread;

		//copia a struct img para outra struct contendo outros parametros, pois so podemos passar um argumento pra thread
		parameterThread.width = I->width;
		parameterThread.height = I->height;
		parameterThread.r = &(I->r[0]);
		parameterThread.g = &(I->g[0]);
		parameterThread.b = &(I->b[0]);
		parameterThread.ganho = valor_ganho;
		parameterThread.flagend = 0;
		//o argumento deve ser um ponteiro
		void * argumento = &parameterThread;
		// crio as threads
    for((parameterThread.t)=0; (parameterThread.t)<NUM_THREADS; (parameterThread.t)++){
			 // condicao para recuperacao do argumento sem que ele mude no meio do caminho
			 parameterThread.flag = 0;
			 //cria a thread com id parameterThread.t e rotina multMatrixthread com argumento 'argumento'
			 rc = pthread_create(&threads[parameterThread.t], NULL, multMatrixthread, argumento);
       if (rc){
				 //rotina de erro
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
			 //fica preso aqui ate que a thread tenha recuperado todo argumento
			 while (!parameterThread.flag);
    }

		//fica preso aqui até a thread terminar SE ACHAR OUTRA COISA, É MELHOR
		while (parameterThread.flagend != NUM_THREADS);

		//checa o tempo no final
		gettimeofday(&t2, NULL);
		//calcula o tempo transcorrido
		deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;      // s para ms
    deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us para ms
		//imprime o tempo de execucao
		printf("Tempo de execucao %f ms\n", deltat );
		return;
 }
