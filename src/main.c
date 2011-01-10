#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef IO_H
#define IO_H
#include "io.h"
#endif

int main(int argc, char **argv)
{
    /* pega parametros do GA */
    Param *para = io_getParameters(argc, argv);

    /* lê base de dados */
    Arff *data = io_readArff(para->fileIn, para->classif);

    /* escolhe features usando seleção própria ou ranking externo */
    Wordset *feat;
    feat = wordset_featureSelection(data->docs, data->docs_size, data->feats, para->nfeat);
	
	/* escreve as feats selecionadas */
	io_fprintFeats(para->fileOut, data->str, feat);

	return 0;
}
