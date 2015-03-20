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

A2C_ERROR A2C_CHOICE_decode_common(int fDER, A2C_CHOICE * data, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH          cbLength;
    int                 cItems = pdesc->pItemDescription->cChildren;
    A2C_LENGTH          cbTL;
    int                 err;
    int                 fConstructed;
    int                 iChoice;
    int                 iClass;
    int                 iContext;
    A2C_TAG_VALUE       iValue;
    A2C_ITEM const *    rgItem = pdesc->pItemDescription->rgChildren;

    unreferenced(ptag);

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
         */
        
        iChoice = pcxtitm->iItem;
            
        /*
         *  If we at the top of the stack - clear the flag
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
         *  We don't care what the tag is for a choice
         *
         *  We can now use the tag to determine which choice we are going to follow
         */

        for (iChoice=0; iChoice<cItems; iChoice++) {
            if ((rgItem[iChoice].tag.iClass == iClass) && (rgItem[iChoice].tag.iValue == iValue)) {
                break;
            }
        }

        if (iChoice == cItems) {
            /*
             *  We never found a good tag.  Die.
             */

            err = -1;
            goto ErrorExit;
        } 

        /*
         *  Set us up for doing an restart operation
         */

        err = A2C_PushContext(pcxt, &iContext);
        if (err != A2C_ERROR_Success) goto ErrorExit;

        /*
         *  Add our data to the context stack
         *
         *  Which item we are processing
         *
         */

        A2C_SetContextNum(pcxt, iContext, iChoice);
    }
     

    /*
     *  Make the call to the decoding function
     */

    if (fDER) {
        err = rgItem[iChoice].pdu->pfnDerDecoder(rgItem[iChoice].cbOffset + (BYTE *)data, rgItem[iChoice].pdu, flags, pcxt, &rgItem[iChoice].tag, pstm);
    }
    else {
        err = rgItem[iChoice].pdu->pfnBerDecoder(rgItem[iChoice].cbOffset + (BYTE *)data, rgItem[iChoice].pdu, flags, pcxt, &rgItem[iChoice].tag, pstm);
    }

    /*
     *  Now deal with error returns
     */

    if (err == A2C_ERROR_needMoreData) {
        /*
         *  Two cases - either someone else has already setup the stack or we still
         *          need to do that
         */

        if (iContext == pcxt->cUsed) {
            /*
             *  If the stack has not already been created - then
             *  defer the creation to our caller.
             */

            A2C_PopContext(pcxt, iContext);
        }
    }
    else if (err == A2C_ERROR_Success) {
        /*
         *  Save which choice we used.
         */
        
        data->index = iChoice;

        /*
         *  Remove the restart setup
         */

        A2C_PopContext(pcxt, iContext);
    }

    
ErrorExit:
    return err;
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_CHOICE_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const *    pitem = &pdesc->pItemDescription->rgChildren[0];
    A2C_CHOICE *        p = (A2C_CHOICE *) pv;
    
    return pitem->pdu->pfnInit(pitem->cbOffset + (BYTE *) p, pitem->pdu);
}

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_CHOICE_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ITEM const * pitem;
    A2C_CHOICE * p = (A2C_CHOICE *) pv;

    /*
     *  Is the desired choice in the range?
     *
     *  We start the choice count at 1, so an initialize value is not legal.
     */

    if ((0 <= p->index) && (p->index < pdesc->pItemDescription->cChildren)) {
        /*
         *  Choice does not emit either a tag or a length, however we should always have a value for the
         *      tag in our structure - so pass that down.
         *
         *  Select the correct choice and ask it to do the encode operation.
         */

        pitem = &pdesc->pItemDescription->rgChildren[p->index];
        return pitem->pdu->pfnRelease(pitem->cbOffset + (BYTE *) p, pitem->pdu);
    }

    return A2C_ERROR_indexOutOfRange;
}

/*
 *  Read in a DER decoded object
 */

A2C_ERROR A2C_CHOICE_decode_der(A2C_CHOICE * data, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_CHOICE_decode_common(TRUE, data, pdesc, flags, pcxt, ptag, pstm);
}

/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_CHOICE_encode_der(A2C_CHOICE const * data, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ITEM const * pitem;

#pragma message("M00QUEST: Can you have a default choice value?")

    unreferenced(ptag);

    /*
     *  Is the desired choice in the range?
     *
     *  We start the choice count at 1, so an initialize value is not legal.
     */

    if ((0 <= data->index) && (data->index < pdesc->pItemDescription->cChildren)) {
        /*
         *  Choice does not emit either a tag or a length, however we should always have a value for the
         *      tag in our structure - so pass that down.
         *
         *  Select the correct choice and ask it to do the encode operation.
         */

        pitem = &pdesc->pItemDescription->rgChildren[data->index];
        return pitem->pdu->pfnDerEncoder(pitem->cbOffset + (BYTE *) data, pitem->pdu, flags, pcxt, &pitem->tag, pstm);
    }

    return -1;
}

