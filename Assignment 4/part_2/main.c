#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"


int ExtractBit(int number, int k, int p) 
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
} 
int main() 
{
	FILE *store;
    store = fopen("BACKING_STORE_2.bin","r");
    int OffSetsInActualFrame, OffsetInL2PageTable, OffsetInL1PageTable;
    
    int* instruction;    
    char *start = "00C17C00";
    char *end = "00C193E8";
    int StartingAddress = (int)strtol(start, NULL, 16);
    int EndingAddress = (int)strtol(end, NULL, 16);
    char *x;
    int a = 0;
    
    while (StartingAddress != EndingAddress)
    {
    	fseek(store,StartingAddress,SEEK_SET);
    	fread(&instruction,1,4,store);
    	printf("%d\n", instruction);
    	//printf("%d\n", a);
    	a += 1;
    	StartingAddress += 8;
    }
    
    

    //PhysicalMemory >= 0xFFFFFF00 ? PhysicalMemory-= 0xFFFFFF00 : PhysicalMemory;
    

}
