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

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "A2C_Der.h"
#include "A2C_Tag.h"
#include "A2C_Stream_Internal.h"

#define FALSE 0
#define TRUE 1

/*
 *
 */

A2C_CONST A2C_ITEM_HEADER A2C_TAG_description = {
    sizeof(A2C_TAG), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_TAG_descriptor = {
    A2C_TAG_init,
    A2C_TAG_release,
    (A2C_decoder_f) A2C_TAG_decode_der,
    (A2C_encoder_f) A2C_TAG_encode_der,
    0,
    0,
    (A2C_compare_f) A2C_TAG_compare,
    (A2C_copy_f) A2C_TAG_copy,
    (A2C_print_f) A2C_TAG_print,
    A2C_TAG_validate,
    &A2C_TAG_description
};

A2C_ERROR A2C_TAG_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    return rgFields[0].pdu->pfnInit(pv, rgFields[0].pdu);
}


A2C_ERROR A2C_TAG_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    return rgFields[0].pdu->pfnRelease(pv, rgFields[0].pdu);
}

/* ---
/// <summary>
/// A2C_TAG_decode_der is the default der integer decoder routine.  This uses the
/// long form of the integer.
/// <para>This function does not allow restarting of scanning.</para>
/// </summary>
/// <param name="pi">pointer to integer data</param>
/// <param name="pdesc">descriptor for the data</param>
/// <param name="flags">control flags</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">override implicit tag</param>
/// <param name="pstm">source bytes to decode from</param>
--- */

A2C_ERROR A2C_TAG_decode_der(A2C_TAG_DATA * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                             A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR           err;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    if (rgFields[0].flags & A2C_FLAGS_TAG_EXPLICIT) {
        err = A2C_TAG_EXPLICIT_Decode(TRUE, (PBYTE) pi, &rgFields[0], flags, pcxt, ptag, pstm);
    }
    else {
        if (ptag == NULL) {
            ptag = &rgFields[0].tag;
        }
        
        err = rgFields[0].pdu->pfnDerDecoder(pi, rgFields[0].pdu, flags, pcxt, ptag, pstm);
    }

    return err;
}

/*
 *  Write out a DER encoded integer
 */

int A2C_TAG_encode_der(A2C_TAG_DATA const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    if (rgFields[0].flags & A2C_FLAGS_TAG_EXPLICIT) {
        err = A2C_TAG_Internal_EncodeDer((PCBYTE) pi, &rgFields[0], flags, pcxt, ptag, pstm);
    }
    else {
        if (ptag == NULL) {
            ptag = &rgFields[0].tag;
        }
        err = rgFields[0].pdu->pfnDerEncoder(pi, rgFields[0].pdu, flags, pcxt, ptag, pstm);
    }
    return err;
}

A2C_ERROR A2C_TAG_decode_ber(A2C_TAG_DATA * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                             A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR           err;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    if (rgFields[0].flags & A2C_FLAGS_TAG_EXPLICIT) {
        err = A2C_TAG_EXPLICIT_Decode(FALSE, (PBYTE) pi, &rgFields[0], flags, pcxt, ptag, pstm);
    }
    else {
        if (ptag == NULL) {
            ptag = &rgFields[0].tag;
        }
        
        err = rgFields[0].pdu->pfnBerDecoder(pi, rgFields[0].pdu, flags, pcxt, ptag, pstm);
    }

    return err;
}

