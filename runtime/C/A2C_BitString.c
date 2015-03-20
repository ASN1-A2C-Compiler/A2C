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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "A2C_Stream_Internal.h"

/*
 *
 */

A2C_CONST A2C_ITEM_HEADER A2C_BIT_STRING_description = {
    sizeof(A2C_BIT_STRING), 0
};

A2C_CONST A2C_ITEM_HEADER A2C_BIT_STRING_NAMED_description = {
    sizeof(A2C_BIT_STRING_NAMED), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_BIT_STRING_descriptor = {
    A2C_zero,
    A2C_BIT_STRING_release,
    (A2C_decoder_f) A2C_BIT_STRING_decode_der,
    (A2C_encoder_f) A2C_BIT_STRING_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_decode_ber,
    (A2C_encoder_f) A2C_BIT_STRING_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_compare,
    (A2C_copy_f) A2C_BIT_STRING_copy,
    (A2C_print_f) A2C_BIT_STRING_print,
    A2C_BIT_STRING_validate,
    &A2C_BIT_STRING_description
};

A2C_CONST A2C_DESCRIPTOR A2C_BIT_STRING_NAMED_descriptor = {
    A2C_zero,
    A2C_BIT_STRING_release,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_der,
    (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_ber,
    (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_NAMED_compare,
    (A2C_copy_f) A2C_BIT_STRING_NAMED_copy,
    (A2C_print_f) A2C_BIT_STRING_NAMED_print,
    A2C_BIT_STRING_NAMED_validate,
    &A2C_BIT_STRING_NAMED_description
};

static const A2C_TAG BitStringTag = {A2C_TAG_CLASS_UNIVERSAL, 3};




/*
 *
 */

int __inline GET_BIT(BYTE * pb, int iBit)
{
    int iByte = iBit/8;

    return pb[iByte] & (0x80 >> iBit%8);
}


/*
 */

A2C_ERROR A2C_BIT_STRING_release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_BIT_STRING * pbs = (A2C_BIT_STRING *) pvData;
    
    unreferenced(pdesc);

    if (pbs->data != NULL) {
        free(pbs->data);
    }

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_BIT_STRING_NAMED_release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_BIT_STRING_release(pvData, pdesc);
}

/*
 */

A2C_ERROR A2C_BIT_STRING_decode_der(A2C_BIT_STRING * pbs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                 A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    BYTE                cbitsUnused;
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    A2C_ERROR           err;
    int                 fConstructed;
    int                 iClass;
    A2C_TAG_VALUE       iValue;

    unreferenced(pcxt);
    unreferenced(flags);
    unreferenced(pdesc);
    
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
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 3)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }

    /*
     *  If this is a constructed value, then fail we don't have the
     *  correct code to deal with that in DER world.
     */
    
    if (fConstructed || (cbLength == -1)) {
        err = A2C_ERROR_malformedDer;
        goto ErrorExit;
    }

    /*
     *  See if we have enougth data
     */

    if (_A2C_Memory_Left(pstm) < cbLength + cbTL) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Skip over the tag and length
     */

    _A2C_Memory_Skip(((A2C_STREAM_MEMORY *) pstm), cbTL);

    /*
     *  Read in count of unused bits
     */
    
    _A2C_Memory_Read(pstm, &cbitsUnused, 1);

    /*
     *  Allocate a buffer to read the result into
     */

    pbs->data = malloc(cbLength-1);
    if (pbs->data == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
    pbs->bufferLength = cbLength-1;

    _A2C_Memory_Read(pstm, pbs->data, cbLength-1);

    /*
     *  Convert count of unused bits to count of bits
     */

    pbs->bitLength = (cbLength-2)*8 + (8-cbitsUnused);

    /*
     *  Return success
     */

    return A2C_ERROR_Success;

ErrorExit:
    if (pbs->data != NULL) {
        free(pbs->data);
        pbs->data = NULL;
    }
                            
    return err;
}

A2C_ERROR A2C_BIT_STRING_encode_der(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags,
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    BYTE                b;
    BYTE                cbitUnused;
    int                 cb;
    int                 cbData;
    int                 cbFull;
    A2C_ERROR           err;

    /*
     *  These parameters are not used
     */
    
    unreferenced(flags);
    unreferenced(pdesc);
    unreferenced(pcxt);

    /*
     *  Output is as follows:
     *  tag
     *  length
     *  # of unused bits in last byte
     *  bit array
     */

    cbData = 1 + (pbs->bitLength+7)/8;
    cbitUnused = (BYTE) (pbs->bitLength % 8);
    if ((cbData > 1) && (cbitUnused > 0)) {
        b = pbs->data[cbData-2];
        b &= (char) (0xff00 >> cbitUnused);
        cbitUnused = 8 - cbitUnused;
        cbFull = cbData - 2;
    }
    else {
        b = 0;
        cbFull = cbData - 1;
    }

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 3
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 3, pstm, &cb);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cb);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the length
     */

    err = _A2C_write_length(cbData, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the bytes in 3 pieces
     *  1.  # of unused bits
     *  2. bits in 8 bit blocks
     *  3. trailing bits (if any)
     */
    
    err = _A2C_write_bytes(&cbitUnused, 1, pstm);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    err = _A2C_write_bytes(pbs->data, cbFull, pstm);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    if (b != 0) {
        err = _A2C_write_bytes(&b, 1, pstm);
    }

    /*
     *  Cleanup and return
     */

ErrorExit:
    return err;
}

