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
#include <stdio.h>

#include "A2C_Stream_Internal.h"



A2C_CONST A2C_ITEM_HEADER A2C_BINARY_description = {
    sizeof(A2C_BINARY), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_BINARY_descriptor = {
    A2C_zero, A2C_nop,
    (A2C_decoder_f) A2C_BINARY_decode_der,
    (A2C_encoder_f) A2C_BINARY_encode_der,
    (A2C_decoder_f) A2C_BINARY_decode_ber,
    (A2C_encoder_f) A2C_BINARY_encode_ber,
    (A2C_compare_f) A2C_BINARY_compare,
    (A2C_copy_f) A2C_BINARY_copy,
    (A2C_print_f) A2C_BINARY_print,
    A2C_BINARY_validate,
    &A2C_BINARY_description
};

/*
 *  Read in the buffer.  We don't know how to decode this, so the best we can do is to
 *      copy the entire blob buffer out and pass it back.  Data should start with TL.
 */

A2C_ERROR A2C_BINARY_decode_der(A2C_BINARY * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                   A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    int         err;
    int         fConstructed;
    int         iClass;
    A2C_TAG_VALUE iValue;

    unreferenced(flags);
    unreferenced(pdesc);
    unreferenced(pcxt);
    unreferenced(ptag);

    /*  M00BUG - do we want to try and do restart here? */
    
    /*
     *  Grab the type and length of this item.  This function will without advancing
     *  the pointer if there are not sufficent bytes for the value in the buffer.
     */

    err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
    if (err < 0) {
        goto ErrorExit;
    }

	/*
	 * Don't care what the tag is
	 */

    /*
     *  Do we have enough data?
     */

    if (_A2C_Memory_Left(pstm) < (cbTL + cbLength)) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Allocate a buffer to read the result into
     */

    pos->data = malloc(cbLength + cbTL);
    if (pos->data == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }


    _A2C_Memory_Read(pstm, pos->data, cbLength + cbTL);
    pos->length = cbLength + cbTL;
    
    return A2C_ERROR_Success;

ErrorExit:
    if (pos->data != NULL) {
        free(pos->data);
        pos->data = NULL;
    }
    return err;
}


/*
 *  Write out a DER encoded bit string
 */

int A2C_BINARY_encode_der(A2C_BINARY const * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;

    unreferenced(pdesc);
    unreferenced(ptag);
    unreferenced(pcxt);
    unreferenced(flags);

    /*
     *  Write out the bytes
     */
    
    cb = _A2C_write_bytes(pos->data, pos->length, pstm);
    if (cb < 0) {
        return cb;
    }
    return cb;
}


A2C_ERROR A2C_BINARY_decode_ber(A2C_BINARY * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                   A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    A2C_STREAM_MEMORY * pstmDst = NULL;

    unreferenced(ptag);
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

        iFrame = A2C_ContextFrame(pcxt);
        pcxtitem = &pcxt->rgCxts[iFrame];

        /*
         *  Grab the saved data
         */

        pstmDst = (A2C_STREAM_MEMORY *) pcxtitem->pbData;

        /*
         *  If we are at the bottom of the stack, clean up.
         */

        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
    }
    else {
        /*
         *  First time in
         *
         *  Create the destination stream
         */

        err = A2C_CreateMemoryStream((PA2C_STREAM *) &pstmDst);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

        /*
         *  Set us up for a restart
         */

        err = A2C_PushContext(pcxt, &iFrame);
        if (err != A2C_ERROR_Success) goto ErrorExit;

        /*
         *  We don't care about the tag
         */
    }


    err = _A2C_Recurse_GetLength(pstm, pstmDst, flags, pcxt, NULL, NULL);
    if (err == A2C_ERROR_needMoreData) {
        /*
         *  If the stack is not allocated - then we don't either
         */

        if (pcxt->rgCxts == NULL) {
            goto ErrorExit;
        }

        /*
         *  Save the stream
         */

        pcxt->rgCxts[iFrame].pbData = (PBYTE) pstmDst;
        pstmDst = NULL;
    }
    else if (err >= A2C_ERROR_Success) {
        /*
         *  Grab the data from the stream and put it into the object
         */

        err = _A2C_Memory_GetData(pstmDst, &pos->data, &pos->length);

        A2C_PopContext(pcxt, iFrame);
    }

ErrorExit:
    if (pstmDst != NULL) _A2C_Memory_Free(pstmDst);
    return err;
}

