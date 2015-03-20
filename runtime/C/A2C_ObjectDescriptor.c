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

#include "A2C.h"

#include <stdlib.h>
#include <memory.h>

/*
 *
 */

A2C_CONST A2C_ITEM_HEADER A2C_OBJECT_DESCRIPTOR_description = {
    sizeof(A2C_OBJECT_DESCRIPTOR), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_OBJECT_DESCRIPTOR_descriptor = {
    A2C_string_init,
    A2C_string_release,
    (A2C_decoder_f) A2C_OBJECT_DESCRIPTOR_decode_der,
    (A2C_encoder_f) A2C_OBJECT_DESCRIPTOR_encode_der,
    (A2C_decoder_f) A2C_OBJECT_DESCRIPTOR_decode_ber,
    (A2C_encoder_f) A2C_OBJECT_DESCRIPTOR_encode_ber,
    (A2C_compare_f) A2C_OBJECT_DESCRIPTOR_compare,
    (A2C_copy_f) A2C_OBJECT_DESCRIPTOR_copy,
    (A2C_print_f) A2C_OBJECT_DESCRIPTOR_print,
    A2C_OBJECT_DESCRIPTOR_validate,
    &A2C_OBJECT_DESCRIPTOR_description
};

const A2C_TAG ObjDescriptorTag = {0, 7};

/*
 *
 */

A2C_ERROR A2C_OBJECT_DESCRIPTOR_decode_der(A2C_OBJECT_DESCRIPTOR * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                 A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &ObjDescriptorTag;
    }
    
    return A2C_GRAPHIC_STRING_decode_der(pi, pdesc, flags, pcxt, ptag, pstm);
}


A2C_ERROR A2C_OBJECT_DESCRIPTOR_encode_der(A2C_OBJECT_DESCRIPTOR const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    if (ptag == NULL) {
        ptag = &ObjDescriptorTag;
    }

    return A2C_GRAPHIC_STRING_encode_der(pbs, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_OBJECT_DESCRIPTOR_decode_ber(A2C_OBJECT_DESCRIPTOR * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                 A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &ObjDescriptorTag;
    }
    
    return A2C_GRAPHIC_STRING_decode_ber(pi, pdesc, flags, pcxt, ptag, pstm);
}


A2C_ERROR A2C_OBJECT_DESCRIPTOR_encode_ber(A2C_OBJECT_DESCRIPTOR const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    if (ptag == NULL) {
        ptag = &ObjDescriptorTag;
    }

    return A2C_GRAPHIC_STRING_encode_ber(pbs, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_OBJECT_DESCRIPTOR_print(A2C_OBJECT_DESCRIPTOR const * pbs, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    unreferenced(iLevel);
    unreferenced(pdesc);
    
    return pstm->pfnWrite(pstm, (PBYTE) pbs->string, (A2C_LENGTH) strlen(pbs->string));
}

int A2C_OBJECT_DESCRIPTOR_compare(A2C_OBJECT_DESCRIPTOR const * pfLeft, A2C_OBJECT_DESCRIPTOR const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_GRAPHIC_STRING_compare(pfLeft, pfRight, pdesc);
}

A2C_ERROR A2C_OBJECT_DESCRIPTOR_copy(A2C_OBJECT_DESCRIPTOR * piLeft, A2C_OBJECT_DESCRIPTOR const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_GRAPHIC_STRING_copy(piLeft, piRight, pdesc);
}

A2C_ERROR A2C_OBJECT_DESCRIPTOR_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    return A2C_GRAPHIC_STRING_validate(pv, pdesc, pcxt, pf, pstm);
}