A2C_ERROR A2C_BIT_STRING_decode_ber(A2C_BIT_STRING * pbs, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag,
                                    A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    A2C_ERROR           err;
    int                 fConstructed;
    int                 iContext;
    int                 iClass;
    int                 iUnused;
    A2C_TAG_VALUE       iValue;
    A2C_STREAM_MEMORY * pstmDst = NULL;

    unreferenced(pdesc);
    
    /*
     *  Are we doing a restart operation?
     */

    if (flags & A2C_FLAGS_MORE_DATA) {
        A2C_CXT_ITEM * pcxtitem;
        /*
         *  Get the details we cached the last time
         *      and bump the last read count
         */

        iContext = A2C_ContextFrame(pcxt);
        pcxtitem = &pcxt->rgCxts[iContext];

        /*
         *  Grab the saved data
         */

        pstmDst = pcxtitem->pstm;

        /*
         *  If the stack is fully used - clear the restart flag
         */

        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
    }
    else {
        /*
         *  First time in
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
            if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 3)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }
        
        /*
         *  Create the destination stream
         */

        err = A2C_CreateMemoryStream((PA2C_STREAM *) &pstmDst);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

        /*
         *  Set us up for a restart
         */

        A2C_PushContext(pcxt, &iContext);

        /*
         *  STart with 0 unused bits in the last block
         */

        iUnused = 0;
    }


    err = _A2C_Recurse_GetLength(pstm, pstmDst, flags, pcxt, &BitStringTag, &iUnused);
    if (err == A2C_ERROR_needMoreData) {
        /*
         *  Save the stream & unused bit count
         */

        pcxt->rgCxts[iContext].pstm = pstmDst;
        pcxt->rgCxts[iContext].iItem = iUnused;
        pstmDst = NULL;
    }
    else if (err >= A2C_ERROR_Success) {
        /*
         *  Grab the data from the stream and put it into the object
         */

        err = _A2C_Memory_GetData(pstmDst, &pbs->data, &pbs->bufferLength);
        if (err == A2C_ERROR_Success) {
            pbs->bitLength = pbs->bufferLength * 8 - iUnused;
        }
    }

ErrorExit:
    if (pstmDst != NULL) _A2C_Memory_Free(pstmDst);
    return err;

}

A2C_ERROR A2C_BIT_STRING_encode_ber(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags,
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*
     *  We never do a constructed bit string - use the DER encoder
     */

    return A2C_BIT_STRING_encode_der(pbs, pdesc, flags, pcxt, ptag, pstm);
}
    

A2C_ERROR A2C_BIT_STRING_print(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc,
                               int iLevel, PA2C_STREAM pstm)
{
    unreferenced(iLevel);
    unreferenced(pdesc);
    unreferenced(pbs);

    return pstm->pfnWrite(pstm, (PBYTE) "BITSTRING", 9);
}

