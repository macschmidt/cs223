//test
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	if (argc != 4  || atoi(argv[1]) < 1)
	{
		printf("Invalid input(s)\n");
		exit(1);
	}
	long n = atol(argv[1]);
	long a = atol(argv[2]);
	long b = atol(argv[3]);
	char input[1000000];
	long c;
	long counter = 0;

	while((c = getchar()) != EOF)
	{
		input[counter] = c;
		counter++;
	}

	long inputLength = counter;
	long buffer = 0;

	if ((inputLength % n) != 0)
	{
		buffer = n - (inputLength % n);
	}

	for (long i = 0; i < buffer; i++)
	{
		input[inputLength + i] = 0;
	}
	inputLength += buffer; // update the length

	for (long x = 0; x < (inputLength / n); x++) // this will go block by block: b is the index of the block we are in
	{
		char block[n];
		// put the relevant part of input into block
		for (long m = 0; m < n; m++)
		{
			block[m] = input[(x * n) + m];
		}

		for (long i = 0; i < n; i++)
		{
			long j = ((a*i + b) % n);

			if (j < 0)
			{
				j+= n;
			}

			putchar(block[j]);
		}

	}
	return 0;
	//printf("\n");


	// printf("new input length: %d\n", inputLength);
	// printf("input: %s", input);
	// printf("n:%d, a:%d, b:%d\n", n, a, b);
}