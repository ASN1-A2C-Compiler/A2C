#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Just decode an integer
 */

int Test1()
{
    I1*  pi1;
    
    Check_DecodeBER(&pi1, &I1_descriptor, 0, "..\\data\\int1-1.bin");

    if (pi1->nData != 10) return 1;

    A2C_Free_I1(pi1);

    return 0;
}

/*
 *  Test #2 - encode an integer and compare to a known result
 */

int Test2()
{
    I1  i1;

    i1.nData = 10;

    Check_EncodeDER(&i1, &I1_descriptor, 0, "..\\data\\int1-1.bin");

    return 0;
}

/*
 *  Test #3 - Run a set of numbers dealing with X.690 encoding rules
 */

int Test3()
{
    S1*  ps1;
    
    Check_DecodeBER(&ps1, &S1_descriptor, 0, "..\\data\\int1-3.bin");

    if (ps1->i1.nData != 0xffffff7f) return 1;
    if (ps1->i2.nData != 0xffffffff) return 1;
    if (ps1->i3.nData != 0x000000ff) return 1;
    if (ps1->i4.nData != 0x0000007f) return 1;

    A2C_Free_S1(ps1);
    return 0;
}

/*
 *  Test #4 - Run a set of numbers dealing with sign extensions
 */

int Test4() 
{
    S1  s1;

    s1.i1.nData = 0xffffff7f;
    s1.i2.nData = 0xffffffff;
    s1.i3.nData = 0x000000ff;
    s1.i4.nData = 0x0000007f;

    Check_EncodeDER(&s1, &S1_descriptor, 0, "..\\data\\int1-3.bin");

    return 0;
}

/*
 *  First simple test - Just decode a huge integer
 */

int Test5()
{
    I2*  pi2;
    
    Check_DecodeBER(&pi2, &I2_descriptor, 0, "..\\data\\int1-1.bin");

    if (pi2->hLength != 1) return 1;
    if (pi2->hData[0] != 10) return 1;

    A2C_Free_I2(pi2);

    return 0;
}

/*
 *  Test #6 - encode an integer and compare to a known result
 */

int Test6()
{
    I2  i1;
    BYTE        rgb[] = {10};

    i1.hLength = 1;
    i1.hData = rgb;

    Check_EncodeDER(&i1, &I2_descriptor, 0, "..\\data\\int1-1.bin");

    return 0;
}

/*
 *  Test #7 - Run a set of numbers dealing with X.690 encoding rules
 */

int Test7()
{
    I4*  ps1;
    
    Check_DecodeBER(&ps1, &I4_descriptor, 0, "..\\data\\int1-3.bin");

    if (ps1->i1.hLength != 2) return 1;
    if (ps1->i1.hData[0] != 0xff) return 1;
    if (ps1->i1.hData[1] != 0x7f) return 1;

    if (ps1->i2.hLength != 1) return 1;
    if (ps1->i2.hData[0] != 0xff) return 1;
    
    if (ps1->i3.hLength != 2) return 1;
    if (ps1->i3.hData[0] != 0) return 1;
    if (ps1->i3.hData[1] != 0xff) return 1;

    if (ps1->i4.hLength != 1) return 1;
    if (ps1->i4.hData[0] != 0x0000007f) return 1;

    A2C_Free_I4(ps1);
    return 0;
}

/*
 *  Test #8 - Run a set of numbers dealing with sign extensions
 */

int Test8() 
{
    I4  s1;
    BYTE        rgb[] = {0xff, 0xff, 0xff, 0x7f,
                         0xff, 0xff, 0xff, 0xff,
                         0x00, 0x00, 0x00, 0xff,
                         0x00, 0x00, 0x00, 0x7f
    };

    s1.i1.hLength = 4;
    s1.i1.hData = &rgb[0]; //0xffffff7f;
    s1.i2.hLength = 4;
    s1.i2.hData = &rgb[4]; // 0xffffffff;
    s1.i3.hLength = 4;
    s1.i3.hData = &rgb[8]; // 0x000000ff;
    s1.i4.hLength = 4;
    s1.i4.hData = &rgb[12]; // 0x0000007f;

    Check_EncodeDER(&s1, &I4_descriptor, 0, "..\\data\\int1-3.bin");

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
    err += Test5();
    err += Test6();
    err += Test7();
    err += Test8();
    err += MemoryCheck();
    
    exit(err);
}
