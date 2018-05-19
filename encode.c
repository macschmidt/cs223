#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	// int lettersBefore = 0;
	// bool firstUpper = false;
	int c;
	int d;
	bool firstUpper = false;

	while ((c = getchar()) != EOF) 
	{
		int first = 0;
		int length = 0;
		bool secondTime = true;

		if (isalpha(c))
		{
			if (isupper(c))
			{
				firstUpper = true;
			}
			else
			{
				firstUpper = false;
			}

			first = c;
			length++;

			while ((d = getchar()) != EOF && isalpha(d))
			{
				length++;

				if (secondTime) // if this is the second char, make it match the old first letter's case
				{
					if (firstUpper)
					{
						putchar(toupper(d));
						secondTime = false;
					}
					else
					{
						putchar(tolower(d));
						secondTime = false;
					}
				}

				else // otherwise this is the third or fourth or fifth etc. char, so just put it and get the next one
				{
					putchar(d);
				}
			}

			if (length != 1)
			{
				putchar(tolower(first));
			}
			else
			{
				putchar(first);
			}

			if (length < 4)
			{
				putchar('a');
				putchar('n');
			}
			else
			{
				putchar('o');
			}

			if (d != EOF) // if d is not the EOF, then we need to put it (no need to change case)
			{
				putchar(d);
			}
			else // if d is the EOF, then we need to quit
			{
				exit(0);
			}
		}

		else
		{
			putchar(c);
		}
	}
}