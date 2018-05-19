#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "deck.h"

typedef struct bigcard bigCard;

struct bigcard
{
	Card myCard;
	bigCard *nextCard;
};

struct deck
{
	bigCard *frontCard;
	bigCard *backCard;

};

// Return true if deck is not empty.
// Running time should be O(1).
int deckNotEmpty(const Deck *d)
{
	if (d->frontCard != NULL)
	{
		return true;
	}

	else
	{
		return false;
	}
}

// Add a card to the bottom of a deck.
// This is not required to do anything special if the card is bogus
//   (e.g. "1C", "?X", "A-").
// Running time should be O(1).
void deckPutCard(Deck *d, Card c)
{
	bigCard *currentCard = malloc(sizeof(bigCard));
	currentCard->myCard = c;
	currentCard->nextCard = NULL;

	if (!deckNotEmpty(d)) // if we have an empty deck, then make this card the front AND back of the deck
	{
		d->frontCard = currentCard;
		d->backCard = currentCard;
	}

	else // otherwise, just update the back of the deck – no need to change the front
	{
		d->backCard->nextCard = currentCard;
		d->backCard = currentCard;
	}
}

// Create a new unshuffled deck of 52 cards,
// ordered by suit then rank:
//
// AC 2C 3C ... KC AD 2D 3D ... KD AH 2H 3H ... KS
Deck *deckCreate(void)
{
	Deck *currentDeck = malloc(sizeof(Deck));
	currentDeck->frontCard = NULL;
	currentDeck->backCard = NULL;

	for (int i = 0; i < 52; i++) // goes from first card to last card
	{
		Card currentCard;
		currentCard.suit = SUITS[i / 13];
		currentCard.rank = RANKS[i % 13];
		deckPutCard(currentDeck, currentCard);
	}

	return currentDeck;
}

// Free all space used by d.
// Running time should be O(length of deck)
void deckDestroy(Deck *d)
{
	if (d->frontCard != NULL) // only run if there are more cards to free
	{
		bigCard *temp;

		while (d->frontCard->nextCard != NULL) // if there are at least two cards to free, then make the second one the front and free the first one
		{
			temp = d->frontCard;
			d->frontCard = d->frontCard->nextCard;
			temp->nextCard = NULL;
			free(temp);
		}
		// now we have freed all but the front card
		temp = d->frontCard;
		d->frontCard = NULL;
		temp->nextCard = NULL;
		free(temp);
	}
	// now we have freed all the bigCards within the deck, so free the deck itself.
	free(d);
}

// Remove and return the top card of a deck.
// If deck is empty, behavior is undefined.
// Running time should be O(1).
Card deckGetCard(Deck *d)
{
	bigCard *oldFrontCard = d->frontCard;
	Card outputCard = oldFrontCard->myCard;

	// if there is another card after the top card, then set that one to be the front of the deck
	if (d->frontCard->nextCard != NULL)
	{
		d->frontCard = d->frontCard->nextCard;
	}
	else // if there is only one card, then set front and next to null
	{
		d->frontCard = NULL;
		// in this scenario, the deck still exists, but it's empty
	}

	free(oldFrontCard);
	return outputCard;
}

// Split a deck into two piles:
//    *d1 is new deck with first n cards in d.
//    *d2 is new deck with remaining cards in d.
// Order of cards is preserved.
// If d contains fewer than n cards, put them all in d1.
// Destroys d.
// Running time should be O(n).
void deckSplit(Deck *d, int n, Deck **d1, Deck **d2)
{
	Deck *deckOne = malloc(sizeof(Deck));
	deckOne->frontCard = NULL;
	Deck *deckTwo = malloc(sizeof(Deck));
	deckTwo->frontCard = d->frontCard;
	deckTwo->backCard = d->backCard; // deckTwo is now pretty much a copy of d in that they point to the same things

	d->frontCard = NULL;
	d->backCard = NULL; // now d pretty much points to nothing, so we can destroy it and not worry
	deckDestroy(d);

	Card temp;
	int counter = 0;

	// we break out of that while loop when: 
	// a) We have no more cards to split (when d/deckTwo contains fewer than n cards, in which case we're done since they're all in d1
	// b) we've finished adding n cards to deck one
	while (deckTwo->frontCard != NULL && counter < n)
	{
		temp = deckGetCard(deckTwo);
		deckPutCard(deckOne, temp);
		counter++;
	}
	
	*d1 = deckOne;
	*d2 = deckTwo;
}

