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

#define TRUE 1
#define FALSE 0

extern const A2C_TAG TagSequence;

#define SORT(a, b, c) qsort(a, b, sizeof(a[0]), c);

typedef struct {
    BYTE *      pb;
    A2C_LENGTH  cb;
} Element;

/*
 *  compare - comparison sort function
 */

int __cdecl compare(const void * a, const void * b)
{
    Element * pa = (Element *) a;
    Element * pb = (Element *) b;
    int r;

    /*
     * Compare as if both are the same length by zero extending the shorter one
     */

    r = memcmp(pa->pb, pb->pb, MIN(pa->cb, pb->cb));
    if (r != 0) return r;

    return pb->cb - pa->cb;
}



/*
 *  Utility routines
 */

A2C_ERROR A2C_SEQUENCE_decode_common(int fDER, A2C_SEQUENCE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                     A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{

#pragma message("M00BUG: SEQUENCE decode does not respect extensibility")

    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    int                 cFields = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int	                fConstructed;
    int                 iClass;
    int                 iContext;
    int                 iField;
    int                 iOptional = 1;
    int                 iRoot = pcxt->iRoot;
    A2C_TAG_VALUE       iValue;
    PBYTE               pb;
    A2C_STREAM_MEMORY * pstm2 = NULL;
    A2C_TAG const *     ptagLocal;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Are we doing a restart operation?
     */
    
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
         *  We need to walk the table starting at the begining so we get propertly
         *      into position to continue with the decode
         *
         *  Initially the only issue is the corect index for optionality.
         */

        for (iField = 0; iField < pcxtitm->iItem; iField++) {
            if ((rgFields[iField].flags & (A2C_FLAGS_OPTIONAL | A2C_FLAGS_DATA_PTR)) == A2C_FLAGS_OPTIONAL) {
                iOptional <<= 1;
            }
        }
            
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
            if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 16)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }

        iField = 0;

        /*
         *  Indefinite length encoding not allowed for DER
         */

        if (fDER) {
            if (cbLength == -1) {
                return A2C_ERROR_malformedDer;
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
     *  Limit the children decode to the bytes in the current object
     */

    if (cbLength == -1) {
        pstm2 = pstm;
    }
    else {
        _A2C_Memory_Init_With_Data(&stm, pstm->pb, MIN(cbLength, (pstm->pbLast - pstm->pb)));
        pstm2 = &stm;
    }

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

    pcxt->rgpbRoots[iRoot] = (BYTE *) pi;

    /*
     *  Start walking the fields to get the parsing done
     */

    for (; iField<cFields; iField++) {
        /*
         *  Get the data offset
         */

        pb = ((PBYTE) pi) + rgFields[iField].cbOffset;

        /*
         *  Deal with Data Pointer questions
         */

        if (rgFields[iField].flags & A2C_FLAGS_DATA_PTR) {
            PBYTE *     ppb = (PBYTE *) pb;
            
            /*
             *   This is a pointer to the object.  We need to allocate the space for the object
             *  and then save it.
             */

            if (*ppb == NULL) {
                /*
                 *  Allocate and zero the object.  We don't want to init it
                 *      becase we don't want to have defaults copyied over
                 */
                
                pb = calloc(1, rgFields[iField].pdu->pItemDescription->cbStruct);
                if (pb == NULL) {
                    err = A2C_ERROR_outOfMemory;
                    goto ErrorExit;
                }

                *ppb = pb;
            }
            else {
                pb = *ppb;
            }
            
        }

        /*
         *  Check if we have an implicit tag that is to be used in the decode. 
         *      If so then make sure that we use it and not the default tag value.
         */

        if (rgFields[iField].flags & A2C_FLAGS_TAG_IMPLICIT) {
            ptagLocal = &rgFields[iField].tag;
        }
        else if (rgFields[iField].flags & A2C_FLAGS_TAG_EXPLICIT) {
            err = A2C_TAG_EXPLICIT_Decode(fDER, pb, &rgFields[iField], flags, pcxt, NULL, pstm2);
            goto ErrorChecks;
        }
        else {
            ptagLocal = NULL;
        }

        /*
         *  Perform the actual decode
         */

        if (fDER) {
            err = rgFields[iField].pdu->pfnDerDecoder(pb, rgFields[iField].pdu, flags, pcxt, ptagLocal, pstm2);
        }
        else {
            err = rgFields[iField].pdu->pfnBerDecoder(pb, rgFields[iField].pdu, flags, pcxt, ptagLocal, pstm2);
        }

        /*
         *  Did we have a need more data failure?  Do we want to restart?
         */

ErrorChecks:
        if ((err == A2C_ERROR_needMoreData) &&
            ((cbLength == -1) || ((cbLength - (pstm2->pb - pstm2->pbBase)) > 0))) {
            /*
             *  We need more data and either
             *          1.  We are doing indefinite length encoded (so not at the end) or
             *          2.  There are some data bytes left in the buffer
             *
             *  If we need more data, but there are no buffers left in the buffer - then the field does
             *  not exist.
             *
             */

            /*
             *  Add our data to the context stack
             *
             *  Which item we are processing
             *
             *  Count of bytes left = 
             *     # bytes in this item - # bytes consumed so far
             */

            pcxt->rgCxts[iContext].iItem = iField;
            if (cbLength == -1) {
                pcxt->rgCxts[iContext].cbLeft = cbLength;
            }
            else {
                pcxt->rgCxts[iContext].cbLeft = cbLength - (A2C_LENGTH) (pstm2->pb - pstm2->pbBase);
            }

            /*
             *  Advance called in buffer by correct # of bytes
             */

            if (pstm2 != pstm) {
                _A2C_Memory_Skip(pstm, pstm2->pb - pstm2->pbBase);
            }

            goto ErrorExit;
        }

        /*
         *  Two different actions based on if the field was optional.
         *
         *  If the field was optional and
         *      decode succeeded - then set the present bit and increment the optional counter
         *      decode failed - just increment the optional counter
         *              Note:  this is not an operational failure
         *
         *  If the field was not optional and the decode failed -- fail the operation
         */
        
        else if (rgFields[iField].flags & A2C_FLAGS_OPTIONAL) {
            if (err >= 0) {
                if ((rgFields[iField].flags & A2C_FLAGS_DATA_PTR) == 0) {
                    pi->optionals |= iOptional;
                    iOptional <<= 1;
                }
            }
            else if ((err == A2C_ERROR_tagMismatch) || (err == A2C_ERROR_needMoreData)) {
                if ((rgFields[iField].flags & A2C_FLAGS_DATA_PTR) == 0) {
                    iOptional <<= 1;
                }
                else {
                    PBYTE * ppb = (PBYTE *) (((PBYTE) pi) + rgFields[iField].cbOffset);

                    rgFields[iField].pdu->pfnRelease(pb, rgFields[iField].pdu);
                    free(pb);

                    *ppb = NULL;
                }
            }
            else {
                goto ErrorExit;
            }
        }

        /*
         *  This field is marked for default.
         *
         *  If decode succeeded - then nothiing to be done
         *  If decode failed - copy over the default value.
         */
        
        else if (rgFields[iField].flags & A2C_FLAGS_DEFAULT) {
            
            if (err >= A2C_ERROR_Success) {
                /*  It parsed - so ignore this */
            }
            else if ((err == A2C_ERROR_tagMismatch) || (err == A2C_ERROR_needMoreData)) {
                /*  M00QUEST - has any parital data been released? */
                
                /*  Copy over the value */

                err = rgFields[iField].pdu->pfnCopy(pb, &rgFields[iField].pvDefault, rgFields[iField].pdu);
                if (err < 0) {
                    goto ErrorExit;
                }
            }
            else {
                goto ErrorExit;
            }
        }
        else if (rgFields[iField].flags & A2C_FLAGS_DEFAULT_PTR) {
            if (err >= A2C_ERROR_Success) {
                /*  It parsed ot ignore this */
            }
            else if ((err == A2C_ERROR_tagMismatch) || (err = A2C_ERROR_needMoreData)) {
                /* M00QEUST - see above note */
                /* Copy over the value */
                /* M00QUEST - What if it really is parsing and needs more data? */

                err = rgFields[iField].pdu->pfnCopy(pb, rgFields[iField].pvDefault, rgFields[iField].pdu);
                if (err < 0) {
                    goto ErrorExit;
                }
            }
            else {
                goto ErrorExit;
            }
        }
        else if (err < A2C_ERROR_Success) {
            if (err == A2C_ERROR_needMoreData) err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        /*
         *  Clear recursion flag
         */

        flags &= ~A2C_FLAGS_MORE_DATA;
    }

    /*
     *  If we did not use the entire blob - fail
     */

    if (cbLength != -1) {
        if (cbLength != (stm.pb - stm.pbBase)) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }
    }

    /*
     *  Advance called in buffer by correct # of bytes
     */

    if (cbLength != -1) {
        _A2C_Memory_Skip(pstm, cbLength);
    }

    /*
     *  Look for the end of octets if needed
     */

    if (cbLength == -1) {
        err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

        /*
         *  Should be tag=0, length=0
         */

        if ((iClass != 0) && (iValue != 0) && (cbLength != 0)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }

        /*
         *  Skip tag and length
         */

        _A2C_Memory_Skip(pstm, cbTL);
    }
        

    /*
     *  Remove the restart setup
     */

    A2C_PopContext(pcxt, iContext);

    /*
     *  Return actual number of bytes consumed
     */

    err = A2C_ERROR_Success;

    /*
     *  Do the clean up and return
     */
    
ErrorExit:
    pcxt->iRoot = iRoot;
    return err;
}


A2C_ERROR _A2C_SEQ_SET_encode(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags,
                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm,
                    int fSet, int fDER)
{
    int                 cb;
    int                 cbTag = 0;
    int                 cbSet = 0;
    int                 cItems = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 i;
    int		        iItem;
    int                 iOptional = 1;
    BYTE *	        pbData;
    BYTE **             ppb;
    A2C_TAG const *     ptagChild;
    int                 rgfOptionals;
    const A2C_ITEM *    rgItems = pdesc->pItemDescription->rgChildren;
    A2C_STREAM_MEMORY   stm;
    Element *	        rgdata = NULL;

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 16 & constucted
     */

    err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cbTag);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    if (fDER) {
        if (fSet) {
            /*
             * Allocate results array
             */

            rgdata = (Element *) calloc(cItems, sizeof(Element));
            if (rgdata == NULL) {
                err = A2C_ERROR_outOfMemory;
                goto ErrorExit;
            }
        }
    }
    else {
        /*
         *  Write out an indefinite length
         */

        err = _A2C_write_length((size_t) -1, pstm, &cb);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }
    }

    /*
     *  Assume that optional exists and grab it.
     */

    rgfOptionals = *((int *) pvData);

    /*
     *  Initialize the stream object we are using
     */
    
    _A2C_Memory_Init(&stm);

    /*
     *  Now start walking through all of the items in the array
     */

    iOptional = 1;
    for (iItem = 0; iItem<cItems; iItem++) {

        /*
         *  Where is the data item?
         */

        pbData = ((PBYTE) pvData) + rgItems[iItem].cbOffset;
            
        /*
         *  Is the item optional and missing?  If so skip forward
         */

        if (rgItems[iItem].flags & A2C_FLAGS_OPTIONAL) {
            if (rgItems[iItem].flags & A2C_FLAGS_DATA_PTR) {
                ppb = (PBYTE *) pbData;

                if (*ppb == NULL) {
                    continue;
                }
            }
            else {
                if ((rgfOptionals & iOptional) == 0) {
                    iOptional <<= 1;
                    continue;
                }
                iOptional <<= 1;
            }
        }

        /*
         *  Is the data acutally a pointer?
         */

        if (rgItems[iItem].flags & A2C_FLAGS_DATA_PTR) {
            pbData = *((PBYTE *) pbData);
        }

        /*
         *  Deal with defaulting
         *
         *  If we default then we don't need to emit anything here
         */

        if (rgItems[iItem].flags & A2C_FLAGS_DEFAULT) {
            i = rgItems[iItem].pdu->pfnCompare(pbData, &rgItems[iItem].pvDefault, rgItems[iItem].pdu);
            if (i == 0) {
                continue;
            }
        }

        /*
         *  Deal with tagging
         */

        if (rgItems[iItem].flags & A2C_FLAGS_TAG_EXPLICIT) {
            if (fDER) {
                err = A2C_TAG_Internal_EncodeDer(pbData, &rgItems[iItem], flags,
                                                 pcxt, NULL, (A2C_STREAM *) &stm);
            }
            else {
                err = A2C_TAG_Internal_EncodeBer(pbData, &rgItems[iItem], flags, pcxt, NULL, pstm);
            }
            goto ErrorChecks;
        }
        else if (rgItems[iItem].flags & A2C_FLAGS_TAG_IMPLICIT) {
            ptagChild = &rgItems[iItem].tag;
        }
        else {
            ptagChild = NULL;
        }

        /*
         *  Call the encoder for this item
         */

        if (fDER) {
            err = rgItems[iItem].pdu->pfnDerEncoder(pbData, rgItems[iItem].pdu, flags, pcxt,
                                                    ptagChild, (A2C_STREAM *) &stm);
        }
        else {
            err = rgItems[iItem].pdu->pfnBerEncoder(pbData, rgItems[iItem].pdu, flags, pcxt,
                                                    ptagChild, pstm);
        }

        /*
         *  Do the error processing
         */

    ErrorChecks:
        if (err < 0) {
            goto ErrorExit;
        }

        if (fDER && fSet) {
            /*
             *  Grab the encoded bytes - this empties stm
             */

            _A2C_Memory_GetData(&stm, &rgdata[iItem].pb, &rgdata[iItem].cb);

            /*
             *  Running total # of bytes
             */
        
            cbSet += rgdata[iItem].cb;
        }
    }

    if (fDER) {
        if (fSet) {
            /*
             *   Now we have the size - emit it
             */

            err = _A2C_write_length(cbSet, pstm, &cb);
            if (err < A2C_ERROR_Success) {
                goto ErrorExit;
            }

            /*
             *  Sort the arrays of data
             */

            SORT(rgdata, cItems, compare);

            /*
             *  Now copy the data out
             */
    
            for (iItem = 0; iItem < cItems; iItem++) {
                err = pstm->pfnWrite(pstm, rgdata[iItem].pb, rgdata[iItem].cb);
                if (err < 0) {
                    goto ErrorExit;
                }
            }

        }
        else {
            int     cbSeq;
        
            /*
             *  Get the number of bytes in the streams
             */
        
            cbSeq = _A2C_Memory_Length(&stm);

            /*
             *  Copy out the length of the data
             */

            err = _A2C_write_length(cbSeq, pstm, &cb);
            if (err < 0) {
                goto ErrorExit;
            }

            /*
             *  Copy out the data to the stream
             */
    
            err = pstm->pfnWrite(pstm, stm.pbBase, cbSeq);
            if (err < A2C_ERROR_Success) {
                goto ErrorExit;
            }
        }
    }
    else {
        /*
         *  Write out an end-of-contents sequence
         */

        err = _A2C_write_tag(0, 0, 0, pstm, &cbTag);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }
    
        err = _A2C_write_length(0, pstm, &cb);
    }

    /*
     *  Now clean up things
     */

