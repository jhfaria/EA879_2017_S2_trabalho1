%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>
#include <pthread.h>
void yyerror(char *c);
int yylex(void);
%}


%union
{
  char    	strval[50];
  int     	ival;
  float		float_value;
}


%token <strval> STRING
%token <ival> VAR IGUAL EOL ASPA DOISVEZES VEZES DIVIDIDO ABRE_COLCHETE FECHA_COLCHETE
%token <float_value> NUMERO
%left SOMA


%%
PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
        liberar_imagem(&I);
    }
    | STRING IGUAL STRING DOISVEZES  NUMERO {
      /* estrutura para aumentar o brilho e salvar em um arquivo diferente usando threads */
      imagem I = abrir_imagem($3);
      newThreads(&I, $5);
      salvar_imagem($1, &I);
      liberar_imagem(&I);

    }

    | STRING IGUAL STRING VEZES NUMERO {
    	/* estrutura para aumentar o brilho e salvar em um arquivo diferente */
    	imagem I = abrir_imagem($3);
    	altera_brilho(&I, $5);
    	salvar_imagem($1, &I);
      liberar_imagem(&I);
    }

    | STRING IGUAL STRING DIVIDIDO NUMERO {
    	/* estrutura para diminuir o brilho e salvar em um arquivo diferente */
    	imagem I = abrir_imagem($3);
    	altera_brilho(&I, (1/$5));
    	salvar_imagem($1, &I);
      liberar_imagem(&I);
    }

    | ABRE_COLCHETE STRING FECHA_COLCHETE {
    	/* imprime o vlaor máximo dos pixels da imagem */
    	imagem I = abrir_imagem($2);
    	valor_maximo(&I);
        liberar_imagem(&I);
    }
    ;
%%


void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}


int main()
{
  FreeImage_Initialise(0);
  yyparse();
  return 0;
}
