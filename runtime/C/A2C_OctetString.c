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
#include <assert.h>

#include "A2C_Stream_Internal.h"

#define TRUE 1


A2C_CONST A2C_ITEM_HEADER A2C_OCTET_STRING_description = {
    sizeof(A2C_OCTET_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_OCTET_STRING_descriptor = {
    A2C_zero,
    A2C_OCTET_STRING_release,
    (A2C_decoder_f) A2C_OCTET_STRING_decode_der,
    (A2C_encoder_f) A2C_OCTET_STRING_encode_der,
    (A2C_decoder_f) A2C_OCTET_STRING_decode_ber,
    (A2C_encoder_f) A2C_OCTET_STRING_encode_ber,
    (A2C_compare_f) A2C_OCTET_STRING_compare,
    (A2C_copy_f) A2C_OCTET_STRING_copy,
    (A2C_print_f) A2C_OCTET_STRING_print,
    A2C_OCTET_STRING_validate,
    &A2C_OCTET_STRING_description
};

static const A2C_TAG OctetStringTag = {A2C_TAG_CLASS_UNIVERSAL, 4};

/***************************  Global Functions *******************************************/

A2C_ERROR A2C_OCTET_STRING_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_OCTET_STRING * ps = (A2C_OCTET_STRING *) pv;

    unreferenced(pdesc);

    if (ps->data != 0) free(ps->data);
    ps->data = 0;

    return A2C_ERROR_Success;
}

/*
 *  Read from a DER string
 */

A2C_ERROR A2C_OCTET_STRING_decode_der(A2C_OCTET_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags,
                                      A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    int         err;
    int         fConstructed;
    int         iClass;
    A2C_TAG_VALUE iValue;

    unreferenced(pdesc);
    unreferenced(flags);
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
     */

    if (ptag != NULL) {
        if ((ptag->iClass != iClass) || (ptag->iValue != iValue)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }
    else {
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 4)) {
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
     *  Allocate a buffer to read the result into
     */

    pos->data = malloc(cbLength);
    if (pos->data == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
    
    pos->length = cbLength;
    _A2C_Memory_Read(pstm, pos->data, cbLength);

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

A2C_ERROR A2C_OCTET_STRING_encode_der(A2C_OCTET_STRING const * pos, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                      A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    int                 cbOut = 0;
    A2C_ERROR   err;

    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 3
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 4, pstm, &cbOut);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cbOut);
    }
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Write out the length byte
     */

    err = _A2C_write_length(pos->length, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Write out the bytes
     */
    
    return _A2C_write_bytes(pos->data, pos->length, pstm);
}

A2C_ERROR A2C_OCTET_STRING_decode_ber(A2C_OCTET_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags,
                                      A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    A2C_ERROR           err;
    int                 fConstructed;
    int                 iContext;
    int                 iClass;
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
            if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 4)) {
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
         *  We don't care about the tag
         */
    }


    err = _A2C_Recurse_GetLength(pstm, pstmDst, flags, pcxt, &OctetStringTag, NULL);
    if (err == A2C_ERROR_needMoreData) {
        /*
         *  Save the stream
         */

        pcxt->rgCxts[iContext].pstm = pstmDst;
        pstmDst = NULL;
    }
    else if (err >= A2C_ERROR_Success) {
        /*
         *  Grab the data from the stream and put it into the object
         */

        err = _A2C_Memory_GetData(pstmDst, &pos->data, &pos->length);
    }

ErrorExit:
    if (pstmDst != NULL) _A2C_Memory_Free(pstmDst);
    return err;
}    

