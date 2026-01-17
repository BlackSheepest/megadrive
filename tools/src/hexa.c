#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        int i = 0;

        printf("Resultat:");
        for (i = 0; i < strlen(argv[1]); i++)
        {
            printf(" %02.2X", (argv[1])[i]);
        }
        printf("\n");
    }
    else
    {
        fprintf(stderr, "Usage: hexa <string>\n");
        return -1;
    }

    return 0;
}