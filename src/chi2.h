#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#ifndef IO_H
#define IO_H
#include "io.h"
#endif

#ifndef WORDSET_H
#define WORDSET_H
#include "wordset.h"
#endif

/* feature selection */
Wordset *chi2_featureSelection(Param *para, Arff *data);
double* chi2_chiSquared(Wordset **docs, int numDocs, int numTermos);
