/** @file 
 * Formats C objects to strings readable by MMA's Get[ ]
 * Doc is in header file.
 * 
 * Tyson Jones 2017
 * tyson.jones@materials.ox.ac.uk
 */
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mmaformatter.h"


const char* BASE_TEN_FORMAT = "*10^";
const char* ARRAY_DELIM_CHARS = ", ";
const char* ARRAY_OUTER_CHARS[] = {"{", "}"};


char* getScientificNotation(double number, int precision) {
	
	// build sci-not format string
	int precDigits = 1 + ((precision>0)? floor(log10(abs(precision))) : 0);
	int formatStrSize = sizeof(char)*(precDigits + 3 + 1); // % d . e NULL
	char *formatStr = malloc(formatStrSize);
	sprintf(formatStr, "%%.%de", precision); // e.g. %.5e
	
	// build sci-not string with e
	ssize_t rawStrSize = snprintf(NULL, 0, formatStr, number);
	char *rawStr = malloc(rawStrSize + 1); // NULL char
	sprintf(rawStr, formatStr, number); // e.g. 1.23e+03
	
	// find the number of chars in our strings/substrings
	int numBase10Chars = strlen(BASE_TEN_FORMAT);
	int rawStrNumChars = rawStrSize/sizeof(char);
	ssize_t newStrSize = rawStrSize + sizeof(BASE_TEN_FORMAT) - sizeof(char);
	
	// build sci-not string with *10^ by...
	char *string = malloc(newStrSize + 1); // NULL
	
	// copying old chars up until e
	int rawInd=0;
	while (rawStr[rawInd] != 'e') {
		string[rawInd] = rawStr[rawInd];
		rawInd += 1;
	}
	// copying *10^ over
	for (int copyInd=0; copyInd < numBase10Chars; copyInd++)
		string[rawInd+copyInd] = BASE_TEN_FORMAT[copyInd];
		
	// copying over the rest of the original string (the exponent)
	for (rawInd++; rawInd < rawStrNumChars; rawInd++)
		string[rawInd + numBase10Chars - 1] = rawStr[rawInd];
	
	// clear 
	free(formatStr);
	free(rawStr);
	
	return string;
}


char* convertDoubleArrToMMA(double* array, int length, int precision) {
	
	// calculate array string length
	int sizeOfLeftBrace = strlen(ARRAY_OUTER_CHARS[0]);
	int sizeOfRightBrace = strlen(ARRAY_OUTER_CHARS[1]);
	int sizeOfDelim = strlen(ARRAY_DELIM_CHARS);
	int sizeOfNum = 11 + precision; // -1.(precision)*10^+(2 or 3)
	int sizeOfTerm = sizeOfNum + sizeOfDelim;
	int sizeOfArr = (
		+ sizeOfLeftBrace			// {
		+ sizeOfTerm*(length - 1)	// 	a, b, c,
		+ sizeOfNum					//  d
		+ sizeOfRightBrace);		// }
	
	char* arrString = malloc(sizeOfArr * sizeof(char));
	memset(arrString, '\0', sizeOfArr);
	
	// writte open-array character
	int strInd = 0;
	for (int outerInd=0; outerInd < sizeOfLeftBrace; outerInd++)
		arrString[strInd + outerInd] = ARRAY_OUTER_CHARS[0][outerInd];
	strInd += sizeOfLeftBrace;
	
	// for each number in the array
	for (int arrInd=0; arrInd < length; arrInd++) {
		
		// convert it to scientific notation
		char* numStr = getScientificNotation(array[arrInd], precision);
		int lenNumStr = strlen(numStr);
		
		// write the sci-not number to the string
		for (int charInd=0; charInd < lenNumStr; charInd++)
			arrString[strInd + charInd] = numStr[charInd];
		strInd += lenNumStr;
		
		// write the array delimiter to the string (except for last)
		if (arrInd < length - 1) {
			for (int delimInd=0; delimInd < sizeOfDelim; delimInd++)
				arrString[strInd + delimInd]  = ARRAY_DELIM_CHARS[delimInd];
			strInd += sizeOfDelim;
		}
	}
	
	// write the close-array chars to the string
	for (int outerInd=0; outerInd < sizeOfRightBrace; outerInd++)
		arrString[strInd + outerInd] = ARRAY_OUTER_CHARS[1][outerInd];
	strInd += sizeOfRightBrace;
	
	return arrString;
}


