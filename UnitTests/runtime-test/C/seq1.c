#include <stdio.h>
#include <crtdbg.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Decode & Encode an empty SEQUENCE
 */

int Test1()
{
    SQ1 *       psq1;

    Check_DecodeBER(&psq1, &SQ1_descriptor, 0, "..\\data\\sq1-1.bin");
    A2C_Free_SQ1(psq1);

    Check_DecodeBER(&psq1, &SQ1_descriptor, 0, "..\\data\\sq1-2.bin");
    A2C_Free_SQ1(psq1);

    Check_DecodeDER(&psq1, &SQ1_descriptor, 0, "..\\data\\sq1-1.bin");
    A2C_Free_SQ1(psq1);

    Check_EncodeDER(psq1, &SQ1_descriptor, 0, "..\\data\\sq1-1.bin");

    Check_EncodeBER(psq1, &SQ1_descriptor, 0, "..\\data\\sq1-2.bin");

    return 0;
}

/*
 *  Test #2 - Encode/Decode a known fixed set of values
 */

int Test2()
{
    SQ2 *       psq2;
    SQ2         sq2 = {{1}, {2}, {3}};

    Check_DecodeBER(&psq2, &SQ2_descriptor, 0, "..\\data\\sq1-3.bin");
    if (psq2->i1.nData != 1) return 1;
    if (psq2->i2.nData != 2) return 1;
    if (psq2->i3.nData != 3) return 1;
    A2C_Free_SQ2(psq2);

    Check_DecodeDER(&psq2, &SQ2_descriptor, 0, "..\\data\\sq1-3.bin");
    if (psq2->i1.nData != 1) return 1;
    if (psq2->i2.nData != 2) return 1;
    if (psq2->i3.nData != 3) return 1;
    A2C_Free_SQ2(psq2);

    Check_DecodeBER(&psq2, &SQ2_descriptor, 0, "..\\data\\sq1-4.bin");
    if (psq2->i1.nData != 1) return 1;
    if (psq2->i2.nData != 2) return 1;
    if (psq2->i3.nData != 3) return 1;
    A2C_Free_SQ2(psq2);

    Check_EncodeDER(&sq2, &SQ2_descriptor, 0, "..\\data\\sq1-3.bin");

    Check_EncodeBER(&sq2, &SQ2_descriptor, 0, "..\\data\\sq1-4.bin");
    
    return 0;
}

/*
 *  Test#3 - All elements optional - test each of the cases
 */

int Test3()
{
    SQ3 * psq;

    // empty

    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-1.bin"); // { }
    if (psq->optionalFields != 0) return 1;
    A2C_Free_SQ3(psq);

    // [1]

    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-5.bin"); // { i1  }
    if (psq->optionalFields != SQ3_i1_present) return 1;
    A2C_Free_SQ3(psq);

    // [2]

    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-6.bin"); // { i2 }
    if (psq->optionalFields != SQ3_i2_present) return 1;
    A2C_Free_SQ3(psq);

    // [3]

    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-7.bin"); // { i3 }
    if (psq->optionalFields != SQ3_i3_present) return 1;
    A2C_Free_SQ3(psq);

    // [1] [2]
    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-8.bin"); // { i1, i2 }
    if (psq->optionalFields != (SQ3_i1_present | SQ3_i2_present)) return 1;
    A2C_Free_SQ3(psq);

    // [1] [3]
    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-9.bin"); // { i1, i3 }
    if (psq->optionalFields != (SQ3_i1_present | SQ3_i3_present)) return 1;
    A2C_Free_SQ3(psq);

    // [2] [3]
    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-10.bin"); // { i2, i3 }
    if (psq->optionalFields != (SQ3_i2_present | SQ3_i3_present)) return 1;
    A2C_Free_SQ3(psq);

    // [1] [2] [3]
    Check_DecodeBER(&psq, &SQ3_descriptor, 0, "..\\data\\sq1-11.bin"); // { i1, i2, i3 }
    if (psq->optionalFields != (SQ3_i1_present | SQ3_i2_present | SQ3_i3_present)) return 1;
    A2C_Free_SQ3(psq);

    return 0;
}

