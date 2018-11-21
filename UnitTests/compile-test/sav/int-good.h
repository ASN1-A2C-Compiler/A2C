#include <A2C.h>
#ifndef __int_good__H__
#define __int_good__H__

#ifdef __cplusplus
extern "C" {
#endif

#define T1__a 2
#define T2__a 3
#define T2__b 1
#define T3__a -1
#define T3__b 0
#define T3__c 1

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

typedef A2C_INTEGER_NATIVE I2;

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

typedef A2C_INTEGER_NATIVE I3;

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

typedef I1 I4;

extern const A2C_DESCRIPTOR I4_descriptor;
extern const A2C_ITEM_HEADER I4_description;
A2C_INLINE A2C_ERROR A2C_Alloc_I4(I4 ** p) { return A2C_Alloc((PVOID *) p, &I4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_I4(I4 * p) { return A2C_Free((PVOID) p, &I4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_I4(I4 * p) { return A2C_Init((PVOID) p, &I4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_I4(I4 * p) { return A2C_Release((PVOID) p, &I4_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_I4(I4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &I4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_I4(I4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &I4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_I4(I4 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &I4_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_I4(I4 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &I4_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_I4(I4 * pDst, I4 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &I4_descriptor); }
A2C_INLINE int A2C_Compare_I4(I4 const * pLeft, I4 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &I4_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_I4(I4 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &I4_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_I4(I4 * p, int * pf) { return A2C_Validate((PVOID) p, &I4_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_I4(I4 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &I4_descriptor, ppcxt, pf, pstm); }

typedef A2C_INTEGER_NATIVE I5;

extern const A2C_DESCRIPTOR I5_descriptor;
extern const A2C_ITEM_HEADER I5_description;
A2C_INLINE A2C_ERROR A2C_Alloc_I5(I5 ** p) { return A2C_Alloc((PVOID *) p, &I5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_I5(I5 * p) { return A2C_Free((PVOID) p, &I5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_I5(I5 * p) { return A2C_Init((PVOID) p, &I5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_I5(I5 * p) { return A2C_Release((PVOID) p, &I5_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_I5(I5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &I5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_I5(I5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &I5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_I5(I5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &I5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_I5(I5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &I5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_I5(I5 * pDst, I5 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &I5_descriptor); }
A2C_INLINE int A2C_Compare_I5(I5 const * pLeft, I5 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &I5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_I5(I5 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &I5_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_I5(I5 * p, int * pf) { return A2C_Validate((PVOID) p, &I5_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_I5(I5 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &I5_descriptor, ppcxt, pf, pstm); }
extern const I5 i5_1;
extern const I5 i5_2;
extern const A2C_INTEGER_NATIVE a;

typedef A2C_INTEGER_NATIVE T1;

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

typedef A2C_INTEGER_NATIVE T2;

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

typedef A2C_INTEGER_NATIVE T3;

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
extern const A2C_INTEGER_NATIVE e;
#ifdef __cplusplus
}
#endif
#endif /* __int_good__H__ */
