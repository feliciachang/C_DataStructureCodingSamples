#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "deck.h"

// Standard suits and ranks
#define SUITS "CDHS"
#define RANKS "A23456789TJQK"

// A single card
// This is small enough that we usually pass it 
// around by copying instead of using pointers.
typedef struct cardNode{
	struct cardNode *next;
	Card c; //c is a card data type
} cardNode;

// A deck of cards
struct deck{
	cardNode *head;
	cardNode *tail;
	int length;
} ;

Deck *deckCreate(void)
{
	//making an empty deck
	Deck *d;
	d = malloc(sizeof(Deck));
	d->length = 52;
	d->head = NULL;
	d->tail = NULL;

	//make an initial linked list node
	// struct cardNode *temp = malloc(sizeof(struct cardNode));

	struct cardNode* temp = NULL;
	struct cardNode* temp2 = NULL;

	for (int i = 0; i < 4; i++){

		for (int j = 0; j < 13; j++){

			if(temp == NULL){
				temp = malloc(sizeof(cardNode));
				temp->c.rank = RANKS[j];
				temp->c.suit = SUITS[i];
				temp->next = NULL;
				d->head = temp;
			}
			else{
				temp2 = malloc(sizeof(cardNode));
				temp2->c.rank = RANKS[j];
				temp2->c.suit = SUITS[i];
				temp2->next = NULL;
				temp->next = temp2;
				temp = temp2;
			}
		}
	}

	d->tail = temp;
	return d;
}


// Free all space used by d.
// Running time should be O(length of deck)
void deckDestroy(Deck *d){

	//if(d->head = NULL){
	//	free(d);
	//}

	while(deckNotEmpty(d)){
		deckGetCard(d);
	}
	free(d->head);
	free(d);

}

// Return true if deck is not empty.
// Running time should be O(1).
int deckNotEmpty(const Deck *d){

	return (d->head != NULL);

}

// Remove and return the top card of a deck.
// If deck is empty, behavior is undefined.
// Running time should be O(1).
Card deckGetCard(Deck *d){

	/*
	if(d->head == NULL){
		return NULL;
	}
	*/
	//is this right??????????
	//if(d->head = NULL){
	//	return;
	//}

	if(d->head == NULL){
		Card c;
		c.suit = 0;
		c.rank = 0;
		return c;
	}


	Card c = d->head->c;
	cardNode *temp = d->head;
	d->head = d->head->next;
	d->length--;


	/*
	if (d->head == NULL){
		d->tail = NULL;
	}
	*/
	free(temp);
	return (c);

	//free(d->head);
	//condition checking if deck has only one element then head = tail??

	//condition checking if deck is empty and give error if not
	//just do return; 

}

// Add a card to the bottom of a deck.
// This is not required to do anything special if the card is bogus
//   (e.g. "1C", "?X", "A-").
// Running time should be O(1).
void deckPutCard(Deck *d, Card c){

	char *a = strchr(SUITS, c.suit);
	char *b = strchr(RANKS, c.rank);
	if(a == 0 || b == 0){
		return;
	}

	cardNode *n = malloc(sizeof(struct cardNode));
	//printf("malloced");
	n->c = c;
	//printf ("%c", n-c.suit);
	//printf("ahhhh");

	n->next = NULL;

	if(d->head == 0){
		d->head = n;
		d->tail = n;
		d->head->next = NULL;
		d->length++;
	}
	//if(d->length == 1)
	else{
		d->tail->next = n;
		d->tail = n;
		d->length++;
	}
	//do i need to define the length for all of them 

	//printf("%d\n", d->tail->next); //are we supposed to printf or return this 

	//DO I NEED THIS?
	/*
	for(int i = 0; i < 4; i++){
		if(c.suit != SUITS[i]){
			return;
		}
	}
	for(int i = 0; i < 12; i++){
		if(c.rank != RANKS[i]){
			return;
		}
	}
	*/
}

