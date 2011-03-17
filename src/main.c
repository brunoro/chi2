#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef CHI2_H
#define CHI2_H
#include "chi2.h"
#endif

int main(int argc, char **argv)
{
    /* pega parametros do GA */
    Param *para = io_getParameters(argc, argv);

    /* lê base de dados */
    Arff *data = io_readArff(para->fileIn, para->classif, para->sep);

    /* escolhe features usando seleção própria ou ranking externo */
    Wordset *feat;
    feat = chi2_featureSelection(para, data);
	
	/* escreve as feats selecionadas */
	io_fprintFeats(para->fileOut, data->str, feat);

	return 0;
}
