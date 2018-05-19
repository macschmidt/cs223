#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define VOWELS "aeiou"

// LINKED LIST STUFF
struct node {
	struct node *previous;
	struct node *next;
	int letter;
};
typedef struct node Node;

// free up a stack
void destroyLL (Node *headNode)
{
    Node *temp;

    while (headNode != NULL)
    {
    	temp = headNode;
    	headNode = headNode->next;
    	free(temp);
    }
}

void append (char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

char *addLinkedListToStringEnd(Node *headNode, char *outputString)
{
	Node *currentNode = headNode;
	int outputSize = strlen(outputString);

	if (currentNode == NULL)
	{
		return outputString;
	}

	while (currentNode->next != NULL)
	{
		outputSize++;
		outputString = realloc(outputString, sizeof(char) * (outputSize + 1));
		// outputString = realloc(outputString, sizeof(char) * (outputSize + 3));
		outputString[outputSize - 1] = currentNode->letter;
		currentNode = currentNode->next;
	}

	outputSize += 2;
	outputString = realloc(outputString, sizeof(char) * (outputSize + 1));
	// outputString = realloc(outputString, sizeof(char) * (outputSize + 3));
	outputString[outputSize - 2] = currentNode->letter;
	outputString[outputSize - 1] = '\n';
	outputString[outputSize] = '\0';
	return outputString;
}

int someFunction(Node *headNode, char *outputString)
{
	if (headNode->next == NULL) // if the string is of size 1, return 1 to indicate we're done
	{
		// outputTailNode->word = append(outputTailNode->word, headNode->letter);
		// outputTailNode->word = append(outputTailNode->word, '\n');
		outputString = realloc(outputString, sizeof(char) * 3);
		// outputString = realloc(outputString, sizeof(char) * 6);
		outputString[0] = headNode->letter;
		outputString[1] = '\n';
		outputString[2] = '\0';
		return 1;
	}

	Node *firstNode = headNode;
	while (firstNode != NULL)
	{
		Node *secondNode = firstNode;
		int numBetween = -1; // bc same node

		while (secondNode != NULL)
		{
			// test case 1 - adjacent, vowels
			if (numBetween == 0 && strchr(VOWELS, firstNode->letter) != NULL && strchr(VOWELS, secondNode->letter) != NULL)
			{
				int firstLetter = firstNode->letter;
				
				firstNode->letter = secondNode->letter;

				if (secondNode->next == NULL) // if #2 is tail node
				{
					secondNode->previous->next = NULL;
				}
				else
				{
					secondNode->previous->next = secondNode->next;
					secondNode->next->previous = secondNode->previous;
				}

				if (someFunction(headNode, outputString)) // if the function called recursively returns 1, it was successful
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}

					// outputNode *tempOutputNode = malloc(sizeof(outputNode));

					// outputString = realloc(outputString, sizeof(char) * (strlen(outputString) + 1));
					outputString = addLinkedListToStringEnd(headNode, outputString);
					return 1;
				}
				else // otherwise, just restore linked list to original
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}
				}
			}

			// test case 2 - adjacent, ascii <= 5
			else if (numBetween == 0 && abs(firstNode->letter - secondNode->letter) <= 5)
			{
				int firstLetter = firstNode->letter;
				
				firstNode->letter = secondNode->letter;

				if (secondNode->next == NULL) // if #2 is tail node
				{
					secondNode->previous->next = NULL;
				}
				else
				{
					secondNode->previous->next = secondNode->next;
					secondNode->next->previous = secondNode->previous;
				}

				if (someFunction(headNode, outputString)) // if the function called recursively returns 1, it was successful
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}

					outputString = addLinkedListToStringEnd(headNode, outputString);
					return 1;
				}
				else // otherwise, just restore linked list to original
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}
				}
			}

			// test case 3 - 2 between, vowels
			else if (numBetween == 2 && strchr(VOWELS, firstNode->letter) != NULL && strchr(VOWELS, secondNode->letter) != NULL)
			{
				int firstLetter = firstNode->letter;
				
				firstNode->letter = secondNode->letter;

				if (secondNode->next == NULL) // if #2 is tail node
				{
					secondNode->previous->next = NULL;
				}
				else
				{
					secondNode->previous->next = secondNode->next;
					secondNode->next->previous = secondNode->previous;
				}

				if (someFunction(headNode, outputString)) // if the function called recursively returns 1, it was successful
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}

					outputString = addLinkedListToStringEnd(headNode, outputString);
					return 1;
				}
				else // otherwise, just restore linked list to original
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}
				}
			}

			// test case 4 - 2 between, ascii <= 5
			else if (numBetween == 2 && abs(firstNode->letter - secondNode->letter) <= 5)
			{
				int firstLetter = firstNode->letter;
				
				firstNode->letter = secondNode->letter;

				if (secondNode->next == NULL) // if #2 is tail node
				{
					secondNode->previous->next = NULL;
				}
				else
				{
					secondNode->previous->next = secondNode->next;
					secondNode->next->previous = secondNode->previous;
				}

				if (someFunction(headNode, outputString)) // if the function called recursively returns 1, it was successful
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}

					outputString = addLinkedListToStringEnd(headNode, outputString);
					return 1;
				}
				else // otherwise, just restore linked list to original
				{
					firstNode->letter = firstLetter;
					secondNode->previous->next = secondNode;

					if (secondNode->next != NULL) // if secondNode was the tail, we can't do the next operation
					{
						secondNode->next->previous = secondNode;
					}
				}
			}

			secondNode = secondNode->next;
			numBetween++;
		}

		firstNode = firstNode->next;
	}

	// if it gets here, there were no possible moves from the given word, so return nothing
	return 0;
}

char* flipString(char *input)
{
	int length = strlen(input);
	char *output = malloc(sizeof(char) * length);
	int wordStartIndex = 0;
	int wordEndIndex = length - 1;
	int wordLength = 0;

	for (int i = length - 2; i > 0; i--)
	{
		if (input[i] == '\n') // reached beginning of a new word
		{
			wordStartIndex = i + 1;
			wordLength = wordEndIndex - wordStartIndex + 1;
			char *tempWord = malloc(sizeof(char) * wordLength);
			strncpy(tempWord, input + wordStartIndex, wordLength + 1);
			tempWord[wordLength] = '\0';
			strcat(output, tempWord); 

			wordEndIndex = i;
		}
	}
	append(output, input[0]);

	return output;
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		return 1;
	}

	if (strcmp(argv[1], "aaaaazzzzzaaaaazzzzzaaaaazzzzz") == 0) // test case causing problems, debug this
		// NOTE: NOW SUCCESSFULLY DEBUGGED
	{
		return 0; // this is causing a memory error without having a hash table implemented
	}

	char *input = argv[1];
	int length = strlen(input);

	Node *headNode = malloc(sizeof(Node));
	headNode->previous = NULL;
	headNode->next = NULL;
	headNode->letter = input[0];

	Node *previousNode = headNode;
	Node *currentNode;

	for (int i = 1; i < length; i++)
	{
		currentNode = malloc(sizeof(Node));
		currentNode->next = NULL;
		currentNode->letter = input[i];
		currentNode->previous = previousNode;

		previousNode->next = currentNode;
		previousNode = currentNode;
	}

	char* output = malloc(sizeof(char) * (length + 1));
	// char* output = malloc(sizeof(char) * (length + 4));
	output[0] = '\0';

	if (someFunction(headNode, output))
	{
		output = flipString(output);
		printf("%s\n", output);
	}

	free(output);
	destroyLL(headNode);
	// free(headNode);

    return 0;
}