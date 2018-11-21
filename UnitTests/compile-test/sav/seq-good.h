#include <A2C.h>
#ifndef __seq_good__H__
#define __seq_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
} S1;

extern const A2C_DESCRIPTOR S1_descriptor;
extern const A2C_ITEM_HEADER S1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S1(S1 ** p) { return A2C_Alloc((PVOID *) p, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S1(S1 * p) { return A2C_Free((PVOID) p, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S1(S1 * p) { return A2C_Init((PVOID) p, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S1(S1 * p) { return A2C_Release((PVOID) p, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S1(S1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S1(S1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S1(S1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S1(S1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S1(S1 * pDst, S1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S1_descriptor); }
A2C_INLINE int A2C_Compare_S1(S1 const * pLeft, S1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S1(S1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S1(S1 * p, int * pf) { return A2C_Validate((PVOID) p, &S1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S1(S1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S1_descriptor, ppcxt, pf, pstm); }
extern const S1 a;
extern const S1 b;

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    struct {
        A2C_INTEGER_NATIVE               i1;
        A2C_INTEGER_NATIVE               i2;
    } s1;
} S2;

extern const A2C_DESCRIPTOR S2_descriptor;
extern const A2C_ITEM_HEADER S2_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S2(S2 ** p) { return A2C_Alloc((PVOID *) p, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S2(S2 * p) { return A2C_Free((PVOID) p, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S2(S2 * p) { return A2C_Init((PVOID) p, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S2(S2 * p) { return A2C_Release((PVOID) p, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S2(S2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S2(S2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S2(S2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S2(S2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S2(S2 * pDst, S2 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S2_descriptor); }
A2C_INLINE int A2C_Compare_S2(S2 const * pLeft, S2 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S2(S2 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S2_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S2(S2 * p, int * pf) { return A2C_Validate((PVOID) p, &S2_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S2(S2 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S2_descriptor, ppcxt, pf, pstm); }
extern const S2 c;

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    S1                                   s1;
} S3;

extern const A2C_DESCRIPTOR S3_descriptor;
extern const A2C_ITEM_HEADER S3_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S3(S3 ** p) { return A2C_Alloc((PVOID *) p, &S3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S3(S3 * p) { return A2C_Free((PVOID) p, &S3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S3(S3 * p) { return A2C_Init((PVOID) p, &S3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S3(S3 * p) { return A2C_Release((PVOID) p, &S3_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S3(S3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S3(S3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S3(S3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S3(S3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S3(S3 * pDst, S3 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S3_descriptor); }
A2C_INLINE int A2C_Compare_S3(S3 const * pLeft, S3 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S3(S3 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S3_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S3(S3 * p, int * pf) { return A2C_Validate((PVOID) p, &S3_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S3(S3 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S3_descriptor, ppcxt, pf, pstm); }
extern const S3 d;
extern const S3 e;

typedef struct {
    unsigned int optionalFields;
#define S4_i2_present 0x1
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    A2C_BOOLEAN                          b1;
} S4;

extern const A2C_DESCRIPTOR S4_descriptor;
extern const A2C_ITEM_HEADER S4_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S4(S4 ** p) { return A2C_Alloc((PVOID *) p, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S4(S4 * p) { return A2C_Free((PVOID) p, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S4(S4 * p) { return A2C_Init((PVOID) p, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S4(S4 * p) { return A2C_Release((PVOID) p, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S4(S4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S4(S4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S4(S4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S4(S4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S4(S4 * pDst, S4 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S4_descriptor); }
A2C_INLINE int A2C_Compare_S4(S4 const * pLeft, S4 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S4(S4 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S4_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S4(S4 * p, int * pf) { return A2C_Validate((PVOID) p, &S4_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S4(S4 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S4_descriptor, ppcxt, pf, pstm); }
extern const S4 f;
extern const S4 g;

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    A2C_BOOLEAN                          b1;
} S5;

extern const A2C_DESCRIPTOR S5_descriptor;
extern const A2C_ITEM_HEADER S5_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S5(S5 ** p) { return A2C_Alloc((PVOID *) p, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S5(S5 * p) { return A2C_Free((PVOID) p, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S5(S5 * p) { return A2C_Init((PVOID) p, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S5(S5 * p) { return A2C_Release((PVOID) p, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S5(S5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S5(S5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S5(S5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S5(S5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S5(S5 * pDst, S5 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S5_descriptor); }
A2C_INLINE int A2C_Compare_S5(S5 const * pLeft, S5 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S5(S5 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S5_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S5(S5 * p, int * pf) { return A2C_Validate((PVOID) p, &S5_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S5(S5 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S5_descriptor, ppcxt, pf, pstm); }
extern const S5 i;
extern const S5 j;
extern const S5 k;

typedef struct {
    unsigned int optionalFields;
#define S6_i1_present 0x1
#define S6_s5_present 0x2
#define S6_b1_present 0x4
    A2C_INTEGER_NATIVE                   i1;
    S5                                   s5;
    A2C_BOOLEAN                          b1;
} S6;

extern const A2C_DESCRIPTOR S6_descriptor;
extern const A2C_ITEM_HEADER S6_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S6(S6 ** p) { return A2C_Alloc((PVOID *) p, &S6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S6(S6 * p) { return A2C_Free((PVOID) p, &S6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S6(S6 * p) { return A2C_Init((PVOID) p, &S6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S6(S6 * p) { return A2C_Release((PVOID) p, &S6_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S6(S6 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S6_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S6(S6 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S6_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S6(S6 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S6_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S6(S6 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S6_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S6(S6 * pDst, S6 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S6_descriptor); }
A2C_INLINE int A2C_Compare_S6(S6 const * pLeft, S6 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S6(S6 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S6_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S6(S6 * p, int * pf) { return A2C_Validate((PVOID) p, &S6_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S6(S6 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S6_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    S1                                   s1;
} S7;

extern const A2C_DESCRIPTOR S7_descriptor;
extern const A2C_ITEM_HEADER S7_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S7(S7 ** p) { return A2C_Alloc((PVOID *) p, &S7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S7(S7 * p) { return A2C_Free((PVOID) p, &S7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S7(S7 * p) { return A2C_Init((PVOID) p, &S7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S7(S7 * p) { return A2C_Release((PVOID) p, &S7_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S7(S7 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S7_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S7(S7 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S7_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S7(S7 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S7_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S7(S7 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S7_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S7(S7 * pDst, S7 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S7_descriptor); }
A2C_INLINE int A2C_Compare_S7(S7 const * pLeft, S7 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S7(S7 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S7_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S7(S7 * p, int * pf) { return A2C_Validate((PVOID) p, &S7_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S7(S7 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S7_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    S1                                   s1;
} S8;

extern const A2C_DESCRIPTOR S8_descriptor;
extern const A2C_ITEM_HEADER S8_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S8(S8 ** p) { return A2C_Alloc((PVOID *) p, &S8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S8(S8 * p) { return A2C_Free((PVOID) p, &S8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S8(S8 * p) { return A2C_Init((PVOID) p, &S8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S8(S8 * p) { return A2C_Release((PVOID) p, &S8_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S8(S8 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S8_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S8(S8 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S8_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S8(S8 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S8_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S8(S8 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S8_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S8(S8 * pDst, S8 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S8_descriptor); }
A2C_INLINE int A2C_Compare_S8(S8 const * pLeft, S8 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S8(S8 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S8_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S8(S8 * p, int * pf) { return A2C_Validate((PVOID) p, &S8_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S8(S8 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S8_descriptor, ppcxt, pf, pstm); }
extern const S1 S7__s1___default;
#ifdef __cplusplus
}
#endif
#endif /* __seq_good__H__ */