ErrorExit:
    if (rgdata != NULL) {
        for (iItem=0; iItem<cItems; iItem++) {
            free(rgdata[iItem].pb);
        }
        free(rgdata);
    }

    _A2C_Memory_Cleanup(&stm);

    return err;
}

/********************************************************************/

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SEQUENCE_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cField = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 iField;
    PBYTE               pb;
    PBYTE               pb2;
    PBYTE *             ppb;
    A2C_ITEM const *    rgField = pdesc->pItemDescription->rgChildren;
    A2C_SEQUENCE *      pseq = (A2C_SEQUENCE *) pv;

    for (iField = 0; iField<cField; iField++) {
        pb = rgField[iField].cbOffset + (PBYTE) pseq;

        /*
         *  If this is a pointer - then allocate the item and
         *      initialize it
         */

        if (rgField[iField].flags & A2C_FLAGS_DATA_PTR) {
            pb2 = NULL;
            err = A2C_Alloc((PVOID) &pb2, rgField[iField].pdu);
            if (err < A2C_ERROR_Success) {
                A2C_Free(pb2, rgField[iField].pdu);
                return err;
            }

            if (rgField[iField].flags & A2C_FLAGS_DEFAULT) {
                err = rgField[iField].pdu->pfnCopy(pb2, &rgField[iField].pvDefault, rgField[iField].pdu);
            }
            else if (rgField[iField].flags & A2C_FLAGS_DEFAULT_PTR) {
                err = rgField[iField].pdu->pfnCopy(pb2, rgField[iField].pvDefault, rgField[iField].pdu);
            }
            else {
                err = rgField[iField].pdu->pfnInit(pb2, rgField[iField].pdu);
            }
                
            if (err < A2C_ERROR_Success) {
                A2C_Free(pb2, rgField[iField].pdu);
                return err;
            }

            ppb = (PBYTE *) pb;
            *ppb = pb2;
            pb2 = NULL;
        }
        else {
            if (rgField[iField].flags & A2C_FLAGS_DEFAULT) {
                err = rgField[iField].pdu->pfnCopy(pb, &rgField[iField].pvDefault, rgField[iField].pdu);
            }
            else if (rgField[iField].flags & A2C_FLAGS_DEFAULT_PTR) {
                err = rgField[iField].pdu->pfnCopy(pb, rgField[iField].pvDefault, rgField[iField].pdu);
            }
            else {
                err = rgField[iField].pdu->pfnInit(pb, rgField[iField].pdu);
            }
            if (err < A2C_ERROR_Success) {
                return err;
            }
        }
    }
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SEQUENCE_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cField = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 iField;
    PBYTE               pb;
    PBYTE *             ppb;
    A2C_ITEM const *    rgField = pdesc->pItemDescription->rgChildren;
    A2C_SEQUENCE *      pseq = (A2C_SEQUENCE *) pv;

    for (iField = 0; iField<cField; iField++) {
        pb = rgField[iField].cbOffset + (PBYTE) pseq;

        if (rgField[iField].flags & A2C_FLAGS_DATA_PTR) {
            ppb = (PBYTE *) pb;
            err = A2C_Free(*ppb, rgField[iField].pdu);
            *ppb = NULL;
        }
        else {
            err = rgField[iField].pdu->pfnRelease(pb, rgField[iField].pdu);
        }

        /*  Error return code is ignored */
    }
    return A2C_ERROR_Success;
}

