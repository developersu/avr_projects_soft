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
	
//	k |= 1<<0;
//	printf("%s\n", byte_to_binary(k));

//	k &= ~(1<<0);
//	printf("%s\n", byte_to_binary(k));


	// & (1<<PD2)


	k = 0b00001000;
	i = 0b00000000;
	printf("k & i: ");
	printf("k = %s and ", byte_to_binary(k));
	printf("i = %s\n", byte_to_binary(i));
	printf("k & i = ");

	k & i ? printf ("yes\n") : printf("no\n");		//no
// ------------------------------------------------------------
	i = 0b00001000;
	printf("k & i: ");
	printf("%s and ", byte_to_binary(k));
	printf("%s\n", byte_to_binary(i));
	printf("k & i = ");

	k & i? printf ("yes\n") : printf("no\n");		//yes
// ------------------------------------------------------------

	i = ~(1<<3);
	printf("k & i: ");
	printf("%s and ", byte_to_binary(k));
	printf("%s\n", byte_to_binary(i));
	printf("k & i = ");

	k & i ? printf ("yes\n") : printf("no\n");		//no
// ------------------------------------------------------------

	i = (1<<3);
	printf("k & i: ");
	printf("%s and ", byte_to_binary(k));
	printf("%s\n", byte_to_binary(i));
	printf("k & i = ");

	k & i ? printf ("yes\n") : printf("no\n");			//yes
	
	int a;
	char o[5] = {0,0,0,0,0};
	for (a=4; a>=0; a--){
		printf("\n%s\n", byte_to_binary(o[a]));
	}

	k = 0b00001000;
	k &= ~(1<<3);
	printf("0b00001000 &= ~(1<<3) = %s\n\n", byte_to_binary(k));

	k = (1<<3);
	printf("k = (1<<3)  = %s\n", byte_to_binary(k));
	printf ("---------------\n\n");
	k = 0b00000000;
	for (a=0; a<8;a++) {
		k |= 1<<a;
		printf("k |= 1<<j  = %s\n", byte_to_binary(k));
	}


	k=0b00000000;
	printf("\n\n%c\n",k);


	k=0b11111111;
	char ip[3];
	itoa(k,ip);
	printf("\n\n%s\n",ip);




	return 0;
}
