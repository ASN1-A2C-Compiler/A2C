#include <A2C.h>
#ifndef __bitstring_good__H__
#define __bitstring_good__H__

#ifdef __cplusplus
extern "C" {
#endif

#define T1__a 2
#define T2__a 3
#define T2__b 1
#define T3__a 3
#define T3__b 0
#define T3__c 1
#define T4__a1 1
extern const A2C_INTEGER_NATIVE a;

typedef A2C_BIT_STRING T0;

extern const A2C_DESCRIPTOR T0_descriptor;
extern const A2C_ITEM_HEADER T0_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T0(T0 ** p) { return A2C_Alloc((PVOID *) p, &T0_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T0(T0 * p) { return A2C_Free((PVOID) p, &T0_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T0(T0 * p) { return A2C_Init((PVOID) p, &T0_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T0(T0 * p) { return A2C_Release((PVOID) p, &T0_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T0(T0 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T0_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T0(T0 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T0_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T0(T0 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T0_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T0(T0 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T0_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T0(T0 * pDst, T0 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T0_descriptor); }
A2C_INLINE int A2C_Compare_T0(T0 const * pLeft, T0 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T0_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T0(T0 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T0_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T0(T0 * p, int * pf) { return A2C_Validate((PVOID) p, &T0_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T0(T0 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T0_descriptor, ppcxt, pf, pstm); }

typedef A2C_BIT_STRING_NAMED T1;

extern const A2C_DESCRIPTOR T1_descriptor;
extern const A2C_ITEM_HEADER T1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T1(T1 ** p) { return A2C_Alloc((PVOID *) p, &T1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T1(T1 * p) { return A2C_Free((PVOID) p, &T1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T1(T1 * p) { return A2C_Init((PVOID) p, &T1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T1(T1 * p) { return A2C_Release((PVOID) p, &T1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T1(T1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T1(T1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T1(T1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T1(T1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T1(T1 * pDst, T1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T1_descriptor); }
A2C_INLINE int A2C_Compare_T1(T1 const * pLeft, T1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T1(T1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T1(T1 * p, int * pf) { return A2C_Validate((PVOID) p, &T1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T1(T1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T1_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T1(A2C_NAMES const ** p) { return A2C_List(&T1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T1(char const * szName, int * pi) { return A2C_NameToInt(&T1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T1(int i, const char ** pszName) { return A2C_IntToName(&T1_descriptor, i, pszName); }

typedef A2C_BIT_STRING_NAMED T2;

extern const A2C_DESCRIPTOR T2_descriptor;
extern const A2C_ITEM_HEADER T2_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T2(T2 ** p) { return A2C_Alloc((PVOID *) p, &T2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T2(T2 * p) { return A2C_Free((PVOID) p, &T2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T2(T2 * p) { return A2C_Init((PVOID) p, &T2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T2(T2 * p) { return A2C_Release((PVOID) p, &T2_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T2(T2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T2(T2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T2(T2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T2(T2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T2(T2 * pDst, T2 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T2_descriptor); }
A2C_INLINE int A2C_Compare_T2(T2 const * pLeft, T2 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T2(T2 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T2_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T2(T2 * p, int * pf) { return A2C_Validate((PVOID) p, &T2_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T2(T2 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T2_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T2(A2C_NAMES const ** p) { return A2C_List(&T2_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T2(char const * szName, int * pi) { return A2C_NameToInt(&T2_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T2(int i, const char ** pszName) { return A2C_IntToName(&T2_descriptor, i, pszName); }
extern const T2 c;
extern const T2 d;
extern const T2 e;
extern const T2 f;
extern const T2 g;
extern const T0 h;
extern const T0 i;

typedef A2C_BIT_STRING_NAMED T3;

extern const A2C_DESCRIPTOR T3_descriptor;
extern const A2C_ITEM_HEADER T3_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T3(T3 ** p) { return A2C_Alloc((PVOID *) p, &T3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T3(T3 * p) { return A2C_Free((PVOID) p, &T3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T3(T3 * p) { return A2C_Init((PVOID) p, &T3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T3(T3 * p) { return A2C_Release((PVOID) p, &T3_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T3(T3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T3(T3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T3(T3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T3(T3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T3(T3 * pDst, T3 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T3_descriptor); }
A2C_INLINE int A2C_Compare_T3(T3 const * pLeft, T3 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T3(T3 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T3_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T3(T3 * p, int * pf) { return A2C_Validate((PVOID) p, &T3_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T3(T3 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T3_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T3(A2C_NAMES const ** p) { return A2C_List(&T3_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T3(char const * szName, int * pi) { return A2C_NameToInt(&T3_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T3(int i, const char ** pszName) { return A2C_IntToName(&T3_descriptor, i, pszName); }
extern const A2C_INTEGER_NATIVE a1;

typedef A2C_BIT_STRING_NAMED T4;

extern const A2C_DESCRIPTOR T4_descriptor;
extern const A2C_ITEM_HEADER T4_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T4(T4 ** p) { return A2C_Alloc((PVOID *) p, &T4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T4(T4 * p) { return A2C_Free((PVOID) p, &T4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T4(T4 * p) { return A2C_Init((PVOID) p, &T4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T4(T4 * p) { return A2C_Release((PVOID) p, &T4_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T4(T4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T4(T4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T4(T4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T4(T4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T4(T4 * pDst, T4 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T4_descriptor); }
A2C_INLINE int A2C_Compare_T4(T4 const * pLeft, T4 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T4(T4 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T4_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T4(T4 * p, int * pf) { return A2C_Validate((PVOID) p, &T4_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T4(T4 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T4_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T4(A2C_NAMES const ** p) { return A2C_List(&T4_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T4(char const * szName, int * pi) { return A2C_NameToInt(&T4_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T4(int i, const char ** pszName) { return A2C_IntToName(&T4_descriptor, i, pszName); }
#ifdef __cplusplus
}
#endif
#endif /* __bitstring_good__H__ */
