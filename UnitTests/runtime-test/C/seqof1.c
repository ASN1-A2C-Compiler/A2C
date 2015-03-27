#include <stdio.h>
#include <crtdbg.h>
#include "asn.h"
#include "utility.h"

/*
 *  FIrst simple test - Decode an empty SEQ OF
 */

int Test1()
{
    SO1 *  pso1;

    Check_DecodeBER(&pso1, &SO1_descriptor, 0, "..\\data\\so1-1.bin");

    if (pso1->count != 0) return 1;

    A2C_Free_SO1(pso1);

    return 0;
}

/*
 *  Test #2 - encode empty and compare to known result
 */

int Test2()
{
    SO1 so1;

    so1.count = 0;

    Check_EncodeDER(&so1, &SO1_descriptor, 0, "..\\data\\so1-1.bin");

    Check_EncodeBER(&so1, &SO1_descriptor, 0, "..\\data\\so1-2.bin");
    
    return 0;
}

/*
 *  Test#3 - decode a sequence of integers
 */

int Test3()
{
    SO1 *  pso1;

    Check_DecodeBER(&pso1, &SO1_descriptor, 0, "..\\data\\so1-3.bin");

    if (pso1->count != 5) return 1;
    if (pso1->array[0].nData != 5) return 1;
    if (pso1->array[1].nData != 3) return 1;
    if (pso1->array[2].nData != -1) return 1;
    if (pso1->array[3].nData != 10) return 1;
    if (pso1->array[4].nData != 5) return 1;

    A2C_Free_SO1(pso1);

    Check_DecodeBER(&pso1, &SO1_descriptor, 0, "..\\data\\so1-4.bin");

    if (pso1->count != 5) return 1;
    if (pso1->array[0].nData != 5) return 1;
    if (pso1->array[1].nData != 3) return 1;
    if (pso1->array[2].nData != -1) return 1;
    if (pso1->array[3].nData != 10) return 1;
    if (pso1->array[4].nData != 5) return 1;

    A2C_Free_SO1(pso1);

    return 0;
}

/*
 *  Test #4 - encode empty and compare to known result
 */

int Test4()
{
    SO1 so1;
    A2C_INTEGER_NATIVE  rg[] = {5, 3, -1, 10, 5 };

    so1.count = 5;
    so1.array = rg;


    Check_EncodeDER(&so1, &SO1_descriptor, 0, "..\\data\\so1-3.bin");

    Check_EncodeBER(&so1, &SO1_descriptor, 0, "..\\data\\so1-4.bin");
    
    return 0;
}

/*
 *  Test #5 - make sure that recursion on release is done
 */

int Test5()
{
    SO2 *  pso2;

    Check_DecodeBER(&pso2, &SO2_descriptor, 0, "..\\data\\so1-6.bin");

    if (pso2->count != 2) return 1;

    A2C_Free_SO2(pso2);

    return 0;
}

/*
 *  Test #6 - SEQ OF SEQ OF INT - encode and decode
 */

int Test6()
{
    SO3 *       pso3;
    SO3         so3;
    SO1         rgso1[3];
    A2C_INTEGER_NATIVE rg[] = {5, 0, 8, 13, -1, 2, 15, 3, 18, 20};

    so3.count = 3;
    so3.array = rgso1;

    rgso1[0].count = 2;
    rgso1[0].array = rg;

    rgso1[1].count = 5;
    rgso1[1].array = &rg[1];

    rgso1[2].count = 10;
    rgso1[2].array = rg;

    Check_EncodeDER(&so3, &SO3_descriptor, 0, "..\\data\\so1-7.bin");

    Check_EncodeBER(&so3, &SO3_descriptor, 0, "..\\data\\so1-8.bin");

    Check_DecodeBER(&pso3, &SO3_descriptor, 0, "..\\data\\so1-7.bin");

    if (pso3->count != 3) return 1;

    A2C_Free_SO3(pso3);


    Check_DecodeBER(&pso3, &SO3_descriptor, 0, "..\\data\\so1-8.bin");

    if (pso3->count != 3) return 1;

    A2C_Free_SO3(pso3);

    return 0;
}



int main(int argc, char * argv[])
{
    int err = 0;

    argc;
    argv;

//    _CrtSetBreakAlloc(90);

    err += Test1();
    err += Test2();
    err += Test3();
    err += Test4();
    err += Test5();
    err += Test6();

    err += MemoryCheck();

    exit(err);
}
