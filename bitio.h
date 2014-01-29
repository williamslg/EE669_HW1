/************************** Start of BITIO.H *************************/

#ifndef _BITIO_H
#define _BITIO_H

#include <stdio.h>
/* file: pointer to input/output filemask 
   mask: Initialized as 0x80(1000000), shifted and used to find bit from rack
   rack: a byte read from file
   pacifier_counter: counter to count number of byte that has been read
*/

typedef struct bit_file {
    FILE *file;
    unsigned char mask;
    int rack;
    int pacifier_counter;
} BIT_FILE;

#ifdef __STDC__

BIT_FILE     *OpenInputBitFile( char *name );
BIT_FILE     *OpenOutputBitFile( char *name );
void          OutputBit( BIT_FILE *bit_file, int bit );
void          OutputBits( BIT_FILE *bit_file,
                          unsigned long code, int count );
int           InputBit( BIT_FILE *bit_file );
unsigned long InputBits( BIT_FILE *bit_file, int bit_count );
void          CloseInputBitFile( BIT_FILE *bit_file );
void          CloseOutputBitFile( BIT_FILE *bit_file );
void          FilePrintBinary( FILE *file, unsigned int code, int bits );

#else   /* __STDC__ */

BIT_FILE     *OpenInputBitFile();
BIT_FILE     *OpenOutputBitFile();
void          OutputBit();
void          OutputBits();
int           InputBit();
unsigned long InputBits();
void          CloseInputBitFile();
void          CloseOutputBitFile();
void          FilePrintBinary();

#endif  /* __STDC__ */

#endif  /* _BITIO_H */

/*************************** End of BITIO.H **************************/

