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

#ifndef __A2C_BIT_STRING_H__
#define __A2C_BIT_STRING_H__

#include "A2C_Base.h"
#include "A2C_OpenType.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct {
    A2C_LENGTH      bitLength;          /* Number of bits used in the buffer */
    A2C_LENGTH      bufferLength;       /* count of bytes allocated for buffer */
    BYTE *          data;               /* buffer */
} A2C_BIT_STRING;

typedef A2C_BIT_STRING A2C_BIT_STRING_NAMED;

extern A2C_CONST A2C_DESCRIPTOR A2C_BIT_STRING_descriptor;
extern A2C_CONST A2C_ITEM_HEADER A2C_BIT_STRING_description;

extern A2C_CONST A2C_DESCRIPTOR A2C_BIT_STRING_NAMED_descriptor;
extern A2C_CONST A2C_ITEM_HEADER A2C_BIT_STRING_NAMED_description;

A2C_ERROR A2C_BIT_STRING_encode_der(A2C_BIT_STRING const *, PC_A2C_DESCRIPTOR, int flags, 
                                    A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);
A2C_ERROR A2C_BIT_STRING_decode_der(A2C_BIT_STRING *, PC_A2C_DESCRIPTOR, int flags, 
                                    A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
A2C_ERROR A2C_BIT_STRING_encode_ber(A2C_BIT_STRING const *, PC_A2C_DESCRIPTOR, int flags,
                                    A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);
A2C_ERROR A2C_BIT_STRING_decode_ber(A2C_BIT_STRING *, PC_A2C_DESCRIPTOR, int flags,
                                    A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);

A2C_ERROR A2C_BIT_STRING_release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);
int A2C_BIT_STRING_compare(A2C_BIT_STRING const * pbsLeft, A2C_BIT_STRING const * pbsRight, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_BIT_STRING_copy(A2C_BIT_STRING * pbsLeft, A2C_BIT_STRING const * pbsRight, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_BIT_STRING_print(A2C_BIT_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
A2C_ERROR A2C_BIT_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);


A2C_INLINE A2C_ERROR A2C_Alloc_BIT_STRING(A2C_BIT_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_BIT_STRING(A2C_BIT_STRING * p) { return A2C_Free((PVOID) p, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_BIT_STRING(A2C_BIT_STRING * p) { return A2C_Init((PVOID) p, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_BIT_STRING(A2C_BIT_STRING * p) { return A2C_Release((PVOID) p, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_BIT_STRING(A2C_BIT_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_BIT_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_BIT_STRING(A2C_BIT_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_BIT_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_BIT_STRING(A2C_BIT_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_BIT_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_BIT_STRING(A2C_BIT_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_BIT_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_BIT_STRING(A2C_BIT_STRING * pDst, A2C_BIT_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_BIT_STRING(A2C_BIT_STRING const * pLeft, A2C_BIT_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_BIT_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_BIT_STRING(A2C_BIT_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_BIT_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_BIT_STRING(A2C_BIT_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_BIT_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_BIT_STRING(A2C_BIT_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_BIT_STRING_descriptor, ppcxt, pf, pstm); } 


A2C_ERROR A2C_BIT_STRING_NAMED_release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_BIT_STRING_NAMED_encode_der(A2C_BIT_STRING const *bs, PC_A2C_DESCRIPTOR,
                                          int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);
A2C_ERROR A2C_BIT_STRING_NAMED_decode_der(A2C_BIT_STRING *bs, PC_A2C_DESCRIPTOR, int flags,
                                          A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
A2C_ERROR A2C_BIT_STRING_NAMED_encode_ber(A2C_BIT_STRING const *bs, PC_A2C_DESCRIPTOR,
                                          int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);
A2C_ERROR A2C_BIT_STRING_NAMED_decode_ber(A2C_BIT_STRING *bs, PC_A2C_DESCRIPTOR, int flags,
                                          A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);

int A2C_BIT_STRING_NAMED_compare(A2C_BIT_STRING_NAMED const * pbsLeft, A2C_BIT_STRING_NAMED const * pbsRight, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_BIT_STRING_NAMED_copy(A2C_BIT_STRING_NAMED * pbsLeft, A2C_BIT_STRING_NAMED const * pbsRight, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_BIT_STRING_NAMED_print(A2C_BIT_STRING_NAMED const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
A2C_ERROR A2C_BIT_STRING_NAMED_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

/*
 *  Helper functions to manipulate bit strings
 */
    
int A2C_BIT_STRING_IsSet(A2C_BIT_STRING const * pbs, int iBit);
A2C_ERROR A2C_BIT_STRING_Set(A2C_BIT_STRING * pbs, int iBit);
A2C_ERROR A2C_BIT_STRING_Clear(A2C_BIT_STRING * pbs, int iBit);

A2C_ERROR A2C_BIT_STRING_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames);
A2C_ERROR A2C_BIT_STRING_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi);
A2C_ERROR A2C_BIT_STRING_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz);


    /*
     *  This is used for bit strings which use the CONTAINS constraint
     */

    typedef struct {
        A2C_LENGTH              bitLength;          /* Number of bits used in the buffer */
        A2C_LENGTH              bufferLength;       /* count of bytes allocated for buffer */
        BYTE *                  data;               /* buffer */
        PC_A2C_DESCRIPTOR	functionPtr;        /* type descriptor */
        PVOID                   objectPtr;          /* type data */
    } A2C_OPEN_BIT_STRING;

    extern A2C_CONST A2C_DESCRIPTOR A2C_OPEN_BIT_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_OPEN_BIT_STRING_description;

    A2C_ERROR A2C_OPEN_BIT_STRING_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc);

    A2C_ERROR A2C_OPEN_BIT_STRING_decode_der(A2C_OPEN_BIT_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OPEN_BIT_STRING_encode_der(A2C_OPEN_BIT_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);

    A2C_ERROR A2C_OPEN_BIT_STRING_decode_ber(A2C_OPEN_BIT_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OPEN_BIT_STRING_encode_ber(A2C_OPEN_BIT_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);

    A2C_ERROR A2C_OPEN_BIT_STRING_print(A2C_OPEN_BIT_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);
    int A2C_OPEN_BIT_STRING_compare(A2C_OPEN_BIT_STRING const * posLeft, A2C_OPEN_BIT_STRING const * posRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OPEN_BIT_STRING_copy(A2C_OPEN_BIT_STRING * posLeft, A2C_OPEN_BIT_STRING const * posRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OPEN_BIT_STRING_validate(A2C_OPEN_BIT_STRING const * pos, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);


    A2C_INLINE A2C_ERROR A2C_Alloc_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE A2C_ERROR A2C_Free_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p) { return A2C_Free((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE A2C_ERROR A2C_Init_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p) { return A2C_Init((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE A2C_ERROR A2C_Release_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p) { return A2C_Release((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE A2C_ERROR A2C_EncodeBer_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_OPEN_BIT_STRING_descriptor, flags, ppcxt, pstm); } 
    A2C_INLINE A2C_ERROR A2C_DecodeBer_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_OPEN_BIT_STRING_descriptor, flags, ppcxt, pb, cb); } 
    A2C_INLINE A2C_ERROR A2C_EncodeDer_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor, flags, ppcxt, pstm); } 
    A2C_INLINE A2C_ERROR A2C_DecodeDer_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_OPEN_BIT_STRING_descriptor, flags, ppcxt, pb, cb); } 
    A2C_INLINE A2C_ERROR A2C_Copy_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * pDst, A2C_OPEN_BIT_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE int A2C_Compare_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING const * pLeft, A2C_OPEN_BIT_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_OPEN_BIT_STRING_descriptor); } 
    A2C_INLINE A2C_ERROR A2C_Print_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor, pstm); } 
    A2C_INLINE A2C_ERROR A2C_Validate_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_OPEN_BIT_STRING_descriptor, pf); } 
    A2C_INLINE A2C_ERROR A2C_Validate2_OPEN_BIT_STRING(A2C_OPEN_BIT_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_BIT_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_OPEN_BIT_STRING_RELATION_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_BIT_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                      A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OPEN_BIT_STRING_RELATION_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_BIT_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                      A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);


#ifdef __cplusplus
}
#endif
#endif /* __A2C_BIT_STRING_H__ */
