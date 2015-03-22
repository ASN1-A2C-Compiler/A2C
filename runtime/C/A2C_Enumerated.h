/* 
 * Copyright 2008 Jim Schaad and Paul Hoffman. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * This software is provided by Jim Schaad and Paul Hoffman "as is" and
 * any express or implied warranties, including, but not limited to, the
 * implied warranties of merchantability and fitness for a particular
 * purpose are disclaimed. In no event shall Jim Schaad and Paul Hoffman
 * or contributors be liable for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not
 * limited to, procurement of substitute goods or services; loss of use,
 * data, or profits; or business interruption) however caused and on any
 * theory of liability, whether in contract, strict liability, or tort
 * (including negligence or otherwise) arising in any way out of the use
 * of this software, even if advised of the possibility of such damage.
 */

#ifndef __A2C_ENUMERATED_H__
#define __A2C_ENUMERATED_H__

#include "A2C_Base.h"
#include "A2C_Integer.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef A2C_INTEGER_HUGE A2C_ENUMERATED_HUGE;
    extern A2C_CONST A2C_DESCRIPTOR A2C_ENUMERATED_HUGE_descriptor;

    typedef A2C_INTEGER_NATIVE A2C_ENUMERATED_NATIVE;
    extern A2C_CONST A2C_DESCRIPTOR A2C_ENUMERATED_NATIVE_descriptor;

    A2C_ERROR A2C_ENUMERATED_HUGE_decode_der(A2C_ENUMERATED_HUGE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_ENUMERATED_HUGE_encode_der(A2C_ENUMERATED_HUGE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);
    A2C_ERROR A2C_ENUMERATED_HUGE_decode_ber(A2C_ENUMERATED_HUGE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_ENUMERATED_HUGE_encode_ber(A2C_ENUMERATED_HUGE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);
    A2C_ERROR A2C_ENUMERATED_HUGE_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

    int A2C_ENUMERATED_HUGE_compare(A2C_ENUMERATED_HUGE const * pfLeft, A2C_ENUMERATED_HUGE const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_ENUMERATED_HUGE_copy(A2C_ENUMERATED_HUGE * piLeft, A2C_ENUMERATED_HUGE const * piRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_ENUMERATED_HUGE_print(A2C_ENUMERATED_HUGE const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);



A2C_INLINE A2C_ERROR A2C_Alloc_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE ** p) { return A2C_Alloc((PVOID *) p, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p) { return A2C_Free((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p) { return A2C_Init((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p) { return A2C_Release((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_ENUMERATED_HUGE_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_ENUMERATED_HUGE_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_ENUMERATED_HUGE_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * pDst, A2C_ENUMERATED_HUGE * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE int A2C_Compare_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE const * pLeft, A2C_ENUMERATED_HUGE const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_ENUMERATED_HUGE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_ENUMERATED_HUGE(A2C_ENUMERATED_HUGE * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_ENUMERATED_HUGE_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_ENUMERATED_NATIVE_decode_der(A2C_ENUMERATED_NATIVE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_ENUMERATED_NATIVE_encode_der(A2C_ENUMERATED_NATIVE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);
    A2C_ERROR A2C_ENUMERATED_NATIVE_decode_ber(A2C_ENUMERATED_NATIVE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_ENUMERATED_NATIVE_encode_ber(A2C_ENUMERATED_NATIVE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);
    A2C_ERROR A2C_ENUMERATED_NATIVE_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

    int A2C_ENUMERATED_NATIVE_compare(A2C_ENUMERATED_NATIVE const * pfLeft, A2C_ENUMERATED_NATIVE const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_ENUMERATED_NATIVE_copy(A2C_ENUMERATED_NATIVE * piLeft, A2C_ENUMERATED_NATIVE const * piRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_ENUMERATED_NATIVE_print(A2C_ENUMERATED_NATIVE const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);


A2C_INLINE A2C_ERROR A2C_Alloc_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE ** p) { return A2C_Alloc((PVOID *) p, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p) { return A2C_Free((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p) { return A2C_Init((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p) { return A2C_Release((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_ENUMERATED_NATIVE_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_ENUMERATED_NATIVE_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_ENUMERATED_NATIVE_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * pDst, A2C_ENUMERATED_NATIVE * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE int A2C_Compare_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE const * pLeft, A2C_ENUMERATED_NATIVE const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_ENUMERATED_NATIVE_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_ENUMERATED_NATIVE(A2C_ENUMERATED_NATIVE * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_ENUMERATED_NATIVE_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_ENUMERATED_List(PC_A2C_DESCRIPTOR, A2C_NAMES const * *);
    A2C_ERROR A2C_ENUMERATED_NameToInt(PC_A2C_DESCRIPTOR, char const *, int *);
    A2C_ERROR A2C_ENUMERATED_IntToName(PC_A2C_DESCRIPTOR, int, char const **);

#ifdef __cplusplus
}
#endif
#endif /* __A2C_ENUMERATED_H__ */
