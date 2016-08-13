#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define BUFSIZE 1024*1024

void FastaToFastq(FILE* fin, FILE* fout)
{
    char *line, *quality;
    size_t length;
    size_t i;

    line = (char*)malloc(BUFSIZE + 1);
    quality = (char*)malloc(BUFSIZE + 1);
    for(i = 0; i < BUFSIZE; i ++)
        quality[i] = 'I';
    quality[BUFSIZE] = '\0';

    while(!feof(fin))
    {
        if(fgets(line, BUFSIZE, fin) != NULL)
        {
            length = strlen(line) - 1;
            if(line[0] == '>')
                fprintf(fout, "@%s", &(line[1]));
            else
            {
                fprintf(fout, "%s", line);
                fprintf(fout, "+\n");
                quality[length] = '\0';
                fprintf(fout, "%s\n", quality);
                quality[length] = 'I';
            }
        }
    }
    free(line);
    free(quality);
}

int main(int argc, char** argv)
{
    FILE* fin;
    fin = stdin;
    if(argc > 1)
    {
        fin = fopen(argv[1], "r");
        if(!fin)
        {
            perror("Error: cannot open the input file");
            exit(1);
        }
    }
    FastaToFastq(fin, stdout);
    fclose(fin);

    return 0;
}
