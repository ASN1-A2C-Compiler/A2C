#include <stdio.h>
#include "A2C.h"
#include "utility.h"

/*
 *  
 */

int Test1()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_BIT_STRING      bs = {0, 0, 0};

    err = A2C_Validate(&bs, &A2C_BIT_STRING_descriptor, &f);

    if ((err != A2C_ERROR_Success) && !f) return 1;

    return 0;
}

/*
 *  
 */

int Test2()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_BIT_STRING      bs = {0};
    BYTE                rgb[20] = {0};

    /*
     *  Should succeed? - yes
     */
    
    err = A2C_Validate(&bs, &A2C_BIT_STRING_descriptor, &f);
    if ((err != A2C_ERROR_Success) && !f) return 1;

    /*
     *  1.  # bits > # bytes
     */

    bs.bitLength = 20;

    err = ValidateCompareToFile(&bs, &A2C_BIT_STRING_descriptor, "../data/bit2-1.bin");
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  Should succeed
     */

    bs.bufferLength = 3;
    bs.data = rgb;
        
    err = A2C_Validate(&bs, &A2C_BIT_STRING_descriptor, &f);
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
