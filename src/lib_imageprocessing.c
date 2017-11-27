#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <FreeImage.h>
#include "imageprocessing.h"


imagem abrir_imagem_malloc(char *nome_do_arquivo)
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

imagem abrir_imagem_mmap(char *nome_do_arquivo)
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

	/* mapeia os vetores de cores na memória */
	I.r = (float *) mmap(NULL, sizeof(float) * x * y,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
	I.g = (float *) mmap(NULL, sizeof(float) * x * y,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
	I.b = (float *) mmap(NULL, sizeof(float) * x * y,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);

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

	
void liberar_imagem_malloc(imagem *I)
{
	free(I->r);
	free(I->g);
	free(I->b);
}


void liberar_imagem_mmap(imagem *I)
{
	/* desmapeia a memória */
	munmap(I->r, sizeof(float));
	munmap(I->g, sizeof(float));
	munmap(I->b, sizeof(float));
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

/* função para alterar o brilho da imagem (varrendo linhas)*/
void altera_brilho(imagem *I, float valor_ganho)
{
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

/******* TRABALHO 2 *******/

/* função para alterar o brilho da imagem (varrendo linhas)*/
void altera_brilho_por_linhas(imagem *I, float valor_ganho)
{
	/* contagem do tempo de execução */
	struct timeval t1, t2;
	double deltat;

	/* checa o tempo no inicio */
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

	/* checa o tempo no final */
	gettimeofday(&t2, NULL);

	/* calcula o tempo transcorrido */
	deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;	  // sec to ms
	deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	/* imprime o tempo de execução */
	printf("Tempo de execução (varrendo por linhas): %f [ms]\n", deltat);

	return;
}


/* função para alterar o brilho da imagem (varrendo colunas)*/
void altera_brilho_por_colunas(imagem *I, float valor_ganho)
{
	/* contagem do tempo de execução */
	struct timeval t1, t2;
	double deltat;

	/* checa o tempo no início */
	gettimeofday(&t1, NULL);

	/* laço para alcançar todos os pixels da imagem */
	for (int j=0; j<I->height; j++)
	{
	 	for (int i=0; i<I->width; i++)
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

	/* checa o tempo no final */
	gettimeofday(&t2, NULL);

	/* calcula o tempo transcorrido */
	deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;	  // sec to ms
	deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	/* imprime o tempo de execução */
	printf("Tempo de execução (varrendo por colunas): %f [ms]\n", deltat);

	return;
}


void * multMatrixthread(void * parameterTh)
 {
  	/* recupera o argumento da thread */
	strThread * parameterThread;
	parameterThread = (strThread *)parameterTh;
	
	/* recupera o thread ID */
	long tid = parameterThread->t;

	/* recupera o ganho */
	float valor_ganho = parameterThread->ganho;

	/* diz pra main que pode criar outra thread, já que essa aqui já pegou tudo */
	parameterThread->flag = 1;

	/* job é o numero de trabalhos que a thread deve fazer */
	int jobs;
	
	/* se for a última e o número de trabalhos não for múltiplo do número de threads, a última faz o que sobra */
	if (tid == (NUM_THREADS - 1))
	{
		jobs = parameterThread->width*parameterThread->height/NUM_THREADS + (parameterThread->width*parameterThread->height)%NUM_THREADS;
	}

	else
	{
		jobs = parameterThread->width*parameterThread->height/NUM_THREADS;
	}

	/* trabalhos das outras threads que a thread deve pular */
	int jobsdone =  parameterThread->width*parameterThread->height/NUM_THREADS * tid;

	/* executa o seu lote de trabalho */
	for (int i = 0; i < jobs; i++) 
	{
		/* para a parte vermelha do pixel */
		if ((parameterThread->r[i+jobsdone] * valor_ganho) <= 255)
		{
			parameterThread->r[i+jobsdone] = (parameterThread->r[i+jobsdone] * valor_ganho);
		}

		else
		{
			parameterThread->r[i+jobsdone] = 255;
		}

		/* para a parte verde do pixel */
		if ((parameterThread->g[i+jobsdone] * valor_ganho) <= 255)
		{
			parameterThread->g[i+jobsdone] = (parameterThread->g[i+jobsdone] * valor_ganho);
		}

		else
		{
			parameterThread->g[i+jobsdone] = 255;
		}

		/* para a parte azul do pixel */
		if ((parameterThread->b[i+jobsdone] * valor_ganho) <= 255)
		{
			parameterThread->b[i+jobsdone] = (parameterThread->b[i+jobsdone] * valor_ganho);
		}

		else
		{
			parameterThread->b[i+jobsdone] = 255;
		}
	}

	/* avisa a main que acabou o trabalho */
	parameterThread->flagend++;

	return NULL;
}


void newThreads(imagem *I, float valor_ganho)
{
	/* contagem do tempo de execução */
	struct timeval t1, t2;
	double deltat;

	/* checa o tempo no início */
	gettimeofday(&t1, NULL);

	/* cria NUM_THREADS threads */
	pthread_t threads[NUM_THREADS];
	int rc;

	strThread parameterThread;

	/* copia a struct img para outra struct contendo outros parâmetros, pois só podemos passar um argumento para a thread */
	parameterThread.width = I->width;
	parameterThread.height = I->height;
	parameterThread.r = &(I->r[0]);
	parameterThread.g = &(I->g[0]);
	parameterThread.b = &(I->b[0]);
	parameterThread.ganho = valor_ganho;
	parameterThread.flagend = 0;

	/* o argumento deve ser um ponteiro */
	void * argumento = &parameterThread;

	/* cria as threads */
	for ((parameterThread.t)=0; (parameterThread.t)<NUM_THREADS; (parameterThread.t)++)
	{
		/* condição para a recuperação do argumento sem que ele mude no meio do caminho */
		parameterThread.flag = 0;

		/* cria a thread com ID parameterThread.t e rotina multMatrixthread com argumento 'argumento' */
		rc = pthread_create(&threads[parameterThread.t], NULL, multMatrixthread, argumento);
		
		if(rc)
		{
			/* rotina de erro */
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}

		/* fica preso aqui ate que a thread tenha recuperado todo argumento */
		while (!parameterThread.flag);
	}

	/* fica preso aqui até a thread terminar */
	while (parameterThread.flagend != NUM_THREADS);

	/* checa o tempo no final */
	gettimeofday(&t2, NULL);

	/* calcula o tempo transcorrido */
	deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;	  // s para ms
	deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us para ms
	
	/* imprime o tempo de execução */
	printf("Tempo de execução (usando threads): %f [ms]\n", deltat);

	return;
}


/* função para alterar o brilho da imagem (por vários processos)*/
void altera_brilho_multi_process(imagem *I, float valor_ganho)
{
	/* contagem do tempo de execução */
	struct timeval t1, t2;
	double deltat;

	/* checa o tempo no início */
	gettimeofday(&t1, NULL);

	int i, j, k, n;

	int passo, first_value, last_value;	

	/* número de processos filhos criados */
	n = 7;

	pid_t childs[n];

	passo = I->width/n;

	for(k=0; k<n; k++)
	{
		/* cria um processo filho */
		childs[k] = fork();

		if(childs[k] == 0)
		{
			/* define os pixels que o processo filho altera */
			first_value = k * passo;

			last_value = first_value + passo;

			if(k == (n - 1)) {last_value = I->width;}

			/* laço para alcançar todos os pixels da imagem */
			for(i=first_value; i<last_value; i++)
			{
			 	for(j=0; j<I->height; j++)
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

		  	/* encerra o processo filho */
		  	_Exit(EXIT_SUCCESS);
		}			
	}

	/* espera todos os processos filhos encerrarem */
	wait(NULL);

  	/*checa o tempo no final */
	gettimeofday(&t2, NULL);

	/* calcula o tempo transcorrido */
	deltat = (t2.tv_sec - t1.tv_sec) * 1000.0;	  // sec to ms
	deltat += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	/* imprime o tempo de execução */
	printf("Tempo de execução (usando processos): %f [ms]\n", deltat);

	return;
}