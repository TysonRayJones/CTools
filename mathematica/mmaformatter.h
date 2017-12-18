#ifndef MMA_FORMATTER_H_
#define MMA_FORMATTER_H_

/**
 * returns 'number' as a MMA compatible scientific notation string 
 * with 'precision' digits after the decimal point. The returned string
 * must be freed.
 * 
 * @brief converts double to a scientific notation string
 * @param number		the double to convert
 * @param precision	number of digits after decimal point in sci-not
 * @return 				the sci-not string e.g."-4.321*10^-03", which
 * 						should be freed
 */
char* getScientificNotation(double number, int precision);

/**
 * returns a string containing a MMA array of numbers in
 * scientific notation with `precision` digits after each decimal point.
 * The returned string must be freed.
 * 
 * @brief converts double array to a MMA array string of sci-not decimals
 * @param array			an array of doubles to convert to sci-not
 * @param length		number of doubles in array
 * @param precision	number of digits after decimal point in sci-not
 * @return 				a MMA array of sci-not e.g."{1.2*10^+3, ...}",
 * 						which should be freed
 */
char* convertDoubleArrToMMA(double* array, int length, int precision);

/**
 * returns a file handle to be passed to subsequent functions for
 * writing data to a MMA Association. File handle must be eventually
 * passed to closeAssocWrite.
 * 
 * @brief begins writing an association to file
 * @param filename		name of the file to save the MMA association to.
 * 						The assocation is read into MMA by Get[filename]
 * @return 				a file handle to pass to subsequent Association
 * 						Write functions, and which must be eventually
 * 						passed to closeAssocWrite
 */
FILE* openAssocWrite(char* filename);

/**
 * @brief adds an int to the association
 * @param file		file handle returned by openAssocWrite
 * @param keyname	key to add to the association
 * @param num		integer to add under key in association
 */
void writeIntToAssoc(FILE* file, char* keyname, int num);

/**
 * @brief adds a sci-not number to the association
 * @param file			file handle returned by openAssocWrite
 * @param keyname		key to add to the association
 * @param num			number to add in sci-not to the association
 * @param precision	number of digits after decimal point in sci-not
 */
void writeDoubleToAssoc(FILE* file, char* keyname, double num, int precision);

/**
 * @brief adds a string to the association
 * @param file		file handle returned by openAssocWrite
 * @param keyname	key to add to the association
 * @param string	string to add under key in the association
 */
void writeStringToAssoc(FILE* file, char* keyname, char* string);

/**
 * @brief adds a MMA array of integers to the association
 * @param file		file handle returned by openAssocWrite
 * @param keyname	key to add to the association
 * @param arr		array of ints to add under key
 * @param length	length of the array of ints
 */
void writeIntArrToAssoc(FILE* file, char* keyname, int* arr, int length);

/**
 * @brief adds an array of long unsigned ints to the association 
 * 		  as a MMA array of integers
 * @param file		file handle returned by openAssocWrite
 * @param keyname	key to add to the association
 * @param arr		array of long unsigned ints to add under key
 * @param length	length of the array
 */
void writeUnsignedLongArrToAssoc(FILE* file, char* keyname, unsigned long *arr, int length);

/**
 * @brief adds a MMA array of sci-not numbers to the association
 * @param file			file handle returned by openAssocWrite
 * @param keyname		key to add to the association
 * @param arr			array of doubles to convert to sci-not
 * @param length		length of the array
 * @param precision	number of digits after decimal in sci-not
 */
void writeDoubleArrToAssoc(FILE* file, char* keyname, double* arr, int length, int precision);

/**
 * @brief formats and finalises the association, so it is ready
 * 		   to read by MMA's Get[filename] function
 * @param file	file handle returned by openAssocWrite
 */
void closeAssocWrite(FILE* file);
	
#endif // MMA_FORMATTER_H_

