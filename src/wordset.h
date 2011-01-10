#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define NOT_INSERTED -1

typedef struct wordset
{
	int *index;
	unsigned int qnt; 
	unsigned int num;
	bool classe;
	bool sorted;
} Wordset;

/* alocação */
Wordset *wordset_new(unsigned int size);
void wordset_destroy(Wordset *ws);

/* funções */
void wordset_setFromClass(Wordset *ws);
bool wordset_isClass(Wordset *ws);
int wordset_access(Wordset *ws, unsigned int index);
bool wordset_insert(Wordset *ws, int term);
bool wordset_search(Wordset *ws, int word);

int wordset_size(Wordset *ws);
void wordset_sort(Wordset *ws);
int cmp(const void *a, const void *b);

/* feature selection */
Wordset *wordset_featureSelection(Wordset **docs, int docs_size, int ntermos, int nfeats);
double* chiSquared(Wordset **docs, int numDocs, int numTermos);

/* etc e debug */
void wordset_print(Wordset *ws);