A2C_ERROR A2C_SEQUENCE_decode_der(A2C_SEQUENCE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_SEQUENCE_decode_common(TRUE, pi, pdesc, flags, pcxt, ptag, pstm);
}

/*
 *  Write out a DER encoded integer
 */

int A2C_SEQUENCE_encode_der(A2C_SEQUENCE const * data, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    if (ptag == NULL) {
        ptag = &TagSequence;
    }

    return _A2C_SEQ_SET_encode(data, pdesc, flags, pcxt, ptag, pstm, FALSE, TRUE);
#if 0
    int                 cb;
    int                 cbSeq = 0;
    int                 cbTag = 0;
    int                 cItems = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 i;
    int		        iItem;
    int                 iOptional = 1;
    BYTE *	        pbData;
    PVOID *             ppv;
    A2C_TAG const *     ptagChild;
    int                 rgfOptionals;
    const A2C_ITEM *    rgItems = pdesc->pItemDescription->rgChildren;
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 16 & constucted
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 1, 16, pstm, &cbTag);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cbTag);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Assume that optional exists and grab it.
     */

    rgfOptionals = data->optionals;

    /*
     *  Initialize all of the stream objects
     */
    
    _A2C_Memory_Init(&stm);

    /*
     *  Now start walking through all of the items in the array
     */

    iOptional = 1;
    for (iItem = 0; iItem<cItems; iItem++) {

        /*
         *  Where is the data item?
         */

        pbData = ((PBYTE) data) + rgItems[iItem].cbOffset;
            
        /*
         *  Is the item optional and missing?  If so skip forward
         */

        if (rgItems[iItem].flags & A2C_FLAGS_OPTIONAL) {

            if (rgItems[iItem].flags & A2C_FLAGS_DATA_PTR) {
                ppv = (PVOID *) pbData;
                
                if (*ppv == NULL) {
                    continue;
                }
            }
            else {
                if ((rgfOptionals & iOptional) == 0) {
                    iOptional <<= 1;
                    continue;
                }
                iOptional <<= 1;
            }
        }

        /*
         *  Pointer to data rather than data?
         */

        if (rgItems[iItem].flags & A2C_FLAGS_DATA_PTR) {
            pbData = *((PBYTE *)pbData);
        }

        /*
         *  Deal with defaulting
         *
         *  If we default then we don't need to emit anything here
         */

        if (rgItems[iItem].flags & A2C_FLAGS_DEFAULT) {
            i = rgItems[iItem].pdu->pfnCompare(pbData, &rgItems[iItem].pvDefault, rgItems[iItem].pdu);
            if (i == 0) {
                continue;
            }
        }

        /*
         *  Deal with tagging
         */

        if (rgItems[iItem].flags & A2C_FLAGS_TAG_EXPLICIT) {
            err = A2C_TAG_Internal_EncodeDer(pbData, &rgItems[iItem], flags, pcxt, NULL, (PA2C_STREAM) &stm);
            goto ErrorChecks;
        }
        else if (rgItems[iItem].flags & A2C_FLAGS_TAG_IMPLICIT) {
            ptagChild = &rgItems[iItem].tag;
        }
        else {
            ptagChild = NULL;
        }

        /*
         *  Call the encoder for this item
         */

        err = rgItems[iItem].pdu->pfnDerEncoder(pbData, rgItems[iItem].pdu, flags, pcxt,
                                                ptagChild, (PA2C_STREAM) &stm);

        /*
         *  Do the error processing
         */

    ErrorChecks:
        if (err < 0) {
            goto ErrorExit;
        }
    }

    /*
     *  Now clean up things
     */

