#include <stdio.h>
#include "A2C.h"
#include "utility.h"

/*
 *  No failure modes for native integer
 */

int Test1()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_INTEGER_NATIVE  i = {9};

    err = A2C_Validate(&i, &A2C_INTEGER_NATIVE_descriptor, &f);

    if ((err != A2C_ERROR_Success) && !f) return 1;

    return 0;
}

/*
 *   INTEGER_HUGE
 */

int Test2()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_INTEGER_HUGE    i = {0};
    BYTE                rgb[20] = {0};

    /*
     *  Should succeed - no
     */
    
    err = ValidateCompareToFile(&i, &A2C_INTEGER_HUGE_descriptor, "../data/int2-1.bin");
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  Compare all errors to the file
     */

    i.hLength = 10;

    err = ValidateCompareToFile(&i, &A2C_INTEGER_HUGE_descriptor, "../data/int2-2.bin");
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  Should succeed
     */

    i.hData = rgb;
    err = A2C_Validate(&i, &A2C_INTEGER_HUGE_descriptor, &f);
    if ((err != A2C_ERROR_Success) && !f) return 1;
    
    return 0;
}    

/*
 *  Driver
 */

int main(int argc, char * argv[])
{
    int         err = 0;

    argc;
    argv;

    /*
     *  Run the tests 
     */

    err += Test1();
    err += Test2();
    err += MemoryCheck();

    exit(err);
}
