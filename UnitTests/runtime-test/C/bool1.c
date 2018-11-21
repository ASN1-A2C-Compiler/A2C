#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode a Boolean
 */

int Test1()
{
    Bo1 * pf;
    
    Check_DecodeBER(&pf, &Bo1_descriptor, 0, "..\\data\\bool1-1.bin");

    if (pf->data != 5) return 1;

    A2C_Free_Bo1(pf);

    return 0;
}

/*
 *  Test #2 - encode a boolean and compare to a known result
 */

int Test2()
{
    Bo1         f;

    f.data = 5;

    // Encode as BER

    Check_EncodeBER(&f, &Bo1_descriptor, 0, "..\\data\\bool1-2.bin");

    //  Encode as DER

    Check_EncodeDER(&f, &Bo1_descriptor, 0, "..\\data\\bool1-2.bin");

    return 0;
}

int Test3()
{
    Bo2 *       pbo2 = NULL;

    Check_DecodeBER(&pbo2, &Bo2_descriptor, 0, "..\\data\\bool1-3.bin");

    if (pbo2->b1.data != 1) return 1;
    if (pbo2->b2.data != 1) return 1;

    A2C_Free_Bo2(pbo2);

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
    err += MemoryCheck();
    
    exit(err);
}
