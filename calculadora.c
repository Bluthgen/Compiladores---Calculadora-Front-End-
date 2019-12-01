#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "asa.h"
#include "executa.h"
#include "simbolos.h"


extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;

int erro_sintatico= 0;

void main( int argc, char **argv ){
    int c;
    char* entrada= "", *saida= "";
    FILE* fout= NULL;
    int mostraArvore= 0;
    /*++argv, --argc;
    if ( argc > 0 )
        yyin = fopen( argv[0], "r" );
    else{
        printf("Por favor forneça o nome de um arquivo contendo os comandos a serem executados\n");
        exit(0);
    }*/

    while((c= getopt(argc, argv, "a:f:h")) != -1){
        switch(c){
            case 'a': mostraArvore= 1; saida= optarg; break;
            case 'h': printf("Utilização: './calc [-a <Nome_do_arquivo> -h] -f <Nome_do_arquivo.calc>' \nOpções:\n  -a \t\tMostrar a árvore sintática abstrata gerada a partir da entrada\n  -h\t\tMostrar esta informação sobre a utilização do programa\n  -f <arg>\tUtilizar o arquivo <arg> (.calc) como entrada (Obrigatorio)\n"); exit(0);
            case 'f': entrada= optarg; break;
            case '?': if(optopt == 'f') printf("Erro: a opção '-f' requer o nome de um arquivo como arqumento\n");
                    else if(optopt == 'a') printf("Erro: a opção '-a' requer o nome de um arquivo como arqumento\n");
                    else if (isprint(optopt))
                        printf("Erro: opção desconhecida '-%c'\n", optopt);
                    else
                        printf("Erro: opção desconhecida `\\x%x'\n", optopt);
                    exit(0);
            default: exit(0);
        }   
    }
    if (strlen(entrada) > 5){
        if(strcmp(strrchr(entrada, '\0') - 5, ".calc")){
            printf("Erro: Por favor forneça o nome de um arquivo de entrada com extensão .calc como entrada\n");
            exit(0);
        }
        yyin = fopen(entrada, "r");
        if(yyin == NULL){
            printf("Erro na leitura do arquivo de entrada %s! O arquivo pode exigir permissões especiais para ser aberto, ou pode não existir.\n", entrada);
            exit(0);
        }
    }else{
        printf("Erro: Por favor forneça o nome de um arquivo válido como entrada\n");
        exit(0);
    }
    if(mostraArvore){
        if(!strcmp(saida, entrada)){
            printf("Erro: O nome do arquivo de saída não pode ser igual ao do arquivo de entrada!\n");
            fclose(yyin);
            exit(0);
        }
        fout= fopen(saida, "w");
        if(fout == NULL){
            printf("Erro na criação do arquivo de saída %s!\n", saida);
            fclose(yyin);
            exit(0);
        }
    }
    struct node* root= raizASA;
    yyparse();
    if(erro_sintatico)
        exit(0);
    if(strcmp("", raizASA->nome)){
        int analise= verificaSemantica(raizASA);
        //dumpTabela(tabela);
        //fclose(tabela);
        if(analise == INDEF){
            printf("O código não pode ser executado devido aos erros\n");
            fclose(yyin);
            exit(0);
        }
        if(mostraArvore){
            printArvore(raizASA, 0, fout);
            fclose(fout);
        }
        //printf("\nAnálise: %d\n", analise);
        
    }
    fclose(yyin);   
    
}
