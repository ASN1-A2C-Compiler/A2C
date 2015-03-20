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

A2C_ERROR _A2C_SEQ_SET_encode(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags,
                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm,
                              int fSet, int fDER);

A2C_TAG     TagSet = {A2C_TAG_CLASS_UNIVERSAL, 17};

__inline int IsFlagSet(int iBit, int * rgf)
{
    return rgf[iBit/32] & (1 << (iBit % 32));
}

__inline void FlagSet(int iBit, int * rgf)
{
    rgf[iBit/32] |= (1 << (iBit % 32));
}


/* ---
/// <summary>
/// </summary>
/// <returns></returns>
--- */

A2C_ERROR A2C_SET_decode_common(int fDER, A2C_SET * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH          cb;
    A2C_LENGTH          cbLength;
    A2C_LENGTH          cbTL;
    int                 cFields = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int	                fConstructed;
    int                 iClass;
    int                 iContext;
    int                 iField;
    int                 iOptional;
    A2C_TAG_VALUE       iValue;
    PBYTE               pb;
    A2C_STREAM_MEMORY * pstm2 = NULL;
    A2C_TAG const *     ptagLocal;
    A2C_ITEM const *    rgFields = pdesc->pItemDescription->rgChildren;
    int                 rgf[4] = {0};
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Are we doing a restart operation?
     */

    if (flags & A2C_FLAGS_MORE_DATA) {
        A2C_CXT_ITEM * pcxtitm;
        /*
         *  Get this specifics for this function
         */

        iContext = A2C_ContextFrame(pcxt);
        pcxtitm = &pcxt->rgCxts[iContext];

        /*
         *  Checking the tag and getting some fields already accomplished
         */

        cbLength = pcxtitm->cbLeft;

        /*
         *  Grab the array which tells us which fields have already been 
         *      filled in
         */

        memcpy(rgf, pcxt->rgCxts[iContext].rgf, sizeof(rgf));

        /*
         *  If we have cleared the stack - clear the flag
         */

        if (A2C_ContextTOS(pcxt)) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }
    }
    else {
        /*
         *  Grab the type and length for this item.
         */

        err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
        if (err < 0) {
            /*
             *  not enough data in buffer for this probably
             */

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
            if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 17)) {
                err = A2C_ERROR_tagMismatch;
                goto ErrorExit;
            }
        }

        /*
         *  Indefinite length encoding not allowed for DER
         */

        if (fDER && (cbLength == -1)) {
            err = A2C_ERROR_malformedDer;
            goto ErrorExit;
        }

        /*
         *  Jump over the tag and length in the stream
         */

        _A2C_Memory_Skip(pstm, cbTL);

        /*
         *  Set us up for a possible restart operation
         */

        err = A2C_PushContext(pcxt, &iContext);
        if (err != A2C_ERROR_Success) goto ErrorExit;

        /*
         *  Setup the flags for field checking
         *
         *  - nada
         */
    }

    /*
     *  Limit the bytes that the children can be decoding from
     */

    if (cbLength == -1) {
        /* Can do this for indefinite length encoding */
        pstm2 = pstm;
    }
    else {
        _A2C_Memory_Init_With_Data(&stm, pstm->pb, MIN(cbLength, (pstm->pbLast - pstm->pb)));
        pstm2 = &stm;
    }

    /*
     *  Loop until we have finished processing the structure
     */

    while (TRUE) {
        /*
         *  See if we are at the end of the buffer for indefinite length encoding
         */

        if (cbLength == -1) {
            err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cb, &cbTL);
            if (err >= A2C_ERROR_Success) {
                /*
                 * We skip any errors as they will be caught and processed later
                 */

                if ((iClass == A2C_TAG_CLASS_UNIVERSAL) && (iValue == 0)) {
                    /*  We have exhausted the buffer - get out of the loop */
                    break;
                }
            }
        }
        else {
            /*  Have we exhausted the buffer in the rest of the cases? */

            if (stm.pbLast == stm.pb) {
                break;
            }
        }
                
        /*
         *  Walk the possible list of fields until one is matched
         */

        iOptional = 1;
        for (iField=0; iField<cFields; iField++) {
            /*
             *  We need to deal with optionality
             */

            if ((iField > 0) && (rgFields[iField-1].flags & A2C_FLAGS_OPTIONAL)) {
                iOptional <<= 1;
            }
            
            /*
             *  Have we already matched this field?
             */

            if (IsFlagSet(iField, rgf)) continue;

            /*
             *  Grab the offset of this field
             */

            pb = ((PBYTE) pi) + rgFields[iField].cbOffset;

            /*
             *  Code to deal with tags
             */

            if (rgFields[iField].flags & A2C_FLAGS_TAG_IMPLICIT) {
                ptagLocal = &rgFields[iField].tag;
            }
            else if (rgFields[iField].flags & A2C_FLAGS_TAG_EXPLICIT) {
                /*
                 *  For an explicit tag, we need to indirectly call the decoder, so we
                 *      actually get a result back here and need to go to the error checking
                 *      code.
                 */
                
                err = A2C_TAG_EXPLICIT_Decode(fDER, pb, &rgFields[iField], flags, pcxt, NULL, pstm2);
                goto ErrorChecks;
            }
            else {
                ptagLocal = NULL;
            }

            /*
             *  Attempt the actual decode operation
             */

            if (fDER) {
                err = rgFields[iField].pdu->pfnDerDecoder(pb, rgFields[iField].pdu, flags,
                                                          pcxt, ptagLocal, pstm2);
            }
            else {
                err = rgFields[iField].pdu->pfnBerDecoder(pb, rgFields[iField].pdu, flags,
                                                          pcxt, ptagLocal, pstm2);
            }

            /*
             *  Now do the different types of error checking
             */

        ErrorChecks:

            if (err == A2C_ERROR_Success) {
                /*
                 *  We have a successful match - this means that
                 *      we found the current field.
                 *  Mark this fact in the tracking and then go to the next item
                 */

                FlagSet(iField, rgf);

                if (rgFields[iField].flags & A2C_FLAGS_OPTIONAL) {
                    pi->optionals |= iOptional;
                }
                break;
            }

            /*
             *  If it is need more data - and there is some data left to be processed
             */
            
            else if ((err == A2C_ERROR_needMoreData) &&
                      ((cbLength == -1) || ((cbLength - (pstm2->pb - pstm2->pbBase)) > 0))) {
                /*
                 *  Add our data to the context stack
                 *
                 *  Which item we are processing
                 *
                 *  Count of bytes left =
                 *      # bytes in this item - # bytes consumed so far
                 *
                 *  Array of bits about which items we have already processed
                 */

               
                pcxt->rgCxts[iContext].iItem = iField;
                if (cbLength == -1) {
                    pcxt->rgCxts[iContext].cbLeft = cbLength;
                }
                else {
                    pcxt->rgCxts[iContext].cbLeft = cbLength - (A2C_LENGTH) (pstm2->pb - pstm2->pbBase);
                }
                memcpy(pcxt->rgCxts[iContext].rgf, rgf, sizeof(rgf));

                /*
                 *  Advanced called in buffer by correct # of bytes
                 */

                if (pstm2 != pstm) {
                    _A2C_Memory_Skip(pstm, stm.pb - stm.pbBase);
                }

                /*
                 *  Finished current processing - return up
                 */
                
                goto ErrorExit;
            }
            else if (err != A2C_ERROR_tagMismatch) {
                if (err == A2C_ERROR_needMoreData) err = A2C_ERROR_malformedEncoding;
                goto ErrorExit;
            }
        }

        /*
         *  OK - need to deal with the different cases at this point.
         *
         *  1.  We found a match - all is well
         *  2.  We did not find a match - need to know if the set is extensible and
         *      this is a legal extensable tag.
         */

        if (iField == cFields) {
            /* Is it extensible ?  */
            err = A2C_ERROR_notExtensible;
            goto ErrorExit;
        }
    }

    /*
     *  Find out of all of the fields have been dealt with and deal with 
     *  optionals
     */

    for (iField=0; iField<cFields; iField++) {
        /*
         *  Was the element filled in? - all is good
         */

        if (IsFlagSet(iField, rgf)) continue;

        /*
         *  is the field default? - if so then fill in the default value
         */

        if (rgFields[iField].flags & A2C_FLAGS_DEFAULT) {
            /*
             *  Grab the offset of this field
             */

            pb = ((PBYTE) pi) + rgFields[iField].cbOffset;

            /*
             *  Copy the default value over
             */

            err = rgFields[iField].pdu->pfnCopy(pb, &rgFields[iField].pvDefault,
                                                rgFields[iField].pdu);
            if (err < 0) {
                goto ErrorExit;
            }
        }
        
        /*
         *  Is the field optional? - if so no problem
         */
        
        else if (rgFields[iField].flags & A2C_FLAGS_OPTIONAL) {
            continue;
        }
        else {
            /*
             *  The field SHOULD be here and is not - 
             *      M00BUG - Extensibilty
             */

            err = A2C_ERROR_missingSetField;
            goto ErrorExit;
        }
    }

    err = A2C_ERROR_Success;

