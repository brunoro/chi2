#include "wordset.h"

/* aloca um wordset */
Wordset *wordset_new(unsigned int size)
{
	Wordset *ws = (Wordset*) malloc(sizeof(Wordset));
	ws->index = (int*)malloc(size * sizeof(int)); 
	ws->qnt = size;
	ws->num = 0;
	ws->sorted = false;
	ws->classe = false;
	return ws;
}

/* desaloca um wordset */
void wordset_destroy(Wordset *ws)
{
	free(ws->index);
}

/* acessa uma posição indexada */
int wordset_access(Wordset *ws, unsigned int ind)
{
	if(ind < ws->qnt)
		return ws->index[ind];
	return NOT_INSERTED;
}

/* faz um documento ser da classe */
void wordset_setFromClass(Wordset *ws)
{
	ws->classe = true;
}

bool wordset_isClass(Wordset *ws)
{
	return ws->classe;
}

/* insere uma palavra */
bool wordset_insert(Wordset *ws, int word)
{
	if(ws->num == ws->qnt)
		return false;
	ws->sorted = false;
	ws->index[ws->num] = word;
	ws->num++;
	return true;
}

void wordset_sort(Wordset *ws)
{
	ws->sorted = true;
	qsort(ws->index, ws->num, sizeof(int), cmp);
}

int cmp(const void *a, const void *b)
{ 
    int *ia = (int*)a;
    int *ib = (int*)b;
    return (*ia < *ib);
} 


/* busca uma palavra e retorna true se ela está no wordset */
bool wordset_search(Wordset *ws, int word)
{
	int i;
    if(word == NOT_INSERTED)
        return false;
	if(!ws->sorted)
		wordset_sort(ws);
	for(i = 0; i < ws->num; i++)
	{
		if(word < ws->index[i])
		{
			i++;
			continue;
		}
		if(word == ws->index[i])
			return true;
		if(word > ws->index[i])
			return false;
	}
	return false;
}

/* retorna o tamanho de um wordset */
int wordset_size(Wordset *ws)
{
	return ws->num;
}

Wordset *wordset_featureSelection(Wordset **docs, int docs_size, int ntermos, int nfeats)
{
	double *pontos;
	int *ranking;
	int i;

	/* roda chi-square */
	pontos = chiSquared(docs, docs_size, ntermos);

	/* monta ranking */
	ranking = malloc(sizeof(int) * ntermos);
	for(i = 0; i < ntermos; i++)
		ranking[i] = i;

	/* usa funcao de comparacao para gerar ranking */
	int cmp_pts(const void *a, const void *b)
	{
		int *ia = (int*)a;
		int *ib = (int*)b;
		if(pontos[*ia] > pontos[*ib])
			return -1;
		if(pontos[*ia] < pontos[*ib])
			return 1;
		return 0;
	}
	qsort(ranking, ntermos, sizeof(int), cmp_pts);

	Wordset *feats = wordset_new(nfeats);
    
	/*
	for(i = 0; i < ntermos; i++)
		printf("%d \t %f\n", ranking[i], pontos[ranking[i]]);
    */

	/* escreve no ranking */
	for(i = 0; i < nfeats; i++)
    {
        if(pontos[ranking[i]] > 0)
    		wordset_insert(feats, ranking[i]);
    }

	free(ranking);
	free(pontos);

	return feats;
}

void wordset_print(Wordset *ws)
{
	unsigned int i;
	for(i = 0; i < ws->num; i++)
		printf("%d\n", wordset_access(ws, i));
	printf("\n");
}

double* chiSquared(Wordset **docs, int numDocs, int numTermos)
{
	int i, j;

	/* valores observados */
	double **n = (double**)malloc(numTermos * sizeof(double*));
	for(i = 0; i < numTermos; i++)
		n[i] = (double*)calloc(4, sizeof(double)); // n00 n01 n10 n11

	for(i = 0; i < numDocs; i++)
	{
		for (j = 0; j < numTermos; j++)
		{
			if(wordset_isClass(docs[i]))
			{
				if(wordset_search(docs[i], j)) // n11
					n[j][3]++;
				else // n01
					n[j][2]++;
			}
			else
			{
				if(wordset_search(docs[i], j)) // n10
					n[j][1]++;
				else // n00
					n[j][0]++;
			}
		}
	}

	/* calcula usando fórmula
     *        (n11 + n10 + n01 + n00)  *  (n11 * n00 - n10 * n01)^2 
     * chi2 = -----------------------------------------------------
     *        (n11 + n01) * (n11 + n10) * (n10 + n00) * (n01 + n00)
     */

	double *chisquared = (double*)malloc(numTermos * sizeof(double));
	double divisor;
	for(i = 0; i < numTermos; i++)
	{
		divisor = ((n[i][3] + n[i][1]) * (n[i][3] + n[i][2]) * (n[i][2] + n[i][0]) * (n[i][1] + n[i][0]));
		chisquared[i] = (divisor == 0) ? 0 : ((n[i][3] + n[i][2] + n[i][1] + n[i][0]) *
		                                     (n[i][3]*n[i][0] - n[i][2]*n[i][1])*(n[i][3]*n[i][0] - n[i][2]*n[i][1])) / 
			                                 divisor;
	}

	return chisquared;
}


/* main de teste
int main()
{
	Wordset *tst = wordset_new(10);
	char palavras[][15] = {1, 2, 10, 5, 1000};
	int i;
	for(i = 0; i < 5; i++)
	{
		printf("%d ", wordset_insert(tst, palavras[i]));
		printf("%ud ", wordset_access(tst, i - 1));
	}

	return 0;
}*/
