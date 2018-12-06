//for my numMultiply function, I drew heavily from the geeks for geeks website on multiplying large numbers.
//https://www.geeksforgeeks.org/multiply-large-numbers-represented-as-strings/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "num.h"

//#define MAX(a,b) a>b ? a : b

/*
 * High-precision arithmetic on non-negative number in base 10.
 */

//typedef struct num Num;

struct num {
	int length;
	int *data;
};

/* constructs a Num from a string */
/* string contains representation of the number
 * in base 10, e.g. numCreate("314159");
 * Returns a null pointer (0) if the string contains any non-digits.
 * Leading zeros are OK: numCreate("012") parses as 12.
 * Empty string parses as 0 */
Num * numCreate(const char *s)
{
	//printf("creating %s\n", s);
	Num *s2;

	s2 = malloc(sizeof(Num));
	if(s2 == 0) { 
		return 0; 
	}

	int i = 0;
	while(strlen(s) > i && s[i] == '0'){ //checking for leading zeros and make sure string length is less than i
		i++;
	}

	int length = strlen(s) - i; //get the proper length without leading zeros

	if(length == 0){ //if string is empty because of 0's length will be zero and must return 0
		s2->length = 1;
		s2->data = malloc(sizeof(int) * 1);
		s2->data[0] = 0;
		return s2;     //p=0 test is still not working...
	}

	s2->length = length; //malloc proper string length
	s2->data = malloc(sizeof(int) * length);

	int j = i;
	
	int str_len = strlen(s);
	while (j < str_len){ //save new array at correct index
		if(!isdigit(s[j])){
			numDestroy(s2);
			return NULL;
		}
		s2->data[j-i] = s[j]-'0'; //j-i becuase j iterates while i stays the same 
		j++;                      
	}
	return s2;
}


/* Free all resources used by a Num */
void numDestroy(Num *n){
	free(n->data);
	free(n);
}
/*
 Get the value of the i-th least significant digit of a Num.
 * Returns 0 if i is out of range.
 * Example:
 *   n = numCreate("12345");
 *   numGetDigit(n, 0) == 5
 *   numGetDigit(n, 3) == 2
 *   numGetDigit(n, 17) == 0
 *   numGetDigit(n, -12) == 0
 */
int numGetDigit(const Num *n, int i){

	if(i >= n->length || i < 0){
		return 0;
	}
	else {
		return n->data[n->length - i - 1];
	}

}


/* add two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num * numAdd(const Num *x, const Num *y){

	//printf("xlength: %d\n", x->length);
	//printf("ylength: %d\n", y->length);
	//printf("max: %d\n", MAX(x->length, y->length));
	
	int newlength;

	if(x->length > y->length){
		newlength = x->length +1;
	}
	else{
		newlength = y->length +1;
	}
	
	//newlength = (MAX(x->length, y->length)) + 1;
	//printf("HERE IS THING%d\n", newlength);

	int carry = 0;
	int intermediate = 0;
	int output = 0;

	Num *addedvalue = malloc(sizeof(Num));
	addedvalue->data = malloc(sizeof(int) * newlength);

	//or Num *addedvalue = numCreate("0");

	addedvalue->length = newlength;

	int ydiff = 0;
	int xdiff = 0;

	if(y->length < x->length){
		ydiff = x->length - y->length;
	}
	else if (x->length < y->length){
		xdiff = y->length - x->length;
	}

	//printf("xdiff: %d\n", xdiff);
	//printf("ydiff: %d\n", ydiff);

	for(int i=0; i<newlength; i++){
		intermediate = 0;
		output = 0;
		if(i < y->length){
			intermediate = (y->data[newlength - i - 2 - ydiff] );
		}
		if (i < x->length){
			intermediate = (x->data[newlength - i - 2 - xdiff] + intermediate);
		}
		intermediate += carry;
		//printf("intermediate: %d\n", intermediate);

		output = intermediate%10;

		//printf("output: %d\n", output);

		carry = intermediate / 10;

		//mprintf("carry: %d\n", carry);

		addedvalue->data[newlength-i-1] = output;
	}

	// leading zero count
	int j = 0;
	while(addedvalue->length > j && addedvalue->data[j] == 0){ //checking for leading zeros and make sure string length is less than i
		j++;
	} 

	// new data array without eading zeroes
	int *finalarray;
	int finallength;
	finallength = addedvalue->length - j;
	finalarray = malloc(sizeof(int) * finallength);
	int k;
	k = j;
	while (k < addedvalue->length){
		finalarray[k-j] = addedvalue->data[k];
		k++;
	}

	// update addedvalue
	addedvalue->length = finallength;
	free(addedvalue->data); //added
	addedvalue->data = finalarray;

	return addedvalue;
}


/* multiply two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num * numMultiply(const Num *x, const Num *y){

	int newlength = (x->length + y->length + 1);
	//printf("%d\n", newlength);

	//int intermediate;

	Num *multipliedvalue = malloc(sizeof(Num));
	multipliedvalue->data = malloc(sizeof(int) * newlength);

	multipliedvalue->length = newlength;

	if(x->length == 0 || y->length == 0){
		return 0; //revise
	}

	if(y->data[0] == 0 || x->data[0] == 0){
		multipliedvalue->data[0] = 0;
		multipliedvalue->length = 1;
		return multipliedvalue;
	}

	int resultx = 0;
	int resulty = 0;

	for(int i = 0; i < multipliedvalue->length; i++){
		multipliedvalue->data[i] = 0;
	}
	//new version
	for(int i = (x->length -1); i>=0; i--){
		int carry=0;
		int xvalue = x->data[i];
		for(int j = (y->length - 1); j>=0; j--){
			int yvalue = y->data[j];
			int sum = (xvalue * yvalue) + multipliedvalue->data[resultx + resulty] + carry;
			carry = sum/10;
			multipliedvalue->data[resultx + resulty] = sum%10;
			resulty++;
		}	
		if (carry>0){
			multipliedvalue->data[resultx + resulty] += carry;
		}
		resultx++;
		resulty = 0;
	}

	int *temparray;
	temparray = malloc(sizeof(int)*multipliedvalue->length);

	for(int i = 0; i<multipliedvalue->length; i++){
		temparray[i] = multipliedvalue->data[multipliedvalue->length - i -1];
	}

	int j = 0;
	while(multipliedvalue->length > j && temparray[j]==0){
		j++;
	}

	int *finalarray;
	int finallength;
	finallength = multipliedvalue->length -j;
	finalarray = malloc(sizeof(int) * finallength);
	int k;
	k = j;
	while(k<multipliedvalue->length){
		finalarray[k-j] = temparray[k];
		k++;
	}
	free(multipliedvalue->data);
	multipliedvalue->data = finalarray;
	multipliedvalue->length = finallength;
	

	free(temparray);
	return multipliedvalue;
}

/* Print the digits of a number to file.
 * Do not print any leading zeros unless n is zero. */
void numPrint(const Num *n, FILE *f){

	if(f == 0){
		printf("error");
		exit(1);
	}
/*
	int i = 0;
	while(n->length > i && n->data[i] == 0){ //checking for leading zeros and make sure string length is less than i
		i++;
	}

	int j = i;*/
	int j = 0;

	while(j < n->length) {
		fprintf(f, "%d", n->data[j]);
		j++;
	}
}








