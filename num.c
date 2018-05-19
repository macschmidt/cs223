#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "num.h"

// #define TEST_ARRAY_LENGTH (10)

// #define BASE (10)

struct num
{
	int numDigits;
	int *digits;
};
//typedef struct num Num;

/* constructs a Num from a string */
/* string contains representation of the number
 * in base 10, e.g. numCreate("314159");
 * Returns a null pointer (0) if the string contains any non-digits.
 * Leading zeros are OK: numCreate("012") parses as 12.
 * Empty string parses as 0 */
Num *numCreate(const char *s)
{
	bool firstDigit = true;
	Num *currentNum = malloc(sizeof(Num));
	currentNum->numDigits = strlen(s);
	int startingLength = currentNum->numDigits;
	currentNum->digits = malloc(sizeof(int));
	int index = 0;

	// if it's an empty string, then it parses as 0
	if (currentNum->numDigits == 0)
	{
		currentNum->digits[0] = 0;
	}

	else
	{
		// otherwise, we're dealing with a legit number, so let's allocate the correct amount of memory and put the numbers in
		for (int i = 0; i < startingLength; i++)
		{
			char currentDigit = s[i];

			// only check stuff if we're dealing with a digit, otherwise return null pointer
			if (!isdigit(currentDigit))
			{
				return NULL;
				break;
			}

			// ignore leading zeros
			if (firstDigit)
			{
				if (currentDigit == '0')
				{
					// don't add it, just iterate and continue (firstDigit is still true)
					// if we're being smart, let's update the number of digits - 1 b/c that number isn't necessary
					currentNum->numDigits--;
				}

				else // otherwise, treat it normally and add it. We also need to now set firstDigit to false
				{
					currentNum->digits = realloc(currentNum->digits, sizeof(int) * (i + 1));
					currentNum->digits[index] = currentDigit - '0';
					firstDigit = false;
					index++;
				}
			}

			else // otherwise, we aren't dealing with the first digit, so no need to check for leading zeros
			{
				currentNum->digits = realloc(currentNum->digits, sizeof(int) * (i + 1));
				currentNum->digits[index] = currentDigit - '0';
				index++;
			}
		}
		// if I break out of this and firstDigit is still true, then we have only seen 0s
		if (firstDigit)
		{
			currentNum->digits[0] = 0;
			currentNum->numDigits = 1;
		}
	}
	return currentNum;
}

/* Free all resources used by a Num */
void numDestroy(Num *n)
{
	free(n->digits);
	free(n);
}

/* Get the value of the i-th least significant digit of a Num.
 * Returns 0 if i is out of range.
 * Example:
 *   n = numCreate("12345");
 *   numGetDigit(n, 0) == 5
 *   numGetDigit(n, 3) == 2
 *   numGetDigit(n, 17) == 0
 *   numGetDigit(n, -12) == 0
 */
int numGetDigit(const Num *n, int i)
{
	if (i > n->numDigits || i < 0)
	{
		return 0;
	}

	else
	{
		int digit = n->digits[n->numDigits - 1 - i];
		return digit;
	}
}

