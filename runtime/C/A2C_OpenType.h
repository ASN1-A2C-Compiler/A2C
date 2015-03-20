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

#ifndef __A2C_OPEN_TYPE_H__
#define __A2C_OPEN_TYPE_H__

#include "A2C_Base.h"
#include "A2C_Object_Set.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*  There are two pairs of data in this structure <pb, cb> and <pfns, pv> only one pair should be set */

    /*  Make sure that this structur starts th same as A2C_OCTET_STRING */
    
    typedef struct {
        A2C_LENGTH              length;                 /* length of data */
        PBYTE                   data;                   /* pointer to data */
        PC_A2C_DESCRIPTOR	functionPtr;            /* type descriptor */
        PVOID                   objectPtr;              /* type data */
    } A2C_OPEN_TYPE;


    extern A2C_DESCRIPTOR A2C_OPEN_TYPE_descriptor;
    extern A2C_ITEM_HEADER A2C_OPEN_TYPE_description;

    A2C_ERROR A2C_OPEN_TYPE_release(PVOID, PC_A2C_DESCRIPTOR);

    A2C_ERROR A2C_OPEN_TYPE_decode_der(PVOID pv, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OPEN_TYPE_encode_der(PCVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);

    A2C_ERROR A2C_OPEN_TYPE_decode_ber(PVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OPEN_TYPE_encode_ber(PCVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);


    int A2C_OPEN_TYPE_compare(PCVOID pfLeft, PCVOID pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OPEN_TYPE_copy(PVOID pfLeft, PCVOID pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OPEN_TYPE_print(PCVOID, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);
A2C_ERROR A2C_OPEN_TYPE_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

    /*
     *
     */

    typedef A2C_OPEN_TYPE A2C_OCTET_OPEN_TYPE;

    extern A2C_DESCRIPTOR A2C_OCTET_OPEN_TYPE_descriptor;
    extern A2C_ITEM_HEADER A2C_OCTET_OPEN_TYPE_description;

    A2C_ERROR A2C_OCTET_OPEN_TYPE_release(PVOID, PC_A2C_DESCRIPTOR);

    A2C_ERROR A2C_OCTET_OPEN_TYPE_decode_der(PVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OCTET_OPEN_TYPE_encode_der(PCVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);

    A2C_ERROR A2C_OCTET_OPEN_TYPE_decode_ber(PVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_OCTET_OPEN_TYPE_encode_ber(PCVOID, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const *, PA2C_STREAM pstm);


    int A2C_OCTET_OPEN_TYPE_compare(PCVOID pfLeft, PCVOID pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OCTET_OPEN_TYPE_copy(PVOID pfLeft, PCVOID pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_OCTET_OPEN_TYPE_print(PCVOID, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);
A2C_ERROR A2C_OCTET_OPEN_TYPE_validate(PCVOID pv, PC_A2C_DESCRIPTOR, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);

A2C_ERROR A2C_RELATION_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OCTET_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                  A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
A2C_ERROR A2C_RELATION_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OCTET_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                  A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);

    const A2C_DESCRIPTOR * A2C_LookupType(const A2C_COMPONENT_RELATIONS * pcr, const A2C_CONTEXT * pcxt);

#ifdef __cplusplus
}
#endif
#endif /* __A2C_OPEN_TYPE_H__ */
