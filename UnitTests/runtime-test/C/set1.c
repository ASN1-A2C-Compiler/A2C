#include <stdio.h>
#include <crtdbg.h>
#include "asn.h"
#include "utility.h"

/*
 *  First simple test - Decode & Encode an empty SEQUENCE
 */

int Test1()
{
    ST1 *       pst1;

    Check_DecodeBER(&pst1, &ST1_descriptor, 0, "..\\data\\st1-1.bin");
    A2C_Free_ST1(pst1);

    Check_DecodeBER(&pst1, &ST1_descriptor, 0, "..\\data\\st1-2.bin");
    A2C_Free_ST1(pst1);

    Check_DecodeDER(&pst1, &ST1_descriptor, 0, "..\\data\\st1-1.bin");
    A2C_Free_ST1(pst1);

    Check_EncodeDER(pst1, &ST1_descriptor, 0, "..\\data\\st1-1.bin");

    Check_EncodeBER(pst1, &ST1_descriptor, 0, "..\\data\\st1-2.bin");

    return 0;
}

/*
 *  Test #2 - Encode/Decode a known fixed set of values
 */

int Test2()
{
    ST2 *       pst2;
    ST2         st2 = {{1}, {2}, {3}};

    Check_DecodeBER(&pst2, &ST2_descriptor, 0, "..\\data\\st1-3.bin");
    if (pst2->i1.nData != 1) return 1;
    if (pst2->i2.nData != 2) return 1;
    if (pst2->i3.nData != 3) return 1;
    A2C_Free_ST2(pst2);

    Check_DecodeDER(&pst2, &ST2_descriptor, 0, "..\\data\\st1-3.bin");
    if (pst2->i1.nData != 1) return 1;
    if (pst2->i2.nData != 2) return 1;
    if (pst2->i3.nData != 3) return 1;
    A2C_Free_ST2(pst2);

    Check_DecodeBER(&pst2, &ST2_descriptor, 0, "..\\data\\st1-4.bin");
    if (pst2->i1.nData != 1) return 1;
    if (pst2->i2.nData != 2) return 1;
    if (pst2->i3.nData != 3) return 1;
    A2C_Free_ST2(pst2);

    Check_EncodeDER(&st2, &ST2_descriptor, 0, "..\\data\\st1-3.bin");

    Check_EncodeBER(&st2, &ST2_descriptor, 0, "..\\data\\st1-4.bin");
    
    return 0;
}

/*
 *  Test#3 - All elements optional - test each of the cases
 */