int A2C_TAG_encode_ber(A2C_TAG_DATA const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    if (rgFields[0].flags & A2C_FLAGS_TAG_EXPLICIT) {
        err = A2C_TAG_Internal_EncodeBer((PCBYTE) pi, &rgFields[0], flags, pcxt, ptag, pstm);
    }
    else {
        if (ptag == NULL) {
            ptag = &rgFields[0].tag;
        }
        err = rgFields[0].pdu->pfnBerEncoder(pi, rgFields[0].pdu, flags, pcxt, ptag, pstm);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_TAG_print creates a UTF8 character string that describes the contents of the structure
/// </summary>
/// <param name="pv"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <param name="pstm"> stream object that recieves the description </param>
/// <returns> error code value </returns>
--- */

int A2C_TAG_print(A2C_TAG_DATA const * pi, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    return rgFields[0].pdu->pfnPrint(pi, rgFields[0].pdu, iLevel, pstm);
}

/* ---
/// <summary>
/// Pass it on to the child - no specific checks to tag
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_TAG_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;

    /*
     *  If not on the stack, put ourselfs there
     */

    *pf = FALSE;
    if (A2C_ContextTOS(pcxt)) {
        err = A2C_PushContext2(pcxt, &iFrame, NULL, 0);
        if (err != A2C_ERROR_Success) return err;
    }
    else {
        iFrame = A2C_ContextFrame(pcxt);
    }

    /*
     *  Are we checking the child?
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        /*
         *  Check the check for validity
         */

        err = pdesc->pItemDescription->rgChildren[0].pdu->pfnValidate(pv, 
                pdesc->pItemDescription->rgChildren[0].pdu, pcxt, pf, pstm);
        if ((err != A2C_ERROR_Success) && !pf) {
            return err;
        }
    }

    A2C_PopContext(pcxt, iFrame);

    return A2C_ERROR_Success;
}

int A2C_TAG_compare(A2C_TAG_DATA const * pfLeft, A2C_TAG_DATA const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    return rgFields[0].pdu->pfnCompare(pfLeft, pfRight, rgFields[0].pdu);
}

int A2C_TAG_copy(A2C_TAG_DATA * pseqLeft, A2C_TAG_DATA const * pseqRight, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;

    return rgFields[0].pdu->pfnCopy(pseqLeft, pseqRight, rgFields[0].pdu);
}

/********************************************************************************************/


A2C_ERROR A2C_TAG_EXPLICIT_Decode(int fDER, PBYTE pb, A2C_ITEM const * pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    A2C_ERROR           err;
    int	                fConstructed;
    int                 iClass;
    int                 iContext;
    A2C_TAG_VALUE       iValue;
    A2C_STREAM_MEMORY * pstm2;
    A2C_STREAM_MEMORY   stm = {{0}};

    if (flags & A2C_FLAGS_MORE_DATA) {
        A2C_CXT_ITEM *  pcxtitm;
        /*
         *  Get the specifics for this function
         */
        
        iContext = A2C_ContextFrame(pcxt);
        pcxtitm = &pcxt->rgCxts[iContext];

        /*
         *  We have already checked the tag and maybe some fields
         */

        cbLength = pcxtitm->cbLeft;

        /*
         *  If we have cleared the stack - clear the flag
         */
        
        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
    }
    else {
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
         */

        if (ptag != NULL) {
            if ((ptag->iClass != iClass) || (ptag->iValue != iValue)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }
        else {
            if ((pdesc->tag.iClass != iClass) || (pdesc->tag.iValue != iValue)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }

        /*
         *  Skip over the tag and length
         */

        _A2C_Memory_Skip(pstm, cbTL);


        /*
         *  Set us up for doing an restart operation
         */

        err = A2C_PushContext(pcxt, &iContext);
        if (err != A2C_ERROR_Success) goto ErrorExit;
    }

    /*
     *  Limit the child decode to the byte in the current object
     */

    if (cbLength == -1) {
        if (fDER) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }
        pstm2 = pstm;
    }
    else {
        _A2C_Memory_Init_With_Data(&stm, pstm->pb, MIN(cbLength, (pstm->pbLast - pstm->pb)));
        pstm2 = &stm;
    }

    /*
     *  Decode the child
     */

    if (fDER) {
        err = pdesc->pdu->pfnDerDecoder(pb, pdesc->pdu, flags, pcxt, NULL, pstm2);
    }
    else {
        err = pdesc->pdu->pfnBerDecoder(pb, pdesc->pdu, flags, pcxt, NULL, pstm2);
    }

    /*
     *  Did we have a failure that says we need more data?
     */

    if ((err == A2C_ERROR_needMoreData) &&
        ((cbLength == -1) || ((cbLength - (stm.pb - stm.pbBase)) > 0))) {
        /*
         *  Add our data to the context stack
         *
         *  Which item we are processing
         *
         *  Count of bytes left = 
         *     # bytes in this item - # bytes consumed so far
         */

        pcxt->rgCxts[iContext].iItem = 0;
        pcxt->rgCxts[iContext].cbLeft = cbLength - (A2C_LENGTH) (stm.pb - stm.pbBase);

        /*
         *  Advance called in buffer by correct # of bytes
         */

        _A2C_Memory_Skip(pstm, stm.pb - stm.pbBase);

        goto ErrorExit;
    }
    else if (err == A2C_ERROR_needMoreData) {
        err = A2C_ERROR_malformedEncoding;
        goto ErrorExit;
    }
    else if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  If we did not use the entire blob - fail
     */

    if (cbLength == -1) {
        err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
        if (err < 0) {
#pragma message("M00BUG: deal with needMoreData in Decode")
            goto ErrorExit;
        }

        if ((iClass != 0) && (iValue != 0)) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        _A2C_Memory_Skip(pstm, cbTL);
    }
    else {
        if (cbLength != (stm.pb - stm.pbBase)) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }
    }

    /*
     *  Advance the passed in buffer by the correct # of bytes
     */

    _A2C_Memory_Skip(pstm, cbLength);

    /*
     *  Remove the restart setup
     */

    A2C_PopContext(pcxt, iContext);

    /*
     *  Return success
     */

    err = A2C_ERROR_Success;

    /*
     *  Do the cleanup and return
     */

ErrorExit:
    return err;
}

