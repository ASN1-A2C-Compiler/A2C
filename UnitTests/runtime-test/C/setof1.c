#include <stdio.h>
#include <crtdbg.h>
#include "asn.h"
#include "utility.h"

/*
 *  FIrst simple test - Decode an empty SEQ OF
 */

int Test1()
{
    StO1 *  psto1;

    Check_DecodeBER(&psto1, &StO1_descriptor, 0, "..\\data\\sto1-1.bin");

    if (psto1->count != 0) return 1;

    A2C_Free_StO1(psto1);

    return 0;
}

/*
 *  Test #2 - encode empty and compare to known result
 */

int Test2()
{
    StO1 sto1;

    sto1.count = 0;

    Check_EncodeDER(&sto1, &StO1_descriptor, 0, "..\\data\\sto1-1.bin");

    Check_EncodeBER(&sto1, &StO1_descriptor, 0, "..\\data\\sto1-2.bin");
    
    return 0;
}

/*
 *  Test#3 - decode a sequence of integers
 */

int Test3()
{
    StO1 *  psto1;

    Check_DecodeBER(&psto1, &StO1_descriptor, 0, "..\\data\\sto1-3.bin");

    if (psto1->count != 5) return 1;
    if (psto1->array[0].nData != 3) return 1;
    if (psto1->array[1].nData != 5) return 1;
    if (psto1->array[2].nData != 5) return 1;
    if (psto1->array[3].nData != 10) return 1;
    if (psto1->array[4].nData != -1) return 1;

    A2C_Free_StO1(psto1);

    Check_DecodeBER(&psto1, &StO1_descriptor, 0, "..\\data\\sto1-4.bin");

    if (psto1->count != 5) return 1;
    if (psto1->array[0].nData != 5) return 1;
    if (psto1->array[1].nData != 3) return 1;
    if (psto1->array[2].nData != -1) return 1;
    if (psto1->array[3].nData != 10) return 1;
    if (psto1->array[4].nData != 5) return 1;

    A2C_Free_StO1(psto1);

    return 0;
}

/*
 *  Test #4 - encode empty and compare to known result
 */

int Test4()
{
    StO1 sto1;
    A2C_INTEGER_NATIVE  rg[] = {5, 3, -1, 10, 5 };

    sto1.count = 5;
    sto1.array = rg;


    Check_EncodeDER(&sto1, &StO1_descriptor, 0, "..\\data\\sto1-3.bin");

    Check_EncodeBER(&sto1, &StO1_descriptor, 0,  "..\\data\\sto1-4.bin");
    
    return 0;
}

/*
 *  Test #5 - make sure that recursion on release is done
 */

int Test5()
{
    StO2 *  psto2;

    Check_DecodeBER(&psto2, &StO2_descriptor, 0, "..\\data\\sto1-6.bin");

    if (psto2->count != 2) return 1;

    A2C_Free_StO2(psto2);

    return 0;
}

/*
 *  Test #6 - SEQ OF SEQ OF INT - encode and decode
 */

int Test6()
{
    StO3 *       psto3;
    StO3         sto3;
    StO1         rgsto1[3];
    A2C_INTEGER_NATIVE rg[] = {5, 0, 8, 13, -1, 2, 15, 3, 18, 20};

    sto3.count = 3;
    sto3.array = rgsto1;

    rgsto1[0].count = 2;
    rgsto1[0].array = rg;

    rgsto1[1].count = 5;
    rgsto1[1].array = &rg[1];

    rgsto1[2].count = 10;
    rgsto1[2].array = rg;

    Check_EncodeDER(&sto3, &StO3_descriptor, 0, "..\\data\\sto1-7.bin");

    Check_EncodeBER(&sto3, &StO3_descriptor, 0, "..\\data\\sto1-8.bin");

    Check_DecodeBER(&psto3, &StO3_descriptor, 0, "..\\data\\sto1-7.bin");

    if (psto3->count != 3) return 1;

    A2C_Free_StO3(psto3);


    Check_DecodeBER(&psto3, &StO3_descriptor, 0, "..\\data\\sto1-8.bin");

    if (psto3->count != 3) return 1;

    A2C_Free_StO3(psto3);

    return 0;
}



int main(int argc, char * argv[])
{
    int err = 0;

    argv;
    argc;

//    _asm{int 3};

    err += Test1();
    err += Test2();
    err += Test3();
    err += Test4();
    err += Test5();
    err += Test6();

    err += MemoryCheck();

    exit(err);
}
