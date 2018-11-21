#include <A2C.h>
#ifndef __null_good__H__
#define __null_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef A2C_NULL I1;

extern const A2C_DESCRIPTOR I1_descriptor;
extern const A2C_ITEM_HEADER I1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_I1(I1 ** p) { return A2C_Alloc((PVOID *) p, &I1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_I1(I1 * p) { return A2C_Free((PVOID) p, &I1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_I1(I1 * p) { return A2C_Init((PVOID) p, &I1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_I1(I1 * p) { return A2C_Release((PVOID) p, &I1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_I1(I1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &I1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_I1(I1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &I1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_I1(I1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &I1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_I1(I1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &I1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_I1(I1 * pDst, I1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &I1_descriptor); }
A2C_INLINE int A2C_Compare_I1(I1 const * pLeft, I1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &I1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_I1(I1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &I1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_I1(I1 * p, int * pf) { return A2C_Validate((PVOID) p, &I1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_I1(I1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &I1_descriptor, ppcxt, pf, pstm); }
extern const I1 i1;
#ifdef __cplusplus
}
#endif
#endif /* __null_good__H__ */
