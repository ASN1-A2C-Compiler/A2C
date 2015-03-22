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
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "A2C_Stream_Internal.h"

const A2C_ITEM_HEADER A2C_NULL_description = {
    sizeof(A2C_NULL), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_NULL_descriptor = {
    A2C_zero,
    A2C_nop,
    (A2C_decoder_f) A2C_NULL_decode_der,
    (A2C_encoder_f) A2C_NULL_encode_der,
    (A2C_decoder_f) A2C_NULL_decode_ber,
    (A2C_encoder_f) A2C_NULL_encode_ber,
    (A2C_compare_f) A2C_NULL_compare,
    (A2C_copy_f) A2C_NULL_copy,
    (A2C_print_f) A2C_NULL_print,
    A2C_NULL_validate,
    &A2C_NULL_description
};

/* ---
/// <summary>
/// </summary>
/// <param name="pf">pointer to the boolean value to encode</param>
/// <param name="pdesc">pointer to descriptor table for the type</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">source bytes for the decoder</param>
/// <returns>A2C_ERROR_Success on success, else error code</returns>
--- */

A2C_ERROR A2C_NULL_decode_der(A2C_NULL * pf, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                 A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    int         err;
    int		fConstructed;
    int         iClass;
    A2C_TAG_VALUE iValue;

    unreferenced(pf);
    unreferenced(flags);
    unreferenced(pdesc);
    unreferenced(pcxt);
    
    /*
     *  Grab the type and length of this item.  This function will without advancing
     *  the pointer if there are not sufficent bytes for the value in the buffer.
     */

    err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
    if (err < 0) {
        goto ErrorExit;
    }

    /*
     *  Check that the tag matches what we expect it to be.
     *
     *  It is either a passed in code set or it is a fixed value
     */

    if (ptag != NULL) {
        if ((ptag->iClass != iClass) || (ptag->iValue != iValue)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }
    else {
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 5)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }

    /*
     *  If this is a constructed value, then fail
     *  If length != 0, then it is not correctly encoded
     */
    
    if (fConstructed || (cbLength != 0)) {
        err = A2C_ERROR_malformedEncoding;
        goto ErrorExit;
    }

    /*
     *  Skip over the tag and length
     */

    _A2C_Memory_Skip(pstm, cbTL);

    /*
     *  No data bytes to read
     */

    err = A2C_ERROR_Success;

    /*
     *  Handle any clean up code and return the error code
     */
    
ErrorExit:
    return err;
}

/* ---
/// <sumary>
/// </summary>
/// <param name="pf">pointer to boolean value</param>
/// <param name="pdesc">pointer to descriptor for item</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">stream to write the encoded bytes on</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_NULL_encode_der(A2C_NULL const * pf, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                 A2C_CONTEXT * pcxt, A2C_TAG const *ptag, PA2C_STREAM pstm)
{
    int         cb;
    A2C_ERROR   err;

    /*
     *  We don't use the following parameters
     */

    unreferenced(pf);
    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    
    /*
     *  Write out the tag for the item.  It could be either the passed in value or the
     *  default item
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 5, pstm, &cb);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cb);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the length = 0 bytes
     */

    err = _A2C_write_length(0, pstm, &cb);

    /*
     *  Write out the data value.
     *
     *  Nothing
     */

ErrorExit:
    return err;
}

/* ---
/// <summary>
/// </summary>
/// <param name="pf">pointer to the boolean value to encode</param>
/// <param name="pdesc">pointer to descriptor table for the type</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">source bytes for the decoder</param>
/// <returns>A2C_ERROR_Success on success, else error code</returns>
--- */

A2C_ERROR A2C_NULL_decode_ber(A2C_NULL * pf, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                 A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*  There are no differences between the BER and DER decoders */

    return A2C_NULL_decode_der(pf, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <sumary>
/// </summary>
/// <param name="pf">pointer to boolean value</param>
/// <param name="pdesc">pointer to descriptor for item</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">stream to write the encoded bytes on</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_NULL_encode_ber(A2C_NULL const * pf, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                 A2C_CONTEXT * pcxt, A2C_TAG const *ptag, PA2C_STREAM pstm)
{
    /*  No known differences between BER and DER for our encoding routine */
    
    return A2C_NULL_encode_der(pf, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_NULL_print is the default boolean value print routine.
/// </summary>
/// <param name="pf">pointer to the boolean value</param>
/// <param name="pdesc">boolean descriptor - ignored</param>
/// <param name="iLevel">indentation level - ignored</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

int A2C_NULL_print(A2C_NULL const * pf, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    unreferenced(pf);
    unreferenced(pdesc);
    unreferenced(iLevel);

    /*
     *  Dump the value onto the stream
     */

    return pstm->pfnWrite(pstm, (PBYTE) "NULL", 4);
}

/* ---
/// <summary>
/// </summary>
/// <param name="pfDest">pointer to destination value</param>
/// <param name="pfSrc">pointer to source value</param>
/// <param name="pdesc">pointer to data descriptor information</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_NULL_copy(A2C_NULL * pfDest, A2C_NULL const * pfSrc, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */
    
    unreferenced(pdesc);

    /*
     *  Copy from source to destination
     */

    *pfDest = *pfSrc;

    /*
     *  Clean up and return error code
     */

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// </summary>
/// <param name="pfLeft">pointer to value #1</param>
/// <param name="pfRight">pointer to value #2</param>
/// <param name="pdesc">pointer to data descriptor information</param>
/// <returns>Return 0 if they are the same, Non-zero if they are different.</returns>
--- */

int A2C_NULL_compare(A2C_NULL const * pfLeft, A2C_NULL const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use these parameters
     */

    unreferenced(pfLeft);
    unreferenced(pfRight);
    unreferenced(pdesc);

    /*
     *  NULL always matches NULL
     */

    return 0;
}

/*
/// <summary>
/// NULL validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
*/

A2C_ERROR A2C_NULL_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    unreferenced(pv);
    unreferenced(pdesc);
    unreferenced(pcxt);
    unreferenced(pstm);

    /*
     *  Always succeed - no known errors here
     */

    *pf = TRUE;
    return A2C_ERROR_Success;
}

