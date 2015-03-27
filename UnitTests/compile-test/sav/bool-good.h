#include <A2C.h>
#ifndef __bool_good__H__
#define __bool_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef A2C_BOOLEAN I1;

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

typedef A2C_BOOLEAN I2;

extern const A2C_DESCRIPTOR I2_descriptor;
extern const A2C_ITEM_HEADER I2_description;
A2C_INLINE A2C_ERROR A2C_Alloc_I2(I2 ** p) { return A2C_Alloc((PVOID *) p, &I2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_I2(I2 * p) { return A2C_Free((PVOID) p, &I2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_I2(I2 * p) { return A2C_Init((PVOID) p, &I2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_I2(I2 * p) { return A2C_Release((PVOID) p, &I2_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_I2(I2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &I2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_I2(I2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &I2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_I2(I2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &I2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_I2(I2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &I2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_I2(I2 * pDst, I2 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &I2_descriptor); }
A2C_INLINE int A2C_Compare_I2(I2 const * pLeft, I2 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &I2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_I2(I2 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &I2_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_I2(I2 * p, int * pf) { return A2C_Validate((PVOID) p, &I2_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_I2(I2 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &I2_descriptor, ppcxt, pf, pstm); }

typedef A2C_BOOLEAN I3;

extern const A2C_DESCRIPTOR I3_descriptor;
extern const A2C_ITEM_HEADER I3_description;
A2C_INLINE A2C_ERROR A2C_Alloc_I3(I3 ** p) { return A2C_Alloc((PVOID *) p, &I3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_I3(I3 * p) { return A2C_Free((PVOID) p, &I3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_I3(I3 * p) { return A2C_Init((PVOID) p, &I3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_I3(I3 * p) { return A2C_Release((PVOID) p, &I3_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_I3(I3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &I3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_I3(I3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &I3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_I3(I3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &I3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_I3(I3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &I3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_I3(I3 * pDst, I3 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &I3_descriptor); }
A2C_INLINE int A2C_Compare_I3(I3 const * pLeft, I3 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &I3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_I3(I3 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &I3_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_I3(I3 * p, int * pf) { return A2C_Validate((PVOID) p, &I3_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_I3(I3 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &I3_descriptor, ppcxt, pf, pstm); }
extern const A2C_BOOLEAN t;
extern const A2C_BOOLEAN f;
extern const A2C_BOOLEAN t2;
#ifdef __cplusplus
}
#endif
#endif /* __bool_good__H__ */
