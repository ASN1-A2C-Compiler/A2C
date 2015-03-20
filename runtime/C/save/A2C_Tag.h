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

#ifndef __A2C_TAG_H__
#define __A2C_TAG_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
        int		        cb;
        BYTE *		        pb;
    } A2C_TAG_DATA;

    typedef int A2C_TAG_NATIVE;

    extern A2C_CONST A2C_DESCRIPTOR A2C_TAG_descriptor;
    extern A2C_CONST A2C_DESCRIPTOR A2C_TAG_NATIVE_descriptor;

    A2C_ERROR A2C_TAG_init(PVOID, PC_A2C_DESCRIPTOR);
    A2C_ERROR A2C_TAG_release(PVOID, PC_A2C_DESCRIPTOR);

    A2C_ERROR A2C_TAG_decode_der(A2C_TAG_DATA *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_TAG_encode_der(A2C_TAG_DATA const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_TAG_decode_ber(A2C_TAG_DATA *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);
    A2C_ERROR A2C_TAG_encode_ber(A2C_TAG_DATA const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);

    int A2C_TAG_compare(A2C_TAG_DATA const * pfLeft, A2C_TAG_DATA const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_TAG_copy(A2C_TAG_DATA * pseqLeft, A2C_TAG_DATA const * pseqRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_TAG_print(A2C_TAG_DATA const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM);

    A2C_ERROR A2C_TAG_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);




    A2C_ERROR A2C_TAG_EXPLICIT_Decode(int fDER, PBYTE, A2C_ITEM const *, int, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY *);

    A2C_ERROR A2C_TAG_Internal_EncodeDer(PCBYTE, A2C_ITEM const *, int, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);
    A2C_ERROR A2C_TAG_Internal_EncodeBer(PCBYTE, A2C_ITEM const *, int, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM);

#ifdef __cplusplus
}
#endif
#endif /* __A2C_INTEGER_H__ */
