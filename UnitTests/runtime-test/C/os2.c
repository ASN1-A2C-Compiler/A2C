#include <stdio.h>
#include <A2C.h>
#include "utility.h"

/*
 *  No failure modes for OCTET STRING
 */

int Test1()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_OCTET_STRING    os = {0, 0};
    BYTE                rgb[10] = {0};

    /*
     *  Empty - this validates
     */
    
    err = A2C_Validate(&os, &A2C_OCTET_STRING_descriptor, &f);

    if ((err != A2C_ERROR_Success) && !f) return 1;

    /*
     *  Length no buffer
     */

    os.length = 10;

    err = ValidateCompareToFile(&os, &A2C_OCTET_STRING_descriptor, "../data/os2-1.bin");
    if (err != A2C_ERROR_Success) return 1;


    /*
     *  Length and buffer
     */

    os.data = rgb;
    
    err = A2C_Validate(&os, &A2C_OCTET_STRING_descriptor, &f);

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
    err += MemoryCheck();

    exit(err);
}