/* add two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num *numAdd(const Num *x, const Num *y)
{
	bool xLonger = false;
	int difference;
	int longerLength;

	if (x->numDigits > y->numDigits)
	{
		xLonger = true;
		longerLength = x->numDigits;
		difference = (x-> numDigits - y->numDigits);
	}
	else if (x->numDigits < y->numDigits)
	{
		longerLength = y->numDigits;
		difference = (y->numDigits - x->numDigits);
	}
	else // lengths are equal
	{
		difference = 0;
		longerLength = x->numDigits; // doesn't matter which I choose, since they're the same length
	}

	// need to pad a second one with leading zeroes
	// let's create a temporary integer array for the shorter one
	int *tempShorter = malloc(sizeof(int) * longerLength);
	// pad zeroes to front
	for (int i = 0; i < difference; i++) // won't do this if equal length
	{
		tempShorter[i] = 0;
	}

	// copy in rest of shorter array
	if (xLonger)
	{
		for (int i = difference; i < longerLength; i++)
		{
			tempShorter[i] = y->digits[i - difference];
		}
	}
	else
	{
		for (int i = difference; i < longerLength; i++)
		{
			tempShorter[i] = x->digits[i - difference];
		}
	}
	// now we have equal sized arrays to do math with, so let's do the addition
	// we're going to add digit by digit, right to left, and put the resulting digit into the array of our output
	// int outputArray[longerLength + 1]; // make room for the extra digit if we need it
	int *outputArray = malloc(sizeof(int) * (longerLength + 1));
	outputArray[0] = 0; // by default, the first character is a zero, but this will change if we need to carry
	bool carry = false;

	if (xLonger) // add the stuff in x with tempShorter to outputArray
	{
		for (int i = longerLength - 1; i >= 0; i--) // right to left
		{
			if (carry)
			{
				tempShorter[i]++;
			}

			int currOutputDigit = x->digits[i] + tempShorter[i];

			outputArray[i + 1] = currOutputDigit % 10;

			if (currOutputDigit > 9)
			{
				carry = true;
			}
			else
			{
				carry = false;
			}
		}

		if (carry)
		{
			outputArray[0] = 1;
		}
	}

	else // add the stuff in y with tempShorter
	{
		for (int i = longerLength - 1; i >= 0; i--) // right to left
		{
			if (carry)
			{
				tempShorter[i]++;
			}

			int currOutputDigit = y->digits[i] + tempShorter[i];

			outputArray[i + 1] = currOutputDigit % 10;

			if (currOutputDigit > 9)
			{
				carry = true;
			}
			else
			{
				carry = false;
			}
		}
		if (carry)
		{
			outputArray[0] = 1;
		}
	}

	char *outputString = malloc(sizeof(char) * (longerLength + 2));

	for (int i = 0; i < longerLength + 1; i++)
	{
		outputString[i] = outputArray[i] + '0';
	}
	outputString[longerLength + 1] = 0; // null terminating
	free(outputArray);

	// now we just create and return the num
	Num *output = numCreate(outputString);
	free(outputString);
	free(tempShorter);

	return output;
}

/* multiply two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num *numMultiply(const Num *x, const Num *y)
{
	bool xLonger = false;
	int longerLength;
	int shorterLength;

	if (x->numDigits > y->numDigits)
	{
		xLonger = true;
		longerLength = x->numDigits;
		shorterLength = y->numDigits;
	}
	else
	{
		longerLength = y->numDigits;
		shorterLength = x->numDigits;
	}

	Num *finalOutput = numCreate("0");

	int numZeros = 0;

	if (xLonger)
	{
		int carry = 0;

		for (int i = shorterLength - 1; i >= 0; i--)
		{
			int thisPartialSumDigits = longerLength + numZeros + 1;
			int partialSum[thisPartialSumDigits]; // keep track of the current round of digit multiplication here
			// can only ever have longer + shorter length digits for the temp sums, which is same as longer + num zeros + 1
			// put the 0s at the end of partialSum
			for (int z = 0; z < numZeros; z++)
			{
				partialSum[longerLength + numZeros - z] = 0;
			}

			for (int j = longerLength - 1; j >= 0; j--)
			{
				int currentNum = x->digits[j] * y->digits[i] + carry;
				partialSum[j + 1] = currentNum % 10;
				carry = currentNum / 10; // truncates, truncates to 0 if currentNum/10 < 10
			}

			partialSum[0] = carry;
			char* tempString = malloc(sizeof(char));

			for (int q = 0; q < thisPartialSumDigits; q++)
			{
				tempString = realloc(tempString, sizeof(char) + q + 1);
				tempString[q] = partialSum[q] + '0';
			}
			tempString[thisPartialSumDigits] = 0;

			Num *tempNum = numCreate(tempString);
			free(tempString);
			finalOutput = numAdd(finalOutput, tempNum);
			numDestroy(tempNum);

			// output += partialSumNum;
			numZeros++;
			carry = 0;
		}
	}

	else // y is longer or they're equal length, so just adjust accordingly
	{
		int carry = 0;

		for (int i = shorterLength - 1; i >= 0; i--) // this is where we are in x
		{
			int thisPartialSumDigits = longerLength + numZeros + 1;
			int partialSum[thisPartialSumDigits]; // keep track of the current round of digit multiplication here
			// can only ever have longer + shorter length digits for the temp sums, which is same as longer + num zeros + 1
			// put the 0s at the end of partialSum

			for (int z = 0; z < numZeros; z++)
			{
				partialSum[thisPartialSumDigits - numZeros + z] = 0;
			} 

			int index = 0;
			for (int j = longerLength - 1; j >= 0; j--) // where we are in y
			{
				int currentNum = y->digits[j] * x->digits[i] + carry;
				partialSum[thisPartialSumDigits - 1 - numZeros - index] = currentNum % 10;
				carry = currentNum / 10; // truncates, truncates to 0 if currentNum/10 < 10
				index++;
			}

			partialSum[0] = carry;
			char* tempString = malloc(sizeof(char));

			for (int q = 0; q < thisPartialSumDigits; q++)
			{
				tempString = realloc(tempString, sizeof(char) + q + 1);
				tempString[q] = partialSum[q] + '0';
			}
			tempString[thisPartialSumDigits] = 0;

			Num *tempNum = numCreate(tempString);
			free(tempString);

			Num *tempOutput = numAdd(finalOutput, tempNum);

			finalOutput->numDigits = tempOutput->numDigits;
			finalOutput->digits = realloc(finalOutput->digits, sizeof(int) * finalOutput->numDigits);
			for (int s = 0; s < tempOutput->numDigits; s++)
			{
				finalOutput->digits[s] = tempOutput->digits[s];
			}

			numDestroy(tempNum);
			numDestroy(tempOutput);

			// output += partialSumNum;
			numZeros++;
			carry = 0;
		}
	}
	return finalOutput;
}

/* Print the digits of a number to file.
 * Do not print any leading zeros unless n is zero. */
void numPrint(const Num *n, FILE *f)
{
	for (int i = 0; i < n->numDigits; i++)
	{
		fprintf(f, "%d", n->digits[i]);
	}
}
/*
int main(int argc, char **argv)
{
	Num *test = numCreate("00999");
	printf("Number of digits in test: %d\n", test->numDigits);
	printf("Test: ");
	numPrint(test, stdout);
	printf("\n\n");
	
	Num *num1 = numCreate("123467890");
	printf("Number of digits in num1: %d\n", num1->numDigits);
	printf("num1: ");
	numPrint(num1, stdout);
	printf("\n\n");

	Num *num2 = numCreate("987654321");
	printf("Number of digits in num2: %d\n", num2->numDigits);
	printf("num2: ");
	numPrint(num2, stdout);
	printf("\n\n");

	Num *num3 = numAdd(num1, num2);
	printf("Number of digits in sum: %d\n", num3->numDigits);
	printf("Sum of num1 and num2: ");
	numPrint(num3, stdout);
	printf("\n\n");
	
	Num *num4 = numMultiply(num1, num2);
	printf("Number of digits in product: %d\n", num4->numDigits);
	printf("Product of num1 and num2: ");
	numPrint(num4, stdout);
	printf("\n");

	numDestroy(test);
	numDestroy(num1);
	numDestroy(num2);
	numDestroy(num3);
	numDestroy(num4);
	
	return 0;
}
*/