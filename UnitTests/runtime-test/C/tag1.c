#include <stdio.h>
#include "asn.h"
#include "utility.h"

/*
 *  Decode, Encode and check contents for set of items
 */

int Test1()
{
    Tag1 *        pvs = NULL;
    Tag1          vs;
    
    
    Check_DecodeBER(&pvs, &Tag1_descriptor, 0, "..\\data\\tag1-1.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag1(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag1_descriptor, 0, 1, "..\\data\\tag1-1.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test2()
{
    Tag2 *        pvs = NULL;
    Tag2          vs;
    
    
    Check_DecodeBER(&pvs, &Tag2_descriptor, 0, "..\\data\\tag1-2.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag2(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag2_descriptor, 0, 1, "..\\data\\tag1-2.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test3()
{
    Tag3 *        pvs = NULL;
    Tag3          vs;
    
    
    Check_DecodeBER(&pvs, &Tag3_descriptor, 0, "..\\data\\tag1-3.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag3(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag3_descriptor, 0, 1, "..\\data\\tag1-3.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test4()
{
    Tag4 *        pvs = NULL;
    Tag4          vs;
    
    
    Check_DecodeBER(&pvs, &Tag4_descriptor, 0, "..\\data\\tag1-4.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag4(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag4_descriptor, 0, 1, "..\\data\\tag1-4.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test5()
{
    Tag5 *        pvs = NULL;
    Tag5          vs;
    
    
    Check_DecodeBER(&pvs, &Tag5_descriptor, 0, "..\\data\\tag1-5.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag5(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag5_descriptor, 0, 1, "..\\data\\tag1-5.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test6()
{
    Tag6 *        pvs = NULL;
    Tag6          vs;
    
    
    Check_DecodeBER(&pvs, &Tag6_descriptor, 0, "..\\data\\tag1-6.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag6(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag6_descriptor, 0, 1, "..\\data\\tag1-6.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  Decode, Encode and check contents for set of items
 */

int Test7()
{
    Tag7 *        pvs = NULL;
    Tag7          vs;
    
    
    Check_DecodeBER(&pvs, &Tag7_descriptor, 0, "..\\data\\tag1-7.bin");

    if (strcmp(pvs->string, "Jones") != 0) return 1;

    A2C_Free_Tag7(pvs);

    vs.string = "Jones";
    
    if (EncodeCompareToFile(&vs, &Tag7_descriptor, 0, 1, "..\\data\\tag1-7.bin") != A2C_ERROR_Success) return 1;

    return 0;
}

/*
 *  
 */

int main(int argc, char * argv[])
{
    int         err = 0;

    argv;
    argc;

    /*
     *  Start running tests - only report failures
     */

//    _asm{int 3}
    err += Test1();
    err += Test2();
    err += Test3();
    err += Test4();
    err += Test5();
    err += Test6();
    err += Test7();

    err += MemoryCheck();
    
    exit(err);
}