int Test3()
{
    ST3 * pst;

    //  none
    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-1.bin"); // { }
    if (pst->optionalFields != 0) return 1;

    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-2.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-1.bin");
    A2C_Free_ST3(pst);

    //  [1]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-5.bin"); // { i1  }
    if (pst->optionalFields != ST3_i1_present) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-20.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-5.bin");
    A2C_Free_ST3(pst);

    //  [2]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-6.bin"); // { i2 }
    if (pst->optionalFields != ST3_i2_present) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-21.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-6.bin");
    A2C_Free_ST3(pst);

    //  [3]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-7.bin"); // { i3 }
    if (pst->optionalFields != ST3_i3_present) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-22.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-7.bin");
    A2C_Free_ST3(pst);

    //  [1] [2]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-8.bin"); // { i1, i2 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present)) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-23.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-8.bin");
    A2C_Free_ST3(pst);

    //  [1] [3]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-9.bin"); // { i1, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i3_present)) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-24.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-9.bin");
    A2C_Free_ST3(pst);

    //  [2] [1]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-10.bin"); // { i1, i2 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present)) return 1;
    A2C_Free_ST3(pst);

    //  [2] [3]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-11.bin"); // { i2, i3 }
    if (pst->optionalFields != (ST3_i2_present | ST3_i3_present)) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-25.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-11.bin");
    A2C_Free_ST3(pst);

    //  [3] [1]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-12.bin"); // { i1, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [3] [2]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-13.bin"); // { i2, i3 }
    if (pst->optionalFields != (ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [1] [2] [3]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-14.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    Check_EncodeBER(pst, &ST3_descriptor, 0, "..\\data\\st1-26.bin");
    Check_EncodeDER(pst, &ST3_descriptor, 0, "..\\data\\st1-14.bin");
    A2C_Free_ST3(pst);

    //  [1] [3] [2]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-15.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [2] [1] [3]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-16.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [2] [3] [1]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-17.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [3] [1] [2]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-18.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    //  [3] [2] [1]

    Check_DecodeBER(&pst, &ST3_descriptor, 0, "..\\data\\st1-19.bin"); // { i1, i2, i3 }
    if (pst->optionalFields != (ST3_i1_present | ST3_i2_present | ST3_i3_present)) return 1;
    A2C_Free_ST3(pst);

    return 0;
}

/*
 *  Test#4 - All elements Defaulted - test each of the cases
 */

int Test4()
{
    ST4 * pst;

    //  none
    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-1.bin"); // { }
    if (pst->i1.nData != 21) return 1;
    if (pst->i2.nData != 22) return 1;
    if (pst->i3.nData != 23) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-2.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-1.bin");
    A2C_Free_ST4(pst);

    //  [1]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-5.bin"); // { i1  }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 22) return 1;
    if (pst->i3.nData != 23) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-20.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-5.bin");
    A2C_Free_ST4(pst);

    //  [2]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-6.bin"); // { i2 }
    if (pst->i1.nData != 21) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 23) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-21.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-6.bin");
    A2C_Free_ST4(pst);

    //  [3]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-7.bin"); // { i3 }
    if (pst->i1.nData != 21) return 1;
    if (pst->i2.nData != 22) return 1;
    if (pst->i3.nData != 3) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-22.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-7.bin");
    A2C_Free_ST4(pst);

    //  [1] [2]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-8.bin"); // { i1, i2 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 23) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-23.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-8.bin");
    A2C_Free_ST4(pst);

    //  [1] [3]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-9.bin"); // { i1, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 22) return 1;
    if (pst->i3.nData != 3) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-24.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-9.bin");
    A2C_Free_ST4(pst);

    //  [2] [1]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-10.bin"); // { i1, i2 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 23) return 1;

    A2C_Free_ST4(pst);

    //  [2] [3]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-11.bin"); // { i2, i3 }
    if (pst->i1.nData != 21) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-25.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-11.bin");
    A2C_Free_ST4(pst);

    //  [3] [1]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-12.bin"); // { i1, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 22) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [3] [2]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-13.bin"); // { i2, i3 }
    if (pst->i1.nData != 21) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [1] [2] [3]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-14.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    Check_EncodeBER(pst, &ST4_descriptor, 0, "..\\data\\st1-26.bin");
    Check_EncodeDER(pst, &ST4_descriptor, 0, "..\\data\\st1-14.bin");
    A2C_Free_ST4(pst);

    //  [1] [3] [2]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-15.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [2] [1] [3]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-16.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [2] [3] [1]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-17.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [3] [1] [2]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-18.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    //  [3] [2] [1]

    Check_DecodeBER(&pst, &ST4_descriptor, 0, "..\\data\\st1-19.bin"); // { i1, i2, i3 }
    if (pst->i1.nData != 1) return 1;
    if (pst->i2.nData != 2) return 1;
    if (pst->i3.nData != 3) return 1;

    A2C_Free_ST4(pst);

    return 0;
}

/*
 *  Test #5 - Tagged fields
 */

int Test5()
{
    ST5 *       pst;
    ST5         st;

    st.f1.string = "F1";
    st.f2.string = "F2";
    st.f3.string = "F3";
    st.f4.string = "F4";
    st.f5.string = "F5";
    st.f6.string = "F6";
    st.f7.string = "F7";

    Check_EncodeDER(&st, &ST5_descriptor, 0, "..\\data\\st1-5-1.bin");
    Check_EncodeBER(&st, &ST5_descriptor, 0, "..\\data\\st1-5-2.bin");
    
    Check_DecodeBER(&pst, &ST5_descriptor, 0, "..\\data\\st1-5-1.bin");
    A2C_Free_ST5(pst);

    Check_DecodeBER(&pst, &ST5_descriptor, 0, "..\\data\\st1-5-2.bin");
    A2C_Free_ST5(pst);

    Check_DecodeDER(&pst, &ST5_descriptor, 0, "..\\data\\st1-5-1.bin");
    A2C_Free_ST5(pst);

    return 0;
}

#if 0
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

//    _CrtSetBreakAlloc(140);

    err += Test1();
    err += Test2();
    err += Test3();
    err += Test4();
    err += Test5();
//    err += Test6();

    err += MemoryCheck();

    exit(err);
}
