#include <A2C.h>
#ifndef __param_good__H__
#define __param_good__H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    const A2C_OBJECT_IDENTIFIER *        id;
    const A2C_DESCRIPTOR *               Type;
} ALGORITHM;
extern const A2C_DESCRIPTOR ALGORITHM_descriptor;
extern const A2C_ITEM_HEADER ALGORITHM_description;

extern const A2C_OBJECT_SET KeyAlgorithms;

typedef struct {
    A2C_INTEGER_NATIVE                   i1;
    struct {
        unsigned int optionalFields;
#define Foo__f2_parameters_present 0x1
        A2C_OBJECT_IDENTIFIER            algorithm;
        A2C_OPEN_TYPE                    parameters;
    } f2;
} Foo;

extern const A2C_DESCRIPTOR Foo_descriptor;
extern const A2C_ITEM_HEADER Foo_description;
A2C_INLINE A2C_ERROR A2C_Alloc_Foo(Foo ** p) { return A2C_Alloc((PVOID *) p, &Foo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Free_Foo(Foo * p) { return A2C_Free((PVOID) p, &Foo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Init_Foo(Foo * p) { return A2C_Init((PVOID) p, &Foo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Release_Foo(Foo * p) { return A2C_Release((PVOID) p, &Foo_descriptor); }
A2C_INLINE A2C_ERROR A2C_EncodeBer_Foo(Foo const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &Foo_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeBer_Foo(Foo ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &Foo_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_EncodeDer_Foo(Foo const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &Foo_descriptor, flags, ppcxt, pstm); }
A2C_INLINE A2C_ERROR A2C_DecodeDer_Foo(Foo ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &Foo_descriptor, flags, ppcxt, pb, cb); }
A2C_INLINE A2C_ERROR A2C_Copy_Foo(Foo * pDst, Foo const * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &Foo_descriptor); }
A2C_INLINE int A2C_Compare_Foo(Foo const * pLeft, Foo const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &Foo_descriptor); }
A2C_INLINE A2C_ERROR A2C_Print_Foo(Foo * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &Foo_descriptor, pstm); }
A2C_INLINE A2C_ERROR A2C_Validate_Foo(Foo * p, int * pf) { return A2C_Validate((PVOID) p, &Foo_descriptor, pf); }
A2C_INLINE A2C_ERROR A2C_Validate2_Foo(Foo * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &Foo_descriptor, ppcxt, pf, pstm); }
#ifdef __cplusplus
}
#endif
#endif /* __param_good__H__ */