A2C_ERROR A2C_BINARY_encode_ber(A2C_BINARY const * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    return A2C_BINARY_encode_der(pos, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_BINARY_print is the default open type value print routine.
/// <para>If the type is fully defined, then it is recursively called in order to
/// do the print out.  If the system only has the bytes then they are dumped.</para>
/// </summary>
/// <param name="pf">pointer to the open type value</param>
/// <param name="pdesc">type descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */


int A2C_BINARY_print(A2C_BINARY const * pot, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;

    unreferenced(pdesc);
    
    /*
     *  Use the OCTET STRING dumpper
     */

    os.length = pot->length;
    os.data = pot->data;

    return A2C_OCTET_STRING_print(&os, NULL, iLevel, pstm);
}
    

int A2C_BINARY_compare(A2C_BINARY const * posLeft, A2C_BINARY const * posRight, PC_A2C_DESCRIPTOR pdesc)
{
    int i;

    unreferenced(pdesc);
    
    /*
     *  Compare the first n bytes
     */

    if (posLeft->length <= posRight->length) {
        i = memcmp(posLeft->data, posRight->data, posLeft->length);
        if (i == 0) return 1;
        return i;
    }
    i = memcmp(posLeft->data, posRight->data, posRight->length);
    if (i == 0) return -1;
    return i;
}

A2C_ERROR A2C_BINARY_copy(A2C_BINARY * posDst, A2C_BINARY const * posSrc, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    posDst->data = malloc(posSrc->length);
    if (posDst->data == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    memcpy(posDst->data, posSrc->data, posSrc->length);
    posDst->length = posSrc->length;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Binary validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
--- */

A2C_ERROR A2C_BINARY_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
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

/************************************************* UTILITIES ****************************************/

A2C_ERROR _A2C_Recurse_GetLength(A2C_STREAM_MEMORY * pstmIn, A2C_STREAM_MEMORY * pstmOut,
                                 int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, int * piBits)
{
    A2C_LENGTH  cb;
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    A2C_ERROR   err;
    int         fConstructed;
    int         iClass;
    int         iFrame;
    A2C_TAG_VALUE iValue;
    PBYTE       pb;
    
    /*
     *  Are we doing a restart operation?  If so we need to do correct setup
     *
     *  Yes we are doing a restart
     */

    if (flags & A2C_FLAGS_MORE_DATA) {
        A2C_CXT_ITEM * pcxtitem;
        /*
         *  Get the details we cached the last time
         *      and bump the last read count
         */


        iFrame = A2C_ContextFrame(pcxt);
        pcxtitem = &pcxt->rgCxts[iFrame];

        /*
         *  We already know the length of this item
         */

        cbLength = pcxtitem->cbLeft;

        /*
         *  If we are on the bottom of the stack, Clear the restart flag
         */

        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
        else {
            /*
             *  We are not at the bottom of the stack - recurse down to the
             *  next frame
             */

            goto SkipTagCheck;
        }
    }
    else {
        /*
         *   First time call in
         *
         *   Grab the type and length of this item
         */

        err = _A2C_get_tag_and_length(pstmIn, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

        /*
         *  If this is definitive length and 
         *      either 1) we are keeping all of the tags or
         *             2) it is not constructed
         *
         *   we can just copy everything over w/o problem
         */

        if ((cbLength != -1) && ((ptag == NULL) || !fConstructed)) {
            /*
             *  if not enough bytes - return the error
             */

            if (_A2C_Memory_Left(pstmIn) < cbTL + cbLength) {
                err = A2C_ERROR_needMoreData;
                goto ErrorExit;
            }

            /*
             *  Copy over the bytes and finish
             */

            if (ptag == NULL) {
                err = _A2C_Memory_Copy(pstmIn, pstmOut, cbTL + cbLength);
            }
            else {
                /*
                 *  M00TODO: Which tag? - is it the correct tag?
                 */

                _A2C_Memory_Skip(pstmIn, cbTL);

                /*
                 *  If we are doing BIT STRING, then the first byte is the # of
                 *      unused bits.  It must always be zero except for the last
                 *      component that is read in.
                 */
                
                if (piBits != NULL) {
                    BYTE        b;

                    if (*piBits != 0) {
                        err = A2C_ERROR_malformedEncoding;
                        goto ErrorExit;
                    }

                    if (cbLength == 0) {
                        err = A2C_ERROR_malformedEncoding;
                        goto ErrorExit;
                    }

                    err = _A2C_Memory_Read(pstmIn, &b, 1);
                    cbLength -= 1;

                    *piBits = b;
                }
                err = _A2C_Memory_Copy(pstmIn, pstmOut, cbLength);
            }
            goto ErrorExit;
        }

        /*
         *  Copy over the tag and length
         */

        if (ptag == NULL) {
            err = _A2C_Memory_Copy(pstmIn, pstmOut, cbTL);
            if (err < A2C_ERROR_Success) {
                goto ErrorExit;
            }
        }
        else {
            _A2C_Memory_Skip(pstmIn, cbTL);
        }

        /*
         *  Set us up for doing a restart operation
         */

        err = A2C_PushContext(pcxt, &iFrame);
        if (err != A2C_ERROR_Success) goto ErrorExit;
    }

    /*
     *  Loop until we are finished
     */

    while (cbLength != 0) {
        /*
         *  Are we at the end of our loop?
         */

        err = _A2C_get_tag_and_length(pstmIn, &iClass, &fConstructed, &iValue, &cb, &cbTL);
        if (err < A2C_ERROR_Success) {
            pb = NULL;  /* remove the warning */
            goto ErrorCheck;
        }

        if ((iClass == A2C_TAG_CLASS_UNIVERSAL) && (iValue == 0)) {
            if (ptag == NULL) {
                _A2C_Memory_Copy(pstmIn, pstmOut, cbTL);
            }
            else {
                _A2C_Memory_Skip(pstmIn, cbTL);
            }
            break;
        }
    SkipTagCheck:

        /*
         *  Grab the current pointer
         */

        pb = pstmIn->pb;

        /*
         *  Not at the end - recurse to copy over this piece of data
         */

        err = _A2C_Recurse_GetLength( pstmIn, pstmOut, flags, pcxt, ptag, piBits );

    ErrorCheck:
        /*
         *  Now do any error checking based on the return code
         */

        if (err == A2C_ERROR_needMoreData) {
            /*
             *  We ran out of data during the decode operation
             */

            /*
             *  Setup our block of data
             */

            pcxt->rgCxts[iFrame].cbLeft = cbLength;

            goto ErrorExit;
        }
        else if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

        /*
         *  Adjust the byte consumed count if needed
         */

        if (cbLength != -1) {
            cbLength = cbLength - (A2C_LENGTH) (pstmIn->pb - pb);
        }
    }

    /*
     *
     */

    err = A2C_ERROR_Success;


    /*
     *  Clean up anything and return
     */

    A2C_PopContext(pcxt, iFrame);
    
ErrorExit:
    return err;
}
