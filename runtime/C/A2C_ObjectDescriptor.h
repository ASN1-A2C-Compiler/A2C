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

#ifndef __A2C_OBJECT_DESCRIPTOR_H__
#define __A2C_OBJECT_DESCRIPTOR_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "A2C_String.h"
    
    typedef A2C_GRAPHIC_STRING A2C_OBJECT_DESCRIPTOR;

    extern A2C_CONST A2C_DESCRIPTOR A2C_OBJECT_DESCRIPTOR_descriptor;

    A2C_ERROR A2C_OBJECT_DESCRIPTOR_decode_der(A2C_OBJECT_DESCRIPTOR *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_OBJECT_DESCRIPTOR_encode_der(A2C_OBJECT_DESCRIPTOR const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_OBJECT_DESCRIPTOR_decode_ber(A2C_OBJECT_DESCRIPTOR *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_OBJECT_DESCRIPTOR_encode_ber(A2C_OBJECT_DESCRIPTOR const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_OBJECT_DESCRIPTOR_print(A2C_OBJECT_DESCRIPTOR const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_OBJECT_DESCRIPTOR_compare(A2C_OBJECT_DESCRIPTOR const * pfLeft, A2C_OBJECT_DESCRIPTOR const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OBJECT_DESCRIPTOR_copy(A2C_OBJECT_DESCRIPTOR * piLeft, A2C_OBJECT_DESCRIPTOR const * piRight, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_OBJECT_DESCRIPTOR_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);


A2C_INLINE A2C_ERROR A2C_Alloc_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR ** p) { return A2C_Alloc((PVOID *) p, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p) { return A2C_Free((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p) { return A2C_Init((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p) { return A2C_Release((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_OBJECT_DESCRIPTOR_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_OBJECT_DESCRIPTOR_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * pDst, A2C_OBJECT_DESCRIPTOR * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE int A2C_Compare_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR const * pLeft, A2C_OBJECT_DESCRIPTOR const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_OBJECT_DESCRIPTOR_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_OBJECT_DESCRIPTOR(A2C_OBJECT_DESCRIPTOR * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_OBJECT_DESCRIPTOR_descriptor, ppcxt, pf, pstm); } 


#ifdef __cplusplus
}
#endif
#endif /* __A2C_OBJECT_DESCRIPTOR_H__ */
