/************************** Start of BITIO.C *************************
 *
 * This utility file contains all of the routines needed to impement
 * bit oriented routines under either ANSI or K&R C.  It needs to be
 * linked with every program used in the entire book.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "bitio.h"
#include "errhand.h"

#define PACIFIER_COUNT 2047

/* open the output file by its name and return its pointer.*/

BIT_FILE *OpenOutputBitFile( name )
char *name;
{
    BIT_FILE *bit_file;

    bit_file = (BIT_FILE *) calloc( 1, sizeof( BIT_FILE ) );
    if ( bit_file == NULL )
        return( bit_file );
    bit_file->file = fopen( name, "wb" );
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->pacifier_counter = 0;
    return( bit_file );
}

/* open the input file by its name and return its pointer.*/

BIT_FILE *OpenInputBitFile( name )
char *name;
{
    BIT_FILE *bit_file;

    bit_file = (BIT_FILE *) calloc( 1, sizeof( BIT_FILE ) );
    if ( bit_file == NULL )
	return( bit_file );
    bit_file->file = fopen( name, "rb" );
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->pacifier_counter = 0;
    return( bit_file );
}

/* close the output file by its pointer.
   If there are some bits being not writed, write them into file.
*/
void CloseOutputBitFile( bit_file )
BIT_FILE *bit_file;
{
    if ( bit_file->mask != 0x80 )
        if ( putc( bit_file->rack, bit_file->file ) != bit_file->rack )
            fatal_error( "Fatal error in CloseBitFile!\n" );
    fclose( bit_file->file );
    free( (char *) bit_file );
}

/* close the input file by its pointer.*/
void CloseInputBitFile( bit_file )
BIT_FILE *bit_file;
{
    fclose( bit_file->file );
    free( (char *) bit_file );
}

/* Output the bit, and the bits are temporally stored in rack.
   bit: 0 or 1, indicating the bit to be writed.
   According to the bit argument, set the byte stored in rack.
   If the whole byte has been set, write it into the output file.
*/
void OutputBit( bit_file, bit )
BIT_FILE *bit_file;
int bit;
{
	/* If the bit is 1, set the corresponding bit in byte. Otherwise, leave it 0*/
    if ( bit )
        bit_file->rack |= bit_file->mask;
    bit_file->mask >>= 1;
	/* If we are done with the byte, write it into output file and reset the byte and mask*/
    if ( bit_file->mask == 0 ) {
	if ( putc( bit_file->rack, bit_file->file ) != bit_file->rack )
	    fatal_error( "Fatal error in OutputBit!\n" );
	else
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		putc( '.', stdout );
	bit_file->rack = 0;
	bit_file->mask = 0x80;
    }
}

/* Output bits of the code. 
   code: the code needed to be writed
   count: the number of bits in the code to be writed. 

*/
void OutputBits( bit_file, code, count )
BIT_FILE *bit_file;
unsigned long code;
int count;
{
    unsigned long mask;

	/* mask is used as a mask and a counter */
    mask = 1L << ( count - 1 );
    while ( mask != 0) {
        if ( mask & code )
            bit_file->rack |= bit_file->mask;
        bit_file->mask >>= 1;
	    /* If we are done with a byte, write it into output file and reset the byte and mask*/
        if ( bit_file->mask == 0 ) {
	    if ( putc( bit_file->rack, bit_file->file ) != bit_file->rack )
		fatal_error( "Fatal error in OutputBit!\n" );
        else if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		putc( '.', stdout );
	    bit_file->rack = 0;
            bit_file->mask = 0x80;
        }
        mask >>= 1;
    }
}

/* Return one bit of the input file.
   Read a byte from the input file and store it in rack untill we get all 8 bits. 
   Return the bit corresponding with the mask.
*/

int InputBit( bit_file )
BIT_FILE *bit_file;
{
    int value;

    /* Decide whether we need a new byte */
    if ( bit_file->mask == 0x80 ) {
        bit_file->rack = getc( bit_file->file );
        if ( bit_file->rack == EOF )
            fatal_error( "Fatal error in InputBit!\n" );
    if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
	    putc( '.', stdout );
    }
    /* Read one bit */
    value = bit_file->rack & bit_file->mask;
    bit_file->mask >>= 1;
    /* If we finish reading all 8 bits, reset the mask. */
    if ( bit_file->mask == 0 )
	bit_file->mask = 0x80;
    return( value ? 1 : 0 );
}

/* Return bit_count bits of the input file.
   Read a byte from the input file and store it in rack untill we get all 8 bits. 
*/
unsigned long InputBits( bit_file, bit_count )
BIT_FILE *bit_file;
int bit_count;
{
    unsigned long mask;
    unsigned long return_value;

	/* mask is used as a mask and a counter */
    mask = 1L << ( bit_count - 1 );
    return_value = 0;
    while ( mask != 0) {
    /* Decide whether we need a new byte */
	if ( bit_file->mask == 0x80 ) {
	    bit_file->rack = getc( bit_file->file );
	    if ( bit_file->rack == EOF )
		//fatal_error( "Fatal error in InputBit!\n" );
		return 256;
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		putc( '.', stdout );
	}
    /* Read one bit and set the return value*/
	if ( bit_file->rack & bit_file->mask )
            return_value |= mask;
        mask >>= 1;
        bit_file->mask >>= 1;
    /* If we finish reading all 8 bits, reset the mask. */
        if ( bit_file->mask == 0 )
            bit_file->mask = 0x80;
    }
    return( return_value );
}

/* Write bits nunmber of code with binary format into file. */
void FilePrintBinary( file, code, bits )
FILE *file;
unsigned int code;
int bits;
{
    unsigned int mask;

    mask = 1 << ( bits - 1 );
    while ( mask != 0 ) {
        if ( code & mask )
            fputc( '1', file );
        else
            fputc( '0', file );
        mask >>= 1;
    }
}

/*************************** End of BITIO.C **************************/

