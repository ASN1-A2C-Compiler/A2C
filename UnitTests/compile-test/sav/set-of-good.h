#include <A2C.h>
#ifndef __set_of_good__H__
#define __set_of_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int count;
    int countAllocated;
    A2C_INTEGER_NATIVE *                 array;
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
A2C_INLINE A2C_ERROR A2C_AllocArray_S1(S1 * p, int c) { return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &S1_descriptor, c); }
A2C_INLINE A2C_ERROR A2C_FreeArray_S1(S1 * p) { return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &S1_descriptor); }
A2C_INLINE A2C_ERROR A2C_ReallocArray_S1(S1 * p, int c) { return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &S1_descriptor, c); }

typedef struct {
    int count;
    int countAllocated;
    A2C_INTEGER_NATIVE *                 i1;
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
A2C_INLINE A2C_ERROR A2C_AllocArray_S2(S2 * p, int c) { return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &S2_descriptor, c); }
A2C_INLINE A2C_ERROR A2C_FreeArray_S2(S2 * p) { return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &S2_descriptor); }
A2C_INLINE A2C_ERROR A2C_ReallocArray_S2(S2 * p, int c) { return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &S2_descriptor, c); }

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    A2C_INTEGER_NATIVE                   i2;
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
    int count;
    int countAllocated;
    S3 *                                 array;
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
A2C_INLINE A2C_ERROR A2C_AllocArray_S4(S4 * p, int c) { return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &S4_descriptor, c); }
A2C_INLINE A2C_ERROR A2C_FreeArray_S4(S4 * p) { return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &S4_descriptor); }
A2C_INLINE A2C_ERROR A2C_ReallocArray_S4(S4 * p, int c) { return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &S4_descriptor, c); }

typedef struct {
    int count;
    int countAllocated;
    S3 *                                 s3;
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
A2C_INLINE A2C_ERROR A2C_AllocArray_S5(S5 * p, int c) { return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &S5_descriptor, c); }
A2C_INLINE A2C_ERROR A2C_FreeArray_S5(S5 * p) { return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &S5_descriptor); }
A2C_INLINE A2C_ERROR A2C_ReallocArray_S5(S5 * p, int c) { return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &S5_descriptor, c); }
extern const S1 v1;
extern const S1 v2;
extern const S2 v3;
extern const S4 v4;
extern const S5 v5;
#ifdef __cplusplus
}
#endif
#endif /* __set_of_good__H__ */
