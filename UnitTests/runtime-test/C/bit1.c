#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode a bit string
 */

int Test1()
{
    B2 * pbs;
    
    Check_DecodeBER(&pbs, &B2_descriptor, 0, DATA_DIR "bit1-1.bin");

    if (pbs->bitLength != 10) return 1;
    if (pbs->bufferLength != 2) return 1;
    if (pbs->data[0] != 0xaa) return 1;
    if (pbs->data[1] != 0x80) return 1;

    A2C_Free_B2(pbs);


    Check_DecodeBER(&pbs, &B3_descriptor, 0, DATA_DIR "bit1-2.bin");

    if (pbs->bitLength != 9) return 1;
    if (pbs->bufferLength != 2) return 1;
    if (pbs->data[0] != 0xaa) return 1;
    if (pbs->data[1] != 0x80) return 1;

    A2C_Free_B2(pbs);

    return 0;
}

/*
 *  Test #2 - encode a bit string and compare to a known result
 */

int Test2()
{
    B2      bs = {0};
    BYTE                rgb[] = { 0xaa, 0x80 };

    /*
     *  Encode 0 bits
     */

    if (EncodeCompareToFile(&bs, &B2_descriptor, 0, 1, DATA_DIR "bit1-4.bin") != A2C_ERROR_Success) return 1;

    /*
     *  Check for triming on DER encoding
     */

    bs.bitLength = 10;
    bs.bufferLength = 2;
    bs.data = rgb;

    if (EncodeCompareToFile(&bs, &B2_descriptor, 0, 1, DATA_DIR "bit1-1.bin") != A2C_ERROR_Success) return 1;

    if (EncodeCompareToFile(&bs, &B3_descriptor, 0, 1, DATA_DIR "bit1-2.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  First decode complex bitstring
 */

int Test3()
{
    B2 * pbs;

    Check_DecodeBER(&pbs, &B2_descriptor, 0, DATA_DIR "bit1-3.bin");

    if (pbs->bitLength != 44) return 1;
    if (pbs->bufferLength != 6) return 1;
    if (pbs->data[0] != 0x0a) return 1;
    if (pbs->data[1] != 0x3b) return 1;
    if (pbs->data[2] != 0x5f) return 1;
    if (pbs->data[3] != 0x29) return 1;
    if (pbs->data[4] != 0x1c) return 1;
    if (pbs->data[5] != 0xd0) return 1;

    A2C_Free_B2(pbs);

    return 0;
}

int Test4()
{
    B1 *        pb1 = NULL;

    Check_DecodeBER(&pb1, &B1_descriptor, 0, DATA_DIR "bit1-5.bin");

    if (pb1->b1.bitLength != 1) return 1;
    if (pb1->b1.bufferLength != 1) return 1;
    if (pb1->b1.data[0] != 0x80) return 1;

    A2C_Free_B1(pb1);

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