int A2C_BIT_STRING_compare(A2C_BIT_STRING const * pbsLeft, A2C_BIT_STRING const * pbsRight,
                           PC_A2C_DESCRIPTOR pdesc)
{
    BYTE        b;
    BYTE        bMask;
    int         cbitLeft;
    int         cbitRight;
    int         cbitMin;
    int         cbLeft = pbsLeft->bitLength/8;
    int         cbRight = pbsRight->bitLength/8;
    PBYTE       pbLeft = pbsLeft->data;
    PBYTE       pbRight = pbsRight->data;
        
    /*
     *  We don't use this parameter
     */

    unreferenced(pdesc);

    /*
     *  Compare all of the complete bytes in the buffer.
     */

    while ((cbRight > 0) && (cbLeft > 0)) {
        if (*pbLeft != *pbRight) {
            return *pbLeft - *pbRight;
        }

        cbRight --;
        cbLeft --;
        pbRight ++;
        pbLeft ++;
    }
    
    /*
     *   Compare the parts of the last bits that are common
     */

    cbitLeft = pbsLeft->bitLength % 8;
    cbitRight = pbsRight->bitLength % 8;
    
    cbitMin = MIN(cbitLeft, cbitRight);
    
    bMask = (BYTE) (0xff << (8-cbitMin));

    b = (*pbLeft & bMask) - (*pbRight & bMask);
    if (b != 0) return b;

    return pbsLeft->bitLength - pbsRight->bitLength;
}

A2C_ERROR A2C_BIT_STRING_copy(A2C_BIT_STRING * pbsDst, A2C_BIT_STRING const * pbsSrc, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */
    
    unreferenced(pdesc);

    /*
     *  Allocate the output buffer
     */
        
    pbsDst->data = malloc((pbsSrc->bitLength+7)/8);
    if (pbsDst->data == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    /*
     *  Copy over the data
     */

    pbsDst->bitLength = pbsSrc->bitLength;
    pbsDst->bufferLength = pbsSrc->bufferLength;
    memcpy(pbsDst->data, pbsSrc->data, (pbsSrc->bitLength+7)/8);
    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// The only validation that we make is to ensure that the number of bits
/// is going to fit into the byte buffer.
/// </summary>
/// <param name="pv">pointer to the object to validate</param>
/// <param name="pdesc">pointer to the object descriptor</param>
/// <param name="pcxt">pointer to context information</param>
/// <param name="pstm">stream to dump the information on</param>
/// <returns>A2C_ERROR_Success or A2C_ERROR_validateFails</returns>
--- */

A2C_ERROR A2C_BIT_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    A2C_BIT_STRING *    pbs = (A2C_BIT_STRING *) pv;
    
    /*
     *  If not already done - push our selfs on the stack
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
     *  Check that the length is long enough
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if (pbs->bitLength > pbs->bufferLength*8) {
            /*
             *  Do we have a buffer to dump to?
             */
        
              A2C_SetContextNum(pcxt, iFrame, -1);

              return A2C_Validate_Print(pdesc, pcxt, "BIT STRING", "bitLength too large", pstm);
        }
    }

    /*
     *  No validate is done - return success
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}

/*******************************************************************************/


A2C_ERROR A2C_BIT_STRING_NAMED_decode_der(A2C_BIT_STRING * pbs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                          A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*
     *  No difference in the decode case
     */
    
    return A2C_BIT_STRING_decode_der(pbs, pdesc, flags, pcxt, ptag, pstm);
}

/*
 *  Write out a DER encoded bit string
 */