// Shuffle two decks together by alternating cards from
// d1 and d2 to obtain new deck.
//
// If d1 is X X X X 
// and d2 is Y Y Y Y Y Y Y,
// return value is X Y X Y X Y X Y Y Y Y.
//
// If d1 is X X X X
// and d2 is Y Y,
// return value is X Y X Y X X.
//
// Running time should be O(length of shorter deck).
// Destroys d1 and d2.
Deck *deckShuffle(Deck *d1, Deck *d2)
{
	Deck *outputDeck = malloc(sizeof(Deck));
	outputDeck->frontCard = NULL;
	outputDeck->backCard = NULL;
	Card temp1;
	Card temp2;

	while (d1->frontCard != NULL && d2->frontCard != NULL)
	{
		temp1 = deckGetCard(d1);
		temp2 = deckGetCard(d2);
		deckPutCard(outputDeck, temp1);
		deckPutCard(outputDeck, temp2);
	}

	Deck *tempDeck = malloc(sizeof(Deck));
	tempDeck->frontCard = NULL;
	tempDeck->backCard = NULL;

	// once that operation is done, we've operated over the course of the shorter deck, so the next operation needs to be in constant time
	// outputDeck has the alternating cards, but not we need to put what's left of the remaining deck on the end of outputDeck

	if (d1->frontCard == NULL && d2->frontCard != NULL) // if d1 runs out of cards first AND d2 has more cards, add d2 to end of outputDeck
	{
		tempDeck->frontCard = d2->frontCard;
		tempDeck->backCard = d2->backCard;

		if (outputDeck->frontCard == NULL)
		{
			outputDeck->frontCard = tempDeck->frontCard;
			outputDeck->backCard = tempDeck->backCard;
		}

		else
		{
			outputDeck->backCard->nextCard = tempDeck->frontCard; // now the tail of outputDeck points to the front of d2
			outputDeck->backCard = tempDeck->backCard; // update outputDeck so the new last node is the end node of d2
		}
	}

	// if d2 runs out of cards AND d1 has more cards, add d1 to end of outputDeck
	else if (d2->frontCard == NULL && d1->frontCard != NULL) // if d2 runs out of cards first AND d1 has more cards, add d1 to end of outputDeck
	{
		tempDeck->frontCard = d1->frontCard;
		tempDeck->backCard = d1->backCard;

		if (outputDeck->frontCard == NULL)
		{
			outputDeck->frontCard = tempDeck->frontCard;
			outputDeck->backCard = tempDeck->backCard;
		}

		else
		{
			outputDeck->backCard->nextCard = tempDeck->frontCard; // now the tail of outputDeck points to the front of d1
			outputDeck->backCard = tempDeck->backCard; // update outputDeck so the new last node is the end node of d1
		}
	}

	// if both d1 and d2 are empty, then there are no more cards to add to outputDeck

	tempDeck->frontCard = NULL;
	tempDeck->backCard = NULL;
	deckDestroy(tempDeck);

	d1->frontCard = NULL;
	d1->backCard = NULL;
	d2->frontCard = NULL;
	d2->backCard = NULL;
	deckDestroy(d1);
	deckDestroy(d2);
	return outputDeck;
}

// Print the contents of deck to f as sequence of ranks/suits
// separated by spaces.
// Example output: "AS TC 9D 3H 5S" (without quotes)
// Running time should be O(length of deck).
void deckPrint(const Deck *d, FILE *f)
{
	if (d->frontCard != NULL) // ONLY PRINTS IF THERE ARE CARDS TO PRINT
	{
		bigCard *temp;  //Declare temp
		temp = d->frontCard;

		while(temp != NULL && temp->nextCard != NULL) // while there is a card at the current location and one after it
		{
			fprintf(f, "%c%c ", temp->myCard.rank, temp->myCard.suit);
			temp = temp->nextCard;
		}
		// printed everything besides the last card, so print that now
		fprintf(f, "%c%c", temp->myCard.rank, temp->myCard.suit);
	}
}

/*
int main(int argc, char **argv)
{
	printf("Creating a deck and printing it.\n");
	Deck *myDeck = deckCreate();
	deckPrint(myDeck, stdout);
	printf("\n\n");

	printf("Removing top card from the deck.\n");
	Card topCard;
	topCard = deckGetCard(myDeck);
	printf("Top Card: %c%c\n", topCard.rank, topCard.suit);
	deckPrint(myDeck, stdout);
	printf("\n\n");

	printf("Putting that card back on the bottom.\n");
	deckPutCard(myDeck, topCard);
	deckPrint(myDeck, stdout);
	printf("\n\n");

	printf("Splitting into 2 decks (top 17 cards into deck one).\n");
	Deck *deckOne;
	Deck *deckTwo;
	deckSplit(myDeck, 17, &deckOne, &deckTwo);
	printf("Deck One: ");
	deckPrint(deckOne, stdout);
	printf("\nDeck Two: ");
	deckPrint(deckTwo, stdout);
	printf("\n\n");

	printf("Shuffling them back together.\n");
	Deck *myNewDeck = deckShuffle(deckOne, deckTwo);
	deckPrint(myNewDeck, stdout);
	printf("\n\n");

	printf("Splitting into a nonempty deck and an empty deck.\n");
	Deck *nonEmptyDeck;
	Deck *emptyDeck;
	deckSplit(myNewDeck, 100000, &nonEmptyDeck, &emptyDeck);
	printf("Non-Empty Deck: ");
	deckPrint(nonEmptyDeck, stdout);
	printf("\nEmpty Deck: ");
	deckPrint(emptyDeck, stdout);
	printf("\n");

	deckDestroy(nonEmptyDeck);
	deckDestroy(emptyDeck);
	
	return 0;
}
*/