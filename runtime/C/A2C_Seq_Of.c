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

#include "A2C_Stream_Internal.h"

#define TRUE 1
#define FALSE 0

typedef struct {
    BYTE *      pb;
    int         cb;
} Element;

const A2C_TAG TagSequence = { A2C_TAG_CLASS_UNIVERSAL, 16 };

/*
 *  Utilities
 */

A2C_ERROR A2C_SEQUENCE_OF_decode_common(int fDER, A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                        A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm)
{
    int                 c = 0;
    A2C_LENGTH          cbLength;
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    A2C_LENGTH          cbTL;
    int                 err = A2C_ERROR_Success;
    int                 fConstructed;
    int                 i;
    int                 iClass;
    int                 iContext;
    int                 iRoot = pcxt->iRoot;
    A2C_TAG_VALUE       iValue;
    A2C_ITEM const *    pItem = pdesc->pItemDescription->rgChildren;
    A2C_STREAM_MEMORY * pstm2 = NULL;
    A2C_TAG const *     ptagLocal;
    PBYTE               rg = NULL;
    A2C_STREAM_MEMORY   stm = {{0}};
    A2C_CXT_ITEM *      pcxtitm = NULL;

    /*
     *  Are we doing a restart operation?
     */

    if (flags & A2C_FLAGS_MORE_DATA) {

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
         *  Which array element are we on?
         */

        i = pcxtitm->iItem;
        rg = pcxtitm->pbData;
        c = pcxtitm->cbData;

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
            err = A2C_ERROR_ICE;
            goto ErrorExit;
        }

        /*
         *  Don't do indefinite if this is DER
         */

        if ((cbLength == -1) && fDER) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }

        i = 0;

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
     *  If we are tagged - everybody is tagged so determine that now.
     */

    if (pItem->flags & A2C_FLAGS_TAG_IMPLICIT) {
        ptagLocal = &pItem->tag;
    }
    else {
        ptagLocal = NULL;
    }

    /*
     *  Loop until we consume all of the bytes
     */

    for ( ; (cbLength == -1) || (cbLength - (pstm2->pb - pstm2->pbBase) > 0); i++) {
        /*
         *  If we are indefinite length - check for the end of bytes string
         */

        if (cbLength == -1) {
            A2C_LENGTH cbLen2;
            err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLen2, &cbTL);
            if (err < 0) {
                goto ErrorHandler;
            }

            /*
             *  If tag=0, length = 0 - done with sequence
             */
            
            if ((iClass == 0) && (iValue == 0) && (cbLen2 == 0)) {
                /*
                 *  Skip over the tag and length
                 */

                _A2C_Memory_Skip(pstm, cbTL);

                /*
                 *  And exit the loop
                 */
                
                break;
            }
        }
         
        /*
         *  Start by allocating a buffer big enough to hold 5 items
         *  We will re-allocate this array as needed until finished
         */

        if (i==c) {
            PBYTE pbTemp;

            pbTemp = realloc(rg, (c+5)*cbStruct);
            if (pbTemp == NULL) {
                free(rg);
                err = A2C_ERROR_outOfMemory;
                goto ErrorExit;
            }

            rg = pbTemp;
            memset(rg+c*cbStruct, 0, 5*cbStruct);

            c += 5;
        }

        /*
         *  M00BUG - need to deal with EXPLICIT & IMPLICITtags
         */