A2C_ERROR A2C_TAG_Internal_EncodeBer(PCBYTE pb, A2C_ITEM const * pdesc, int flags,
                                     A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    A2C_LENGTH          cbTag;
    A2C_ERROR           err;
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Which tag to use?  passed in wins
     */

    if (ptag == NULL) {
        ptag = &pdesc->tag;
    }

    /*
     *  Emit the tag
     */

    err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Write out an indefinite length
     */
    
    err = _A2C_write_length((size_t) -1, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Encode the child
     */

    err = pdesc->pdu->pfnBerEncoder(pb, pdesc->pdu, flags, pcxt, NULL, pstm);
    
    /*
     *  Did we have a failure?
     */

    if (err < 0) {
        goto ErrorExit;
    }

    /*
     *  Write out an end-of-contents sequence
     */

    err = _A2C_write_tag(0, 0, 0, pstm, &cbTag);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }
    
    err = _A2C_write_length(0, pstm, &cb);

ErrorExit:
    /*
     *  Cleanup
     */

    _A2C_Memory_Cleanup(&stm);
    
    return err;
}


A2C_ERROR A2C_TAG_Internal_EncodeDer(PCBYTE pb, A2C_ITEM const * pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    A2C_ERROR           err;
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Which tag to emit?
     *    Use the one we have if none is passed in.
     */

    if (ptag == NULL) {
        ptag = &pdesc->tag;
    }

    /*
     *  Emit the tag
     */

    err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Need to build the item below us before we can continue.
     */

    _A2C_Memory_Init(&stm);

    /*
     *  Encode the child
     */

    err = pdesc->pdu->pfnDerEncoder(pb, pdesc->pdu, flags, pcxt, NULL, (PA2C_STREAM) &stm);
    
    /*
     *  Did we have a failure?
     */

    if (err < 0) {
        goto ErrorExit;
    }

    /*
     *  Write out the length of the data
     */

    err = _A2C_write_length(stm.pb - stm.pbBase, pstm, &cb);
    if (err < 0) {
        goto ErrorExit;
    }

    /*
     *  Write out the blob of data now
     */

    err = pstm->pfnWrite(pstm, stm.pbBase, stm.pb - stm.pbBase);
    if (err < 0) {
        goto ErrorExit;
    }

ErrorExit:
    /*
     *  Cleanup
     */

    _A2C_Memory_Cleanup(&stm);
    
    return err;
}
