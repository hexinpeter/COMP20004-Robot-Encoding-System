/* 
 COMP20007 Design of Algorithms 

 Read in an estimate (upper bound) of terrain heights (2D in row-major
 form), the actual terrain heights, a list of locations to visit in order, and 
 then traverse the actual grid in the order given recording actual heights.

 Write output to input file name with extension .bit.

 Also compute the battery use by the robot and print it to stdout.

 Author: Andrew Turpin (aturpin@unimelb.edu.au) && Xin He
 Date: May 2014

 Usage: see usage()
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "grid.h"
#include "bitfile.h"
#include "huffman.h"

#define MOVES_PER_BATTERY 25
#define PBFLIMIT 30
#define ASCII_ZERO 48

int storeLen(int len, char *output);
int intToPBF(int input, char *output, int outputLimit);
unsigned intToBin(unsigned k);
int storeData(unsigned data, char *output);
void printArray(char *array, int size);


/*
** Print usage message
*/
void
usage(char *exeName) { 
    fprintf(stderr,"Usage: %s estimateFileName actualFileName plotterFileName\n",exeName);
}


/*
** Read command line params, call search
*/
int 
main(int argc, char *argv[]) {

    if (argc != 4) {
        usage(argv[0]);
        return(-1);
    }

        // plotter is not really a "grid" but why not use the read routine
        // to save coding!
    grid_t *sonar   = readGrid(strcmp(argv[1],"-")?fopen(argv[1],"r"):stdin, TRUE);
    grid_t *actual  = readGrid(strcmp(argv[2],"-")?fopen(argv[2],"r"):stdin, TRUE);
    grid_t *plotter = readGrid(strcmp(argv[3],"-")?fopen(argv[3],"r"):stdin, FALSE);

    assert(sonar);
    assert(actual);
    assert(plotter);

        // create output file
    char i = argv[2][6];
    char output_file_name[] = "actual1.txt.bit";
    output_file_name[6] = i;
    
    bit_file_t *bitfile = BitFileOpen(output_file_name, BF_WRITE);
    assert(bitfile);

    uint32_t bits_sent=0, moves_made=0;
    data_t current_position = plotter->data[0];
    for(int i = 0 ; i < plotter->width * plotter->height ; i++) {
        data_t new_position = plotter->data[i];
        moves_made += gridDistance(plotter, current_position, new_position);
        current_position = new_position;
        
        //store the PBF of difference between sonar reading and actual height in tempArray
        char tempArray[PBFLIMIT];
        int inaccuracy = sonar->data[current_position] - actual->data[current_position];
        int numBits, i;
        numBits = intToPBF(inaccuracy, tempArray, PBFLIMIT);
        
        //put the PBF into output file
        for (i = 0; i < numBits; i++) {
            BitFilePutBit((int)tempArray[i] - ASCII_ZERO, bitfile);
        }
        
        bits_sent += numBits;
      }

    assert(!BitFileClose(bitfile));

    printf("M: %10d B: %10d Bat: %10.1f\n",
        moves_made, bits_sent, 
        (float)moves_made/(float)MOVES_PER_BATTERY + (float)bits_sent);

    return 0;
}

/*
Convert an integer into Peter's Binary Form, and stores the binary form into a charater array with each character storing one bit of information.
Peter's Binary Form is composed of two parts.
The length defining part is initially 3 bits, specifying the length of the data part. If the 3 bits are all 1 (i.e. the length is equal to or greater than 7), another 3 bits are added. The rule applies for the subsequent 3-bits parts. The value is read as the sum of all the 3-bits part.
The data part stores the binary form of the integer.
NOTE: if the integer is 0, it will be stored as 000, no data part will be stored
 
PARAMETERS: input - integer to be converted
            output - character array used to store PBF
            outputLimit - the size of the character array
 
RETURN: the total number of bits of the Peter's Binary Form
*/
int intToPBF(int input, char *output, int outputLimit) {
    if (input == 0) {
        sprintf(output, "00");
        return 2;
    }
    
    unsigned bin, counter;
    bin = intToBin(input);
    
    //get the length of the binary number
    int length = sprintf(output, "%d", bin);
    
    counter = storeLen(length, output);
    
    if (counter + length > outputLimit) {
        fprintf(stderr, "PBFLIMIT too small");
        EXIT_FAILURE;
    }
    
    counter += storeData(bin, output+counter);
    return counter;
}


/*
Convert a decimal integer into its binary form, stored still as a decimal integer
*/
unsigned intToBin(unsigned k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * intToBin(k / 2);
}


/*
Store the length defining part of PBF into the output character array
PARAMETERS: len - the length of data part
            output - output character array 
RETURN: the number of characters stored
*/
int storeLen(int len, char *output) {
    int parts = len/3 + 1, remainder, i, counter = 0;
    
    //except the last 3-bits part, all the other parts should be "111"
    for (i = parts; i > 1; i--) {
        sprintf(output, "11");
        counter += 2;
    }
    
    //now the last 3-bits part
    remainder = len % 3;
    remainder = intToBin(remainder);
    
    if ( 0 <= remainder && remainder < 10) {
        sprintf(output+counter, "0");
        counter += 1;
        sprintf(output+counter, "%d", remainder);
        counter += 1;
    } else if ( 10 <= remainder && remainder < 100) {
        sprintf(output+counter, "%d", remainder);
        counter += 2;
    } else {
        fprintf(stderr, "error occurs in storeLen funciton, remainder = %d", remainder);
        EXIT_FAILURE;
    }
    
    return counter;
}


/*
Store the data part of PBF into the output character array
PARAMETERS: data - data part in binary
            output - output character array
RETURN: the number of characters stored
*/
int storeData(unsigned data, char *output) {
    return sprintf(output, "%d", data);
}


/*
Print out a Character array of size "size"
Used for debugging
*/
void printArray(char *array, int size) {
    printf("the array is: \n");
    for (int i = 0; i < size; i++) {
        printf("%c", array[i]);
    }
    printf("\n");
}













