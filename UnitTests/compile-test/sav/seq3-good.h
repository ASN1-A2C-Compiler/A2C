#include <A2C.h>
#ifndef __seq3_good__H__
#define __seq3_good__H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _S1 * PS1;

typedef struct {
    unsigned int optionalFields;
#define S2_s1_present 0x1
    A2C_INTEGER_NATIVE                   i2;
    PS1                                  s1;
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

typedef struct _S1 {
    A2C_INTEGER_NATIVE                   i1;
    S2                                   s2;
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
    A2C_OBJECT_IDENTIFIER                type;
    A2C_INTEGER_NATIVE                   value;
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
    int count;
    int countAllocated;
    S6 *                                 array;
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

typedef struct {
    int count;
    int countAllocated;
    S5 *                                 array;
} SO4;

extern const A2C_DESCRIPTOR SO4_descriptor;
extern const A2C_ITEM_HEADER SO4_description;
A2C_INLINE A2C_ERROR A2C_Alloc_SO4(SO4 ** p) { return A2C_Alloc((PVOID *) p, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_SO4(SO4 * p) { return A2C_Free((PVOID) p, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_SO4(SO4 * p) { return A2C_Init((PVOID) p, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_SO4(SO4 * p) { return A2C_Release((PVOID) p, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_SO4(SO4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &SO4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_SO4(SO4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &SO4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_SO4(SO4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &SO4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_SO4(SO4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &SO4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_SO4(SO4 * pDst, SO4 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &SO4_descriptor); }
A2C_INLINE int A2C_Compare_SO4(SO4 const * pLeft, SO4 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_SO4(SO4 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &SO4_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_SO4(SO4 * p, int * pf) { return A2C_Validate((PVOID) p, &SO4_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_SO4(SO4 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &SO4_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_AllocArray_SO4(SO4 * p, int c) { return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &SO4_descriptor, c); }
A2C_INLINE A2C_ERROR A2C_FreeArray_SO4(SO4 * p) { return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &SO4_descriptor); }
A2C_INLINE A2C_ERROR A2C_ReallocArray_SO4(SO4 * p, int c) { return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &SO4_descriptor, c); }

typedef struct {
    int index;
    union {
#define S3__u_c1_index 0
        SO4                              c1;
    } u;
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
#ifdef __cplusplus
}
#endif
#endif /* __seq3_good__H__ */
