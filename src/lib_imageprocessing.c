#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "imageprocessing.h"

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

	      	/* tentativa 1 */
	      	// if (I->r[idx] > valor_maximo) {valor_maximo = I->r[idx];}

	      	// if (I->g[idx] > valor_maximo) {valor_maximo = I->g[idx];}

	      	// if (I->b[idx] > valor_maximo) {valor_maximo = I->b[idx];}

	      	/* tentativa 2 */
	      	/* sqrt: raiz quadrada; pow: potência */
	      	/* "norma" do valor do pixel */
	      	valor_aux = sqrt(pow(I->r[idx], 2.0) + pow(I->g[idx], 2.0) + pow(I->b[idx], 2.0));

	      	if (valor_aux > valor_maximo) {valor_maximo = valor_aux;}
    	}
  	}

  	/* imprime o valor máximo */
  	printf("%f\n", valor_maximo);

	return;
}
