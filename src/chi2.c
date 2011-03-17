#include "chi2.h"

Wordset *chi2_featureSelection(Param *para, Arff *data)
{
	double *pontos;
	int *ranking;
	int i;

	/* roda chi-square */
	pontos = chi2_chiSquared(data->docs, data->docs_size, data->feats);

	/* monta ranking */
	ranking = malloc(sizeof(int) * data->feats);
	for(i = 0; i < data->feats; i++)
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
	qsort(ranking, data->feats, sizeof(int), cmp_pts);

	Wordset *feats = wordset_new(para->nfeat);
    
	/*
	for(i = 0; i < data->feats; i++)
		printf("%d \t %f\n", ranking[i], pontos[ranking[i]]);
    */

	/* escreve no ranking e evita termos de peso 0 */
	for(i = 0; i < para->nfeat; i++)
    {
        if(pontos[ranking[i]] > 0)
    		wordset_insert(feats, ranking[i]);
    }

    /* imprime ranking */
    if(para->rank)
    {
        for(i = 0; i < data->feats; i++)
            printf("%d\t%s\n", i, data->str[ranking[i]]);
    }

	free(ranking);
	free(pontos);

	return feats;
}

double* chi2_chiSquared(Wordset **docs, int numDocs, int numTermos)
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