#pragma message("M00BUG - tagging on SEQUENCE OF [0] ...")

        /*
         *  Setup the decode frames
         */

        pcxt->rgpbRoots[iRoot] = rg+i*cbStruct;

        /*
         *  Call out to decode the object
         */

        if (fDER) {
            err = pItem->pdu->pfnDerDecoder(rg+i*cbStruct, pItem->pdu, flags, pcxt, ptagLocal, pstm2);
        }
        else {
            err = pItem->pdu->pfnBerDecoder(rg+i*cbStruct, pItem->pdu, flags, pcxt, ptagLocal, pstm2);
        }

        /*
         *  We may need to go on a restart here
         */

    ErrorHandler:

        if ((err == A2C_ERROR_needMoreData) &&
            ((cbLength == -1) || ((cbLength - (pstm2->pb - pstm2->pbBase)) > 0))) {
            /*
             *  Add our data to the context stack
             *
             *  Which item we are processing
             *
             *  Count of bytes left = 
             *     # bytes in this item - # bytes consumed so far
             */

            pcxt->rgCxts[iContext].iItem = i;
            if (cbLength == -1) {
                pcxt->rgCxts[iContext].cbLeft = cbLength;
            }
            else {
                pcxt->rgCxts[iContext].cbLeft = cbLength - (A2C_LENGTH) (pstm2->pb - pstm2->pbBase);
            }
            pcxt->rgCxts[iContext].pbData = rg;
            pcxt->rgCxts[iContext].cbData = c;

            rg = NULL;

            /*
             *  Advance called in buffer by correct # of bytes
             */

            if (pstm != pstm2) {
                _A2C_Memory_Skip(pstm, stm.pb - stm.pbBase);
            }

            goto ErrorExit;
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
     *  Advance called in buffer by correct # of bytes
     */

    if (pstm != pstm2) {
        _A2C_Memory_Skip(pstm, stm.pb - stm.pbBase);
    }

    /*
     *  save the data in the return structure.
     */

    pData->count = i;
    pData->countAllocated = i;
    pData->array = rg;
    pcxt->iRoot = iRoot;

    /*
     *  Remove the restart setup
     */


    if (pcxtitm != NULL) {
        pcxtitm->cbData = 0;
        pcxtitm->pbData = NULL;
    }
    
    A2C_PopContext(pcxt, iContext);


    return A2C_ERROR_Success;

ErrorExit:
    if (rg != NULL) free(rg);
    pcxt->iRoot = iRoot;
    return err;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SEQUENCE_OF_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_SEQUENCE_OF *   pData = (A2C_SEQUENCE_OF *) pv;

    unreferenced(pdesc);
    
    /*  Just zero it */

    pData->count = 0;
    pData->countAllocated = 0;
    pData->array = 0;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SEQUENCE_OF_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;    
    int                 iItem;
    PBYTE               pb;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;
    A2C_SEQUENCE_OF *   pData = (A2C_SEQUENCE_OF *) pv;
    
    pb = (PBYTE) pData->array;
    
    for (iItem=0; iItem<pData->count; iItem++, pb += cbStruct) {

        rgItems[0].pdu->pfnRelease(pb, rgItems[0].pdu);
    }

    free(pData->array);

    return A2C_ERROR_Success;
}
        

A2C_ERROR A2C_SEQUENCE_OF_decode_der(A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &TagSequence;
    }
    
    return A2C_SEQUENCE_OF_decode_common(TRUE, pData, pdesc, flags, pcxt, ptag, pstm);
}

/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_SEQUENCE_OF_encode_der(A2C_SEQUENCE_OF const * data, PC_A2C_DESCRIPTOR pdesc, int flags,
                                     A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int		        cb;
    int                 cbSeq = 0;
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    int                 cbTag = 0;
    A2C_ERROR           err = A2C_ERROR_Success;
    int		        iItem;
    BYTE *	        pbData;
    A2C_STREAM_MEMORY   stm;
    A2C_TAG const *     ptagLocal;
    Element *           rgdata = NULL;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;;

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
     *  Do we have an override tag?
     */
#pragma message("M00QUEST: Explict Tags?")

    if (rgItems[0].flags & A2C_FLAGS_TAG_IMPLICIT) {
        ptagLocal = &rgItems[0].tag;
    }
    else {
        ptagLocal = NULL;
    }
        
    /*
     *  Now start walking through all of the items in the array
     */

    pbData = (PBYTE) data->array;

    /*
     *  Initialize the output stream
     */
    
    _A2C_Memory_Init(&stm);
        
    for (iItem = 0; iItem<data->count; iItem++, pbData += cbStruct) {

        /*
         *  Call the encoder for this item
         */

        err = rgItems[0].pdu->pfnDerEncoder(pbData, rgItems[0].pdu, flags, pcxt, ptagLocal, (PA2C_STREAM) &stm);
        if (err < 0) {
            goto ErrorExit;
        }
    }

    /*
     *  Get the final length
     */

    cbSeq = _A2C_Memory_Length(&stm);

    /*
     *  Now we know the final size - emit that
     */

    err = _A2C_write_length(cbSeq, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }
    
    /*
     *  Now copy the data out
     */

    err = pstm->pfnWrite(pstm, stm.pbBase, cbSeq);
    
    /*
     *  Return count of bytes encoded
     */

ErrorExit:
    _A2C_Memory_Cleanup(&stm);
    if (rgdata != NULL) {
        for (iItem = 0; iItem<data->count; iItem++) {
            free(rgdata[iItem].pb);
        }
        free(rgdata);
    }
            
    return err;
}

