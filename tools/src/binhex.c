#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        long l = strtol(argv[1], NULL, 2);

        printf("Resultat : '%0X'\n", l);
    }
    else
    {
        fprintf(stderr, "Usage: binhex <bin>\n");
        return -1;
    }

    return 0;
}