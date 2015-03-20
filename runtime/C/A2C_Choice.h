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

#ifndef __A2C_CHOICE_H__
#define __A2C_CHOICE_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct {
        int                     index;
    } A2C_CHOICE;

    extern A2C_DESCRIPTOR A2C_CHOICE_descriptor;

    A2C_ERROR A2C_CHOICE_init(PVOID, PC_A2C_DESCRIPTOR);
    A2C_ERROR A2C_CHOICE_release(PVOID, PC_A2C_DESCRIPTOR);

    A2C_ERROR A2C_CHOICE_decode_der(A2C_CHOICE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_CHOICE_encode_der(A2C_CHOICE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);
    A2C_ERROR A2C_CHOICE_decode_ber(A2C_CHOICE *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
    A2C_ERROR A2C_CHOICE_encode_ber(A2C_CHOICE const *, PC_A2C_DESCRIPTOR, int flags, A2C_CONTEXT *, A2C_TAG const * ptag, PA2C_STREAM pstm);

    int A2C_CHOICE_compare(A2C_CHOICE const * pfLeft, A2C_CHOICE const * pfRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_CHOICE_copy(A2C_CHOICE * piLeft, A2C_CHOICE const * piRight, PC_A2C_DESCRIPTOR pdesc);
    A2C_ERROR A2C_CHOICE_print(A2C_CHOICE const *, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);

    A2C_ERROR A2C_CHOICE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm);
#ifdef __cplusplus
}
#endif
#endif /* __A2C_CHOICE_H__ */