ErrorExit:

    return err;
}


/*************************************************************************/

/* ---
/// <summary>
/// </summary>
--- */


A2C_ERROR A2C_SET_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cField = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 iField;
    PBYTE               pb;
    A2C_ITEM const *    rgField = pdesc->pItemDescription->rgChildren;
    A2C_SET *           pset = (A2C_SET *) pv;

#pragma message("M00TODO: A2C_SET_init does not set defaults")

    for (iField = 0; iField<cField; iField++) {
        pb = rgField[iField].cbOffset + (PBYTE) pset;

        err = rgField[iField].pdu->pfnInit(pb, rgField[iField].pdu);
        if (err < A2C_ERROR_Success) {
            return err;
        }
    }
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SET_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cField = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 iField;
    PBYTE               pb;
    A2C_ITEM const *    rgField = pdesc->pItemDescription->rgChildren;
    A2C_SET *           pset = (A2C_SET *) pv;

    for (iField = 0; iField<cField; iField++) {
        pb = rgField[iField].cbOffset + (PBYTE) pset;

        err = rgField[iField].pdu->pfnRelease(pb, rgField[iField].pdu);

        /*  M00QUEST: Error return code is ignored */
    }
    return A2C_ERROR_Success;
}

/*
 *  Write out a DER encoded integer
 */