A2C_ERROR A2C_BIT_STRING_NAMED_encode_der(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    A2C_ERROR           err;
    int			i;
    A2C_BIT_STRING      temp;

    /*
     *  Unused parameters
     */
    
    unreferenced(pdesc);
    
    
    /*
     *  Create a copy of the input string
     */

    cb = (pbs->bitLength+7)/8;
    temp.data = malloc(cb);
    if (temp.data == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    memcpy(temp.data, pbs->data, cb);
    
    /*
     *  Per clause 11.2.2 - remove trailing zero bits if named bit string
     */

    for (i=pbs->bitLength-1; i>=0; i--) {
        if (GET_BIT(temp.data, i)) {
            /*
             *  We have a set bit - we are done removing bits
             */
            break;
        }
    }
    temp.bitLength = i+1;

    /*
     *  Call the normal emitter
     */

    err = A2C_BIT_STRING_encode_der(&temp, pdesc, flags, pcxt, ptag, pstm);

    /*
     *  Free up our allocated buffer
     */
    
    free(temp.data);

    /*
     *  Return error code
     */

    return err;
}

A2C_ERROR A2C_BIT_STRING_NAMED_decode_ber(A2C_BIT_STRING * pbs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                          A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /* M00BUG */

    return A2C_BIT_STRING_NAMED_decode_der(pbs, pdesc, flags, pcxt, ptag, pstm);
}


A2C_ERROR A2C_BIT_STRING_NAMED_encode_ber(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /* M00BUG */

    return A2C_BIT_STRING_NAMED_encode_der(pbs, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_BIT_STRING_NAMED_print(A2C_BIT_STRING const * pbs, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         fFirst = 1;
    int         iName;
    A2C_LENGTH  iBit;
    A2C_NAMES const *   pNames = &pdesc->pItemDescription->names;
    char const * sz;
    char        rgch[20];

    unreferenced(iLevel);

    for (iBit=0; iBit<pbs->bitLength; iBit++) {
        if (GET_BIT(pbs->data, iBit) != 0) {
            sz = NULL;
            for (iName=0; iName<pNames->count; iName++) {
                if (iBit == (A2C_LENGTH) pNames->listArrayPtr[iName].number) {
                    sz = pNames->listArrayPtr[iName].name;
                    break;
                }
            }

            if (!fFirst) {
                err = pstm->pfnWrite(pstm, (PBYTE) " | ", 3);
                if (err < A2C_ERROR_Success) {
                    return err;
                }
            }

            if (sz == NULL) {
                snprintf(rgch, sizeof(rgch), "%d", iBit);
                rgch[sizeof(rgch)-1] = 0;
                sz = rgch;
            }

            err = pstm->pfnWrite(pstm, (PBYTE) sz, strlen(sz));
            if (err < A2C_ERROR_Success) {
                return err;
            }

            fFirst = 0;
        }
    }

    return A2C_ERROR_Success;
}

int A2C_BIT_STRING_NAMED_compare(A2C_BIT_STRING_NAMED const * pbsLeft, A2C_BIT_STRING_NAMED const * pbsRight,
                                 PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  M00BUG - this function should strip trailing zeros before doing the comparison!!!
     */
    
    return A2C_BIT_STRING_compare(pbsLeft, pbsRight, pdesc);
}

A2C_ERROR A2C_BIT_STRING_NAMED_copy(A2C_BIT_STRING_NAMED * pbsLeft, A2C_BIT_STRING_NAMED const * pbsRight,
                              PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_BIT_STRING_copy(pbsLeft, pbsRight, pdesc);
}


/******************************************* HELPER FUNCTIONS *******************************************/

/* ---
/// <summary>
/// Determine if a bit is set
/// </summary>
/// <param name="pbs">pointer to the bit string structure</param>
/// <param name="iBit">index (zero-based) of the bit to check</param>
/// <returns>FALSE if not set, TRUE if set</returns>
--- */

A2C_ERROR A2C_BIT_STRING_IsSet(A2C_BIT_STRING const * pbs, int iBit)
{
    int         i;
    int         ib;

    /*
     *  If we are past the last known bit - then return false
     */
        
    if (((A2C_LENGTH)iBit) >= pbs->bitLength) return A2C_ERROR_indexOutOfRange;

    /*
     *  Compute the byte number we are going to look at
     */

    ib = iBit/8;

    /*
     *  If we overrun the buffer - don't blow up
     */
    
    if (((A2C_LENGTH)ib) >= pbs->bufferLength) return A2C_ERROR_indexOutOfRange;

    /*
     *  Get the answer
     */

    i = (pbs->data[ib] & (0x80 >> (iBit % 8)));

    return (i != 0) ? A2C_ERROR_True : A2C_ERROR_False;
}

/* ---
/// <summary>
/// Set a specified bit in the buffer.
/// This function will increase the value of bitLength, but not bufferLength.
/// </summary>
/// <param name="pbs">Buffer to set the buffer in</param>
/// <param name="iBit">index (zero-based) of bit to be set<param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_BIT_STRING_Set(A2C_BIT_STRING * pbs, int iBit)
{
    A2C_LENGTH      ib = (size_t) (iBit/8);

    /*
     *  is this bit beyond the end of the buffer?
     */
    
    if (ib >= pbs->bufferLength) return A2C_ERROR_indexOutOfRange;

    /*
     *  Set the bit value
     */

    pbs->data[ib] |= (0x80 >> (iBit % 8));

    /*
     *  Update bitLength if necessary
     */

    if (((A2C_LENGTH) iBit) >= pbs->bitLength) {
        pbs->bitLength = iBit + 1;
    }

    return A2C_ERROR_Success;
}
    
/* ---
/// <summary>
/// Clear a specified bit in the buffer.
/// This function will increase the value of bitLength, but not bufferLength.
/// </summary>
/// <param name="pbs">Buffer to set the buffer in</param>
/// <param name="iBit">index (zero-based) of bit to be set<param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_BIT_STRING_Clear(A2C_BIT_STRING * pbs, int iBit)
{
    A2C_LENGTH ib = (size_t) (iBit/8);
    BYTE        b;

    /*
     *  is this bit beyond the end of the buffer?
     */
    
    if (ib >= pbs->bufferLength) return A2C_ERROR_indexOutOfRange;

    /*
     *  Clear the bit value
     */

    b = (BYTE) ~(0x80 >> (iBit % 8));
    pbs->data[ib] &= b;

    /*
     *  Update bitLength if necessary
     */

    if (((A2C_LENGTH) iBit) >= pbs->bitLength) {
        pbs->bitLength = iBit + 1;
    }

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_BIT_STRING_NAMED_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    return A2C_BIT_STRING_validate(pv, pdesc, pcxt, pf, pstm);
}
/*********************************************************************************/


A2C_ERROR A2C_BIT_STRING_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames)
{
    *pNames = &pdesc->pItemDescription->names;

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_BIT_STRING_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi)
{
    int                 i;
    A2C_NAMES const *   pNames = &pdesc->pItemDescription->names;

    for (i=0; i<pNames->count; i++) {
        if (strcmp(pNames->listArrayPtr[i].name, szName) == 0) {
            *pi = pNames->listArrayPtr[i].number;
            return A2C_ERROR_Success;
        }
    }

    return A2C_ERROR_notFound;
}

