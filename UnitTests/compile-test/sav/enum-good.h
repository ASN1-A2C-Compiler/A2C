#include <A2C.h>
#ifndef __enum_good__H__
#define __enum_good__H__

#ifdef __cplusplus
extern "C" {
#endif

#define T1__a 2
#define T2__a 3
#define T2__b 5
#define T3__a -1
#define T3__b 0
#define T3__c 1
#define T4__a 0
#define T4__b 1
#define T4__c 2
#define T4__d 3
#define T4__e 4
#define T4__f 5
#define T8__a 0
#define T8__b 1
#define T5__a 1
#define T5__b 2
#define T6__a 1
#define T6__b 2
#define T6__c 3
#define T6__d 4
#define T7__a 1
#define T7__b 2
#define T7__c 3
#define T7__d 4
#define C__b 3
#define C__a 0
#define C__c 1
#define D__a 0
#define D__b 1
#define D__c 2
#define A1__a 0
#define A1__b 1
#define A1__c 2
#define B1__c 0
#define B1__a 1
#define B1__b 2
#define B1__d 3
#define C1__a 0
#define C1__b 1
#define C1__c 3
#define C1__d 4
#define D1__z 25
#define D1__a 0
#define D1__d 1
extern const A2C_INTEGER_NATIVE a;

typedef A2C_ENUMERATED_NATIVE T1;

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

typedef A2C_ENUMERATED_NATIVE T2;

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

typedef A2C_ENUMERATED_NATIVE T3;

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

typedef A2C_ENUMERATED_NATIVE T4;

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

typedef A2C_ENUMERATED_NATIVE T8;

extern const A2C_DESCRIPTOR T8_descriptor;
extern const A2C_ITEM_HEADER T8_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T8(T8 ** p) { return A2C_Alloc((PVOID *) p, &T8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T8(T8 * p) { return A2C_Free((PVOID) p, &T8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T8(T8 * p) { return A2C_Init((PVOID) p, &T8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T8(T8 * p) { return A2C_Release((PVOID) p, &T8_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T8(T8 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T8_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T8(T8 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T8_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T8(T8 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T8_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T8(T8 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T8_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T8(T8 * pDst, T8 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T8_descriptor); }
A2C_INLINE int A2C_Compare_T8(T8 const * pLeft, T8 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T8_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T8(T8 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T8_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T8(T8 * p, int * pf) { return A2C_Validate((PVOID) p, &T8_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T8(T8 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T8_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T8(A2C_NAMES const ** p) { return A2C_List(&T8_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T8(char const * szName, int * pi) { return A2C_NameToInt(&T8_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T8(int i, const char ** pszName) { return A2C_IntToName(&T8_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE T5;

extern const A2C_DESCRIPTOR T5_descriptor;
extern const A2C_ITEM_HEADER T5_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T5(T5 ** p) { return A2C_Alloc((PVOID *) p, &T5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T5(T5 * p) { return A2C_Free((PVOID) p, &T5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T5(T5 * p) { return A2C_Init((PVOID) p, &T5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T5(T5 * p) { return A2C_Release((PVOID) p, &T5_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T5(T5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T5(T5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T5(T5 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T5_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T5(T5 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T5_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T5(T5 * pDst, T5 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T5_descriptor); }
A2C_INLINE int A2C_Compare_T5(T5 const * pLeft, T5 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T5_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T5(T5 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T5_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T5(T5 * p, int * pf) { return A2C_Validate((PVOID) p, &T5_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T5(T5 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T5_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T5(A2C_NAMES const ** p) { return A2C_List(&T5_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T5(char const * szName, int * pi) { return A2C_NameToInt(&T5_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T5(int i, const char ** pszName) { return A2C_IntToName(&T5_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE T6;

extern const A2C_DESCRIPTOR T6_descriptor;
extern const A2C_ITEM_HEADER T6_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T6(T6 ** p) { return A2C_Alloc((PVOID *) p, &T6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T6(T6 * p) { return A2C_Free((PVOID) p, &T6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T6(T6 * p) { return A2C_Init((PVOID) p, &T6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T6(T6 * p) { return A2C_Release((PVOID) p, &T6_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T6(T6 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T6_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T6(T6 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T6_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T6(T6 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T6_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T6(T6 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T6_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T6(T6 * pDst, T6 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T6_descriptor); }
A2C_INLINE int A2C_Compare_T6(T6 const * pLeft, T6 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T6_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T6(T6 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T6_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T6(T6 * p, int * pf) { return A2C_Validate((PVOID) p, &T6_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T6(T6 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T6_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T6(A2C_NAMES const ** p) { return A2C_List(&T6_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T6(char const * szName, int * pi) { return A2C_NameToInt(&T6_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T6(int i, const char ** pszName) { return A2C_IntToName(&T6_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE T7;

extern const A2C_DESCRIPTOR T7_descriptor;
extern const A2C_ITEM_HEADER T7_description;
A2C_INLINE A2C_ERROR A2C_Alloc_T7(T7 ** p) { return A2C_Alloc((PVOID *) p, &T7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_T7(T7 * p) { return A2C_Free((PVOID) p, &T7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_T7(T7 * p) { return A2C_Init((PVOID) p, &T7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_T7(T7 * p) { return A2C_Release((PVOID) p, &T7_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_T7(T7 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &T7_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_T7(T7 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &T7_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_T7(T7 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &T7_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_T7(T7 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &T7_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_T7(T7 * pDst, T7 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &T7_descriptor); }
A2C_INLINE int A2C_Compare_T7(T7 const * pLeft, T7 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &T7_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_T7(T7 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &T7_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_T7(T7 * p, int * pf) { return A2C_Validate((PVOID) p, &T7_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_T7(T7 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &T7_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_T7(A2C_NAMES const ** p) { return A2C_List(&T7_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_T7(char const * szName, int * pi) { return A2C_NameToInt(&T7_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_T7(int i, const char ** pszName) { return A2C_IntToName(&T7_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE C;

extern const A2C_DESCRIPTOR C_descriptor;
extern const A2C_ITEM_HEADER C_description;
A2C_INLINE A2C_ERROR A2C_Alloc_C(C ** p) { return A2C_Alloc((PVOID *) p, &C_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_C(C * p) { return A2C_Free((PVOID) p, &C_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_C(C * p) { return A2C_Init((PVOID) p, &C_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_C(C * p) { return A2C_Release((PVOID) p, &C_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_C(C const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &C_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_C(C ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &C_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_C(C const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &C_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_C(C ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &C_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_C(C * pDst, C const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &C_descriptor); }
A2C_INLINE int A2C_Compare_C(C const * pLeft, C const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &C_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_C(C * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &C_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_C(C * p, int * pf) { return A2C_Validate((PVOID) p, &C_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_C(C * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &C_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_C(A2C_NAMES const ** p) { return A2C_List(&C_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_C(char const * szName, int * pi) { return A2C_NameToInt(&C_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_C(int i, const char ** pszName) { return A2C_IntToName(&C_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE D;

extern const A2C_DESCRIPTOR D_descriptor;
extern const A2C_ITEM_HEADER D_description;
A2C_INLINE A2C_ERROR A2C_Alloc_D(D ** p) { return A2C_Alloc((PVOID *) p, &D_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_D(D * p) { return A2C_Free((PVOID) p, &D_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_D(D * p) { return A2C_Init((PVOID) p, &D_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_D(D * p) { return A2C_Release((PVOID) p, &D_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_D(D const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &D_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_D(D ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &D_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_D(D const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &D_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_D(D ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &D_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_D(D * pDst, D const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &D_descriptor); }
A2C_INLINE int A2C_Compare_D(D const * pLeft, D const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &D_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_D(D * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &D_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_D(D * p, int * pf) { return A2C_Validate((PVOID) p, &D_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_D(D * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &D_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_D(A2C_NAMES const ** p) { return A2C_List(&D_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_D(char const * szName, int * pi) { return A2C_NameToInt(&D_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_D(int i, const char ** pszName) { return A2C_IntToName(&D_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE A1;

extern const A2C_DESCRIPTOR A1_descriptor;
extern const A2C_ITEM_HEADER A1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_A1(A1 ** p) { return A2C_Alloc((PVOID *) p, &A1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_A1(A1 * p) { return A2C_Free((PVOID) p, &A1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_A1(A1 * p) { return A2C_Init((PVOID) p, &A1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_A1(A1 * p) { return A2C_Release((PVOID) p, &A1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_A1(A1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_A1(A1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_A1(A1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_A1(A1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_A1(A1 * pDst, A1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A1_descriptor); }
A2C_INLINE int A2C_Compare_A1(A1 const * pLeft, A1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_A1(A1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_A1(A1 * p, int * pf) { return A2C_Validate((PVOID) p, &A1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_A1(A1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A1_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_A1(A2C_NAMES const ** p) { return A2C_List(&A1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_A1(char const * szName, int * pi) { return A2C_NameToInt(&A1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_A1(int i, const char ** pszName) { return A2C_IntToName(&A1_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE B1;

extern const A2C_DESCRIPTOR B1_descriptor;
extern const A2C_ITEM_HEADER B1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_B1(B1 ** p) { return A2C_Alloc((PVOID *) p, &B1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_B1(B1 * p) { return A2C_Free((PVOID) p, &B1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_B1(B1 * p) { return A2C_Init((PVOID) p, &B1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_B1(B1 * p) { return A2C_Release((PVOID) p, &B1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_B1(B1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &B1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_B1(B1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &B1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_B1(B1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &B1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_B1(B1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &B1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_B1(B1 * pDst, B1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &B1_descriptor); }
A2C_INLINE int A2C_Compare_B1(B1 const * pLeft, B1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &B1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_B1(B1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &B1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_B1(B1 * p, int * pf) { return A2C_Validate((PVOID) p, &B1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_B1(B1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &B1_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_B1(A2C_NAMES const ** p) { return A2C_List(&B1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_B1(char const * szName, int * pi) { return A2C_NameToInt(&B1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_B1(int i, const char ** pszName) { return A2C_IntToName(&B1_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE C1;

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
A2C_INLINE A2C_ERROR A2C_List_C1(A2C_NAMES const ** p) { return A2C_List(&C1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_C1(char const * szName, int * pi) { return A2C_NameToInt(&C1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_C1(int i, const char ** pszName) { return A2C_IntToName(&C1_descriptor, i, pszName); }

typedef A2C_ENUMERATED_NATIVE D1;

extern const A2C_DESCRIPTOR D1_descriptor;
extern const A2C_ITEM_HEADER D1_description;
A2C_INLINE A2C_ERROR A2C_Alloc_D1(D1 ** p) { return A2C_Alloc((PVOID *) p, &D1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_D1(D1 * p) { return A2C_Free((PVOID) p, &D1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_D1(D1 * p) { return A2C_Init((PVOID) p, &D1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_D1(D1 * p) { return A2C_Release((PVOID) p, &D1_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_D1(D1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &D1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_D1(D1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &D1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_D1(D1 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &D1_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_D1(D1 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &D1_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_D1(D1 * pDst, D1 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &D1_descriptor); }
A2C_INLINE int A2C_Compare_D1(D1 const * pLeft, D1 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &D1_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_D1(D1 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &D1_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_D1(D1 * p, int * pf) { return A2C_Validate((PVOID) p, &D1_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_D1(D1 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &D1_descriptor, ppcxt, pf, pstm); }
A2C_INLINE A2C_ERROR A2C_List_D1(A2C_NAMES const ** p) { return A2C_List(&D1_descriptor, p); }
A2C_INLINE A2C_ERROR A2C_NameToInt_D1(char const * szName, int * pi) { return A2C_NameToInt(&D1_descriptor, szName, pi); }
A2C_INLINE A2C_ERROR A2C_IntToName_D1(int i, const char ** pszName) { return A2C_IntToName(&D1_descriptor, i, pszName); }
#ifdef __cplusplus
}
#endif
#endif /* __enum_good__H__ */