// Split a deck into two piles:
//    *d1 is new deck with first n cards in d.
//    *d2 is new deck with remaining cards in d.
// Order of cards is preserved.
// If d contains fewer than n cards, put them all in d1.
// Destroys d.
// Running time should be O(n).
void deckSplit(Deck *d, int n, Deck **d1, Deck **d2){
//d1 and d2 might not be empty???
	
	//struct deck *d1;
	*d1 = malloc(sizeof(struct deck));
	(*d1)->length = 0;
	(*d1)->head = NULL;
	(*d1)->tail = NULL;
	
	Card c;
	//struct cardNode* temp = NULL;
	//struct cardNode* temp2 = NULL; 

	int counter = 0;
	//struct cardNode* newDeck = NULL;

	//if length of d is less than n
	while(counter < n && deckNotEmpty(d)){
		//d1 = malloc(sizeof(struct deck) * d->length);
		c = deckGetCard(d);
		deckPutCard(*d1, c);
		//(*d1)->length++;
		counter++;
	}
	(*d1)->length = counter;
	//printf("LENGTH OF D1 %d\n", (*d1)->length);
	(*d2) = d;
	//printf("LENGTH OF D %d\n", d->length);
	(*d2)->length = d->length;
	//(d->length) = (*d2)->length;
	//(*d2)->length = d->length;
	//printf("LENGTH OF D2 %d\n", (*d2)->length);
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
Deck *deckShuffle(Deck *d1, Deck *d2){

	struct deck *newDeck;
	newDeck = malloc(sizeof(struct deck));
	newDeck->length = 0;
	newDeck->head = NULL;
	newDeck->tail = NULL;

	if(d2->head == NULL){
		newDeck = d1;
		newDeck->head = d1->head;
		newDeck->tail = d1->tail;
		return newDeck;
	}
	if(d1->head == NULL){
		newDeck = d2;
		newDeck->head = d2->head;
		newDeck->tail = d2->tail;
		return newDeck;
	}

	int shorter;
	Card c;
	//Deck *shorterpoint;
	Deck *longerpoint;
	if(d1->length < d2->length){

		shorter = d1->length;
		//shorterpoint = d1;
		longerpoint = d2;
	}
	else {
		shorter = d2->length;
		//shorterpoint = d2;
		longerpoint = d1;
	}
	//printf("newDeck")
	//printf("length: %d\n", d1->length);
	for(int i = 0; i < shorter; i++){
		c = deckGetCard(d1);
		//printf("%c%c \n", c.rank, c.suit);
		deckPutCard(newDeck, c);

		c = deckGetCard(d2);
		deckPutCard(newDeck, c);
	}
	//deckPrint(longerpoint, stdout);
	if(deckNotEmpty (longerpoint)){
		newDeck->tail->next = longerpoint->head;
		newDeck->tail = longerpoint->tail;
		newDeck->length += longerpoint->length;
	}
	/*
	if(deckNotEmpty (longerpoint)){
		newDeck->tail = longerpoint->head;
		newDeck->tail->next = longerpoint->head->next;
	}
	*/

	free(d1);
	free(d2);
	//printf("D SHUFFLE LENGTH: %d\n", newDeck->length);
	return newDeck;
}

// Print the contents of deck to f as sequence of ranks/suits
// separated by spaces.
// Example output: "AS TC 9D 3H 5S" (without quotes)
// Running time should be O(length of deck).
void deckPrint(const Deck *d, FILE *f){

	if(f==0) return;

	struct cardNode *temp;
	temp = d->head;

	while(temp != NULL){
		fprintf(f, "%c%c", temp->c.rank, temp->c.suit);
		if(temp->next != NULL){
			fprintf(f, " ");
		}
		temp = temp->next;
	}

	/*
	fprintf(f, "%c%c", temp->c.rank, temp->c.suit);
	while(temp->next != 0){
	fprintf(f, " ");
	temp = temp->next;
	}
	*/
}