A2C_ERROR A2C_CHOICE_decode_ber(A2C_CHOICE * data, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_CHOICE_decode_common(FALSE, data, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_CHOICE_encode_ber(A2C_CHOICE const * data, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ITEM const * pitem;

    unreferenced(ptag);

    /*
     *  Is the desired choice in the range?
     *
     *  We start the choice count at 1, so an initialize value is not legal.
     */

    if ((0 <= data->index) && (data->index < pdesc->pItemDescription->cChildren)) {
        /*
         *  Choice does not emit either a tag or a length, however we should always 
         *      have a value for the tag in our structure - so pass that down.
         *
         *  Select the correct choice and ask it to do the encode operation.
         */

        pitem = &pdesc->pItemDescription->rgChildren[data->index];
        return pitem->pdu->pfnBerEncoder(pitem->cbOffset + (BYTE *) data, pitem->pdu, flags, pcxt, &pitem->tag, pstm);
    }

    return -1;
}


/* ---
/// <summary>
/// A2C_CHOICE_print is the default choice value print routine.
/// <para>This function wraps the output in a pair of braces and indents the child output by one level.
/// The child used is determined by the choice value.  If the value is out of ranged this is not
//  considred to be an error, instead a message to that effect is emitted.</para>
/// </summary>
/// <param name="pf">pointer to the boolean value</param>
/// <param name="pdesc">choice descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_CHOICE_print(A2C_CHOICE const * pData, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_ERROR                   err;
    A2C_ITEM const *            pitem;

    /*
     *  Is the desired choice in the range?
     *
     *  We start the choice count at 1, so an initialize value is not legal.
     */

    if ((0 <= pData->index) && (pData->index < pdesc->pItemDescription->cChildren)) {
        /*
         *   Mark this as being a choice - and tag in which choice it is
         */

        err = pstm->pfnWrite(pstm, (PBYTE) "{\n", 2);
        iLevel += 1;
        _A2C_Indent(pstm, iLevel);

        pitem = &pdesc->pItemDescription->rgChildren[pData->index];
        
        pstm->pfnWrite(pstm, (PBYTE) pitem->szFieldName, strlen(pitem->szFieldName));
        err = pstm->pfnWrite(pstm, (PBYTE) " ", 1);

        err = pitem->pdu->pfnPrint(pitem->cbOffset + (BYTE *) pData, pitem->pdu, iLevel, pstm);
        if (err < 0) {
            return err;
        }

        pstm->pfnWrite(pstm, (PBYTE) "\n", 1);
        _A2C_Indent(pstm, iLevel-1);
        return pstm->pfnWrite(pstm, (PBYTE) "}", 1);
    }

    return pstm->pfnWrite(pstm, (PBYTE) "Unrecognized Choice", 19);
}

int A2C_CHOICE_compare(A2C_CHOICE const * pLeft, A2C_CHOICE const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    int                 i;
    A2C_ITEM const *    pitem;
    
    /*  Start by comparing the choices - if not equal return that result */
    
    i = pRight->index - pLeft->index;
    if (i != 0) return i;

    /*  Now do the compare on each of the elements */
    /*  Is the choice legal? */

    if ((0 <= pLeft->index) && (pLeft->index < pdesc->pItemDescription->cChildren)) {
        /*  Grab the descriptor for the item */
        
        pitem = &pdesc->pItemDescription->rgChildren[pLeft->index];

        return pitem->pdu->pfnCompare(pitem->cbOffset + (BYTE *) pLeft, pitem->cbOffset + (BYTE *) pRight, pitem->pdu);
    }

    /*  Out of range compare - just say they don't match */
    
    return -1;
}

A2C_ERROR A2C_CHOICE_copy(A2C_CHOICE * pDst, A2C_CHOICE const * pSrc, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ERROR           err;
    A2C_ITEM const *    pitem;
    
    /*  Is the choice legal? */

    if ((0 <= pSrc->index) && (pSrc->index < pdesc->pItemDescription->cChildren)) {
        /*  Grab the descriptor for the item */
        
        pitem = &pdesc->pItemDescription->rgChildren[pSrc->index];
        pDst->index = pSrc->index;

        err = pitem->pdu->pfnCopy(pitem->cbOffset + (BYTE *) pDst, pitem->cbOffset + (BYTE *) pSrc, pitem->pdu);
        
        return err;
    }

    /*  Out of range choice - error */
    
    return -1;
}

/* ---
/// <summary>
/// 
/// </summary>
/// <param name="pv">pointer to the object to validate</param>
/// <param name="pdesc">pointer to the object descriptor</param>
/// <param name="pcxt">pointer to context information</param>
/// <param name="pstm">stream to dump the information on</param>
/// <returns>A2C_ERROR_Success or A2C_ERROR_validateFails</returns>
--- */

A2C_ERROR A2C_CHOICE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    A2C_ERROR           err = A2C_ERROR_Success;
    int                 iFrame;
    A2C_CHOICE *        pChoice = (A2C_CHOICE *) pv;
    A2C_ITEM const *    pitem;

    /*
     *  If not currently validating the choice, then push our context onto
     *  the stack.
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
     *  Are we to the point of checking the index range?
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        /*
         *  Check for index # out of range
         */
        
        if ((pChoice->index < 0) || (pChoice->index >= pdesc->pItemDescription->cChildren)) {
            /*
             *  On return don't check the selected type!
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            /*
             *  Dump the error to the stream
             */

            return A2C_Validate_Print(pdesc, pcxt, "CHOICE", "choice selection is out of range", pstm);
        }

        /*
         *  Now check the selected type
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check the selected type?
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {

        /*
         *  Put the field name onto the context stack
         */
        
        pitem = &pdesc->pItemDescription->rgChildren[pChoice->index];

        A2C_SetContextName(pcxt, iFrame, pitem->szFieldName);

        /*
         *  Make the correct type based call to check the type
         */

        err = pitem->pdu->pfnValidate(pitem->cbOffset + (BYTE *) pChoice, pitem->pdu, pcxt, pf, pstm);

        /*
         *  We have no more to check
         */

        A2C_SetContextNum(pcxt, iFrame, -1);

        /*
         *  One a validation failure - allow for restart
         */

        if ((err != A2C_ERROR_Success) || (pf == FALSE)) {
            return err;
        }
    }

    *pf = TRUE;

    /*
     *  We have finished our validateion - remove our context & return result code
     */

    A2C_PopContext(pcxt, iFrame);
    
    return err;
}
