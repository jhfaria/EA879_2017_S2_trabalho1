#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#define NUM_THREADS 7

typedef struct
{
  	unsigned int width, height;
  	float *r, *g, *b;
} imagem;

typedef struct {
    unsigned int width, height;
    float *r, *g, *b;
    long t;
    float ganho;
    int flag, flagend;

} strThread;

imagem abrir_imagem(char *nome_do_arquivo);

void salvar_imagem(char *nome_do_arquivo, imagem *I);

void liberar_imagem(imagem *i);

/* funções criadas pelo grupo */
void altera_brilho(imagem *I, float valor_ganho);

void altera_brilho_por_linhas(imagem *I, float valor_ganho);

void altera_brilho_por_colunas(imagem *I, float valor_ganho);

void valor_maximo(imagem *I);

void *multMatrixthread(void * parameterTh);

void newThreads(imagem *I, float valor_ganho);

void altera_brilho_multi_process(imagem *I, float valor_ganho);

#endif
