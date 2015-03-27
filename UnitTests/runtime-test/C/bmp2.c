#include <stdio.h>
#include "A2C.h"
#include "utility.h"

/*
 *  Validate two simple strings
 *      1. Empty
 *      2. 10 chars long
 */

int Test1()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    uint16              rgch[] = {0};
    uint16              rgch2[] = {1, 2, 0x33, 0x333, 0x20,
                                   0xff3, 0xe4ef, 0xf23, 0xe3, 'a', 0};
    A2C_BMP_STRING      bmp;
    A2C_BMP_STRING      bmp2;

    bmp.string = rgch;
    bmp2.string = rgch2;

    err = A2C_Validate(&bmp, &A2C_BMP_STRING_descriptor, &f);
    if ((err != A2C_ERROR_Success) && !f) return 1;


    err = A2C_Validate(&bmp2, &A2C_BMP_STRING_descriptor, &f);
    if ((err != A2C_ERROR_Success) && !f) return 1;

    return 0;
}

/*
 *  Now check for the different failure cases.
 */

int Test2()
{
    A2C_ERROR           err;
    int                 f = FALSE;
    A2C_BMP_STRING      bmp = {0};
    uint16              rgch[20] = {0};

    /*
     *  1.  null string
     */
    
    err = ValidateCompareToFile(&bmp, &A2C_BMP_STRING_descriptor, "../data/bmp2-1.bin");;
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  2.  0xffff
     */

    bmp.string = rgch;
    rgch[0] = 0xffff;

    err = ValidateCompareToFile(&bmp, &A2C_BMP_STRING_descriptor, "../data/bmp2-2.bin");
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  3.  0xfffe
     */

    bmp.string = rgch;
    rgch[0] = 0xfffe;

    err = ValidateCompareToFile(&bmp, &A2C_BMP_STRING_descriptor, "../data/bmp2-3.bin");
    if (err != A2C_ERROR_Success) return 1;

    /*
     *  4.  0xd900
     */

    bmp.string = rgch;
    rgch[0] = 0xd900;

    err = ValidateCompareToFile(&bmp, &A2C_BMP_STRING_descriptor, "../data/bmp2-4.bin");
    if (err != A2C_ERROR_Success) return 1;


    /*
     *  5.  0xfdd0
     */

    bmp.string = rgch;
    rgch[0] = 0xfdd0;

    err = ValidateCompareToFile(&bmp, &A2C_BMP_STRING_descriptor, "../data/bmp2-5.bin");
    if (err != A2C_ERROR_Success) return 1;


    /*
     *  6. Should succeed
     */

    rgch[0] = 35;
        
    err = A2C_Validate(&bmp, &A2C_BMP_STRING_descriptor, &f);
    if ((err != A2C_ERROR_Success) && !f) return 1;


    return 0;
}    

/*
 *  Driver
 */

int main(int argc, char * argv[])
{
    int         err = 0;

    /*
     *  Run the tests 
     */

    err += Test1();
    err += Test2();
    err += MemoryCheck();

    exit(err);
}
