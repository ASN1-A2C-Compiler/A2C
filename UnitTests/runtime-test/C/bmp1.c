#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode an empty string
 */

int Test1()
{
    A2C_BMP_STRING * pbmp;
    
    Check_DecodeDER(&pbmp, &A2C_BMP_STRING_descriptor, 0, "..\\data\\bmp1-1.bin");

    if (pbmp->string == NULL) return 1;
    if (pbmp->string[0] != 0) return 1;

    A2C_Free_BMP_STRING(pbmp);

    return 0;
}

#if 0
/*
 *  Test #2 - encode a bit string and compare to a known result
 */

int Test2()
{
    A2C_BMP_STRING      bs = {0};
    BYTE                rgb[] = { 0xaa, 0x80 };

    /*
     *  Encode 0 bits
     */

    if (EncodeCompareToFile(&bs, &A2C_BMP_STRING_descriptor, 0, 1, "..\\data\\bit1-4.bin") != A2C_ERROR_Success) return 1;

    /*
     *  Check for triming on DER encoding
     */

    bs.bitLength = 10;
    bs.bufferLength = 2;
    bs.data = rgb;

    if (EncodeCompareToFile(&bs, &A2C_BMP_STRING_descriptor, 0, 1, "..\\data\\bit1-1.bin") != A2C_ERROR_Success) return 1;

    if (EncodeCompareToFile(&bs, &A2C_BMP_STRING_NAMED_descriptor, 0, 1, "..\\data\\bit1-2.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  First decode complex bitstring
 */

int Test3()
{
    A2C_BMP_STRING * pbs;

    if (DecodeFromFile(&pbs, &A2C_BMP_STRING_descriptor, 0, "..\\data\\bit1-3.bin") != A2C_ERROR_Success) {
        return 1;
    }

    if (pbs->bitLength != 44) return 1;
    if (pbs->bufferLength != 6) return 1;
    if (pbs->data[0] != 0x0a) return 1;
    if (pbs->data[1] != 0x3b) return 1;
    if (pbs->data[2] != 0x5f) return 1;
    if (pbs->data[3] != 0x29) return 1;
    if (pbs->data[4] != 0x1c) return 1;
    if (pbs->data[5] != 0xd0) return 1;

    A2C_Free_BMP_STRING(pbs);

    return 0;
}
#endif 

/*
 *  
 */

int main(int argc, char * argv[])
{
    int         err = 0;

    /*
     *  Start running tests - only report failures
     */

    err += Test1();
//    err += Test2();
//    err += Test3();

    err += MemoryCheck();
    
    exit(err);
}
