#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode a bit string
 */

int Test1()
{
    int                 i;
    Os1 *  pos = NULL;
    
    Check_DecodeBER(&pos, &Os1_descriptor, 0, "..\\data\\os1-1.bin");

    if (pos->length != 17) return 1;
    for (i=0; i<17; i++) {
        if (pos->data[i] != i+1) return 1;
    }

    A2C_Free_Os1(pos);

    return 0;
}

/*
 *  Test #2 - encode an octet string and compare to a known result
 */

int Test2()
{
    int                 i;
    Os1 *  pos = NULL;
    
    Check_DecodeBER_By1(&pos, &Os1_descriptor, 0, "..\\data\\os1-1.bin");

    if (pos->length != 17) return 1;
    for (i=0; i<17; i++) {
        if (pos->data[i] != i+1) return 1;
    }

    A2C_Free_Os1(pos);

    return 0;
}

/*
 *  Test #2 - encode an octet string and compare to a known result
 */

int Test3()
{
    Os1    os = {0};
    
    if (EncodeCompareToFile(&os, &Os1_descriptor, 0, 1, "..\\data\\os1-2.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

int Test4()
{
    Os2 *       pos2 = NULL;

    Check_DecodeBER(&pos2, &Os2_descriptor, 0, "..\\data\\os1-3.bin");

    A2C_Free_Os2(pos2);

    return 0;
}

/*
 *  
 */

int main(int argc, char * argv[])
{
    int         err = 0;

    argc;
    argv;

//    _CrtSetBreakAlloc(87);

    /*
     *  Start running tests - only report failures
     */

    err += Test1();
    err += Test2();
    err += Test3();
    err += Test4();

    err += MemoryCheck();
    
    exit(err);
}
