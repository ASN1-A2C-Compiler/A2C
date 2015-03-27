#include <stdio.h>
#include <crtdbg.h>
#include "asn.h"
#include "utility.h"

/*
 *  Test #1 - Just Decode something
 */

int Test1()
{
    ST2_1 *       psq;

    Check_DecodeBER(&psq, &ST2_1_descriptor, 0, "..\\data\\st2-1.bin");

    if (psq->i1->nData != 1) return 1;
    if (psq->i2->nData != 0x12) return 1;
    if (psq->i3->nData != 5) return 1;

    A2C_Free_ST2_1(psq);
    return 0;
}

/*
 *  Test #2 - Decode all of the different options here.
 */

int Test2()
{
    ST2_1 *     psq = NULL;

    /*  2 & 3 absent */
    
    Check_DecodeBER(&psq, &ST2_1_descriptor, 0, "..\\data\\st2-2.bin");

    if (psq->i1->nData != 1) return 1;
    if (psq->i2 != NULL) return 1;
    if (psq->i3->nData != 5) return 1;

    A2C_Free_ST2_1(psq);

    /*  2 present & 3 absent */

    Check_DecodeBER(&psq, &ST2_1_descriptor, 0, "..\\data\\st2-1.bin");

    if (psq->i1->nData != 1) return 1;
    if (psq->i2->nData != 0x12) return 1;
    if (psq->i3->nData != 5) return 1;

    A2C_Free_ST2_1(psq);

    /*  2 absent & 3 present */

    Check_DecodeBER(&psq, &ST2_1_descriptor, 0, "..\\data\\st2-3.bin");

    if (psq->i1->nData != 1) return 1;
    if (psq->i2 != NULL) return 1;
    if (psq->i3->nData != 6) return 1;

    A2C_Free_ST2_1(psq);

    /*  2 & 3 present */

    Check_DecodeBER(&psq, &ST2_1_descriptor, 0, "..\\data\\st2-4.bin");

    if (psq->i1->nData != 1) return 1;
    if (psq->i2->nData != 0x12) return 1;
    if (psq->i3->nData != 6) return 1;

    A2C_Free_ST2_1(psq);

    return 0;
}

/*
 *  Test #3 - Encode al of the cases from Test2 - all encoded using DER
 */

int Test3()
{
    ST2_1       sq = {0};
    A2C_INTEGER_NATIVE i1 = {1};
    A2C_INTEGER_NATIVE i2 = {0x12};
    A2C_INTEGER_NATIVE i3 = {5};
    A2C_INTEGER_NATIVE i4 = {6};

    sq.i1 = &i1;
    sq.i3 = &i3;

    /*  2 & 3 absent */
    
    Check_EncodeDER(&sq, &ST2_1_descriptor, 0, "..\\data\\st2-2.bin");

    /*  2 present & 3 absent */

    sq.i2 = &i2;
    Check_EncodeDER(&sq, &ST2_1_descriptor, 0, "..\\data\\st2-1.bin");

    /*  2 absent & 3 present */

    sq.i2 = NULL;
    sq.i3 = &i4;
    Check_EncodeDER(&sq, &ST2_1_descriptor, 0, "..\\data\\st2-3.bin");

    /*  2 & 3 present */

    sq.i2 = &i2;
    Check_EncodeDER(&sq, &ST2_1_descriptor, 0, "..\\data\\st2-4.bin");
    
    return 0;
}

/*
 *  Test #4 - Do an explicit tag
 */

int Test4()
{
    ST2_2       sq = {0};
    ST2_2 *     psq;
    A2C_INTEGER_NATIVE i1 = {1};

    Check_DecodeBER(&psq, &ST2_2_descriptor, 0, "..\\data\\st2-5.bin");

    if (psq->i1->nData != 1) return 1;

    A2C_Free_ST2_2(psq);

    sq.i1 = &i1;

    Check_EncodeDER(&sq, &ST2_2_descriptor, 0, "..\\data\\st2-5.bin");

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

    err += MemoryCheck();

    if (err > 0) printf("Error count = %d\n", err);

    exit(err);
}
