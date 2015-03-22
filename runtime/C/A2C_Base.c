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


/****************************************** UTILITY ROUTINES ****************************************/

static A2C_ERROR _A2C_Decode(int fDER, PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags,
                             A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb)
{
    A2C_CONTEXT         cxt = {0};
    int                 err;
    PBYTE               pbAlloc = NULL;
    A2C_CONTEXT *       pcxt = &cxt;
    PVOID               pv = NULL;
    A2C_STREAM_MEMORY   stm = {{0}};

    /*
     *  Determine if this is a first call or a continuation call.  Setup is done differently
     *  for these two cases.
     */
    
    if (flags & A2C_FLAGS_MORE_DATA) {
        /*
         *  Grab the old context pointer
         */

        pcxt = *ppcxt;                  /*  M00BUG - copy back into cxt and cleanup? */
        
        /*
         *  Setup the buffer to point to 
         *      data from old context pointer followed by
         *      data just passed in
         */

        pbAlloc = malloc(pcxt->cbBuffer + cb);
        if (pbAlloc == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorReturn;
        }

        memcpy(pbAlloc, pcxt->pbBuffer, pcxt->cbBuffer);
        memcpy(pbAlloc + pcxt->cbBuffer, pb, cb);

        cxt.cUsed = pcxt->cUsed;
        cxt.cAlloc = pcxt->cAlloc;
        cxt.rgCxts = pcxt->rgCxts;
        pcxt->rgCxts = NULL;

        /*
         *  If we have no frames, then clear the restart flag
         */

        if (cxt.cUsed == 0) {
            flags &= ~A2C_FLAGS_MORE_DATA;
        }

        /*
         *  Setup the stream buffer to point to ALL of the data
         */

        _A2C_Memory_Init_With_Data(&stm, pbAlloc, cb + pcxt->cbBuffer);

        /*
         *  We should have been passed the data structure back in
         *      point to it.
         */

        pv = *ppv;

        /*
         *  Clean up
         */

        A2C_FreeContext(pcxt);
        *ppcxt = NULL;
        pcxt = NULL;

    }
    else {
        /*
         *  Setup the stream buffer to point to the passed in data
         */
    
        _A2C_Memory_Init_With_Data(&stm, pb, cb);

        if (flags & A2C_FLAGS_BUFFER_PROVIDED) {
            /*
             *  We passed in a buffer to decode into
             */

            pv = *ppv;
        }
        else {
            /*
             *  Allocate the data structure we are going to decode into
             *
             *  Omit calling the initializer - we don't want default
             *      values - and there is no other setup 
             */
        
            pv = calloc(1, pdesc->pItemDescription->cbStruct);
            if (pv == NULL) {
                err = A2C_ERROR_outOfMemory;
                goto ErrorReturn;
            }
        }
    }

    /*
     *  Setup the decode root array and initialize it
     */

    cxt.cRoots = 100;
    cxt.iRoot = 1;
    cxt.rgpbRoots = calloc(cxt.cRoots, sizeof(cxt.rgpbRoots[0]));
    if (cxt.rgpbRoots == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorReturn;
    }
    cxt.rgpbRoots[0] = pv;

    /*
     *  Make the call into the system to do the acutal decode based
     *  on the descriptor structure we were passed.
     */

    if (fDER) {
        err = pdesc->pfnDerDecoder(pv, pdesc, flags, &cxt, NULL, &stm);
    }
    else {
        err = pdesc->pfnBerDecoder(pv, pdesc, flags, &cxt, NULL, &stm);
    }

    /*
     *  Look at the return value here
     */

    if (err == A2C_ERROR_Success) {
        /*
         *  Succeeded - clean all up and return the data structure.
         */

        *ppv = pv;
        pv = NULL;

    }
    else if ((err == A2C_ERROR_needMoreData) && (ppcxt != NULL)) {
        /*
         *  We said we needed more data, and we have the ability
         *      to return a context structure so we can deal with that.
         */

        pcxt = calloc(sizeof(*pcxt), 1);
        if (pcxt == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorReturn;
        }

        /*
         *  Copy over the unused data
         */

        pcxt->cbBuffer = (A2C_LENGTH) (stm.pbLast - stm.pb);
        pcxt->pbBuffer = malloc(pcxt->cbBuffer);
        if (pcxt->pbBuffer == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorReturn;
        }

        memcpy(pcxt->pbBuffer, stm.pb, pcxt->cbBuffer);

        /*
         *  Copy copy over the chain of context stuctures 
         */

        pcxt->cUsed = cxt.cUsed;
        pcxt->cAlloc = cxt.cAlloc;
        pcxt->rgCxts = cxt.rgCxts;
        cxt.rgCxts = NULL;

        /*
         *  Setup and protect the return variables
         *
         */

        *ppv = pv;
        pv = NULL;

        *ppcxt = pcxt;
        pcxt = NULL;
    }

ErrorReturn:
    /*
     *  Clean everything up and return the error.
     */

    _A2C_Memory_Cleanup(&stm);
    if (cxt.rgCxts != NULL) free(cxt.rgCxts);
    if (cxt.rgpbRoots != NULL) free(cxt.rgpbRoots);

    /*
     *  Release the allocated structure - and
     *  clean it out if this is a return call
     */
    
    if (pv != NULL) {
        free(pv);
        *ppv = NULL;
    }

    /*
     *  Release the context structure - and
     *  clean it of passed in pointer
     */
    
    if ((pcxt != NULL) && (pcxt != &cxt)) {
        if (pcxt->rgCxts != NULL) free(pcxt->rgCxts);
        free(pcxt);
        *ppcxt = NULL;
    }

    /*
     *  Release allocated data buffer
     */

    if (pbAlloc != NULL) {
        free(pbAlloc);
    }
    
    return err;
}