A2C_ERROR A2C_SET_encode_der(A2C_SET const * pset, PC_A2C_DESCRIPTOR pdesc, int flags,
                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    if (ptag == NULL) {
        ptag = &TagSet;
    }

    return _A2C_SEQ_SET_encode(pset, pdesc, flags, pcxt, ptag, pstm, TRUE, TRUE);
#if 0
    int			cb;
    int                 cbSet = 0;
    int                 cItems = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 i;
    int			iItem;
    int                 iOptional = 1;
    BYTE *		pbData;
    A2C_TAG const *     ptagLocal;
    Element *	        rgdata = NULL;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;
    A2C_STREAM_MEMORY   stm;

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 17 & constucted
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 1, 17, pstm, &cb);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cb);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     * Allocate results array
     */

    rgdata = (Element *) calloc(cItems, sizeof(Element));
    if (rgdata == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }

    /*
     *  Give me a place to dump intermediate results
     */

    _A2C_Memory_Init(&stm);

    /*
     *  Now start walking through all of the items in the array
     */

    for (iItem = 0; iItem<cItems; iItem++) {
        /*
         *  Where is the data item?
         */

        pbData = &(((BYTE *) pset)[rgItems[iItem].cbOffset]);

        /*
         *  Is the item optional and missing?  If so skip forward
         */

        if (rgItems[iItem].flags & A2C_FLAGS_OPTIONAL) {

            if (pset->optionals & iOptional) {
                iOptional <<= 1;
            }
            else {
                iOptional <<= 1;
                continue;
            }
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
         *  Check if we have an implicit tag that is to be used in the decode. 
         *      If so then make sure that we use it and not the default tag value.
         */

       if (rgItems[iItem].flags & A2C_FLAGS_TAG_IMPLICIT) {
            ptagLocal = &rgItems[iItem].tag;
        }
        else if (rgItems[iItem].flags & A2C_FLAGS_TAG_EXPLICIT) {
            err = A2C_TAG_Internal_EncodeDer(pbData, &rgItems[iItem], flags, pcxt, NULL, (PA2C_STREAM) &stm);
            goto ErrorExit;
        }
        else {
            ptagLocal = NULL;
        }

        /*
         *  Call the encoder for this item to get the length
         */

        err = rgItems[iItem].pdu->pfnDerEncoder(pbData, rgItems[iItem].pdu, flags, pcxt, ptagLocal, (PA2C_STREAM) &stm);
        if (err < A2C_ERROR_Success) {
            goto ErrorExit;
        }

    }

    /*
     *  Return count of bytes encoded
     */

    err = A2C_ERROR_Success;

    /*
     *  Cleanup and return any errors
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
#endif
}

A2C_ERROR A2C_SET_decode_der(A2C_SET * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                             A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_SET_decode_common(TRUE, pi, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_SET_encode_ber(A2C_SET const * data, PC_A2C_DESCRIPTOR pdesc, int flags,
                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    if (ptag == NULL) {
        ptag = &TagSet;
    }

    return _A2C_SEQ_SET_encode(data, pdesc, flags, pcxt, ptag, pstm, TRUE, FALSE);

#if 0    
    /* M00QUEST - why not call A2C_SEQUENCE_encode_ber? */

    int                 cb;
    int                 cbTag = 0;
    int                 cItems = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 i;
    int		        iItem;
    int                 iOptional = 1;
    BYTE *	        pbData;
    A2C_TAG const *     ptagChild;
    int                 rgfOptionals;
    const A2C_ITEM *    rgItems = pdesc->pItemDescription->rgChildren;;

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 16 & constucted
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 1, 17, pstm, &cbTag);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 1, ptag->iValue, pstm, &cbTag);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out an indefinite length
     */

    err = _A2C_write_length((size_t) -1, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Assume that optional exists and grab it.
     */

    rgfOptionals = data->optionals;

    /*
     *  Now start walking through all of the items in the array
     */

    iOptional = 1;
    for (iItem = 0; iItem<cItems; iItem++) {

        /*
         *  Where is the data item?
         */

        pbData = &(((PBYTE) data)[rgItems[iItem].cbOffset]);
            
        /*
         *  Is the item optional and missing?  If so skip forward
         */

        if (rgItems[iItem].flags & A2C_FLAGS_OPTIONAL) {
            if ((rgfOptionals & iOptional) == 0) {
                iOptional <<= 1;
                continue;
            }
            iOptional <<= 1;
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
            err = A2C_TAG_Internal_EncodeBer(pbData, &rgItems[iItem], flags, pcxt, NULL, pstm);
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
                                                     ptagChild, pstm);

        /*
         *  Do the error processing
         */

    ErrorChecks:
        if (err < 0) {
            goto ErrorExit;
        }
    }

    /*
     *  Write out an end-of-contents sequence
     */

    err = _A2C_write_tag(0, 0, 0, pstm, &cbTag);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }
    
    err = _A2C_write_length(0, pstm, &cb);

    /*
     *  Now clean up things
     */

