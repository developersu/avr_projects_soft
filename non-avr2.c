#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *byte_to_binary(char x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

int main ()
{
	char i,j,k;
	


	k = 0;
	i = 0;

	k |= 1<<3;
	
	printf("k = %s \n", byte_to_binary(k));
//	printf("i = %s\n", byte_to_binary(i));
//	printf("k & i = ");

	k & (1<<3) ? printf ("yes\n") : printf("no\n");		//no
	!(k & (1<<3)) ? printf ("yes\n") : printf("no\n");		//no

	return 0;
}
