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

#ifndef __A2C_STRING_H__
#define __A2C_STRING_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef unsigned short uint16;
    typedef unsigned int uint32;

    
    typedef  struct {
        uint16 * string;
    } A2C_BMP_STRING;

    typedef struct {
        char * string;
    } A2C_GENERAL_STRING;

    typedef struct {
        char * string;
    } A2C_GRAPHIC_STRING;

    typedef struct {
        char * string;
    } A2C_IA5_STRING;

    typedef struct {
        char * string;
    } A2C_NUMERIC_STRING;

    typedef struct {
        char * string;
    } A2C_PRINTABLE_STRING;

    typedef struct {
        char * string;
    } A2C_TELETEX_STRING;

    typedef struct {
        uint32 * string;
    } A2C_UNIVERSAL_STRING;

    typedef struct {
        char * string;
    } A2C_UTF8_STRING;

    typedef struct {
        char * string;
    } A2C_VIDEOTEX_STRING;

    typedef struct {
        char * string;
    } A2C_VISIBLE_STRING;

    extern A2C_CONST A2C_DESCRIPTOR A2C_BMP_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_BMP_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_GENERAL_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_GENERAL_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_GRAPHIC_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_GRAPHIC_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_IA5_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_IA5_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_NUMERIC_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_NUMERIC_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_PRINTABLE_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_PRINTABLE_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_TELETEX_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_TELETEX_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_UNIVERSAL_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_UNIVERSAL_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_UTF8_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_UTF8_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_VIDEOTEX_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_VIDEOTEX_STRING_description;
    extern A2C_CONST A2C_DESCRIPTOR A2C_VISIBLE_STRING_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_VISIBLE_STRING_description;

    A2C_ERROR A2C_BMP_STRING_decode_der(A2C_BMP_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_BMP_STRING_encode_der(A2C_BMP_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_BMP_STRING_decode_ber(A2C_BMP_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_BMP_STRING_encode_ber(A2C_BMP_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    int A2C_BMP_STRING_compare(A2C_BMP_STRING const * pLeft, A2C_BMP_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_BMP_STRING_copy(A2C_BMP_STRING * pLeft, A2C_BMP_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_BMP_STRING_print(A2C_BMP_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    A2C_ERROR A2C_BMP_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_BMP_STRING(A2C_BMP_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_BMP_STRING(A2C_BMP_STRING * p) { return A2C_Free((PVOID) p, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_BMP_STRING(A2C_BMP_STRING * p) { return A2C_Init((PVOID) p, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_BMP_STRING(A2C_BMP_STRING * p) { return A2C_Release((PVOID) p, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_BMP_STRING(A2C_BMP_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_BMP_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_BMP_STRING(A2C_BMP_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_BMP_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_BMP_STRING(A2C_BMP_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_BMP_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_BMP_STRING(A2C_BMP_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_BMP_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_BMP_STRING(A2C_BMP_STRING * pDst, A2C_BMP_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_BMP_STRING(A2C_BMP_STRING const * pLeft, A2C_BMP_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_BMP_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_BMP_STRING(A2C_BMP_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_BMP_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_BMP_STRING(A2C_BMP_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_BMP_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_BMP_STRING(A2C_BMP_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_BMP_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_GENERAL_STRING_decode_der(A2C_GENERAL_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_GENERAL_STRING_encode_der(A2C_GENERAL_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_GENERAL_STRING_decode_ber(A2C_GENERAL_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_GENERAL_STRING_encode_ber(A2C_GENERAL_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    int A2C_GENERAL_STRING_compare(A2C_GENERAL_STRING const * pLeft, A2C_GENERAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_GENERAL_STRING_copy(A2C_GENERAL_STRING * pLeft, A2C_GENERAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_GENERAL_STRING_print(A2C_GENERAL_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    A2C_ERROR A2C_GENERAL_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_GENERAL_STRING(A2C_GENERAL_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_GENERAL_STRING(A2C_GENERAL_STRING * p) { return A2C_Free((PVOID) p, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_GENERAL_STRING(A2C_GENERAL_STRING * p) { return A2C_Init((PVOID) p, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_GENERAL_STRING(A2C_GENERAL_STRING * p) { return A2C_Release((PVOID) p, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_GENERAL_STRING(A2C_GENERAL_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_GENERAL_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_GENERAL_STRING(A2C_GENERAL_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_GENERAL_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_GENERAL_STRING(A2C_GENERAL_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_GENERAL_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_GENERAL_STRING(A2C_GENERAL_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_GENERAL_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_GENERAL_STRING(A2C_GENERAL_STRING * pDst, A2C_GENERAL_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_GENERAL_STRING(A2C_GENERAL_STRING const * pLeft, A2C_GENERAL_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_GENERAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_GENERAL_STRING(A2C_GENERAL_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_GENERAL_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_GENERAL_STRING(A2C_GENERAL_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_GENERAL_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_GENERAL_STRING(A2C_GENERAL_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_GENERAL_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_GRAPHIC_STRING_decode_der(A2C_GRAPHIC_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_GRAPHIC_STRING_encode_der(A2C_GRAPHIC_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_GRAPHIC_STRING_decode_ber(A2C_GRAPHIC_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_GRAPHIC_STRING_encode_ber(A2C_GRAPHIC_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    int A2C_GRAPHIC_STRING_compare(A2C_GRAPHIC_STRING const * pLeft, A2C_GRAPHIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_GRAPHIC_STRING_copy(A2C_GRAPHIC_STRING * pLeft, A2C_GRAPHIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_GRAPHIC_STRING_print(A2C_GRAPHIC_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    A2C_ERROR A2C_GRAPHIC_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_GRAPHIC_STRING(A2C_GRAPHIC_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p) { return A2C_Free((PVOID) p, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p) { return A2C_Init((PVOID) p, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p) { return A2C_Release((PVOID) p, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_GRAPHIC_STRING(A2C_GRAPHIC_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_GRAPHIC_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_GRAPHIC_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_GRAPHIC_STRING(A2C_GRAPHIC_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_GRAPHIC_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_GRAPHIC_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_GRAPHIC_STRING(A2C_GRAPHIC_STRING * pDst, A2C_GRAPHIC_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_GRAPHIC_STRING(A2C_GRAPHIC_STRING const * pLeft, A2C_GRAPHIC_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_GRAPHIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_A2C_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_GRAPHIC_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_GRAPHIC_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_GRAPHIC_STRING(A2C_GRAPHIC_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_GRAPHIC_STRING_descriptor, ppcxt, pf, pstm); } 

    A2C_ERROR A2C_IA5_STRING_decode_der(A2C_IA5_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_IA5_STRING_encode_der(A2C_IA5_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_IA5_STRING_decode_ber(A2C_IA5_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_IA5_STRING_encode_ber(A2C_IA5_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_IA5_STRING_print(A2C_IA5_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_IA5_STRING_compare(A2C_IA5_STRING const * pLeft, A2C_IA5_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_IA5_STRING_copy(A2C_IA5_STRING * pLeft, A2C_IA5_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_IA5_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_IA5_STRING(A2C_IA5_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_IA5_STRING(A2C_IA5_STRING * p) { return A2C_Free((PVOID) p, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_IA5_STRING(A2C_IA5_STRING * p) { return A2C_Init((PVOID) p, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_IA5_STRING(A2C_IA5_STRING * p) { return A2C_Release((PVOID) p, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_IA5_STRING(A2C_IA5_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_IA5_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_IA5_STRING(A2C_IA5_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_IA5_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_IA5_STRING(A2C_IA5_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_IA5_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_IA5_STRING(A2C_IA5_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_IA5_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_IA5_STRING(A2C_IA5_STRING * pDst, A2C_IA5_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_IA5_STRING(A2C_IA5_STRING const * pLeft, A2C_IA5_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_IA5_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_IA5_STRING(A2C_IA5_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_IA5_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_IA5_STRING(A2C_IA5_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_IA5_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_IA5_STRING(A2C_IA5_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_IA5_STRING_descriptor, ppcxt, pf, pstm); } 



    A2C_ERROR A2C_NUMERIC_STRING_decode_der(A2C_NUMERIC_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_NUMERIC_STRING_encode_der(A2C_NUMERIC_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_NUMERIC_STRING_decode_ber(A2C_NUMERIC_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_NUMERIC_STRING_encode_ber(A2C_NUMERIC_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_NUMERIC_STRING_print(A2C_NUMERIC_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_NUMERIC_STRING_compare(A2C_NUMERIC_STRING const * pLeft, A2C_NUMERIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_NUMERIC_STRING_copy(A2C_NUMERIC_STRING * pLeft, A2C_NUMERIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_NUMERIC_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_NUMERIC_STRING(A2C_NUMERIC_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_NUMERIC_STRING(A2C_NUMERIC_STRING * p) { return A2C_Free((PVOID) p, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_NUMERIC_STRING(A2C_NUMERIC_STRING * p) { return A2C_Init((PVOID) p, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_NUMERIC_STRING(A2C_NUMERIC_STRING * p) { return A2C_Release((PVOID) p, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_NUMERIC_STRING(A2C_NUMERIC_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_NUMERIC_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_NUMERIC_STRING(A2C_NUMERIC_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_NUMERIC_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_NUMERIC_STRING(A2C_NUMERIC_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_NUMERIC_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_NUMERIC_STRING(A2C_NUMERIC_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_NUMERIC_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_NUMERIC_STRING(A2C_NUMERIC_STRING * pDst, A2C_NUMERIC_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_NUMERIC_STRING(A2C_NUMERIC_STRING const * pLeft, A2C_NUMERIC_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_NUMERIC_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_NUMERIC_STRING(A2C_NUMERIC_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_NUMERIC_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_NUMERIC_STRING(A2C_NUMERIC_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_NUMERIC_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_NUMERIC_STRING(A2C_NUMERIC_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_NUMERIC_STRING_descriptor, ppcxt, pf, pstm); } 



    A2C_ERROR A2C_PRINTABLE_STRING_decode_der(A2C_PRINTABLE_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_PRINTABLE_STRING_encode_der(A2C_PRINTABLE_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_PRINTABLE_STRING_decode_ber(A2C_PRINTABLE_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_PRINTABLE_STRING_encode_ber(A2C_PRINTABLE_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_PRINTABLE_STRING_print(A2C_PRINTABLE_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_PRINTABLE_STRING_compare(A2C_PRINTABLE_STRING const * pLeft, A2C_PRINTABLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_PRINTABLE_STRING_copy(A2C_PRINTABLE_STRING * pLeft, A2C_PRINTABLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_PRINTABLE_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_PRINTABLE_STRING(A2C_PRINTABLE_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p) { return A2C_Free((PVOID) p, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p) { return A2C_Init((PVOID) p, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p) { return A2C_Release((PVOID) p, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_PRINTABLE_STRING(A2C_PRINTABLE_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_PRINTABLE_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_PRINTABLE_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_PRINTABLE_STRING(A2C_PRINTABLE_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_PRINTABLE_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_PRINTABLE_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_PRINTABLE_STRING(A2C_PRINTABLE_STRING * pDst, A2C_PRINTABLE_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_PRINTABLE_STRING(A2C_PRINTABLE_STRING const * pLeft, A2C_PRINTABLE_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_PRINTABLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_PRINTABLE_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_PRINTABLE_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_PRINTABLE_STRING(A2C_PRINTABLE_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_PRINTABLE_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_TELETEX_STRING_decode_der(A2C_TELETEX_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_TELETEX_STRING_encode_der(A2C_TELETEX_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_TELETEX_STRING_decode_ber(A2C_TELETEX_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_TELETEX_STRING_encode_ber(A2C_TELETEX_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_TELETEX_STRING_print(A2C_TELETEX_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_TELETEX_STRING_compare(A2C_TELETEX_STRING const * pLeft, A2C_TELETEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_TELETEX_STRING_copy(A2C_TELETEX_STRING * pLeft, A2C_TELETEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_TELETEX_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_A2C_TELETEX_STRING(A2C_TELETEX_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p) { return A2C_Free((PVOID) p, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p) { return A2C_Init((PVOID) p, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p) { return A2C_Release((PVOID) p, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_A2C_TELETEX_STRING(A2C_TELETEX_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_TELETEX_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_TELETEX_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_A2C_TELETEX_STRING(A2C_TELETEX_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_TELETEX_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_TELETEX_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_A2C_TELETEX_STRING(A2C_TELETEX_STRING * pDst, A2C_TELETEX_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_A2C_TELETEX_STRING(A2C_TELETEX_STRING const * pLeft, A2C_TELETEX_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_TELETEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_TELETEX_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_TELETEX_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_A2C_TELETEX_STRING(A2C_TELETEX_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_TELETEX_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_UNIVERSAL_STRING_decode_der(A2C_UNIVERSAL_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_UNIVERSAL_STRING_encode_der(A2C_UNIVERSAL_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_UNIVERSAL_STRING_decode_ber(A2C_UNIVERSAL_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_UNIVERSAL_STRING_encode_ber(A2C_UNIVERSAL_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_UNIVERSAL_STRING_print(A2C_UNIVERSAL_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_UNIVERSAL_STRING_compare(A2C_UNIVERSAL_STRING const * pLeft, A2C_UNIVERSAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_UNIVERSAL_STRING_copy(A2C_UNIVERSAL_STRING * pLeft, A2C_UNIVERSAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_UNIVERSAL_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p) { return A2C_Free((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p) { return A2C_Init((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p) { return A2C_Release((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_UNIVERSAL_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_UNIVERSAL_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_UNIVERSAL_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * pDst, A2C_UNIVERSAL_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING const * pLeft, A2C_UNIVERSAL_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_UNIVERSAL_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_A2C_UNIVERSAL_STRING(A2C_UNIVERSAL_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_UNIVERSAL_STRING_descriptor, ppcxt, pf, pstm); } 



    A2C_ERROR A2C_UTF8_STRING_decode_der(A2C_UTF8_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_UTF8_STRING_encode_der(A2C_UTF8_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_UTF8_STRING_decode_ber(A2C_UTF8_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_UTF8_STRING_encode_ber(A2C_UTF8_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_UTF8_STRING_print(A2C_UTF8_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_UTF8_STRING_compare(A2C_UTF8_STRING const * pLeft, A2C_UTF8_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_UTF8_STRING_copy(A2C_UTF8_STRING * pLeft, A2C_UTF8_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_UTF8_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_A2C_UTF8_STRING(A2C_UTF8_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_A2C_UTF8_STRING(A2C_UTF8_STRING * p) { return A2C_Free((PVOID) p, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_A2C_UTF8_STRING(A2C_UTF8_STRING * p) { return A2C_Init((PVOID) p, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_A2C_UTF8_STRING(A2C_UTF8_STRING * p) { return A2C_Release((PVOID) p, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_A2C_UTF8_STRING(A2C_UTF8_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_UTF8_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_A2C_UTF8_STRING(A2C_UTF8_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_UTF8_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_A2C_UTF8_STRING(A2C_UTF8_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_UTF8_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_A2C_UTF8_STRING(A2C_UTF8_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_UTF8_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_A2C_UTF8_STRING(A2C_UTF8_STRING * pDst, A2C_UTF8_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_A2C_UTF8_STRING(A2C_UTF8_STRING const * pLeft, A2C_UTF8_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_UTF8_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_A2C_UTF8_STRING(A2C_UTF8_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_UTF8_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_A2C_UTF8_STRING(A2C_UTF8_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_UTF8_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_A2C_UTF8_STRING(A2C_UTF8_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_UTF8_STRING_descriptor, ppcxt, pf, pstm); } 



    A2C_ERROR A2C_VIDEOTEX_STRING_decode_der(A2C_VIDEOTEX_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_VIDEOTEX_STRING_encode_der(A2C_VIDEOTEX_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_VIDEOTEX_STRING_decode_ber(A2C_VIDEOTEX_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_VIDEOTEX_STRING_encode_ber(A2C_VIDEOTEX_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_VIDEOTEX_STRING_print(A2C_VIDEOTEX_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_VIDEOTEX_STRING_compare(A2C_VIDEOTEX_STRING const * pLeft, A2C_VIDEOTEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_VIDEOTEX_STRING_copy(A2C_VIDEOTEX_STRING * pLeft, A2C_VIDEOTEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_VIDEOTEX_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p) { return A2C_Free((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p) { return A2C_Init((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p) { return A2C_Release((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_VIDEOTEX_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_VIDEOTEX_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_VIDEOTEX_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * pDst, A2C_VIDEOTEX_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING const * pLeft, A2C_VIDEOTEX_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_VIDEOTEX_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_A2C_VIDEOTEX_STRING(A2C_VIDEOTEX_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_VIDEOTEX_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_VISIBLE_STRING_decode_der(A2C_VISIBLE_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_VISIBLE_STRING_encode_der(A2C_VISIBLE_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_VISIBLE_STRING_decode_ber(A2C_VISIBLE_STRING *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_VISIBLE_STRING_encode_ber(A2C_VISIBLE_STRING const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_VISIBLE_STRING_print(A2C_VISIBLE_STRING const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);
    int A2C_VISIBLE_STRING_compare(A2C_VISIBLE_STRING const * pLeft, A2C_VISIBLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_VISIBLE_STRING_copy(A2C_VISIBLE_STRING * pLeft, A2C_VISIBLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_VISIBLE_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_INLINE A2C_ERROR A2C_Alloc_VISIBLE_STRING(A2C_VISIBLE_STRING ** p) { return A2C_Alloc((PVOID *) p, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Free_VISIBLE_STRING(A2C_VISIBLE_STRING * p) { return A2C_Free((PVOID) p, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Init_VISIBLE_STRING(A2C_VISIBLE_STRING * p) { return A2C_Init((PVOID) p, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Release_VISIBLE_STRING(A2C_VISIBLE_STRING * p) { return A2C_Release((PVOID) p, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_EncodeBer_VISIBLE_STRING(A2C_VISIBLE_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeBer((PCVOID) p, &A2C_VISIBLE_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeBer_VISIBLE_STRING(A2C_VISIBLE_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeBer((PVOID *) p, &A2C_VISIBLE_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_EncodeDer_VISIBLE_STRING(A2C_VISIBLE_STRING const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) { return A2C_EncodeDer((PVOID) p, &A2C_VISIBLE_STRING_descriptor, flags, ppcxt, pstm); } 
A2C_INLINE A2C_ERROR A2C_DecodeDer_VISIBLE_STRING(A2C_VISIBLE_STRING * p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) { return A2C_DecodeDer((PVOID *) p, &A2C_VISIBLE_STRING_descriptor, flags, ppcxt, pb, cb); } 
A2C_INLINE A2C_ERROR A2C_Copy_VISIBLE_STRING(A2C_VISIBLE_STRING * pDst, A2C_VISIBLE_STRING * pSrc) { return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE int A2C_Compare_VISIBLE_STRING(A2C_VISIBLE_STRING const * pLeft, A2C_VISIBLE_STRING const * pRight) { return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &A2C_VISIBLE_STRING_descriptor); } 
A2C_INLINE A2C_ERROR A2C_Print_VISIBLE_STRING(A2C_VISIBLE_STRING * p, PA2C_STREAM pstm) { return A2C_Print((PVOID) p, &A2C_VISIBLE_STRING_descriptor, pstm); } 
A2C_INLINE A2C_ERROR A2C_Validate_VISIBLE_STRING(A2C_VISIBLE_STRING * p, int * pf) { return A2C_Validate((PVOID) p, &A2C_VISIBLE_STRING_descriptor, pf); } 
A2C_INLINE A2C_ERROR A2C_Validate2_VISIBLE_STRING(A2C_VISIBLE_STRING * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) { return A2C_Validate2((PVOID) p, &A2C_VISIBLE_STRING_descriptor, ppcxt, pf, pstm); } 


    A2C_ERROR A2C_string_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_string_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __A2C_STRING_H__ */
