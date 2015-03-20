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

#ifndef __A2C_BINARY_H__
#define __A2C_BINARY_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*  There are two pairs of data in this structure <pb, cb> and <pfns, pv> only one pair should be set */
    
    typedef struct {
        A2C_LENGTH              length;                     /* length of data */
        PBYTE                   data;                       /* pointer to data */
    } A2C_BINARY;

    extern A2C_CONST A2C_DESCRIPTOR A2C_BINARY_descriptor;
    extern A2C_CONST A2C_ITEM_HEADER A2C_BINARY_description;

    A2C_ERROR A2C_BINARY_decode_der(A2C_BINARY *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_BINARY_encode_der(A2C_BINARY const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);
    A2C_ERROR A2C_BINARY_decode_ber(A2C_BINARY *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_BINARY_encode_ber(A2C_BINARY const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);

    int A2C_BINARY_compare(A2C_BINARY const * pfLeft, A2C_BINARY const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_BINARY_copy(A2C_BINARY * pfLeft, A2C_BINARY const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_BINARY_print(A2C_BINARY const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);
    A2C_ERROR A2C_BINARY_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_ERROR _A2C_Recurse_GetLength(A2C_STREAM_MEMORY * pstmIn, A2C_STREAM_MEMORY * pstmOut, int flags,
                                 A2C_CONTEXT * pcxt, A2C_TAG const *, int * piBits);

#ifdef __cplusplus
}
#endif
#endif /* __A2C_BINARY_H__ */
