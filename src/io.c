#include "io.h"

/* pega parâmetros de linha de comando */
Param *io_getParameters(int argc, char **argv)
{
    Param *para = (Param*)malloc(sizeof(Param));
    
	para->sep = malloc(STRING_SIZE*sizeof(char));
    para->fileIn = malloc(STRING_SIZE*sizeof(char));
	para->fileOut = malloc(STRING_SIZE*sizeof(char));
	para->classif = malloc(STRING_SIZE*sizeof(char));
    para->rank = false;

    int c;
    while((c = getopt(argc, argv, "i:o:c:n:s:r")) != -1)
    {
        switch(c)
        {
            case 'i':
                strcpy(para->fileIn, optarg);
                break;

            case 'o':
                strcpy(para->fileOut, optarg);
                break;

            case 'c':
                strcpy(para->classif, optarg);
                break;

            case 'n':
                para->nfeat = atoi(optarg);
                break;
            
            case 's':
                strcpy(para->sep, optarg);
                break;

            case 'r':
                para->rank = true;
                break;

            default:
                printf("ERROR: invalid argument.\n");
                exit(1);
        }
    }
    return para;
}

/* verifica se string é número ou n */
bool io_isNumber(char *str)
{
	int p = 0;
	while(str[p] != '\0')
	{
		if((str[p] < 48) || (str[p] > 57))
			return false;
		p++;
	}
	return true;
}

bool io_isValidStr(char *str)
{
	int p = 0;
	while(str[p] != '\0')
	{
        /* only A-Z, a-z, dot and underscore */;
		if(!( ((str[p] >= 65) && (str[p] <= 122)) || (str[p] == 46) || (str[p] == 95)))
			return false;
		p++;
	}
	return true;
}

/* retorna pointer para ts, tsc, tam */
Arff *io_readArff(char *filename, char *classif, char *sep)
{
    FILE *fileIn = fopen(filename, "r");

	char *line = malloc(LINE_SIZE * sizeof(char));
	Arff *arff = (Arff*) malloc(sizeof(Arff));
	arff->docs_size = 0; 
	arff->feats = -1; /* tem um attribute com o nome da classe */

    char *fim;
	long attr_line = 0,
         docpos = 0,
		 size = 0;

	/* passa uma vez para ver o tamanho das coisas */
	while((fim = fgets(line, LINE_SIZE, fileIn)))
	{
		if(line[0] == '@')
		{   
            /* pega onde começam as feats */
            if(line[1] == 'r' || line[1] == 'R')
            {
                fim = fgets(line, LINE_SIZE, fileIn);
                attr_line = ftell(fileIn);
            }
			/* attribute */
			if(line[1] == 'a' || line[1] == 'A') 
				arff->feats++;
			/* data */
			else if(line[1] == 'd' || line[1] == 'D')
			{
				/* pega pos para voltar */
				docpos = ftell(fileIn);
			}
		}
		/* documento */
		else if(line[0] == '{')
			size++;
	}

    /* pega strings para mostrar feats */
    int i, j;
    bool aspa;
    fseek(fileIn, attr_line, SEEK_SET);
    arff->str = (char**) malloc(sizeof(char*)*(arff->feats + 1));
    for(i = 0; i < arff->feats; i++)
    {
        aspa = false;
        arff->str[i] = (char*) malloc(sizeof(char)*200);
        fim = fgets(line, LINE_SIZE, fileIn);
        /* checa se é aspa ou não */
        if(line[11] == '\'')
        {
            for(j = 12; line[j] != '\''; j++)
                arff->str[i][j - 12] = line[j];
        }
        else
        {
            for(j = 11; line[j] != ' '; j++)
                arff->str[i][j - 11] = line[j];
        }
    }

	/* aloca docs */
	arff->docs_size  = size;
	arff->docs = (Wordset**)malloc(arff->docs_size * sizeof(Wordset*));

	/* passa a segunda vez preenchendo as coisas */
	fseek(fileIn, docpos, SEEK_SET);

	char *tok, *pnt, 
         *tok_n = (char*)malloc(sizeof(char)*STRING_SIZE);
	int  num;
	bool prim;
	Wordset *tmp;

	for(i = 0; fgets(line, LINE_SIZE, fileIn); i++)
	{
		/* conta coisas na linha */
		num = 0;
		tok = strtok_r(line, "{,}", &pnt);
		while(tok != NULL)
		{
			num++;
			tok = strtok_r(NULL, "{,}", &pnt);
		}
		tmp = wordset_new(num);

		/* coloca valores */
		prim = true;

		/* volta no arquivo e pega linha denovo */
    	*fim = fseek(fileIn, docpos, SEEK_SET);
		fim = fgets(line, LINE_SIZE, fileIn);

		tok = strtok_r(line, "{, }", &pnt);
		while(tok != NULL)
		{
            strncpy(tok_n, tok, STRING_SIZE);
			if(io_isNumber(tok))
			{
				if(prim)
					wordset_insert(tmp, atoi(tok));
			}
			/* ve se classe está contida */
			else if(io_isSubClass(classif, tok, sep))
            {
				wordset_setFromClass(tmp);
            }

			prim = !prim;
			tok = strtok_r(NULL, "{, }", &pnt);
		}

        /* salva documento */
        arff->docs[i] = tmp;

		/* guarda posição para voltar denovo */
		docpos = ftell(fileIn);
	}
    
    fclose(fileIn);

	free(line);
	return arff;
}

void io_fprintFeats(char *fileOut, char **featStr, Wordset *feats)
{
    FILE *out = fopen(fileOut, "w");

	int i;
	for(i = 0; i < feats->num; i++)
		fprintf(out, "%s\n", featStr[wordset_access(feats, i)]);

    fclose(out);
}

bool io_isSubClass(char *s1, char *s2, char *sep)
{
    char *s1_  = (char*) malloc(sizeof(char) * STRING_SIZE),
         *s2_  = (char*) malloc(sizeof(char) * STRING_SIZE);

    strncpy(s1_, s1, STRING_SIZE);
    strncpy(s2_, s2, STRING_SIZE);
         
    char *pnt1 = NULL,
         *pnt2 = NULL,
         *tok1 = strtok_r(s1_, sep, &pnt1),
         *tok2 = strtok_r(s2_, sep, &pnt2);

    while(tok1 != NULL)
    {
        /* se s2 C s1, então |s1| >= |s2| */
        if(tok2 == NULL)
            return false;
        if((strcmp(tok1, tok2) != 0))
            return false;

        tok1 = strtok_r(NULL, sep, &pnt1);
        tok2 = strtok_r(NULL, sep, &pnt2);
    }

    free(s1_);
    free(s2_);

    return true;
}