A2C_ERROR A2C_SEQUENCE_OF_decode_ber(A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &TagSequence;
    }

    return A2C_SEQUENCE_OF_decode_common(FALSE, pData, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_SEQUENCE_OF_encode_ber(A2C_SEQUENCE_OF const * data, PC_A2C_DESCRIPTOR pdesc, int flags,
                                     A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int		        cb;
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    int                 cbTag = 0;
    A2C_ERROR           err = A2C_ERROR_Success;
    int		        iItem;
    BYTE *	        pbData;
    A2C_TAG const *     ptagLocal;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;;

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
        return err;
    }

    /*
     *  Write out the length
     */

    err = _A2C_write_length((size_t) -1, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Do we have an override tag?
     */

    if (rgItems[0].flags & A2C_FLAGS_TAG_IMPLICIT) {
        ptagLocal = &rgItems[0].tag;
    }
    else {
        ptagLocal = NULL;
    }
        
    /*
     *  Now start walking through all of the items in the array
     */

    pbData = (BYTE *) data->array;
        
    for (iItem = 0; iItem<data->count; iItem++, pbData += cbStruct) {
        /*
         *  Call the encoder for this item
         */

        err = rgItems[0].pdu->pfnBerEncoder(pbData, rgItems[0].pdu, flags, pcxt, ptagLocal, pstm);
        if (err < 0) {
            goto ErrorExit;
        }
    }

    /*
     *  Write out the end of bytes TL
     */

    err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 0, pstm, &cbTag);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    err = _A2C_write_length(0, pstm, &cbTag);
    
    /*
     *  Cleanup and return final error
     */

ErrorExit:
            
    return err;
}


/* ---
///
--- */

int A2C_SEQUENCE_OF_print(A2C_SEQUENCE_OF const * pdata, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm) 
{
    int              cb;
    int              cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    int              iItem;
    PBYTE            pbData = (BYTE *) pdata->array;
    A2C_ITEM const * rgItems = pdesc->pItemDescription->rgChildren;

    /*
     *  Walk the list and emit each item in order
     */

    pstm->pfnWrite(pstm, (PBYTE) "{\n", 2);
    iLevel += 1;
    _A2C_Indent(pstm, iLevel);

    for (iItem=0; iItem<pdata->count; iItem++, pbData += cbStruct) {
        cb = rgItems[0].pdu->pfnPrint(pbData, rgItems[0].pdu, iLevel, pstm);
        if (cb < 0) {
            return cb;
        }
        if (iItem < pdata->count-1) {
            pstm->pfnWrite(pstm, (PBYTE) "\n", 1);
            _A2C_Indent(pstm, iLevel);
        }
    }

    pstm->pfnWrite(pstm, (PBYTE) "\n", 1);
    _A2C_Indent(pstm, iLevel-1);
    return pstm->pfnWrite(pstm, (PBYTE) "}", 1);
}


int A2C_SEQUENCE_OF_compare(A2C_SEQUENCE_OF const * pLeft, A2C_SEQUENCE_OF const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    int         cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    int         cItem;
    int         i;
    int         iItem;
    PBYTE       pbLeft = (PBYTE) pLeft->array;
    PBYTE       pbRight = (PBYTE) pRight->array;
    A2C_ITEM const * rgItems = pdesc->pItemDescription->rgChildren;
    
    cItem = MIN(pLeft->count, pRight->count);
    
    for (iItem=0; iItem<cItem; iItem++, pbLeft += cbStruct, pbRight += cbStruct) {
        i = rgItems[0].pdu->pfnCompare(pbLeft, pbRight, rgItems[0].pdu);
        if (i != 0) {
            return i;
        }
    }

    return pLeft->count - pRight->count;
}