/*****************************************  PUBLIC FUNCTIONS ***************************************/

/* ---
/// <summary>
/// A2C_Version returns the current release version number
/// </summary>
/// <param name="version">Returns the current release version
/// <returns>A2C_ERROR_Success</returns>
--- */

A2C_ERROR A2C_Version(int * piVersion)
{
    *piVersion = 8;

    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// A2C_zero is the default initializer.  It assumes that only the item needs initializing.
/// </summary>
/// <param name="pvData">structure to be initialized</param>
/// <param name="pdesc">structure describing the workings of pvData</param>
/// <returns>0</returns>
--- */
    
A2C_ERROR A2C_zero(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    memset(pvData, 0, pdesc->pItemDescription->cbStruct);

    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// A2C_nop is the default de-initializer.  It does nothing but return
/// </summary>
/// <param name="pvData">structure to be de-initialized</param>
/// <param name="pdesc">structure describing the workings of pvData</param>
/// <returns>0</returns>
--- */
    
A2C_ERROR A2C_nop(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pvData);
    unreferenced(pdesc);
    
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// A2C_Alloc allocates an item from the heap, initializes it and returns.
/// </summary>
/// <param name="ppv">location to return the item</param>
/// <param name="pdesc"> pointer structure describing the object to be allcoated </param>
/// <returns>A2C_ERROR code </returns>
--- */

A2C_ERROR A2C_Alloc(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ERROR   err;
    PVOID       pv = calloc(1, pdesc->pItemDescription->cbStruct);
    
    if (pv == NULL) return A2C_ERROR_outOfMemory;
    
    err = pdesc->pfnInit(pv, pdesc);
    if (err < A2C_ERROR_Success) return err;

    *ppv = pv;
    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// Use A2C_Init to initialize a data structure.
/// </summary>
/// <param name="pvData">structure to be initialized</param>
/// <param name="pdesc">structure describing the workings of pvData</param>
/// <returns>0</returns>
--- */
    
A2C_ERROR A2C_Init(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    return pdesc->pfnInit(pvData, pdesc);
}

A2C_ERROR A2C_Release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    return pdesc->pfnRelease(pvData, pdesc);
}

A2C_ERROR A2C_Free(PVOID pvData, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  If we were passed a NULL pointer, do something good
     */
    
    if (pvData == NULL) return A2C_ERROR_Success;
        
    /*
     *  Clean up any data hanging around
     *
     *  Ignore any errors returned
     */
    
    A2C_Release(pvData, pdesc);

    /*
     *  Now get rid of the memory
     */

    free(pvData);
    
    return A2C_ERROR_Success;
}

A2C_ERROR A2C_EncodeDer(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm)
{
    unreferenced(ppcxt)

    return pdesc->pfnDerEncoder(pvData, pdesc, flags, NULL, NULL, pstm);
}

/* ---
/// <summary>
/// A2C_DecodeDer is the top level decode function for DER encoded objects.
/// <para>This function can be called in one of two different ways.  Either for an initial decode, or to
/// supply more data to continue the decode operation.</para>
/// <para>For the intial call, the data structure will be allocated and returned in ppv.
/// If continuation is possible, then a pointer should be passed in for ppcxt.  If no continuation is required,
/// ppcxt can be passed in as NULL.</para>
/// <para>For continuation calls, the returned ppv and ppcxt values should be returned again.  Additionally,
/// FLAGS_MORE_DATA is passed into the flags field.</para>
/// </summary>
/// <param name="ppv">returns data indirectly at this point</param>
/// <param name="pdesc">type descriptor for the item to be decoded</param>
/// <param name="flags">expected flags are: FLAGS_MORE_DATA</param>
/// <param name="ppcxt">if not NULL, then a context structure will be returned in the event that more data is
///  required to finish decoding the structure.</param>
/// <param name="pb">pointer to data to be decoded</param>
/// <param name="cb">count of bytes to be decoded</param>
/// <returns>error code.</returns>
--- */

A2C_ERROR A2C_DecodeDer(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb)
{
    return _A2C_Decode(TRUE, ppv, pdesc, flags, ppcxt, pb, cb);
}

A2C_ERROR A2C_EncodeBer(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm)
{
    unreferenced(ppcxt)

    return pdesc->pfnBerEncoder(pvData, pdesc, flags, NULL, NULL, pstm);
}

/* ---
/// <summary>
/// A2C_DecodeBer is the top level decode function for DER encoded objects.
/// <para>This function can be called in one of two different ways.  Either for an initial decode, or to
/// supply more data to continue the decode operation.</para>
/// <para>For the intial call, the data structure will be allocated and returned in ppv.
/// If continuation is possible, then a pointer should be passed in for ppcxt.  If no continuation is required,
/// ppcxt can be passed in as NULL.</para>
/// <para>For continuation calls, the returned ppv and ppcxt values should be returned again.  Additionally,
/// FLAGS_MORE_DATA is passed into the flags field.</para>
/// </summary>
/// <param name="ppv">returns data indirectly at this point</param>
/// <param name="pdesc">type descriptor for the item to be decoded</param>
/// <param name="flags">expected flags are: FLAGS_MORE_DATA</param>
/// <param name="ppcxt">if not NULL, then a context structure will be returned in the event that more data is
///  required to finish decoding the structure.</param>
/// <param name="pb">pointer to data to be decoded</param>
/// <param name="cb">count of bytes to be decoded</param>
/// <returns>error code.</returns>
--- */

A2C_ERROR A2C_DecodeBer(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb)
{
    return _A2C_Decode(FALSE, ppv, pdesc, flags, ppcxt, pb, cb);
}

/* ---
/// <summary>
/// A2C_Print creates a UTF8 character string that describes the contents of the structure
/// </summary>
/// <param name="pv"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <param name="pstm"> stream object that recieves the description </param>
/// <returns> error code value </returns>
--- */

A2C_ERROR A2C_Print(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    
    err = pdesc->pfnPrint(pv, pdesc, 0, pstm);
    return err;
}

/* ---
/// <summary>
/// A2C_Compare
/// </summary>
/// <param name="pvLeft"> pointer to the data structure</param>
/// <param name="pvRight"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <returns>0 if they are the same </returns>
--- */

int A2C_Compare(PCVOID pvLeft, PCVOID pvRight, PC_A2C_DESCRIPTOR pdesc)
{
    return pdesc->pfnCompare(pvLeft, pvRight, pdesc);
}

/* ---
/// <summary>
/// A2C_Copy
/// </summary>
/// <param name="pvDest"> pointer to the data structure</param>
/// <param name="pvSrc"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <returns>0 if they are the same </returns>
--- */

A2C_ERROR A2C_Copy(PVOID pvDest, PCVOID pvSrc, PC_A2C_DESCRIPTOR pdesc)
{
    return pdesc->pfnCopy(pvDest, pvSrc, pdesc);
}


/* ---
/// <summary>
/// Checks that the data in an element is legal.
/// </summary>
/// <param name="pv">pointer to the object to be validated</param>
/// <param name="pdesc">pointer to the descriptor of the object to be validated</param>
/// <returns>A2C_ERROR_Success if data is valid</returns>
--- */

A2C_ERROR A2C_Validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int * pf)
{
    return A2C_Validate2(pv, pdesc, NULL, pf, NULL);
}

/* ---
/// <summary>
/// Checks that the data in an element is validate, information describing the
/// error is placed on the stream.  Output is restricted to the UTF8 character set.
/// <para>
/// Only data on the first error is returned.  More errors can be obtained by passing
/// back the context information returned until the success code is returned.
/// </para>
/// </summary>
/// <param name="pv">pointer to the object to be validated</param>
/// <param name="pdesc">pointer to the descriptor of the object to be validated</param>
/// <param name="ppcxt">pointer to context information carried across calls</param>
/// <param name="pstm">pointer to stream to hold descriptive error</param>
/// <returns>A2C_ERROR_Success if the data is valid.</returns>
--- */

A2C_ERROR A2C_Validate2(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT ** ppcxt,
                        int * pf, A2C_STREAM * pstm)
{
    A2C_ERROR           err;
    A2C_CONTEXT *       pcxtLocal = NULL;
    
    /*
     *  Is this a first time call?
     */

    if ((ppcxt == NULL) || (*ppcxt == NULL)) {
        /*
         *   We don't need to grab any context information.
         */

        pcxtLocal = calloc(sizeof(A2C_CONTEXT), 1);
        if (pcxtLocal == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
        
    }
    else {
        /*
         *  Return call - use the passed in context information
         */

        pcxtLocal = *ppcxt;
        *ppcxt = NULL;
        pcxtLocal->iCurrent = 0;
    }

    /*
     *  Let's make the actual call in
     */

    err = pdesc->pfnValidate(pv, pdesc, pcxtLocal, pf, pstm);

    /*
     *  Depending on the error - cleanup the context stack
     */

    if ((err == A2C_ERROR_Success) && (*pf == FALSE) && (ppcxt != NULL)) {
        *ppcxt = pcxtLocal;
        pcxtLocal = NULL;
    }

ErrorExit:
    if (pcxtLocal != NULL) A2C_FreeContext(pcxtLocal);
    return err;
}


/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_Validate_Print(PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt,
                             const char * szType, char const * szError, A2C_STREAM * pstm)
{
    A2C_ERROR   err;
    int         i;
    int         f = FALSE;

    unreferenced(pcxt);
    unreferenced(pdesc);

    /*
     *  NOP if no stream is passed in
     */

    if (pstm == NULL) {
        return A2C_ERROR_Success;
    }
    
    /*
     *  Dump the error message onto the stream.
     *
     *  Line 1:  List of field names to this location
     *  Line 2:  Type being validated
     *  Line 3:  Error Text
     */

    /*
     *  Write out the list of field names
     */

    for (i=0; i<pcxt->cUsed; i++) {
        if (pcxt->rgCxts[i].pbData != NULL) {
            if (f) {
                err = pstm->pfnWrite(pstm, (PBYTE) ".", 1);
                if (err != A2C_ERROR_Success) return err;
            }
            err = pstm->pfnWrite(pstm, pcxt->rgCxts[i].pbData,
                                 strlen((char *) pcxt->rgCxts[i].pbData));
            if (err != A2C_ERROR_Success) return err;
            f = TRUE;
        }
    }
    if (!f) {
        err = pstm->pfnWrite(pstm, (PBYTE) "no path", 7);
        if (err != A2C_ERROR_Success) return err;
    }
            
    err = pstm->pfnWrite(pstm, (PBYTE) "\n", 1);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Write out the type of the object
     */
    
    err = pstm->pfnWrite(pstm, (PBYTE) szType, strlen(szType));
    if (err != A2C_ERROR_Success) return err;

    err = pstm->pfnWrite(pstm, (PBYTE) "\n", 1);
    if (err != A2C_ERROR_Success) return err;

    /*
     * Write out the text of the message
     */

    err = pstm->pfnWrite(pstm, (PBYTE) szError, strlen(szError));
    if (err != A2C_ERROR_Success) return err;

    err = pstm->pfnWrite(pstm, (PBYTE) "\n", 1);

    return err;
}

/* ---
/// <summary>
/// A2C_ContextFrame returns the current frame #.  This function MUST be called only once 
/// per layer as it increments the count of frames seen
/// </summary>
/// <param name="pcxt">pointer to context structure</param>
/// <returns>Frame number</returns>
--- */

int A2C_ContextFrame(A2C_CONTEXT * pcxt)
{
    int         i = pcxt->iCurrent;

    pcxt->iCurrent += 1;

    return i;
}

/* ---
/// <summary>
/// PUsh a new context frame onto the stack
/// </summary>
/// <param name="pcxt">pointer to the current context</param>
/// <param name="piFrame">returns the frame number</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_PushContext(A2C_CONTEXT * pcxt, int * piFrame)
{
    /*
     *  Don't push unless at the top of the stack
     */
    
    if (pcxt->iCurrent != pcxt->cUsed) return A2C_ERROR_ICE;

    /*
     *  Allocate more space if needed
     */

    if (pcxt->cUsed == pcxt->cAlloc) {
        A2C_CXT_ITEM * p;

        /*
         *  Try and grow the buffer
         */

        p = realloc(pcxt->rgCxts, sizeof(A2C_CXT_ITEM) * (pcxt->cAlloc + 10));
        if (p == NULL) return A2C_ERROR_outOfMemory;
        memset(&p[pcxt->cUsed], 0, 10*sizeof(A2C_CXT_ITEM));

        /*
         *  Succeeded - update the pointers
         */

        pcxt->cAlloc += 10;
        pcxt->rgCxts = p;
    }

    /*
     *  Increment the count and current - returning the current
     */

    *piFrame = pcxt->iCurrent;

    pcxt->iCurrent += 1;
    pcxt->cUsed += 1;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Push a new context onto a frame and initialize it with legal values
/// </summary>
/// <param name="pcxt">pointer to current context</param>
/// <param name="piFrame">returns the frame number</param>
/// <param name="szName">frame name</param>
/// <param name="iTag">tag number</param>
/// <returns> A2C_ERROR number</returns>
--- */

A2C_ERROR A2C_PushContext2(A2C_CONTEXT * pcxt, int * piFrame, char const * szName, int iTag)
{
    A2C_ERROR   err;

    err = A2C_PushContext(pcxt, piFrame);
    if (err != A2C_ERROR_Success) return err;
    
    /*
     *  Save the passed in items
     */

    pcxt->rgCxts[*piFrame].pbData = (PBYTE) szName;
    pcxt->rgCxts[*piFrame].iItem = iTag;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// A2C_PopContext removes the top context from the stack and released the data
/// </summary>
/// <param name="pcxt">pointer to context information</param>
/// <param name="iFrame">frame number we are poping off</param>
/// <returns>none</param>
--- */

A2C_ERROR A2C_PopContext(A2C_CONTEXT * pcxt, int iFrame)
{
    A2C_CXT_ITEM *      pitm = &pcxt->rgCxts[iFrame];
    
    /*
     *  Is it the correct frame number?
     */

    if (pcxt->iCurrent-1 != iFrame) return A2C_ERROR_ICE;

    /*
     *  free any data running around
     */

    if (pitm->cbData != 0) {
        free(pitm->pbData);
    }

    pcxt->iCurrent -= 1;
    pcxt->cUsed -= 1;

    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// Get the context number
/// </summary>
/// <param name="pcxt">pointer to context information</param>
/// <param name="iFrame">frame to retrieve the tag from </param>
/// <returns>tag number</returns>
--- */

int A2C_ContextNum(A2C_CONTEXT * pcxt, int iFrame)
{
    /*
     *  Check for out of range error
     */
    
    if ((iFrame < 0) || (iFrame > pcxt->cUsed)) return -1;

    return pcxt->rgCxts[iFrame].iItem;
}

/* ---
/// <summary>
/// Set the context number for a given frame
/// </summary>
/// <param name="pcxt">pointer to context info</param>
/// <param name="iFrame">frame number to set</param>
/// <param name="iTag">value to be set</param>
/// <returns>A2C_ERROR</param>
--- */

A2C_ERROR A2C_SetContextNum(A2C_CONTEXT * pcxt, int iFrame, int iTag)
{
    /*
     *  Check for out of range error
     */

    if ((iFrame < 0) || (iFrame > pcxt->cUsed)) return -1;

    /*
     *  Save the value
     */

    pcxt->rgCxts[iFrame].iItem = iTag;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Set the context name for a given frame
/// </summary>
/// <param name="pcxt">pointer to context info</param>
/// <param name="iFrame">frame number to set</param>
/// <param name="szName">value to be set</param>
/// <returns>A2C_ERROR</param>
--- */

A2C_ERROR A2C_SetContextName(A2C_CONTEXT * pcxt, int iFrame, char const * szName)
{
    /*
     *  Check for out of range error
     */

    if ((iFrame < 0) || (iFrame > pcxt->cUsed)) return -1;

    /*
     *  Save the value
     */

    pcxt->rgCxts[iFrame].pbData = (PBYTE) szName;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check to see if we are on the top of the stack
/// </summary>
/// <param name="pcxt">pointer to context information<param>
/// <returns> TRUE if at the top of the stack</returns>
--- */

int A2C_ContextTOS(A2C_CONTEXT * pcxt)
{
    return pcxt->iCurrent == pcxt->cUsed;
}

/* ---
/// <summary>
/// Free all data related to a context structure
/// </summary>
--- */

A2C_ERROR A2C_FreeContext(A2C_CONTEXT * pcxt)
{
    if (pcxt->rgCxts != NULL) {
        /* M00BUG - free internals */
        free(pcxt->rgCxts);
    }

    if (pcxt->pbBuffer != NULL) free(pcxt->pbBuffer);
    if (pcxt->rgpbRoots != NULL) free(pcxt->rgpbRoots);
    
    free(pcxt);
    
    return A2C_ERROR_Success;
}

A2C_ERROR A2C_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames)
{
    *pNames = &pdesc->pItemDescription->names;

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi)
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

A2C_ERROR A2C_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz)
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
