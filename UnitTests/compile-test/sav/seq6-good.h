#include <A2C.h>
#ifndef __seq6_good__H__
#define __seq6_good__H__

#ifdef __cplusplus
extern "C" {
#endif

#define E1__v1 1
#define I1__v1 2

typedef A2C_ENUMERATED_NATIVE E1;

extern const A2C_DESCRIPTOR E1_descriptor;
extern const A2C_ITEM_HEADER E1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_E1(E1 ** p) { return A2C_Alloc((PVOID *) p, &E1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_E1(E1 * p) { return A2C_Free((PVOID) p, &E1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_E1(E1 * p) { return A2C_Init((PVOID) p, &E1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_E1(E1 * p) { return A2C_Release((PVOID) p, &E1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_E1(E1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &E1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_E1(E1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &E1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_E1(E1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &E1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_E1(E1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &E1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_E1(E1 * pDst, E1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &E1_descriptor); }
A2C_INLINE int A2C_Compare_E1(E1 const * pLeft, E1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &E1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_E1(E1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &E1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_E1(E1 * p, int * pf) { return A2C_Validate((PVOID) p, &E1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_E1(E1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &E1_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_E1(A2C_NAMES const ** p) { return A2C_List(&E1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_E1(char const * szName, int * pi) { return A2C_NameToInt(&E1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_E1(int i, const char ** pszName) { return A2C_IntToName(&E1_descriptor, i, pszName); }

typedef A2C_INTEGER_NATIVE I1;

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
A2C_INLINE A2C_ERROR A2C_List_I1(A2C_NAMES const ** p) { return A2C_List(&I1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_I1(char const * szName, int * pi) { return A2C_NameToInt(&I1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_I1(int i, const char ** pszName) { return A2C_IntToName(&I1_descriptor, i, pszName); }

typedef struct {
    E1                                   f1;
    I1                                   f2;
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
#endif /* __seq6_good__H__ */
