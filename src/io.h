#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef WORDSET_H
#define WORDSET_H
#include "wordset.h"
#endif

#define STRING_SIZE 100
#define LINE_SIZE 100000

typedef struct Param
{
    int  nfeat;
    char *fileIn,
	     *fileOut,
	     *classif,
         *sep;
}Param;

typedef struct Arff
{
    Wordset **docs;
    unsigned docs_size;
    
    char **str;
    int feats;
}Arff;

/* pega parâmetros de linha de comando */
Param *io_getParameters(int argc, char **argv);

/* verifica se string é número ou n */
bool io_isNumber(char *str);

/* verifica se é um nome válido para classe */
bool io_isValidStr(char *str);

/* retorna pointer para ts, tsc, tam */
Arff *io_readArff(char *filename, char *classif, char *sep);

/* escreve features num arquivo */
void io_fprintFeats(char *fileOut, char **featStr, Wordset *feats);

/* verifica se uma classe é subclasse de outra : s1 C s2 */
bool io_isSubClass(char *s1, char *s2, char *sep);
