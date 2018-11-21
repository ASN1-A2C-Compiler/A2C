#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode a NULL
 */

int Test1()
{
    N1 * pf;
    
    Check_DecodeBER(&pf, &N1_descriptor, 0, DATA_DIR "null1-1.bin");

    A2C_Free_N1(pf);

    return 0;
}

/*
 *  Test #2 - encode a NULL and compare to a known result
 */

int Test2()
{
    N1         f;

    // Encode as BER

    Check_EncodeBER(&f, &N1_descriptor, 0, DATA_DIR "null1-1.bin");

    //  Encode as DER

    Check_EncodeDER(&f, &N1_descriptor, 0, DATA_DIR "null1-1.bin");

    return 0;
}

int Test3()
{
    N2* pn2 = NULL;

    Check_DecodeBER(&pn2, &N2_descriptor, 0, DATA_DIR "null1-2.bin");

    A2C_Free_N2(pn2);

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
