#include <A2C.h>
#ifndef __open_good__H__
#define __open_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    const A2C_OBJECT_IDENTIFIER *        id;
    const A2C_DESCRIPTOR *               Type;
} CONTENT_TYPE;
extern const A2C_DESCRIPTOR CONTENT_TYPE_descriptor;
extern const A2C_ITEM_HEADER CONTENT_TYPE_description;
extern const A2C_OBJECT_IDENTIFIER id_ct;
extern const A2C_OBJECT_IDENTIFIER id_data;
extern const CONTENT_TYPE ct_T1;
extern const A2C_OBJECT_IDENTIFIER id_ct2;
extern const CONTENT_TYPE ct_T2;

typedef struct {
    int index;
    union {
#define ct_T3__Type__u_teletexString_index 0
        A2C_TELETEX_STRING               teletexString;
#define ct_T3__Type__u_printableString_index 1
        A2C_PRINTABLE_STRING             printableString;
#define ct_T3__Type__u_universalString_index 2
        A2C_UNIVERSAL_STRING             universalString;
#define ct_T3__Type__u_utf8String_index 3
        A2C_UTF8_STRING                  utf8String;
#define ct_T3__Type__u_bmpString_index 4
        A2C_BMP_STRING                   bmpString;
    } u;
} ct_T3__Type;

