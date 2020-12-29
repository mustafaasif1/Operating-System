#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"

int main()
{
    FILE* fp,*store;
    fp = fopen("addresses.txt","r");
    store = fopen("BACKING_STORE_1.bin","r");
    char data[256], PhysicalMemory[64][256], *str;
    int lastIndex, i, j, logicalAddress, readOrWrite, value = 0, TotalReferences = 0, pageFaults = 0, pageTable[256];
    bool x = false;
    float hits = 0.0;
    
    for(i = 0; i < 256; i++)
        pageTable[i] = -1;

    printf("Logical Address | Physical Address | Read/Write | Value | Page Fault\n");

    while(!feof(fp))
    {
        fscanf(fp,"%x %d",&logicalAddress, &readOrWrite);
        logicalAddress = logicalAddress&0xFFFF;
        str = (readOrWrite == 0) ? "Read" : "Write";
        int last16 = ((1 << 16) - 1) & logicalAddress;
        int OffSets = ((1 << 8) - 1) & last16;
        int pageNumber = ((1 << 8) - 1) & (last16 >> (9 - 1));
        
        if(pageTable[pageNumber] == -1) {
            pageFaults++;
            fseek(store,pageNumber*256,SEEK_SET);
            fread(&PhysicalMemory[lastIndex],1,256,store);

            for(int i = 0; i < 256; i++)
                pageTable[i] = pageTable[i] == lastIndex ? -1 : pageTable[i];

            pageTable[pageNumber] = lastIndex;
            lastIndex = (lastIndex+1) % 64;
            value = PhysicalMemory[pageTable[pageNumber]][OffSets];
            value >= 0xFFFFFF00 ? value-= 0xFFFFFF00 : value;
            value = readOrWrite == 1 ? value/2 : value;
            PhysicalMemory[pageTable[pageNumber]][OffSets] = value;
            x = true;
        } else {
            value = PhysicalMemory[pageTable[pageNumber]][OffSets];
            value >= 0xFFFFFF00 ? value -= 0xFFFFFF00 : value;
            value = readOrWrite == 1 ? value/2 : value;
            PhysicalMemory[pageTable[pageNumber]][OffSets] = value;
            x = false;
            hits++;
        }

        TotalReferences++;
        printf("0x%x ", logicalAddress);
        printf(" 0x%x%x ", pageTable[pageNumber],OffSets);            
        printf(" %s ",str);
        printf(" 0x%x ", value);
        x == true ? printf(" Yes \n") : printf(" No \n");
    }
    
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Hit rate: %f\n",hits/TotalReferences * 100.0);
    printf("Fault rate: %f\n",100.0 - hits/TotalReferences * 100.0);
    return 0;
}