FILE* openAssocWrite(char* filename) {
	
	FILE* file = fopen(filename, "w");
	fprintf(file, "<|\n");
	return file;
}


void writeIntToAssoc(FILE* file, char* keyname, int num) {
	
	fprintf(file, "\"%s\" -> %d,\n", keyname, num);
}


void writeDoubleToAssoc(FILE* file, char* keyname, double num, int precision) {
	
	char* sciNotStr = getScientificNotation(num, precision);
	fprintf(file, "\"%s\" -> %s,\n", keyname, sciNotStr);
	free(sciNotStr);
}


void writeStringToAssoc(FILE* file, char* keyname, char* string) {
	
	fprintf(file, "\"%s\" -> \"%s\",\n", keyname, string);
}


void writeIntArrToAssoc(FILE* file, char* keyname, int* arr, int length) {
	
	fprintf(file, "\"%s\" -> {", keyname);
	for (int i=0; i < length-1; i++)
		fprintf(file, "%d, ", arr[i]);
	fprintf(file, "%d},\n", arr[length-1]);
}


void writeDoubleArrToAssoc(FILE* file, char* keyname, double* arr, int length, int precision) {
	
	char* arrStr = convertDoubleArrToMMA(arr, length, precision);
	fprintf(file, "\"%s\" -> %s,\n", keyname, arrStr);
	free(arrStr);
}


void writeUnsignedLongArrToAssoc(FILE* file, char* keyname, unsigned long *arr, int length) {
	
	fprintf(file, "\"%s\" -> {", keyname);
	for (int i=0; i < length-1; i++)
		fprintf(file, "%lu, ", arr[i]);
	fprintf(file, "%lu},\n", arr[length-1]);
}


void closeAssocWrite(FILE* file) {
	
	// delete comma (and newline necessarily)
	fseek(file, -2, SEEK_END);
	
	// restore newline and close Associonary
	fprintf(file, "\n|>");
	fclose(file);
}


/*
 * tests equality, then frees the first argument
 */
void check(char *stringToFree, char *staticString) {
	assert(strcmp(stringToFree, staticString) == 0);
	free(stringToFree);
}


void unitTests() {
	
	// reals (scientific notation)
	check(getScientificNotation(-.25, 4),"-2.5000*10^-01");
	check(getScientificNotation(1, 1),"1.0*10^+00");
	check(getScientificNotation(9, 0),"9*10^+00");
	check(getScientificNotation(143E-110, 1), "1.4*10^-108");
	
	// arrays
	double nums[] = {.555555, 1, -0.0000000123};
	int someInts[] = {4, 3, 2};
	printf(convertDoubleArrToMMA(nums, 3, 2)); printf("\n");
	
	
	check(
		convertDoubleArrToMMA(nums, 3, 2), 
		"{5.56*10^-01, 1.00*10^+00, -1.23*10^-08}");
	
	// Associonaries
	FILE* file = openAssocWrite("mytestfile.txt");
	writeDoubleArrToAssoc(file, "AssocArr", nums, 3, 4);
	writeIntArrToAssoc(file, "anIntArr", someInts, 3);
	writeIntToAssoc(file, "anInt", -123);
	writeDoubleToAssoc(file, "aDouble", -.0000005232, 3);
	closeAssocWrite(file);
	 
}


/*
int main (int narg, char **varg) {
	unitTests();
	
	double data[5] = {1.013, 10.331, .59102, 5.0, 6.0};
	
	FILE* file = openAssocWrite("assoc.txt");
	writeStringToAssoc(file, "patient", "Tyson");
	writeIntToAssoc(file, "trial", 112);
	writeDoubleArrToAssoc(file, "conductivity", data, 5, 2);
	closeAssocWrite(file);
	
	return EXIT_SUCCESS;
}
*/