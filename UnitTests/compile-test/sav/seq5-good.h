#include <A2C.h>
#ifndef __seq5_good__H__
#define __seq5_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int index;
    union {
#define C1__u_i1_index 0
        A2C_INTEGER_NATIVE               i1;
#define C1__u_b1_index 1
        A2C_BOOLEAN                      b1;
    } u;
} C1;

extern const A2C_DESCRIPTOR C1_descriptor;
extern const A2C_ITEM_HEADER C1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_C1(C1 ** p) { return A2C_Alloc((PVOID *) p, &C1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_C1(C1 * p) { return A2C_Free((PVOID) p, &C1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_C1(C1 * p) { return A2C_Init((PVOID) p, &C1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_C1(C1 * p) { return A2C_Release((PVOID) p, &C1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_C1(C1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &C1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_C1(C1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &C1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_C1(C1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &C1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_C1(C1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &C1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_C1(C1 * pDst, C1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &C1_descriptor); }
A2C_INLINE int A2C_Compare_C1(C1 const * pLeft, C1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &C1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_C1(C1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &C1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_C1(C1 * p, int * pf) { return A2C_Validate((PVOID) p, &C1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_C1(C1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &C1_descriptor, ppcxt, pf, pstm); }

typedef C1 C2;

extern const A2C_DESCRIPTOR C2_descriptor;
extern const A2C_ITEM_HEADER C2_description;
A2C_INLINE A2C_ERROR A2C_Alloc_C2(C2 ** p) { return A2C_Alloc((PVOID *) p, &C2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_C2(C2 * p) { return A2C_Free((PVOID) p, &C2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_C2(C2 * p) { return A2C_Init((PVOID) p, &C2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_C2(C2 * p) { return A2C_Release((PVOID) p, &C2_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_C2(C2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &C2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_C2(C2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &C2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_C2(C2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &C2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_C2(C2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &C2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_C2(C2 * pDst, C2 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &C2_descriptor); }
A2C_INLINE int A2C_Compare_C2(C2 const * pLeft, C2 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &C2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_C2(C2 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &C2_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_C2(C2 * p, int * pf) { return A2C_Validate((PVOID) p, &C2_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_C2(C2 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &C2_descriptor, ppcxt, pf, pstm); }

typedef struct {
    C1                                   c1;
    C1                                   c2;
    C2                                   c3;
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
#ifdef __cplusplus
}
#endif
#endif /* __seq5_good__H__ */