A2C_ERROR A2C_BIT_STRING_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz)
{
    int                 i;
    A2C_NAMES const *   pNames = &pdesc->pItemDescription->names;

    for (i=0; i<pNames->count; i++) {
        if (iValue == pNames->listArrayPtr[i].number) {
            *psz = pNames->listArrayPtr[i].name;
            return A2C_ERROR_Success;
        }
    }

    return A2C_ERROR_notFound;
}

/***************************** OPEN BIT STRING ********************************/

A2C_CONST A2C_ITEM_HEADER A2C_OPEN_BIT_STRING_description = {
    sizeof(A2C_OPEN_BIT_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_OPEN_BIT_STRING_descriptor = {
    A2C_zero,
    A2C_OPEN_BIT_STRING_release,
    (A2C_decoder_f) A2C_OPEN_BIT_STRING_decode_der,
    (A2C_encoder_f) A2C_OPEN_BIT_STRING_encode_der,
    (A2C_decoder_f) A2C_OPEN_BIT_STRING_decode_ber,
    (A2C_encoder_f) A2C_OPEN_BIT_STRING_encode_ber,
    (A2C_compare_f) A2C_OPEN_BIT_STRING_compare,
    (A2C_copy_f) A2C_OPEN_BIT_STRING_copy,
    (A2C_print_f) A2C_OPEN_BIT_STRING_print,
    A2C_OPEN_BIT_STRING_validate,
    &A2C_OPEN_BIT_STRING_description
};

/* ---
/// <summary>
/// release any data allocated
/// </summary>
/// <param name="pv">pointer to the open type value</param>
/// <param name="pdesc">pointr to the descriptor table</param>
/// <returns>A2C_ERROR_Sucess</returns>
--- */

A2C_ERROR A2C_OPEN_BIT_STRING_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_OPEN_BIT_STRING * pobs = (A2C_OPEN_BIT_STRING *) pv;
    unreferenced(pdesc);
    
    /*
     *  Free a blob of data
     */
    
    if (pobs->data != NULL) {
        free(pobs->data);
    }

    /*
     *  Free a structured data item
     */
    
    if (pobs->objectPtr != NULL) {
        pobs->functionPtr->pfnRelease(pobs->objectPtr, pobs->functionPtr);

        free(pobs->objectPtr);
    }
    
    return A2C_ERROR_Success;
}

/*
 *  This function exists, but it should never be called by anyone
 */

A2C_ERROR A2C_OPEN_BIT_STRING_decode_der(A2C_OPEN_BIT_STRING * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                           A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(poos);
    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    unreferenced(ptag);
    unreferenced(pstm);

    return A2C_ERROR_ICE;
}

A2C_ERROR A2C_OPEN_BIT_STRING_encode_der(A2C_OPEN_BIT_STRING const * pobs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR                           err;
    A2C_BIT_STRING                      bs = {0};
    PA2C_STREAM                         pstm2 = NULL;

    unreferenced(pdesc);

    /*
     *  If we have the encoded bytes already - then we can just wrap it into a blob and encode the blob
     */

    if (pobs->objectPtr == NULL) {
        /*
         *  Setup as an BIT STRING and encode it
         */


        bs.bitLength = pobs->bitLength;
        bs.bufferLength = pobs->bufferLength;
        bs.data = pobs->data;

        return A2C_BIT_STRING_encode_der(&bs, &A2C_BIT_STRING_descriptor, flags, pcxt, ptag, pstm);
    }

    /*
     *  The data still needs to be encoded.
     *
     *  We assume that the data is always short - so just encode it in a single blob
     */

    err = A2C_CreateMemoryStream(&pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = pobs->functionPtr->pfnDerEncoder(pobs->objectPtr, pobs->functionPtr, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = A2C_GetDataFromStream(pstm2, &bs.data, &bs.bitLength);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  bit length is 8 times longer that byte length
     */
    
    bs.bitLength *= 8;

    /*
     *  Use the normal encoder to do the encoding - it's just easier
     */

    err = A2C_BIT_STRING_encode_der(&bs, &A2C_BIT_STRING_descriptor, flags, pcxt, ptag, pstm);

    
ErrorExit:
    /*
     *  Do the needed cleanup
     */

    A2C_FreeStream(pstm2);
    free(bs.data);

    /*
     *  Free any data that is left
     */
    
    return err;
}


/*
 *  This function exists, but it should never be called by anyone
 */

A2C_ERROR A2C_OPEN_BIT_STRING_decode_ber(A2C_OPEN_BIT_STRING * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                           A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(poos);
    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    unreferenced(ptag);
    unreferenced(pstm);

    return A2C_ERROR_ICE;
}

A2C_ERROR A2C_OPEN_BIT_STRING_encode_ber(A2C_OPEN_BIT_STRING const * pobs, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR                           err;
    A2C_BIT_STRING                      bs = {0};
    PA2C_STREAM                         pstm2 = NULL;

    unreferenced(pdesc);

    /*
     *  If we have the encoded bytes already - then we can just wrap it into a blob and encode the blob
     */

    if (pobs->objectPtr == NULL) {
        /*
         *  Setup as an BIT STRING and encode it
         */


        bs.bitLength = pobs->bitLength;
        bs.bufferLength = pobs->bufferLength;
        bs.data = pobs->data;

        return A2C_BIT_STRING_encode_ber(&bs, &A2C_BIT_STRING_descriptor, flags, pcxt, ptag, pstm);
    }

    /*
     *  The data still needs to be encoded.
     *
     *  We assume that the data is always short - so just encode it in a single blob
     */

    err = A2C_CreateMemoryStream(&pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = pobs->functionPtr->pfnBerEncoder(pobs->objectPtr, pobs->functionPtr, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = A2C_GetDataFromStream(pstm2, &bs.data, &bs.bitLength);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  bit length is 8 times longer that byte length
     */
    
    bs.bitLength *= 8;

    /*
     *  Use the normal encoder to do the encoding - it's just easier
     */

    err = A2C_BIT_STRING_encode_ber(&bs, &A2C_BIT_STRING_descriptor, flags, pcxt, ptag, pstm);

    
ErrorExit:
    /*
     *  Do the needed cleanup
     */

    A2C_FreeStream(pstm2);
    free(bs.data);

    /*
     *  Free any data that is left
     */
    
    return err;
}

/* ---
/// <summary>
/// A2C_OPEN_BIT_STRING_print is the default open type value print routine.
/// <para>If the type is fully defined, then it is recursively called in order to
/// do the print out.  If the system only has the bytes then they are dumped.</para>
/// </summary>
/// <param name="pf">pointer to the open type value</param>
/// <param name="pdesc">type descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>A2C_ERROR_Success on success, A2C_ERROR on failure.</return>
--- */


A2C_ERROR A2C_OPEN_BIT_STRING_print(A2C_OPEN_BIT_STRING const * pobs, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_BIT_STRING    bs = {0};

    unreferenced(pdesc);
    
    /*
     *  If we have information on the structure, use it
     */

    if (pobs->objectPtr != NULL) {
        return pobs->functionPtr->pfnPrint(pobs->objectPtr, pobs->functionPtr, iLevel, pstm);
    }

    /*
     *  Use the BIT STRING dumpper
     */

    bs.bitLength = pobs->bitLength;
    bs.data = pobs->data;

    return A2C_BIT_STRING_print(&bs, NULL, iLevel, pstm);
}
    
/* ---
/// <summary>
/// The OPEN TYPE compare function behaves as follows:
///   If both are encoded data - compare the encoded data
///   If both are decoded data - compare the decoded data
///   else fail compare
/// </summary>
/// <param name="pLeft"> first value to be compared </param>
/// <param name="pRight"> second value to be compared </param>
/// <param name="pdesc"> type descriptor structure </param>
/// <return> 0 if the same, any other value if different </return>
--- */

int A2C_OPEN_BIT_STRING_compare(A2C_OPEN_BIT_STRING const * pLeft, A2C_OPEN_BIT_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    
    /*  Are they both encoded data? */
    /*  If so do the standard comparision */

    if (pLeft->data != NULL) {
        A2C_BIT_STRING  bs1 = {0};
        A2C_BIT_STRING  bs2 = {0};

        if (pRight->data == NULL) return -1;

        /*
         *  Setup for a normal bit string comparison
         */

        bs1.bitLength = pLeft->bitLength;
        bs1.data = pLeft->data;

        bs2.bitLength = pRight->bitLength;
        bs2.data = pRight->data;

        return A2C_BIT_STRING_compare(&bs1, &bs2, &A2C_BIT_STRING_descriptor);
    }

    /*  Both are decoded data */
    /*  Use the encoded function to do the comparison */

    if (pLeft->functionPtr != pRight->functionPtr) return -1;

    return pLeft->functionPtr->pfnCompare(pLeft->objectPtr, pRight->objectPtr, pLeft->functionPtr);
}

/* ---
/// <summary>
/// The OPEN TYPE copy function behaves as follows:
///   If source is encoded data - copy the encoded data
///   If source is decoded data - copy the decoded data
/// </summary>
/// <param name="pLeft"> destination value </param>
/// <param name="pRight"> source value  </param>
/// <param name="pdesc"> type descriptor structure </param>
/// <return> 0 if the same, any other value if different </return>
--- */

A2C_ERROR A2C_OPEN_BIT_STRING_copy(A2C_OPEN_BIT_STRING * pobsLeft, A2C_OPEN_BIT_STRING const * pobsRight, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ERROR   err;

    unreferenced(pdesc);
    
    /*  If source is encoded, copy data in encoded format */

    if (pobsRight->data != NULL) {
        pobsLeft->data = malloc(pobsRight->bufferLength);
        
        if (pobsLeft->data == NULL) return A2C_ERROR_outOfMemory;

        memcpy(pobsLeft->data, pobsRight->data, pobsRight->bufferLength);
        pobsLeft->bufferLength = pobsRight->bufferLength;
        pobsLeft->bitLength = pobsRight->bitLength;

        return A2C_ERROR_Success;
    }

    err = pobsRight->functionPtr->pfnCopy(&pobsLeft->objectPtr, pobsRight->objectPtr, pobsRight->functionPtr);
    if (err >= A2C_ERROR_Success) {
        pobsLeft->functionPtr = pobsRight->functionPtr;
    }

    return err;
}

/* ---
/// <summary>
/// Check no checks
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_OPEN_BIT_STRING_validate(A2C_OPEN_BIT_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    unreferenced(pv);
    unreferenced(pdesc);
    unreferenced(pcxt);
    unreferenced(pstm);
    
    *pf = TRUE;
    return A2C_ERROR_Success;
}


A2C_ERROR A2C_OPEN_BIT_STRING_RELATION_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_BIT_STRING * pobs,
                                                  PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                                  A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR *      pfn = A2C_LookupType(pInfo, pcxt);
    A2C_BIT_STRING              bs = {0};
    A2C_ERROR                   err;
    A2C_STREAM_MEMORY *         pstm2 = NULL;
    
    unreferenced(pdesc);

    /*
     *  We make the assumption that only "small" things are eer placed inside of BIT STRINGs
     *  (which is a true statement for most IETF protocols).  We therefore just decode the
     *  bit string and then apply any other processing later - unlike the case of OCTET STRINGs
     *  where we assume that large amounts of data might exist (see CMS).
     */

    /*
     *  Start by passing it off to normal bit string processing
     */

    
    err = A2C_BIT_STRING_decode_der(&bs, &A2C_BIT_STRING_descriptor, flags,
                                    pcxt, ptag, pstm);
    
    if (err < A2C_ERROR_Success) goto ErrorExit;

    
    if (pfn == NULL) {
        /*
         *  We do not have any idea of what this structure is - just return the
         *  embedded struture
         */

        pobs->bitLength = bs.bitLength;
        pobs->bufferLength = bs.bufferLength;
        pobs->data = bs.data;

        /*
         *  return the success and the structure
         */
        
        return err;
    }

    /*
     *  We must have a round number of bytes in order to decode anything
     */

    if ((bs.bitLength & 0x7) != 0) {
        /*
         *  This should potentially be an error condition rather than just a return
         *      of the data.  The end user is going to have problems trying to decode
         *      it in any event.
         */

        pobs->bitLength = bs.bitLength;
        pobs->bufferLength = bs.bufferLength;
        pobs->data = bs.data;

        /*
         *  return the success and the structure
         */
        
        return err;
    }        

    /*
     *  Ask for the data to be decoded
     */
    
    /*  Allocate and init the buffer to hold this */

    A2C_Alloc(&pobs->objectPtr, pfn);
    if (pobs->objectPtr == NULL) goto ErrorExit;

    /*
     *  Allocate a buffer to read the result into
     */

    err = A2C_CreateMemoryStreamFromBytes((A2C_STREAM **) &pstm2, bs.data, bs.bitLength/8);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Now call the decoder for this item
     */

    err = pfn->pfnDerDecoder(pobs->objectPtr, pfn, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;
    
    pobs->functionPtr = pfn;

    /*
     *  Clean out the temp field we were using 
     */
    
    A2C_BIT_STRING_release(&bs, &A2C_BIT_STRING_descriptor);
    _A2C_Memory_Free(pstm2);
    return err;

ErrorExit:

    if (pstm2 != NULL) _A2C_Memory_Free(pstm2);
    A2C_BIT_STRING_release(&bs, &A2C_BIT_STRING_descriptor);
    if (pobs->objectPtr != NULL) {
        pfn->pfnRelease(pobs->objectPtr, pfn);
        free(pobs->objectPtr);
        pobs->objectPtr = NULL;
    }
    return err;
}

A2C_ERROR A2C_OPEN_BIT_STRING_RELATION_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_BIT_STRING * pobs,
                                                  PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                                  A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR *      pfn = A2C_LookupType(pInfo, pcxt);
    A2C_BIT_STRING              bs = {0};
    A2C_ERROR                   err;
    A2C_STREAM_MEMORY *         pstm2 = NULL;

    unreferenced(pdesc);

    /*
     *  We make the assumption that only "small" things are eer placed inside of BIT STRINGs
     *  (which is a true statement for most IETF protocols).  We therefore just decode the
     *  bit string and then apply any other processing later - unlike the case of OCTET STRINGs
     *  where we assume that large amounts of data might exist (see CMS).
     */

    /*
     *  Start by passing it off to normal bit string processing
     */

    
    err = A2C_BIT_STRING_decode_ber(&bs, &A2C_BIT_STRING_descriptor, flags,
                                    pcxt, ptag, pstm);
    
    if (err < A2C_ERROR_Success) goto ErrorExit;

    
    if (pfn == NULL) {
        /*
         *  We do not have any idea of what this structure is - just return the
         *  embedded struture
         */

        pobs->bitLength = bs.bitLength;
        pobs->bufferLength = bs.bufferLength;
        pobs->data = bs.data;

        /*
         *  return the success and the structure
         */
        
        return err;
    }

    /*
     *  We must have a round number of bytes in order to decode anything
     */

    if ((bs.bitLength & 0x7) != 0) {
        /*
         *  This should potentially be an error condition rather than just a return
         *      of the data.  The end user is going to have problems trying to decode
         *      it in any event.
         */

        pobs->bitLength = bs.bitLength;
        pobs->bufferLength = bs.bufferLength;
        pobs->data = bs.data;

        /*
         *  return the success and the structure
         */
        
        return err;
    }        

    /*
     *  Ask for the data to be decoded
     */
    
    /*  Allocate and init the buffer to hol this */

    A2C_Alloc(&pobs->objectPtr, pfn);
    if (pobs->objectPtr == NULL) goto ErrorExit;

    /*
     *  Allocate a buffer to read the result into
     */

    err = A2C_CreateMemoryStreamFromBytes((A2C_STREAM **) &pstm2, bs.data, bs.bitLength/8);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Now call the decoder for this item
     */

    err = pfn->pfnBerDecoder(pobs->objectPtr, pfn, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;
    
    pobs->functionPtr = pfn;

    /*
     *  Clean out the temp field we were using 
     */
    
    A2C_BIT_STRING_release(&bs, &A2C_BIT_STRING_descriptor);
    _A2C_Memory_Free(pstm2);
    return err;

ErrorExit:

    if (pstm2 != NULL) _A2C_Memory_Free(pstm2);
    A2C_BIT_STRING_release(&bs, &A2C_BIT_STRING_descriptor);
    if (pobs->objectPtr != NULL) {
        pfn->pfnRelease(pobs->objectPtr, pfn);
        free(pobs->objectPtr);
        pobs->objectPtr = NULL;
    }
    return err;
}