A2C_ERROR A2C_OCTET_STRING_encode_ber(A2C_OCTET_STRING const * pos, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                      A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*
     *  We never do a constructed OCTET STRING here.
     *
     *  Pass off the work to the DER encoder
     */

    return A2C_OCTET_STRING_encode_der(pos, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_OCTET_STRING_print is the default octet string value print routine.
/// </summary>
/// <param name="pf">pointer to the boolean value</param>
/// <param name="pdesc">type descriptor - ignored</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_OCTET_STRING_print(A2C_OCTET_STRING const * pos, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_LENGTH          i;
    PBYTE               pb;
    char *              pch;
    static const char   rgchHex[] = "0123456789ABCDEF";
    char                rgch[16*3+1];
    
    unreferenced(pdesc);

    memset(rgch, ' ', sizeof(rgch));
    rgch[sizeof(rgch)-1] = 0;

    /*
     *  Move down a line and dump out in groups of 16 bytes indented by one level
     */

    iLevel += 1;
    pstm->pfnWrite(pstm, (PBYTE) "\n", 1);

    for (i=0, pch = rgch, pb = pos->data; i<pos->length; i++, pch += 3, pb++) {
        pch[0] = rgchHex[(*pb)>>4];
        pch[1] = rgchHex[(*pb) & 0xf];

        if (i % 16 == 15) {
            _A2C_Indent(pstm, iLevel);
            pstm->pfnWrite(pstm, (PBYTE) rgch, sizeof(rgch)-1);
            pstm->pfnWrite(pstm, (PBYTE) "\n", 1);

            pch = rgch-3;
        }
    }

    *pch = 0;
    _A2C_Indent(pstm, iLevel);
    pstm->pfnWrite(pstm, (PBYTE) rgch, (A2C_LENGTH) strlen(rgch));

    return 1;
}

int A2C_OCTET_STRING_compare(A2C_OCTET_STRING const * posLeft, A2C_OCTET_STRING const * posRight,
                             PC_A2C_DESCRIPTOR pdesc)
{
    int         cbLeft = posLeft->length;
    int         cbRight = posRight->length;
    int         i;

    /*
     *  We don't use this parameter
     */

    unreferenced(pdesc);

    /*
     *  Do the comparison
     */

    i = memcmp(posLeft->data, posRight->data, MIN(cbLeft, cbRight));
    if (i != 0) return i;

    return cbLeft - cbRight;
}

A2C_ERROR A2C_OCTET_STRING_copy(A2C_OCTET_STRING * posLeft, A2C_OCTET_STRING const * posRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */

    unreferenced(pdesc);

    /*
     *  Allocate the memory
     */

    posLeft->data = malloc(posRight->length);
    if (posLeft->data == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    /*
     *  Copy over the data
     */

    posLeft->length = posRight->length;
    memcpy(posLeft->data, posRight->data, posRight->length);

    /*
     *  Return error code
     */

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// If they have a length, they must have a buffer.
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_OCTET_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    A2C_OCTET_STRING *  pos = (A2C_OCTET_STRING *) pv;

    unreferenced(pdesc);

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
     *  Check that there is a buffer
     */
    
    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if ((pos->length > 0) && (pos->data == NULL)) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            return A2C_Validate_Print(pdesc, pcxt, "OCTET STRING", "Must have data if length > 0", pstm);
        }
    }

    /*
     *  No validate is done - return success
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}

/***************************** OPEN OCTET STRING ********************************/

A2C_CONST A2C_ITEM_HEADER A2C_OPEN_OCTET_STRING_description = {
    sizeof(A2C_OPEN_OCTET_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_OPEN_OCTET_STRING_descriptor = {
    A2C_zero,
    A2C_OPEN_OCTET_STRING_release,
    (A2C_decoder_f) A2C_OPEN_OCTET_STRING_decode_der,
    (A2C_encoder_f) A2C_OPEN_OCTET_STRING_encode_der,
    (A2C_decoder_f) A2C_OPEN_OCTET_STRING_decode_ber,
    (A2C_encoder_f) A2C_OPEN_OCTET_STRING_encode_ber,
    (A2C_compare_f) A2C_OPEN_OCTET_STRING_compare,
    (A2C_copy_f) A2C_OPEN_OCTET_STRING_copy,
    (A2C_print_f) A2C_OPEN_OCTET_STRING_print,
    A2C_OPEN_OCTET_STRING_validate,
    &A2C_OPEN_OCTET_STRING_description
};

/* ---
/// <summary>
/// release any data allocated
/// </summary>
/// <param name="pv">pointer to the open type value</param>
/// <param name="pdesc">pointr to the descriptor table</param>
/// <returns>A2C_ERROR_Sucess</returns>
--- */

A2C_ERROR A2C_OPEN_OCTET_STRING_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_OPEN_OCTET_STRING * poos = (A2C_OPEN_OCTET_STRING *) pv;
    /*
     *  Pass on the call - we are using that data structure
     */
    
    return A2C_OPEN_TYPE_release(poos, pdesc);
}

/*
 *  This function exists, but it should never be called by anyone
 */

A2C_ERROR A2C_OPEN_OCTET_STRING_decode_der(A2C_OPEN_OCTET_STRING * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                           A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(poos);
    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    unreferenced(ptag);
    unreferenced(pstm);

    /*
     *  Error if this function is called
     */

    return A2C_ERROR_ICE;
}

A2C_ERROR A2C_OPEN_OCTET_STRING_encode_der(A2C_OPEN_OCTET_STRING const * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                                 cb;
    int                                 cbOut;
    A2C_ERROR                           err;
    A2C_OCTET_STRING                    os = {0};
    PA2C_STREAM                         pstm2 = NULL;

    unreferenced(pdesc);

    /*
     *  If we have the encoded bytes already - then we can just wrap it into a blob and encode the blob
     */

    if (poos->objectPtr == NULL) {
        /*
         *  Setup as an OCTET STRING and encode it
         */


        os.length = poos->length;
        os.data = poos->data;

        return A2C_OCTET_STRING_encode_ber(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
    }

    /*
     *  The data still needs to be encoded.
     *
     *  1. Create a new stream to encode into 
     *  2. Get the data from the stream
     *  3. Emit the data 
     */

    err = A2C_CreateMemoryStream(&pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = poos->functionPtr->pfnBerEncoder(poos->objectPtr, poos->functionPtr, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = A2C_GetDataFromStream(pstm2, &os.data, &os.length);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  Write out the Tag and length
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 4, pstm, &cbOut);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cbOut);
    }
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = _A2C_write_length(os.length, pstm, &cb);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  Write out the bytes
     */

    cb = _A2C_write_bytes(os.data, os.length, pstm);
    if (cb < 0) {
        err = cb;
        goto ErrorExit;
    }

ErrorExit:
    A2C_FreeStream(pstm2);
    free(os.data);

    /*
     *  Free any data that is left
     */
    
    return err;
}


/*
 *  This function exists, but it should never be called by anyone
 */

A2C_ERROR A2C_OPEN_OCTET_STRING_decode_ber(A2C_OPEN_OCTET_STRING * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                           A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(poos);
    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    unreferenced(ptag);
    unreferenced(pstm);

    /*
     *  Error if this function is called
     */

    return A2C_ERROR_ICE;
}

A2C_ERROR A2C_OPEN_OCTET_STRING_encode_ber(A2C_OPEN_OCTET_STRING const * poos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                                 cb;
    int                                 cbOut;
    A2C_ERROR                           err;
    A2C_OCTET_STRING                    os = {0};
    PA2C_STREAM                         pstm2 = NULL;

    unreferenced(pdesc);

    /*
     *  If we have the encoded bytes already - then we can just wrap it into a blob and encode the blob
     */

    if (poos->objectPtr == NULL) {
        /*
         *  Setup as an OCTET STRING and encode it
         */


        os.length = poos->length;
        os.data = poos->data;

        return A2C_OCTET_STRING_encode_ber(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
    }

    /*
     *  The data still needs to be encoded.
     *  M00BUG - We always DER encode the contents - we could actually make this stream more effectively
     *  by doing the following:
     *  a) Create a new stream object which will just write onto the old stream object after doing an OCTET wrap
     *  b) For the inner object - do the ber encode writting on the new stream object
     *  c) Make the new stream object do a buffering of up to 1K or so.
     */

    err = A2C_CreateMemoryStream(&pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = poos->functionPtr->pfnBerEncoder(poos->objectPtr, poos->functionPtr, flags, pcxt, NULL, pstm2);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = A2C_GetDataFromStream(pstm2, &os.data, &os.length);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  Write out the Tag and length
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 4, pstm, &cbOut);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cbOut);
    }
    if (err < A2C_ERROR_Success) goto ErrorExit;

    err = _A2C_write_length(os.length, pstm, &cb);
    if (err < A2C_ERROR_Success) goto ErrorExit;

    /*
     *  Write out the bytes
     */


    cb = _A2C_write_bytes(os.data, os.length, pstm);
    if (cb < 0) {
        err = cb;
        goto ErrorExit;
    }

ErrorExit:
    
    /*
     *  Free any data that is left
     */

    A2C_FreeStream(pstm2);
    free(os.data);
    
    return err;
}

/* ---
/// <summary>
/// A2C_OPEN_OCTET_STRING_print is the default open type value print routine.
/// <para>If the type is fully defined, then it is recursively called in order to
/// do the print out.  If the system only has the bytes then they are dumped.</para>
/// </summary>
/// <param name="pf">pointer to the open type value</param>
/// <param name="pdesc">type descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>A2C_ERROR_Success on success, A2C_ERROR on failure.</return>
--- */


A2C_ERROR A2C_OPEN_OCTET_STRING_print(A2C_OPEN_OCTET_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;
    A2C_OPEN_OCTET_STRING const * pot = (A2C_OPEN_OCTET_STRING const *) pv;

    unreferenced(pdesc);
    
    /*
     *  If we have information on the structure, use it
     */

    if (pot->objectPtr != NULL) {
        return pot->functionPtr->pfnPrint(pot->objectPtr, pot->functionPtr, iLevel, pstm);
    }

    /*
     *  Use the OCTET STRING dumpper
     */

    os.length = pot->length;
    os.data = pot->data;

    return A2C_OCTET_STRING_print(&os, NULL, iLevel, pstm);
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

int A2C_OPEN_OCTET_STRING_compare(A2C_OPEN_OCTET_STRING const * pooLeft, A2C_OPEN_OCTET_STRING const * pooRight, PC_A2C_DESCRIPTOR pdesc)
{
    int  i;

    unreferenced(pdesc);
    
    /*  Are they both encoded data? */
    /*  If so do the standard comparision */

    if (pooLeft->data != NULL) {
        if (pooRight->data == NULL) return -1;

        i = pooRight->length - pooLeft->length;
        if (i != 0) return i;

        if (i > 0) {
            i = memcmp(pooRight->data, pooLeft->data, pooLeft->length);
            if (i == 0) {
                i = 1;
            }
            return i;
        }
        
        i = memcmp(pooRight->data, pooLeft->data, pooRight->length);
        if (i == 0) {
            i = -1;
        }
        return i;
    }

    /*  Both are decoded data */
    /*  Use the encoded function to do the comparison */

    if (pooLeft->functionPtr != pooRight->functionPtr) return -1;

    return pooLeft->functionPtr->pfnCompare(pooLeft->objectPtr, pooRight->objectPtr, pooLeft->functionPtr);
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

A2C_ERROR A2C_OPEN_OCTET_STRING_copy(A2C_OPEN_OCTET_STRING * poosLeft, A2C_OPEN_OCTET_STRING const * poosRight, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ERROR   err;

    unreferenced(pdesc);
    
    /*  If source is encoded, copy data in encoded format */

    if (poosRight->data != NULL) {
        poosLeft->data = malloc(poosRight->length);
        if (poosLeft->data == NULL) return A2C_ERROR_outOfMemory;

        memcpy(poosLeft->data, poosRight->data, poosRight->length);
        poosLeft->length = poosRight->length;

        return A2C_ERROR_Success;
    }

    A2C_Alloc(&poosLeft->objectPtr, poosRight->functionPtr);
    if (poosLeft->objectPtr == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    err = poosRight->functionPtr->pfnCopy(poosLeft->objectPtr, poosRight->objectPtr, poosRight->functionPtr);
    if (err >= A2C_ERROR_Success) {
        poosLeft->functionPtr = poosRight->functionPtr;
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

A2C_ERROR A2C_OPEN_OCTET_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    unreferenced(pv);
    unreferenced(pdesc);
    unreferenced(pcxt);
    unreferenced(pstm);
    
    *pf = TRUE;
    return A2C_ERROR_Success;
}


/*
/// <summary>
/// This is the decoder function for bytes contained within an OCTET STRING
/// </summary>
*/

A2C_ERROR A2C_OPEN_OCTET_STRING_RELATION_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                              A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR * pfn = A2C_LookupType(pInfo, pcxt);
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    int                 err;
    int                 fConstructed;
    int                 iClass;
    int                 iContext;
    int                 iRoot = pcxt->iRoot;
    A2C_TAG_VALUE       iValue;
    A2C_STREAM_MEMORY * pstm2 = NULL;
    A2C_STREAM_MEMORY   stm = {{0}};

    unreferenced(pdesc);

    /*
     *  If we don't know what we are looking at, then we can just pass it off
     *  to the normal OCTET STRING processing
     */

    if (pfn == NULL) {
        A2C_ERROR               err;
        A2C_OCTET_STRING        os = {0};

        err = A2C_OCTET_STRING_decode_der(&os, &A2C_OCTET_STRING_descriptor, flags,
                                          pcxt, ptag, pstm);

        if (err == A2C_ERROR_Success) {
            /*
             *  Copy the answer from the os structure to the open type structure
             */

            pos->length = os.length;
            pos->data = os.data;
        }

        /*
         *  Finished processing - return the error
         */

        return err;
    }

    /*
     *  Are doing a restart or initial call?
     */

    if (flags & A2C_FLAGS_MORE_DATA) {
        /*
         *  It's a restart
         *
         *  Start by figuring out where we are
         */
        
        A2C_CXT_ITEM *          pcxtitm;

        iContext = A2C_ContextFrame(pcxt);
        pcxtitm = &pcxt->rgCxts[iContext];

        /*
         *  We have already checked the tag and have this many bytes left to process
         */

        cbLength = pcxtitm->cbLeft;

        /*
         *  If we have clared the stack - clear the flag
         */

        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
    }
    else {
        /*
         *  Do the initial setup work
         *
         *  Start by checking the tag information
         *
         *  Grab the type and length of this item.  We don't advance the pointer
         */

        err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
        if (err < A2C_ERROR_Success) goto ErrorExit;

        /*
         *  Is the tag what we want it to be?
         */

        if (ptag != NULL) {
            if ((ptag->iClass != iClass) || (ptag->iValue != iValue)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }
        else {
            if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 16)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }

        /*
         *  We can't have an indefinite encoding for DER
         */

        if (fConstructed) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }

        /*
         *  Skip over the tag and length
         */

        _A2C_Memory_Skip(pstm, cbTL);

        /*
         *  Setup for doing a restart operation if necessary
         */

        err = A2C_PushContext(pcxt, &iContext);
        if (err < A2C_ERROR_Success) goto ErrorExit;

        /*
         *  We need to setup the object to do the decode operation in
         */

        A2C_Alloc(&pos->objectPtr, pfn);
        if (pos->objectPtr == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
        pos->functionPtr = pfn;
    }

    /*
     *  Limit the number of bytes that can be consume by the embedded object
     */

    if (cbLength == -1) {
        err = A2C_ERROR_ICE;
        goto ErrorExit;
    }

    _A2C_Memory_Init_With_Data(&stm, pstm->pb, MIN(cbLength, (pstm->pbLast - pstm->pb)));
    pstm2 = &stm;

    /*
     *  Check if we need to increase the frame array
     */

    if (pcxt->cRoots == pcxt->iRoot) {
        return A2C_ERROR_ICE;
    }
        
    pcxt->iRoot += 1;

    /*
     *  Setup the decode frames
     */

    pcxt->rgpbRoots[iRoot] = (BYTE *) pos->objectPtr;

    /*
     *  Start doing the decode
     */

    err = pos->functionPtr->pfnDerDecoder(pos->objectPtr, pfn, flags, pcxt, NULL, pstm2);

    if (err == A2C_ERROR_needMoreData) {
        /*
         *  We need more data
         */
        
        if ((cbLength - (pstm2->pb - pstm2->pbBase)) > 0) {
            /*
             *  Not all of the bytes have been comsumed yet
             */

            /*
             *  Add our data to the context stack
             *
             */

            pcxt->rgCxts[iContext].cbLeft = cbLength - (A2C_LENGTH) (pstm2->pb - pstm2->pbBase);

            /*
             *  Advance the stream by the correct # of bytes
             */

            if (pstm2 != pstm) {
                _A2C_Memory_Skip(pstm, pstm2->pb - pstm2->pbBase);
            }
        }
        else {
            err = A2C_ERROR_malformedEncoding;
        }

        goto ErrorExit;
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  If we did not use the entire blob - fail
     */

    if (cbLength != (stm.pb - stm.pbBase)) {
        err = A2C_ERROR_malformedDer;
        goto ErrorExit;
    }

    /*
     *  We have a success point - advance the buffer byt the correct # of bytes
     */

    _A2C_Memory_Skip(pstm, cbLength);

    /*
     *  Remove the restart setup
     */

    A2C_PopContext(pcxt, iContext);

    /*
     *  We have succeeded
     */

    err = A2C_ERROR_Success;
            
ErrorExit:

    pcxt->iRoot = iRoot;
    return err;
}

A2C_ERROR A2C_OPEN_OCTET_STRING_RELATION_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OCTET_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                                    A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR * pfn = A2C_LookupType(pInfo, pcxt);
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    int         err;
    int         fConstructed;
    int         iClass;
    A2C_TAG_VALUE iValue;
#ifdef UNUSED
    BYTE *      pb = NULL;
#endif
    
    unreferenced(pdesc);
    
    /*
     *  If we don't know what we are looking at, then we can just pass it off
     *  to the normal OCTET STRING processing
     */

    if (pfn == NULL) {
        A2C_ERROR               err;
        A2C_OCTET_STRING        os = {0};

        err = A2C_OCTET_STRING_decode_ber(&os, &A2C_OCTET_STRING_descriptor, flags,
                                          pcxt, ptag, pstm);

        if (err == A2C_ERROR_Success) {
            /*
             *  Copy the answer from the os structure to the open type structure
             */

            pos->length = os.length;
            pos->data = os.data;
        }

        /*
         *  Finished processing - return the error
         */

        return err;
    }

    /*
     *  Grab the type and length of this item.  This function will fail without
     *  advancing the pointer if there are not sufficient bytes in the buffer
     */

    err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
    if (err < 0) {
        goto ErrorExit;
    }
    
    /*
     *  Check that the tag matches what we expect it to be.
     */

    if (ptag != NULL) {
        if ((ptag->iClass != iClass) && (ptag->iValue != iValue)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }
    else {
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 4)) {
            err = A2C_ERROR_tagMismatch;
        }
    }

    /*
     *  If this is a constructed value, then fail we don't have the
     *  corect code to deal with that in the DER world.
     */

    if (fConstructed || (cbLength == -1)) {
        err = A2C_ERROR_malformedDer;
        goto ErrorExit;
    }

    /*
     *  See if we have enough data
     */

    if (_A2C_Memory_Left(pstm) < cbLength + cbTL) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Skip over the tag and length
     */

    _A2C_Memory_Skip(((A2C_STREAM_MEMORY *) pstm), cbTL);

    /*  Allocate and init */

    A2C_Alloc(&pos->objectPtr, pfn);
    if (pos->objectPtr == NULL) {
        return A2C_ERROR_outOfMemory;
    }

#ifdef UNUSED
    /*
     *  Allocate a buffer to read the result into
     */

    pb = malloc(cbLength);
    if (pb == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
#endif

    /*
     *  Now call the decoder for this item
     */

    pos->functionPtr = pfn;
    err = pfn->pfnBerDecoder(pos->objectPtr, pfn, flags, pcxt, NULL, pstm);

#ifdef UNUSED
    free(pb);
#endif

    return err;

ErrorExit:
    return err;
}
