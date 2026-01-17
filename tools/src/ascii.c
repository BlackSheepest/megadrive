#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    if (argc > 1)
    {
        char tmp[1024];

        int i = 1;
        for (i = 1; i < argc; i++)
        {
            long l = strtol(argv[i], NULL, 16);

            tmp[i-1] = (char)l;
        }
        tmp[i-1] = '\0';

        printf("Resultat : '%s'\n", tmp);
    }
    else
    {
        fprintf(stderr, "Usage: ascii <hexa>+\n");
        return -1;
    }

    return 0;
}