ErrorExit:
    return err;
#endif
}

A2C_ERROR A2C_SET_decode_ber(A2C_SET * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_SET_decode_common(FALSE, pi, pdesc, flags, pcxt, ptag, pstm);
}

/*
 */

A2C_ERROR A2C_SET_print(A2C_SET const * pi, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    int                         cb;
    int                         cChildren = pdesc->pItemDescription->cChildren;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbData = (PBYTE) pi;
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

        _A2C_Indent(pstm, iLevel);
        pstm->pfnWrite(pstm, (PBYTE) rgChildren[i].szFieldName, (A2C_LENGTH) strlen(rgChildren[i].szFieldName));
        pstm->pfnWrite(pstm, (PBYTE) " ", 1);
        cb = rgChildren[i].pdu->pfnPrint(pbData + rgChildren[i].cbOffset, rgChildren[i].pdu, iLevel, pstm);
        pstm->pfnWrite(pstm, (PBYTE) "\n", 1);

        if (cb < 0) {
            return cb;
        }
    }
    _A2C_Indent(pstm, iLevel-1);
    pstm->pfnWrite(pstm, (PBYTE) "}", 1);
    
    return 1;
}

int A2C_SET_compare(A2C_SET const * pLeft, A2C_SET const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    int                         cChildren = pdesc->pItemDescription->cChildren;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbLeft = (PBYTE) pLeft;
    PBYTE                       pbRight = (PBYTE) pRight;
    int                         ret;
    A2C_ITEM const *            rgChildren = pdesc->pItemDescription->rgChildren;
    int                         rgfOptionals = pLeft->optionals;

    for (i=0; i<cChildren; i++) {
        /*  Play with optionality of fields */
        
        if (rgChildren[i].flags & A2C_FLAGS_OPTIONAL) {
            /*  If the optional flags don't match - then they are not the same */
            
            if ((pLeft->optionals & iOptional) != (pRight->optionals & iOptional)) {
                return -1;
            }

            /*  If the optional element is absent - then skip to next element */
            
            if ((rgfOptionals & iOptional) == 0) {
                iOptional <<= 1;
                continue;
            }
            
            iOptional <<= 1;
        }

        /*  Compare the elements */

        ret = rgChildren[i].pdu->pfnCompare(pbLeft + rgChildren[i].cbOffset, pbRight + rgChildren[i].cbOffset, rgChildren[i].pdu);

        /*  If a mis-match has been found - return that */
        
        if (ret != 0) return ret;
    }

    /*  Return comparision == match */
    
    return 0;
}

