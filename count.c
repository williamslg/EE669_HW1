#include <stdio.h>
#include <string.h>
#include "count.h"
#include "bitio.h"

void CharCount(char *argv)
{
	int i, sum=0;
	unsigned long byte;
	BIT_FILE *input_file;
	//get pointers of the input file and output file by argument from command line
	input_file = OpenInputBitFile(argv);

	//initialize symboy counter array symbo[NUM]
	memset(symbol, 0, sizeof(int)*NUM);
	while (1) {
		byte = InputBits(input_file, 8);
        symbol[byte]++;		
		if (byte == 256) {
			break;
		}
	}
    for (i=0; i<NUM; i++) {
		sum += symbol[i];
	}
	printf("total weight: %d\n", sum);
}