int A2C_SEQUENCE_OF_copy(A2C_SEQUENCE_OF * pLeft, A2C_SEQUENCE_OF const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    int         cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    A2C_ERROR   err;
    int         iItem;
    PBYTE       pbLeft;
    PBYTE       pbMem;
    PBYTE       pbRight = (PBYTE) pRight->array;
    A2C_ITEM const * rgItems = pdesc->pItemDescription->rgChildren;

    /*  Allocate the destination buffer */

    pbMem = pbLeft = malloc(pRight->count * cbStruct);
    if (pbLeft == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    for (iItem=0; iItem<pRight->count; iItem++, pbLeft += cbStruct, pbRight += cbStruct) {
        err = rgItems[0].pdu->pfnCopy(pbLeft, pbRight, rgItems[0].pdu);
        if (err < A2C_ERROR_Success) {
            
            /*  Cleanup? -- M00BUG */
            return err;
        }
    }

    pLeft->count = pRight->count;
    pLeft->countAllocated = pRight->count;
    pLeft->array = pbMem;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check the string exists
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_SEQUENCE_OF_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    A2C_ERROR           err;
    int                 i;
    int                 iFrame;
    PBYTE               pb;
    A2C_SEQUENCE_OF *   pseq = (A2C_SEQUENCE_OF *) pv;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;

    /*
     *  If not already done - push our selfs on the stack
     */

    *pf = FALSE;
    if (!A2C_ContextTOS(pcxt)) {
        err = A2C_PushContext2(pcxt, &iFrame, NULL, 0);
        if (err != A2C_ERROR_Success) return err;
    }
    else {
        iFrame = A2C_ContextFrame(pcxt);
    }

    /*
     *  Check that the buffer is allocated
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if ((pseq->count > 0) && (pseq->array == NULL)) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            /*
             *  Print error
             */

            return A2C_Validate_Print(pdesc, pcxt, "SEQUENCE OF", "Count > 0, but no buffer", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check that the not too many items are used
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        if (pseq->count > pseq->countAllocated) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            /*
             *  Print error
             */

            return A2C_Validate_Print(pdesc, pcxt, "SEQUENCE OF", "count > countAllocated", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 2);
    }

    /*
     *  Make the recursive call on each of the elements
     */

    pb = (PBYTE) pseq->array;
    for (i=A2C_ContextNum(pcxt, iFrame)-2; i<pseq->count; i++) {
        A2C_SetContextNum(pcxt, iFrame, i+2);
        A2C_SetContextName(pcxt, iFrame, "*");

        err = rgItems[0].pdu->pfnValidate(pb + i*cbStruct, rgItems[0].pdu, pcxt, pf, pstm);
        if ((err != A2C_ERROR_Success) || (pf == FALSE)) return err;
    }

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);
    
    *pf = TRUE;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_AllocArray(A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int cAlloc)
{
    A2C_ERROR   err;
    int         i;
    PBYTE       pb;

    /*  Allocate the space to hold things */
    
    pb = calloc(pdesc->pItemDescription->cbStruct, cAlloc);
    if (pb == NULL) return A2C_ERROR_outOfMemory;

    /*  Initialize the space to hold things */

    for (i=0; i<cAlloc; i++) {
        err = A2C_Init(pb + i*pdesc->pItemDescription->cbStruct, pdesc);
        if (err < A2C_ERROR_Success) {
            /*  We have an error - release everything that was allocated */
            /*  M00BUG */

            free(pb);
            return err;
        }
    }

    /*  Fill in the data structure with out data */

    pData->count = 0;
    pData->countAllocated = cAlloc;
    pData->array = pb;
    
    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_FreeArray(A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc)
{
    int         cbStruct = pdesc->pItemDescription->cbStruct;
    int         i;
    PBYTE       pb= pData->array;
    
    /*  Start by releasing the data in items. */
    /*  We are going to ignore any errors in this since we can't really recover */

    for (i=0; i<pData->countAllocated; i++) {
        A2C_Release(pb + i*cbStruct, pdesc);
    }

    free(pb);

    pData->count = 0;
    pData->countAllocated = 0;
    pData->array = NULL;

    return A2C_ERROR_Success;
}


A2C_ERROR A2C_ReallocArray(A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int cAlloc)
{
    unreferenced(pData);
    unreferenced(pdesc);
    unreferenced(cAlloc);
#pragma message ("M00TODO: A2C_ReallocArray")
    return -1;
}
