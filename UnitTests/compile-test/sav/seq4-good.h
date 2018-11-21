#include <A2C.h>
#ifndef __seq4_good__H__
#define __seq4_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int unused;
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

typedef struct {
    /* Start Extension */
    A2C_EXTENSION                        _extension;
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

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
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

typedef struct {
    /* Start Extension */
    A2C_EXTENSION                        _extension;
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

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
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

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
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
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_EXTENSION                        _extension;
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
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
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

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S9;

extern const A2C_DESCRIPTOR S9_descriptor;
extern const A2C_ITEM_HEADER S9_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S9(S9 ** p) { return A2C_Alloc((PVOID *) p, &S9_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S9(S9 * p) { return A2C_Free((PVOID) p, &S9_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S9(S9 * p) { return A2C_Init((PVOID) p, &S9_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S9(S9 * p) { return A2C_Release((PVOID) p, &S9_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S9(S9 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S9_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S9(S9 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S9_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S9(S9 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S9_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S9(S9 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S9_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S9(S9 * pDst, S9 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S9_descriptor); }
A2C_INLINE int A2C_Compare_S9(S9 const * pLeft, S9 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S9_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S9(S9 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S9_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S9(S9 * p, int * pf) { return A2C_Validate((PVOID) p, &S9_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S9(S9 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S9_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
} S10;

extern const A2C_DESCRIPTOR S10_descriptor;
extern const A2C_ITEM_HEADER S10_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S10(S10 ** p) { return A2C_Alloc((PVOID *) p, &S10_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S10(S10 * p) { return A2C_Free((PVOID) p, &S10_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S10(S10 * p) { return A2C_Init((PVOID) p, &S10_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S10(S10 * p) { return A2C_Release((PVOID) p, &S10_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S10(S10 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S10_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S10(S10 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S10_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S10(S10 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S10_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S10(S10 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S10_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S10(S10 * pDst, S10 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S10_descriptor); }
A2C_INLINE int A2C_Compare_S10(S10 const * pLeft, S10 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S10_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S10(S10 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S10_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S10(S10 * p, int * pf) { return A2C_Validate((PVOID) p, &S10_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S10(S10 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S10_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S11;

extern const A2C_DESCRIPTOR S11_descriptor;
extern const A2C_ITEM_HEADER S11_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S11(S11 ** p) { return A2C_Alloc((PVOID *) p, &S11_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S11(S11 * p) { return A2C_Free((PVOID) p, &S11_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S11(S11 * p) { return A2C_Init((PVOID) p, &S11_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S11(S11 * p) { return A2C_Release((PVOID) p, &S11_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S11(S11 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S11_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S11(S11 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S11_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S11(S11 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S11_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S11(S11 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S11_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S11(S11 * pDst, S11 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S11_descriptor); }
A2C_INLINE int A2C_Compare_S11(S11 const * pLeft, S11 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S11_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S11(S11 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S11_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S11(S11 * p, int * pf) { return A2C_Validate((PVOID) p, &S11_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S11(S11 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S11_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S12;

extern const A2C_DESCRIPTOR S12_descriptor;
extern const A2C_ITEM_HEADER S12_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S12(S12 ** p) { return A2C_Alloc((PVOID *) p, &S12_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S12(S12 * p) { return A2C_Free((PVOID) p, &S12_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S12(S12 * p) { return A2C_Init((PVOID) p, &S12_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S12(S12 * p) { return A2C_Release((PVOID) p, &S12_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S12(S12 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S12_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S12(S12 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S12_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S12(S12 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S12_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S12(S12 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S12_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S12(S12 * pDst, S12 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S12_descriptor); }
A2C_INLINE int A2C_Compare_S12(S12 const * pLeft, S12 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S12_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S12(S12 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S12_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S12(S12 * p, int * pf) { return A2C_Validate((PVOID) p, &S12_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S12(S12 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S12_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_EXTENSION                        _extension;
} S13;

extern const A2C_DESCRIPTOR S13_descriptor;
extern const A2C_ITEM_HEADER S13_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S13(S13 ** p) { return A2C_Alloc((PVOID *) p, &S13_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S13(S13 * p) { return A2C_Free((PVOID) p, &S13_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S13(S13 * p) { return A2C_Init((PVOID) p, &S13_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S13(S13 * p) { return A2C_Release((PVOID) p, &S13_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S13(S13 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S13_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S13(S13 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S13_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S13(S13 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S13_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S13(S13 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S13_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S13(S13 * pDst, S13 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S13_descriptor); }
A2C_INLINE int A2C_Compare_S13(S13 const * pLeft, S13 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S13_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S13(S13 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S13_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S13(S13 * p, int * pf) { return A2C_Validate((PVOID) p, &S13_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S13(S13 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S13_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S14;

extern const A2C_DESCRIPTOR S14_descriptor;
extern const A2C_ITEM_HEADER S14_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S14(S14 ** p) { return A2C_Alloc((PVOID *) p, &S14_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S14(S14 * p) { return A2C_Free((PVOID) p, &S14_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S14(S14 * p) { return A2C_Init((PVOID) p, &S14_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S14(S14 * p) { return A2C_Release((PVOID) p, &S14_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S14(S14 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S14_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S14(S14 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S14_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S14(S14 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S14_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S14(S14 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S14_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S14(S14 * pDst, S14 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S14_descriptor); }
A2C_INLINE int A2C_Compare_S14(S14 const * pLeft, S14 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S14_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S14(S14 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S14_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S14(S14 * p, int * pf) { return A2C_Validate((PVOID) p, &S14_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S14(S14 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S14_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S15;

extern const A2C_DESCRIPTOR S15_descriptor;
extern const A2C_ITEM_HEADER S15_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S15(S15 ** p) { return A2C_Alloc((PVOID *) p, &S15_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S15(S15 * p) { return A2C_Free((PVOID) p, &S15_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S15(S15 * p) { return A2C_Init((PVOID) p, &S15_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S15(S15 * p) { return A2C_Release((PVOID) p, &S15_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S15(S15 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S15_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S15(S15 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S15_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S15(S15 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S15_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S15(S15 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S15_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S15(S15 * pDst, S15 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S15_descriptor); }
A2C_INLINE int A2C_Compare_S15(S15 const * pLeft, S15 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S15_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S15(S15 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S15_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S15(S15 * p, int * pf) { return A2C_Validate((PVOID) p, &S15_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S15(S15 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S15_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
} S16;

extern const A2C_DESCRIPTOR S16_descriptor;
extern const A2C_ITEM_HEADER S16_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S16(S16 ** p) { return A2C_Alloc((PVOID *) p, &S16_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S16(S16 * p) { return A2C_Free((PVOID) p, &S16_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S16(S16 * p) { return A2C_Init((PVOID) p, &S16_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S16(S16 * p) { return A2C_Release((PVOID) p, &S16_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S16(S16 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S16_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S16(S16 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S16_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S16(S16 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S16_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S16(S16 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S16_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S16(S16 * pDst, S16 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S16_descriptor); }
A2C_INLINE int A2C_Compare_S16(S16 const * pLeft, S16 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S16_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S16(S16 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S16_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S16(S16 * p, int * pf) { return A2C_Validate((PVOID) p, &S16_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S16(S16 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S16_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S17;

extern const A2C_DESCRIPTOR S17_descriptor;
extern const A2C_ITEM_HEADER S17_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S17(S17 ** p) { return A2C_Alloc((PVOID *) p, &S17_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S17(S17 * p) { return A2C_Free((PVOID) p, &S17_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S17(S17 * p) { return A2C_Init((PVOID) p, &S17_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S17(S17 * p) { return A2C_Release((PVOID) p, &S17_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S17(S17 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S17_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S17(S17 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S17_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S17(S17 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S17_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S17(S17 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S17_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S17(S17 * pDst, S17 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S17_descriptor); }
A2C_INLINE int A2C_Compare_S17(S17 const * pLeft, S17 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S17_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S17(S17 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S17_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S17(S17 * p, int * pf) { return A2C_Validate((PVOID) p, &S17_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S17(S17 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S17_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S18;

extern const A2C_DESCRIPTOR S18_descriptor;
extern const A2C_ITEM_HEADER S18_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S18(S18 ** p) { return A2C_Alloc((PVOID *) p, &S18_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S18(S18 * p) { return A2C_Free((PVOID) p, &S18_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S18(S18 * p) { return A2C_Init((PVOID) p, &S18_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S18(S18 * p) { return A2C_Release((PVOID) p, &S18_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S18(S18 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S18_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S18(S18 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S18_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S18(S18 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S18_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S18(S18 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S18_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S18(S18 * pDst, S18 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S18_descriptor); }
A2C_INLINE int A2C_Compare_S18(S18 const * pLeft, S18 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S18_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S18(S18 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S18_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S18(S18 * p, int * pf) { return A2C_Validate((PVOID) p, &S18_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S18(S18 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S18_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S19;

extern const A2C_DESCRIPTOR S19_descriptor;
extern const A2C_ITEM_HEADER S19_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S19(S19 ** p) { return A2C_Alloc((PVOID *) p, &S19_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S19(S19 * p) { return A2C_Free((PVOID) p, &S19_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S19(S19 * p) { return A2C_Init((PVOID) p, &S19_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S19(S19 * p) { return A2C_Release((PVOID) p, &S19_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S19(S19 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S19_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S19(S19 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S19_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S19(S19 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S19_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S19(S19 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S19_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S19(S19 * pDst, S19 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S19_descriptor); }
A2C_INLINE int A2C_Compare_S19(S19 const * pLeft, S19 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S19_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S19(S19 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S19_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S19(S19 * p, int * pf) { return A2C_Validate((PVOID) p, &S19_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S19(S19 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S19_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S20;

extern const A2C_DESCRIPTOR S20_descriptor;
extern const A2C_ITEM_HEADER S20_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S20(S20 ** p) { return A2C_Alloc((PVOID *) p, &S20_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S20(S20 * p) { return A2C_Free((PVOID) p, &S20_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S20(S20 * p) { return A2C_Init((PVOID) p, &S20_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S20(S20 * p) { return A2C_Release((PVOID) p, &S20_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S20(S20 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S20_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S20(S20 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S20_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S20(S20 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S20_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S20(S20 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S20_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S20(S20 * pDst, S20 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S20_descriptor); }
A2C_INLINE int A2C_Compare_S20(S20 const * pLeft, S20 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S20_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S20(S20 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S20_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S20(S20 * p, int * pf) { return A2C_Validate((PVOID) p, &S20_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S20(S20 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S20_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S21;

extern const A2C_DESCRIPTOR S21_descriptor;
extern const A2C_ITEM_HEADER S21_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S21(S21 ** p) { return A2C_Alloc((PVOID *) p, &S21_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S21(S21 * p) { return A2C_Free((PVOID) p, &S21_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S21(S21 * p) { return A2C_Init((PVOID) p, &S21_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S21(S21 * p) { return A2C_Release((PVOID) p, &S21_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S21(S21 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S21_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S21(S21 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S21_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S21(S21 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S21_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S21(S21 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S21_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S21(S21 * pDst, S21 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S21_descriptor); }
A2C_INLINE int A2C_Compare_S21(S21 const * pLeft, S21 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S21_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S21(S21 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S21_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S21(S21 * p, int * pf) { return A2C_Validate((PVOID) p, &S21_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S21(S21 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S21_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S22;

extern const A2C_DESCRIPTOR S22_descriptor;
extern const A2C_ITEM_HEADER S22_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S22(S22 ** p) { return A2C_Alloc((PVOID *) p, &S22_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S22(S22 * p) { return A2C_Free((PVOID) p, &S22_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S22(S22 * p) { return A2C_Init((PVOID) p, &S22_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S22(S22 * p) { return A2C_Release((PVOID) p, &S22_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S22(S22 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S22_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S22(S22 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S22_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S22(S22 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S22_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S22(S22 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S22_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S22(S22 * pDst, S22 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S22_descriptor); }
A2C_INLINE int A2C_Compare_S22(S22 const * pLeft, S22 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S22_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S22(S22 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S22_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S22(S22 * p, int * pf) { return A2C_Validate((PVOID) p, &S22_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S22(S22 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S22_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S23;

extern const A2C_DESCRIPTOR S23_descriptor;
extern const A2C_ITEM_HEADER S23_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S23(S23 ** p) { return A2C_Alloc((PVOID *) p, &S23_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S23(S23 * p) { return A2C_Free((PVOID) p, &S23_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S23(S23 * p) { return A2C_Init((PVOID) p, &S23_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S23(S23 * p) { return A2C_Release((PVOID) p, &S23_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S23(S23 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S23_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S23(S23 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S23_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S23(S23 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S23_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S23(S23 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S23_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S23(S23 * pDst, S23 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S23_descriptor); }
A2C_INLINE int A2C_Compare_S23(S23 const * pLeft, S23 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S23_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S23(S23 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S23_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S23(S23 * p, int * pf) { return A2C_Validate((PVOID) p, &S23_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S23(S23 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S23_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S24;

extern const A2C_DESCRIPTOR S24_descriptor;
extern const A2C_ITEM_HEADER S24_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S24(S24 ** p) { return A2C_Alloc((PVOID *) p, &S24_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S24(S24 * p) { return A2C_Free((PVOID) p, &S24_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S24(S24 * p) { return A2C_Init((PVOID) p, &S24_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S24(S24 * p) { return A2C_Release((PVOID) p, &S24_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S24(S24 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S24_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S24(S24 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S24_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S24(S24 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S24_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S24(S24 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S24_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S24(S24 * pDst, S24 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S24_descriptor); }
A2C_INLINE int A2C_Compare_S24(S24 const * pLeft, S24 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S24_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S24(S24 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S24_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S24(S24 * p, int * pf) { return A2C_Validate((PVOID) p, &S24_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S24(S24 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S24_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S25;

extern const A2C_DESCRIPTOR S25_descriptor;
extern const A2C_ITEM_HEADER S25_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S25(S25 ** p) { return A2C_Alloc((PVOID *) p, &S25_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S25(S25 * p) { return A2C_Free((PVOID) p, &S25_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S25(S25 * p) { return A2C_Init((PVOID) p, &S25_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S25(S25 * p) { return A2C_Release((PVOID) p, &S25_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S25(S25 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S25_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S25(S25 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S25_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S25(S25 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S25_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S25(S25 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S25_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S25(S25 * pDst, S25 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S25_descriptor); }
A2C_INLINE int A2C_Compare_S25(S25 const * pLeft, S25 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S25_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S25(S25 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S25_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S25(S25 * p, int * pf) { return A2C_Validate((PVOID) p, &S25_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S25(S25 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S25_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S26;

extern const A2C_DESCRIPTOR S26_descriptor;
extern const A2C_ITEM_HEADER S26_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S26(S26 ** p) { return A2C_Alloc((PVOID *) p, &S26_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S26(S26 * p) { return A2C_Free((PVOID) p, &S26_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S26(S26 * p) { return A2C_Init((PVOID) p, &S26_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S26(S26 * p) { return A2C_Release((PVOID) p, &S26_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S26(S26 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S26_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S26(S26 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S26_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S26(S26 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S26_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S26(S26 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S26_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S26(S26 * pDst, S26 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S26_descriptor); }
A2C_INLINE int A2C_Compare_S26(S26 const * pLeft, S26 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S26_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S26(S26 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S26_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S26(S26 * p, int * pf) { return A2C_Validate((PVOID) p, &S26_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S26(S26 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S26_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S27;

extern const A2C_DESCRIPTOR S27_descriptor;
extern const A2C_ITEM_HEADER S27_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S27(S27 ** p) { return A2C_Alloc((PVOID *) p, &S27_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S27(S27 * p) { return A2C_Free((PVOID) p, &S27_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S27(S27 * p) { return A2C_Init((PVOID) p, &S27_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S27(S27 * p) { return A2C_Release((PVOID) p, &S27_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S27(S27 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S27_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S27(S27 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S27_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S27(S27 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S27_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S27(S27 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S27_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S27(S27 * pDst, S27 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S27_descriptor); }
A2C_INLINE int A2C_Compare_S27(S27 const * pLeft, S27 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S27_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S27(S27 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S27_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S27(S27 * p, int * pf) { return A2C_Validate((PVOID) p, &S27_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S27(S27 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S27_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S28;

extern const A2C_DESCRIPTOR S28_descriptor;
extern const A2C_ITEM_HEADER S28_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S28(S28 ** p) { return A2C_Alloc((PVOID *) p, &S28_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S28(S28 * p) { return A2C_Free((PVOID) p, &S28_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S28(S28 * p) { return A2C_Init((PVOID) p, &S28_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S28(S28 * p) { return A2C_Release((PVOID) p, &S28_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S28(S28 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S28_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S28(S28 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S28_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S28(S28 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S28_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S28(S28 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S28_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S28(S28 * pDst, S28 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S28_descriptor); }
A2C_INLINE int A2C_Compare_S28(S28 const * pLeft, S28 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S28_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S28(S28 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S28_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S28(S28 * p, int * pf) { return A2C_Validate((PVOID) p, &S28_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S28(S28 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S28_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S29;

extern const A2C_DESCRIPTOR S29_descriptor;
extern const A2C_ITEM_HEADER S29_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S29(S29 ** p) { return A2C_Alloc((PVOID *) p, &S29_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S29(S29 * p) { return A2C_Free((PVOID) p, &S29_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S29(S29 * p) { return A2C_Init((PVOID) p, &S29_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S29(S29 * p) { return A2C_Release((PVOID) p, &S29_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S29(S29 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S29_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S29(S29 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S29_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S29(S29 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S29_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S29(S29 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S29_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S29(S29 * pDst, S29 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S29_descriptor); }
A2C_INLINE int A2C_Compare_S29(S29 const * pLeft, S29 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S29_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S29(S29 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S29_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S29(S29 * p, int * pf) { return A2C_Validate((PVOID) p, &S29_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S29(S29 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S29_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
    A2C_INTEGER_NATIVE                   i3;
    A2C_INTEGER_NATIVE                   i4;
} S30;

extern const A2C_DESCRIPTOR S30_descriptor;
extern const A2C_ITEM_HEADER S30_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S30(S30 ** p) { return A2C_Alloc((PVOID *) p, &S30_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S30(S30 * p) { return A2C_Free((PVOID) p, &S30_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S30(S30 * p) { return A2C_Init((PVOID) p, &S30_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S30(S30 * p) { return A2C_Release((PVOID) p, &S30_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S30(S30 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S30_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S30(S30 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S30_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S30(S30 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S30_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S30(S30 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S30_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S30(S30 * pDst, S30 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S30_descriptor); }
A2C_INLINE int A2C_Compare_S30(S30 const * pLeft, S30 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S30_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S30(S30 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S30_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S30(S30 * p, int * pf) { return A2C_Validate((PVOID) p, &S30_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S30(S30 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S30_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_EXTENSION                        _extension;
} S31;

extern const A2C_DESCRIPTOR S31_descriptor;
extern const A2C_ITEM_HEADER S31_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S31(S31 ** p) { return A2C_Alloc((PVOID *) p, &S31_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S31(S31 * p) { return A2C_Free((PVOID) p, &S31_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S31(S31 * p) { return A2C_Init((PVOID) p, &S31_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S31(S31 * p) { return A2C_Release((PVOID) p, &S31_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S31(S31 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S31_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S31(S31 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S31_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S31(S31 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S31_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S31(S31 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S31_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S31(S31 * pDst, S31 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S31_descriptor); }
A2C_INLINE int A2C_Compare_S31(S31 const * pLeft, S31 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S31_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S31(S31 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S31_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S31(S31 * p, int * pf) { return A2C_Validate((PVOID) p, &S31_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S31(S31 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S31_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_EXTENSION                        _extension;
} S32;

extern const A2C_DESCRIPTOR S32_descriptor;
extern const A2C_ITEM_HEADER S32_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S32(S32 ** p) { return A2C_Alloc((PVOID *) p, &S32_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S32(S32 * p) { return A2C_Free((PVOID) p, &S32_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S32(S32 * p) { return A2C_Init((PVOID) p, &S32_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S32(S32 * p) { return A2C_Release((PVOID) p, &S32_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S32(S32 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S32_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S32(S32 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S32_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S32(S32 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S32_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S32(S32 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S32_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S32(S32 * pDst, S32 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S32_descriptor); }
A2C_INLINE int A2C_Compare_S32(S32 const * pLeft, S32 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S32_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S32(S32 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S32_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S32(S32 * p, int * pf) { return A2C_Validate((PVOID) p, &S32_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S32(S32 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S32_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S33;

extern const A2C_DESCRIPTOR S33_descriptor;
extern const A2C_ITEM_HEADER S33_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S33(S33 ** p) { return A2C_Alloc((PVOID *) p, &S33_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S33(S33 * p) { return A2C_Free((PVOID) p, &S33_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S33(S33 * p) { return A2C_Init((PVOID) p, &S33_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S33(S33 * p) { return A2C_Release((PVOID) p, &S33_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S33(S33 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S33_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S33(S33 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S33_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S33(S33 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S33_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S33(S33 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S33_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S33(S33 * pDst, S33 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S33_descriptor); }
A2C_INLINE int A2C_Compare_S33(S33 const * pLeft, S33 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S33_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S33(S33 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S33_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S33(S33 * p, int * pf) { return A2C_Validate((PVOID) p, &S33_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S33(S33 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S33_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S34;

extern const A2C_DESCRIPTOR S34_descriptor;
extern const A2C_ITEM_HEADER S34_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S34(S34 ** p) { return A2C_Alloc((PVOID *) p, &S34_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S34(S34 * p) { return A2C_Free((PVOID) p, &S34_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S34(S34 * p) { return A2C_Init((PVOID) p, &S34_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S34(S34 * p) { return A2C_Release((PVOID) p, &S34_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S34(S34 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S34_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S34(S34 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S34_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S34(S34 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S34_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S34(S34 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S34_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S34(S34 * pDst, S34 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S34_descriptor); }
A2C_INLINE int A2C_Compare_S34(S34 const * pLeft, S34 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S34_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S34(S34 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S34_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S34(S34 * p, int * pf) { return A2C_Validate((PVOID) p, &S34_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S34(S34 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S34_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S35;

extern const A2C_DESCRIPTOR S35_descriptor;
extern const A2C_ITEM_HEADER S35_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S35(S35 ** p) { return A2C_Alloc((PVOID *) p, &S35_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S35(S35 * p) { return A2C_Free((PVOID) p, &S35_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S35(S35 * p) { return A2C_Init((PVOID) p, &S35_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S35(S35 * p) { return A2C_Release((PVOID) p, &S35_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S35(S35 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S35_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S35(S35 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S35_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S35(S35 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S35_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S35(S35 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S35_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S35(S35 * pDst, S35 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S35_descriptor); }
A2C_INLINE int A2C_Compare_S35(S35 const * pLeft, S35 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S35_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S35(S35 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S35_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S35(S35 * p, int * pf) { return A2C_Validate((PVOID) p, &S35_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S35(S35 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S35_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S36;

extern const A2C_DESCRIPTOR S36_descriptor;
extern const A2C_ITEM_HEADER S36_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S36(S36 ** p) { return A2C_Alloc((PVOID *) p, &S36_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S36(S36 * p) { return A2C_Free((PVOID) p, &S36_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S36(S36 * p) { return A2C_Init((PVOID) p, &S36_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S36(S36 * p) { return A2C_Release((PVOID) p, &S36_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S36(S36 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S36_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S36(S36 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S36_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S36(S36 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S36_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S36(S36 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S36_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S36(S36 * pDst, S36 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S36_descriptor); }
A2C_INLINE int A2C_Compare_S36(S36 const * pLeft, S36 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S36_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S36(S36 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S36_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S36(S36 * p, int * pf) { return A2C_Validate((PVOID) p, &S36_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S36(S36 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S36_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
} S37;

extern const A2C_DESCRIPTOR S37_descriptor;
extern const A2C_ITEM_HEADER S37_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S37(S37 ** p) { return A2C_Alloc((PVOID *) p, &S37_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S37(S37 * p) { return A2C_Free((PVOID) p, &S37_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S37(S37 * p) { return A2C_Init((PVOID) p, &S37_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S37(S37 * p) { return A2C_Release((PVOID) p, &S37_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S37(S37 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S37_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S37(S37 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S37_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S37(S37 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S37_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S37(S37 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S37_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S37(S37 * pDst, S37 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S37_descriptor); }
A2C_INLINE int A2C_Compare_S37(S37 const * pLeft, S37 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S37_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S37(S37 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S37_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S37(S37 * p, int * pf) { return A2C_Validate((PVOID) p, &S37_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S37(S37 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S37_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_EXTENSION                        _extension;
} S38;

extern const A2C_DESCRIPTOR S38_descriptor;
extern const A2C_ITEM_HEADER S38_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S38(S38 ** p) { return A2C_Alloc((PVOID *) p, &S38_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S38(S38 * p) { return A2C_Free((PVOID) p, &S38_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S38(S38 * p) { return A2C_Init((PVOID) p, &S38_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S38(S38 * p) { return A2C_Release((PVOID) p, &S38_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S38(S38 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S38_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S38(S38 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S38_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S38(S38 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S38_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S38(S38 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S38_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S38(S38 * pDst, S38 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S38_descriptor); }
A2C_INLINE int A2C_Compare_S38(S38 const * pLeft, S38 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S38_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S38(S38 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S38_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S38(S38 * p, int * pf) { return A2C_Validate((PVOID) p, &S38_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S38(S38 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S38_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S39;

extern const A2C_DESCRIPTOR S39_descriptor;
extern const A2C_ITEM_HEADER S39_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S39(S39 ** p) { return A2C_Alloc((PVOID *) p, &S39_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S39(S39 * p) { return A2C_Free((PVOID) p, &S39_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S39(S39 * p) { return A2C_Init((PVOID) p, &S39_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S39(S39 * p) { return A2C_Release((PVOID) p, &S39_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S39(S39 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S39_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S39(S39 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S39_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S39(S39 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S39_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S39(S39 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S39_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S39(S39 * pDst, S39 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S39_descriptor); }
A2C_INLINE int A2C_Compare_S39(S39 const * pLeft, S39 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S39_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S39(S39 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S39_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S39(S39 * p, int * pf) { return A2C_Validate((PVOID) p, &S39_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S39(S39 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S39_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S40;

extern const A2C_DESCRIPTOR S40_descriptor;
extern const A2C_ITEM_HEADER S40_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S40(S40 ** p) { return A2C_Alloc((PVOID *) p, &S40_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S40(S40 * p) { return A2C_Free((PVOID) p, &S40_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S40(S40 * p) { return A2C_Init((PVOID) p, &S40_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S40(S40 * p) { return A2C_Release((PVOID) p, &S40_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S40(S40 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S40_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S40(S40 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S40_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S40(S40 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S40_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S40(S40 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S40_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S40(S40 * pDst, S40 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S40_descriptor); }
A2C_INLINE int A2C_Compare_S40(S40 const * pLeft, S40 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S40_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S40(S40 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S40_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S40(S40 * p, int * pf) { return A2C_Validate((PVOID) p, &S40_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S40(S40 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S40_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    A2C_INTEGER_NATIVE                   e2;
    A2C_EXTENSION                        _extension;
} S41;

extern const A2C_DESCRIPTOR S41_descriptor;
extern const A2C_ITEM_HEADER S41_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S41(S41 ** p) { return A2C_Alloc((PVOID *) p, &S41_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S41(S41 * p) { return A2C_Free((PVOID) p, &S41_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S41(S41 * p) { return A2C_Init((PVOID) p, &S41_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S41(S41 * p) { return A2C_Release((PVOID) p, &S41_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S41(S41 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S41_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S41(S41 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S41_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S41(S41 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S41_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S41(S41 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S41_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S41(S41 * pDst, S41 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S41_descriptor); }
A2C_INLINE int A2C_Compare_S41(S41 const * pLeft, S41 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S41_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S41(S41 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S41_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S41(S41 * p, int * pf) { return A2C_Validate((PVOID) p, &S41_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S41(S41 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S41_descriptor, ppcxt, pf, pstm); }

typedef struct {
    /* Start Extension */
    /* Exception */
    A2C_INTEGER_NATIVE                   e1;
    /* Extension Group */
    A2C_INTEGER_NATIVE                   e3;

    A2C_EXTENSION                        _extension;
} S42;

extern const A2C_DESCRIPTOR S42_descriptor;
extern const A2C_ITEM_HEADER S42_description;
A2C_INLINE A2C_ERROR A2C_Alloc_S42(S42 ** p) { return A2C_Alloc((PVOID *) p, &S42_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_S42(S42 * p) { return A2C_Free((PVOID) p, &S42_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_S42(S42 * p) { return A2C_Init((PVOID) p, &S42_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_S42(S42 * p) { return A2C_Release((PVOID) p, &S42_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_S42(S42 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &S42_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_S42(S42 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &S42_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_S42(S42 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &S42_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_S42(S42 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &S42_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_S42(S42 * pDst, S42 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &S42_descriptor); }
A2C_INLINE int A2C_Compare_S42(S42 const * pLeft, S42 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &S42_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_S42(S42 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &S42_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_S42(S42 * p, int * pf) { return A2C_Validate((PVOID) p, &S42_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_S42(S42 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &S42_descriptor, ppcxt, pf, pstm); }
#ifdef __cplusplus
}
#endif
#endif /* __seq4_good__H__ */