ErrorExit:
    _A2C_Memory_Cleanup(&stm);

    return err;
#endif
}

A2C_ERROR A2C_SEQUENCE_decode_ber(A2C_SEQUENCE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_SEQUENCE_decode_common(FALSE, pi, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// </summary>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_SEQUENCE_encode_ber(A2C_SEQUENCE const * data, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    
    if (ptag == NULL) {
        ptag = &TagSequence;
    }
    
    return _A2C_SEQ_SET_encode(data, pdesc, flags, pcxt, ptag, pstm, FALSE, FALSE);
}

/*
 */

A2C_ERROR A2C_SEQUENCE_print(A2C_SEQUENCE const * pi, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    int                         cChildren = pdesc->pItemDescription->cChildren;
    A2C_ERROR                   err;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbData;
    A2C_ITEM const *            rgChildren = pdesc->pItemDescription->rgChildren;
    int                         rgfOptionals = pi->optionals;

    pstm->pfnWrite(pstm, (PBYTE) "{\n", 2);
    iLevel += 1;
    for (i=0; i<cChildren; i++) {
        if (rgChildren[i].flags & A2C_FLAGS_OPTIONAL) {
            if ((rgfOptionals & iOptional) == 0) {
                iOptional <<= 1;
                continue;
            }
            else {
                iOptional <<= 1;
            }
        }

        /*
         *  Offset of the data item
         */

        pbData = rgChildren[i].cbOffset + (PBYTE) pi;

        /*
         *  Its a pointer not a structure
         */

        if (rgChildren[i].flags & A2C_FLAGS_DATA_PTR) {
            pbData = *((PBYTE *) pbData);
        }

        /*
         *  Dump out the data
         */

        _A2C_Indent(pstm, iLevel);
        
        pstm->pfnWrite(pstm, (PBYTE) rgChildren[i].szFieldName, (A2C_LENGTH) strlen(rgChildren[i].szFieldName));
        pstm->pfnWrite(pstm, (PBYTE) " ", 1);
        err = rgChildren[i].pdu->pfnPrint(pbData, rgChildren[i].pdu, iLevel, pstm);
        pstm->pfnWrite(pstm, (PBYTE) "\n", 1);

        if (err < A2C_ERROR_Success) {
            return err;
        }
    }
    _A2C_Indent(pstm, iLevel-1);
    pstm->pfnWrite(pstm, (PBYTE) "}", 1);
    
    return A2C_ERROR_Success;
}

int A2C_SEQUENCE_compare(A2C_SEQUENCE const * pseqLeft, A2C_SEQUENCE const * pseqRight, PC_A2C_DESCRIPTOR pdesc)
{
    int                         cChildren = pdesc->pItemDescription->cChildren;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbLeft = (PBYTE) pseqLeft;
    PBYTE                       pbRight = (PBYTE) pseqRight;
    int                         ret;
    A2C_ITEM const *            rgChildren = pdesc->pItemDescription->rgChildren;
    int                         rgfOptionals = pseqLeft->optionals;

    for (i=0; i<cChildren; i++) {
        /*  Play with optionality of fields */
        
        if (rgChildren[i].flags & A2C_FLAGS_OPTIONAL) {
            /*  If the optional flags don't match - then they are not the same */
            
            if ((pseqLeft->optionals & iOptional) != (pseqRight->optionals & iOptional)) {
                return -1;
            }

            /*  If the optional element is absent - then skip to next element */
            
            if ((rgfOptionals & iOptional) == 0) {
                iOptional <<= 1;
                continue;
            }
            
            iOptional <<= 1;
        }

        /*
         *  Get the data addresses
         */

        pbLeft = rgChildren[i].cbOffset + (PBYTE) pseqLeft;
        pbRight = rgChildren[i].cbOffset + (PBYTE) pseqRight;

        if (rgChildren[i].flags & A2C_FLAGS_DATA_PTR) {
            pbLeft = *((PBYTE *) pbLeft);
            pbRight = *((PBYTE *) pbRight);
        }

        /*  Compare the elements */

        ret = rgChildren[i].pdu->pfnCompare(pbLeft, pbRight, rgChildren[i].pdu);

        /*  If a mis-match has been found - return that */
        
        if (ret != 0) return ret;
    }

    /*  Return comparision == match */
    
    return 0;
}

A2C_ERROR A2C_SEQUENCE_copy(A2C_SEQUENCE * pseqDst, A2C_SEQUENCE const * pseqSrc, PC_A2C_DESCRIPTOR pdesc)
{
    int                         cChildren = pdesc->pItemDescription->cChildren;
    A2C_ERROR                   err;
    int                         f;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbDst = (PBYTE) pseqDst;
    PBYTE                       pbSrc = (PBYTE) pseqSrc;
    A2C_ITEM const *            rgChildren = pdesc->pItemDescription->rgChildren;
    int                         rgfOptionals = pseqSrc->optionals;

    for (i=0; i<cChildren; i++) {
        f = 0;
        
        if (rgChildren[i].flags & A2C_FLAGS_OPTIONAL) {
            if ((rgfOptionals & iOptional) == 0) {
                iOptional <<= 1;
                continue;
            }
            else {
                f = iOptional;
                iOptional <<= 1;
            }
        }

        /*
         *  Compute data address
         */

        pbDst = rgChildren[i].cbOffset + (PBYTE) pseqDst;
        pbSrc = rgChildren[i].cbOffset + (PBYTE) pseqSrc;

        if (rgChildren[i].flags & A2C_FLAGS_DATA_PTR) {
            pbSrc = *((PBYTE *) pbSrc);
            err = A2C_Alloc((PVOID) &pbDst, rgChildren[i].pdu);
            if (err < A2C_ERROR_Success) {
                return err;
            }
            pbDst = *((PBYTE *) pbDst);
        }

        err = rgChildren[i].pdu->pfnCopy(pbDst, pbSrc, rgChildren[i].pdu);
            
        if (err < A2C_ERROR_Success) {
            if (rgChildren[i].flags & A2C_FLAGS_DATA_PTR) {
                PBYTE * ppb;
                A2C_Free(pbDst, rgChildren[i].pdu);
                ppb = (PBYTE *) (rgChildren[i].cbOffset + (PBYTE) pseqDst);
                *ppb = NULL;
            }
            return err;
        }

        pseqDst->optionals |= f;
    }
            
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Run the check on all present items.
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_SEQUENCE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    /*
     *  Pass it onto the set - same work to be done
     */

    return A2C_SET_validate(pv, pdesc, pcxt, pf, pstm);
}
