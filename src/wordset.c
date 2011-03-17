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

void wordset_print(Wordset *ws)
{
	unsigned int i;
	for(i = 0; i < ws->num; i++)
		printf("%d\n", wordset_access(ws, i));
	printf("\n");
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
