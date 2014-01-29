#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "bitio.h"
#include "hw1.h"
#include "count.h"
#include "huff.h"

int main(int argc, char *argv[])
{
	CommandProcess(argc, argv);
	CharCount(argv[1]);
	List list;
	ListInit(&list);
	ListDisplay(&list);

	Node *root = TreeBuild(&list);
	Table table[NUM];
	memset(table, 0, sizeof(Table)*NUM);

	CodeCreate(root, 0, 0, 0, table);
	int i;
	for (i=0; i<NUM; i++) {
		printf("symbol: %x code: %x length: %d weight: %d\n", i, table[i].code, table[i].length, table[i].weight);
	}
    return 0;
}

void Usage()
{
	fprintf(stderr, "usage: ./hw1 inputfile outputfile\n");
	exit(1);
}

void CommandProcess(int argc, char *argv[])
{
	struct stat info;
	if (argc != 3) {
		Usage();
	}
	else {
		stat(argv[1], &info);
		if (!S_ISREG(info.st_mode)) {
			fprintf(stderr, "Please check your input file name!\n");
			Usage();
		}
	}
}