/*
 *  Test#4 - All elements DEFAULT - test each of the cases
 */

int Test4()
{
    SQ4 * psq;

    // empty

    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-1.bin"); // { }
    if (psq->i1.nData != 21) return 1;
    if (psq->i2.nData != 22) return 1;
    if (psq->i3.nData != 23) return 1;
    A2C_Free_SQ4(psq);

    // [1]

    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-5.bin"); // { i1  }
    if (psq->i1.nData != 1) return 1;
    if (psq->i2.nData != 22) return 1;
    if (psq->i3.nData != 23) return 1;
    A2C_Free_SQ4(psq);

    // [2]

    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-6.bin"); // { i2 }
    if (psq->i1.nData != 21) return 1;
    if (psq->i2.nData != 2) return 1;
    if (psq->i3.nData != 23) return 1;
    A2C_Free_SQ4(psq);

    // [3]

    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-7.bin"); // { i3 }
    if (psq->i1.nData != 21) return 1;
    if (psq->i2.nData != 22) return 1;
    if (psq->i3.nData != 3) return 1;
    A2C_Free_SQ4(psq);

    // [1] [2]
    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-8.bin"); // { i1, i2 }
    if (psq->i1.nData != 1) return 1;
    if (psq->i2.nData != 2) return 1;
    if (psq->i3.nData != 23) return 1;
    A2C_Free_SQ4(psq);

    // [1] [3]
    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-9.bin"); // { i1, i3 }
    if (psq->i1.nData != 1) return 1;
    if (psq->i2.nData != 22) return 1;
    if (psq->i3.nData != 3) return 1;
    A2C_Free_SQ4(psq);

    // [2] [3]
    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-10.bin"); // { i2, i3 }
    if (psq->i1.nData != 21) return 1;
    if (psq->i2.nData != 2) return 1;
    if (psq->i3.nData != 3) return 1;
    A2C_Free_SQ4(psq);

    // [1] [2] [3]
    Check_DecodeBER(&psq, &SQ4_descriptor, 0, "..\\data\\sq1-11.bin"); // { i1, i2, i3 }
    if (psq->i1.nData != 1) return 1;
    if (psq->i2.nData != 2) return 1;
    if (psq->i3.nData != 3) return 1;
    A2C_Free_SQ4(psq);

    return 0;
}

#if 0
/*
 *  Test #5 - make sure that recursion on release is done
 */

int Test5()
{
    SO2 *  pso2;

    if (DecodeFromFile(&pso2, &SO2_descriptor, 0, "..\\data\\so1-6.bin") != A2C_ERROR_Success) {
        return 1;
    }

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

    if (EncodeCompareToFile(&so3, &SO3_descriptor, 0, TRUE, "..\\data\\so1-7.bin") != A2C_ERROR_Success) return 1;

    if (EncodeCompareToFile(&so3, &SO3_descriptor, 0, FALSE, "..\\data\\so1-8.bin") != A2C_ERROR_Success) return 1;

    if (DecodeFromFile(&pso3, &SO3_descriptor, 0, "..\\data\\so1-7.bin") != A2C_ERROR_Success) {
        return 1;
    }

    if (pso3->count != 3) return 1;

    A2C_Free_SO3(pso3);


    if (DecodeFromFile(&pso3, &SO3_descriptor, 0, "..\\data\\so1-8.bin") != A2C_ERROR_Success) {
        return 1;
    }

    if (pso3->count != 3) return 1;

    A2C_Free_SO3(pso3);

    return 0;
}
#endif

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
//    err += Test5();
//    err += Test6();

    err += MemoryCheck();

    exit(err);
}