A2C_ERROR A2C_SET_copy(A2C_SET * pLeft, A2C_SET const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    int                         cChildren = pdesc->pItemDescription->cChildren;
    A2C_ERROR                   err;
    int                         f;
    int                         i;
    int                         iOptional = 1;
    PBYTE                       pbDst = (PBYTE) pLeft;
    PBYTE                       pbSrc = (PBYTE) pRight;
    A2C_ITEM const *            rgChildren = pdesc->pItemDescription->rgChildren;
    int                         rgfOptionals = pRight->optionals;

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

        err = rgChildren[i].pdu->pfnCopy(pbDst + rgChildren[i].cbOffset, pbSrc + rgChildren[i].cbOffset, rgChildren[i].pdu);
        if (err < A2C_ERROR_Success) return err;

        pLeft->optionals |= f;
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

A2C_ERROR A2C_SET_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    int                 cFields = pdesc->pItemDescription->cChildren;
    A2C_ERROR           err;
    int                 i;
    int                 iFrame;
    int                 iItem;
    int                 iOptional;
    PBYTE               pb =  (PBYTE) pv;
    A2C_ITEM const *    rgChildren = pdesc->pItemDescription->rgChildren;

    /*
     *  If not already done - push our selfs on the stack
     */

    *pf = FALSE;
    
    if (A2C_ContextTOS(pcxt)) {
        err = A2C_PushContext2(pcxt, &iFrame, NULL, 0);
        if (err != A2C_ERROR_Success) return err;
        iOptional = 1;
        iItem = 0;
    }
    else {
        iFrame = A2C_ContextFrame(pcxt);
        
        /*
         *  Determine what optional element we should be on
         */

        iOptional = 1;
        iItem = A2C_ContextNum(pcxt, iFrame);

        for (i=0; i<iItem; i++) {
            if (rgChildren[iItem].flags & A2C_FLAGS_OPTIONAL) {
                iOptional <<= 1;
            }
        }

    }

    for ( ; iItem < cFields; iItem++) {
        /*
         *  Is this element present?
         */

        if (rgChildren[iItem].flags & A2C_FLAGS_OPTIONAL) {
            if ((rgChildren[iItem].flags & iOptional) == 0) {
                /*
                 *  Not present
                 */
                
                iOptional <<= 1;
                continue;
            }
            else {
                /*
                 * present
                 */

                iOptional <<= 1;
            }
        }

        /*
         *  Mark where we are
         */
        
        A2C_SetContextNum(pcxt, iFrame, iItem);
        A2C_SetContextName(pcxt, iFrame, rgChildren[iItem].szFieldName);

        /*
         *  Check this element
         */

        err = rgChildren[iItem].pdu->pfnValidate(pb + rgChildren[iItem].cbOffset, rgChildren[iItem].pdu, pcxt, pf, pstm);
        if ((err != A2C_ERROR_Success) || (*pf == FALSE)) {
            return err;
        }
    }

    *pf = TRUE;
    return A2C_ERROR_Success;
}