extern const A2C_DESCRIPTOR ct_T3__Type_descriptor;
extern const A2C_ITEM_HEADER ct_T3__Type_description;
A2C_INLINE A2C_ERROR A2C_Alloc_ct_T3__Type(ct_T3__Type ** p) { return A2C_Alloc((PVOID *) p, &ct_T3__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_ct_T3__Type(ct_T3__Type * p) { return A2C_Free((PVOID) p, &ct_T3__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_ct_T3__Type(ct_T3__Type * p) { return A2C_Init((PVOID) p, &ct_T3__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_ct_T3__Type(ct_T3__Type * p) { return A2C_Release((PVOID) p, &ct_T3__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_ct_T3__Type(ct_T3__Type const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &ct_T3__Type_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_ct_T3__Type(ct_T3__Type ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &ct_T3__Type_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_ct_T3__Type(ct_T3__Type const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &ct_T3__Type_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_ct_T3__Type(ct_T3__Type ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &ct_T3__Type_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_ct_T3__Type(ct_T3__Type * pDst, ct_T3__Type const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &ct_T3__Type_descriptor); }
A2C_INLINE int A2C_Compare_ct_T3__Type(ct_T3__Type const * pLeft, ct_T3__Type const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &ct_T3__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_ct_T3__Type(ct_T3__Type * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &ct_T3__Type_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_ct_T3__Type(ct_T3__Type * p, int * pf) { return A2C_Validate((PVOID) p, &ct_T3__Type_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_ct_T3__Type(ct_T3__Type * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &ct_T3__Type_descriptor, ppcxt, pf, pstm); }
extern const CONTENT_TYPE ct_T3;

typedef A2C_PRINTABLE_STRING ct_T4__Type;

extern const A2C_DESCRIPTOR ct_T4__Type_descriptor;
extern const A2C_ITEM_HEADER ct_T4__Type_description;
A2C_INLINE A2C_ERROR A2C_Alloc_ct_T4__Type(ct_T4__Type ** p) { return A2C_Alloc((PVOID *) p, &ct_T4__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_ct_T4__Type(ct_T4__Type * p) { return A2C_Free((PVOID) p, &ct_T4__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_ct_T4__Type(ct_T4__Type * p) { return A2C_Init((PVOID) p, &ct_T4__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_ct_T4__Type(ct_T4__Type * p) { return A2C_Release((PVOID) p, &ct_T4__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_ct_T4__Type(ct_T4__Type const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &ct_T4__Type_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_ct_T4__Type(ct_T4__Type ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &ct_T4__Type_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_ct_T4__Type(ct_T4__Type const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &ct_T4__Type_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_ct_T4__Type(ct_T4__Type ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &ct_T4__Type_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_ct_T4__Type(ct_T4__Type * pDst, ct_T4__Type const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &ct_T4__Type_descriptor); }
A2C_INLINE int A2C_Compare_ct_T4__Type(ct_T4__Type const * pLeft, ct_T4__Type const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &ct_T4__Type_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_ct_T4__Type(ct_T4__Type * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &ct_T4__Type_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_ct_T4__Type(ct_T4__Type * p, int * pf) { return A2C_Validate((PVOID) p, &ct_T4__Type_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_ct_T4__Type(ct_T4__Type * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &ct_T4__Type_descriptor, ppcxt, pf, pstm); }
extern const CONTENT_TYPE ct_T4;

extern const A2C_OBJECT_SET ContentSet;

typedef struct {
    A2C_OBJECT_IDENTIFIER                contentType;
    A2C_OPEN_TYPE                        content;
} ContentInfo;

extern const A2C_DESCRIPTOR ContentInfo_descriptor;
extern const A2C_ITEM_HEADER ContentInfo_description;
A2C_INLINE A2C_ERROR A2C_Alloc_ContentInfo(ContentInfo ** p) { return A2C_Alloc((PVOID *) p, &ContentInfo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_ContentInfo(ContentInfo * p) { return A2C_Free((PVOID) p, &ContentInfo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_ContentInfo(ContentInfo * p) { return A2C_Init((PVOID) p, &ContentInfo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_ContentInfo(ContentInfo * p) { return A2C_Release((PVOID) p, &ContentInfo_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_ContentInfo(ContentInfo const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &ContentInfo_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_ContentInfo(ContentInfo ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &ContentInfo_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_ContentInfo(ContentInfo const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &ContentInfo_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_ContentInfo(ContentInfo ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &ContentInfo_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_ContentInfo(ContentInfo * pDst, ContentInfo const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &ContentInfo_descriptor); }
A2C_INLINE int A2C_Compare_ContentInfo(ContentInfo const * pLeft, ContentInfo const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &ContentInfo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_ContentInfo(ContentInfo * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &ContentInfo_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_ContentInfo(ContentInfo * p, int * pf) { return A2C_Validate((PVOID) p, &ContentInfo_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_ContentInfo(ContentInfo * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &ContentInfo_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_OBJECT_IDENTIFIER                contentType;
    struct {
        int count;
        int countAllocated;
        A2C_OPEN_TYPE *                  array;
    } content;
} ContentInfo2;

extern const A2C_DESCRIPTOR ContentInfo2_descriptor;
extern const A2C_ITEM_HEADER ContentInfo2_description;
A2C_INLINE A2C_ERROR A2C_Alloc_ContentInfo2(ContentInfo2 ** p) { return A2C_Alloc((PVOID *) p, &ContentInfo2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_ContentInfo2(ContentInfo2 * p) { return A2C_Free((PVOID) p, &ContentInfo2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_ContentInfo2(ContentInfo2 * p) { return A2C_Init((PVOID) p, &ContentInfo2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_ContentInfo2(ContentInfo2 * p) { return A2C_Release((PVOID) p, &ContentInfo2_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_ContentInfo2(ContentInfo2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &ContentInfo2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_ContentInfo2(ContentInfo2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &ContentInfo2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_ContentInfo2(ContentInfo2 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &ContentInfo2_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_ContentInfo2(ContentInfo2 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &ContentInfo2_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_ContentInfo2(ContentInfo2 * pDst, ContentInfo2 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &ContentInfo2_descriptor); }
A2C_INLINE int A2C_Compare_ContentInfo2(ContentInfo2 const * pLeft, ContentInfo2 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &ContentInfo2_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_ContentInfo2(ContentInfo2 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &ContentInfo2_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_ContentInfo2(ContentInfo2 * p, int * pf) { return A2C_Validate((PVOID) p, &ContentInfo2_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_ContentInfo2(ContentInfo2 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &ContentInfo2_descriptor, ppcxt, pf, pstm); }

typedef struct {
    A2C_OBJECT_IDENTIFIER                contentType;
    A2C_OPEN_OCTET_STRING                content;
} ContentInfo3;

extern const A2C_DESCRIPTOR ContentInfo3_descriptor;
extern const A2C_ITEM_HEADER ContentInfo3_description;
A2C_INLINE A2C_ERROR A2C_Alloc_ContentInfo3(ContentInfo3 ** p) { return A2C_Alloc((PVOID *) p, &ContentInfo3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_ContentInfo3(ContentInfo3 * p) { return A2C_Free((PVOID) p, &ContentInfo3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_ContentInfo3(ContentInfo3 * p) { return A2C_Init((PVOID) p, &ContentInfo3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_ContentInfo3(ContentInfo3 * p) { return A2C_Release((PVOID) p, &ContentInfo3_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_ContentInfo3(ContentInfo3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &ContentInfo3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_ContentInfo3(ContentInfo3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &ContentInfo3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_ContentInfo3(ContentInfo3 const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &ContentInfo3_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_ContentInfo3(ContentInfo3 ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &ContentInfo3_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_ContentInfo3(ContentInfo3 * pDst, ContentInfo3 const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &ContentInfo3_descriptor); }
A2C_INLINE int A2C_Compare_ContentInfo3(ContentInfo3 const * pLeft, ContentInfo3 const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &ContentInfo3_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_ContentInfo3(ContentInfo3 * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &ContentInfo3_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_ContentInfo3(ContentInfo3 * p, int * pf) { return A2C_Validate((PVOID) p, &ContentInfo3_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_ContentInfo3(ContentInfo3 * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &ContentInfo3_descriptor, ppcxt, pf, pstm); }
extern const A2C_OBJECT_IDENTIFIER ct_T3__id;
extern const A2C_OBJECT_IDENTIFIER ct_T4__id;
#ifdef __cplusplus
}
#endif
#endif /* __open_good__H__ */
