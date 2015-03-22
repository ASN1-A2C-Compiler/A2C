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


A2C_ITEM_HEADER A2C_OPEN_TYPE_description = {
    sizeof(A2C_OPEN_TYPE), 0
};

A2C_DESCRIPTOR A2C_OPEN_TYPE_descriptor = {
    A2C_zero,
    A2C_OPEN_TYPE_release,
    A2C_OPEN_TYPE_decode_der,
    A2C_OPEN_TYPE_encode_der,
    A2C_OPEN_TYPE_decode_ber,
    A2C_OPEN_TYPE_encode_ber,
    A2C_OPEN_TYPE_compare,
    A2C_OPEN_TYPE_copy,
    A2C_OPEN_TYPE_print,
    A2C_OPEN_TYPE_validate,
    &A2C_OPEN_TYPE_description
};

A2C_ITEM_HEADER A2C_OCTET_OPEN_TYPE_description = {
    sizeof(A2C_OCTET_OPEN_TYPE), 0
};

A2C_DESCRIPTOR A2C_OCTET_OPEN_TYPE_descriptor = {
    A2C_zero,
    A2C_OCTET_OPEN_TYPE_release,
    A2C_OCTET_OPEN_TYPE_decode_der,
    A2C_OCTET_OPEN_TYPE_encode_der,
    A2C_OCTET_OPEN_TYPE_decode_ber,
    A2C_OCTET_OPEN_TYPE_encode_ber,
    A2C_OCTET_OPEN_TYPE_compare,
    A2C_OCTET_OPEN_TYPE_copy,
    A2C_OCTET_OPEN_TYPE_print,
    A2C_OCTET_OPEN_TYPE_validate,
    &A2C_OCTET_OPEN_TYPE_description
};


/* ---
/// <summary>
/// release any data allocated
/// </summary>
/// <param name="pv">pointer to the open type value</param>
/// <param name="pdesc">pointr to the descriptor table</param>
/// <returns>A2C_ERROR_Sucess</returns>
--- */

A2C_ERROR A2C_OPEN_TYPE_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_OPEN_TYPE * pData = (A2C_OPEN_TYPE *) pv;
    
    unreferenced(pdesc);

    /*
     *  If we have an object pointer - recurse down and release things
     */
    
    if (pData->objectPtr != NULL) {
        pData->functionPtr->pfnRelease(pData->objectPtr, pData->functionPtr);

        free(pData->objectPtr);
    }

    /*
     *  If we have encoded data - release the memory buffer
     */

    if (pData->data != NULL) {
        free(pData->data);
    }

    return A2C_ERROR_Success;
}


/*
 *  Read in the buffer.  We don't know how to decode this, so the best we can do is to
 *      copy the entire blob buffer out and pass it back.  Data should start with TL.
 */

A2C_ERROR A2C_OPEN_TYPE_decode_der(PVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                   A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    int         err;
    int         fConstructed;
    int         iClass;
    A2C_TAG_VALUE iValue;
    A2C_OPEN_TYPE * pos = (A2C_OPEN_TYPE *) pv;

    unreferenced(flags);
    unreferenced(pdesc);
    unreferenced(pcxt);

    /*
     *  It is an error to come in with a tag, since the tag should always be an explicit tag
     */

    if (ptag != NULL) {
        return A2C_ERROR_ICE;
    }

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
 *  Write out a
 */

A2C_ERROR A2C_OPEN_TYPE_encode_der(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OPEN_TYPE const * pos = (A2C_OPEN_TYPE const *) pv;
    
    unreferenced(pdesc);

    /*
     *  Use the decode data structure if it exists
     */

    if (pos->objectPtr != NULL) {
        return pos->functionPtr->pfnDerEncoder(pos->objectPtr, pos->functionPtr, flags, pcxt, ptag, pstm);
    }

    /*
     *  Write out the bytes
     */
    
    return _A2C_write_bytes(pos->data, pos->length, pstm);
}

A2C_ERROR A2C_OPEN_TYPE_decode_ber(PVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                   A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_BINARY  bin = {0};
    A2C_ERROR   err;
    A2C_OPEN_TYPE * pos = (A2C_OPEN_TYPE *) pv;
    
    /*
     *  If we got here, then we are basically treating this as a binary blob
     */

    err = A2C_BINARY_decode_ber(&bin, pdesc, flags, pcxt, ptag, pstm);
    if (err < A2C_ERROR_Success) return err;

    /*
     *  Copy over the results and return
     */

    pos->length = bin.length;
    pos->data = bin.data;

    return A2C_ERROR_Success;
}

int A2C_OPEN_TYPE_encode_ber(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    A2C_OPEN_TYPE const * pos = (A2C_OPEN_TYPE const *) pv;

    unreferenced(pdesc);

    /*
     *  If the data is below us - push it out
     */

    if (pos->objectPtr != 0) {
        return pos->functionPtr->pfnBerEncoder(pos->objectPtr, pos->functionPtr, flags, pcxt, ptag, pstm);
    }

    /*
     *  Write out the bytes
     */
    
    cb = _A2C_write_bytes(pos->data, pos->length, pstm);
    if (cb < 0) {
        return cb;
    }
    return cb;
}


/* ---
/// <summary>
/// A2C_OPEN_TYPE_print is the default open type value print routine.
/// <para>If the type is fully defined, then it is recursively called in order to
/// do the print out.  If the system only has the bytes then they are dumped.</para>
/// </summary>
/// <param name="pf">pointer to the open type value</param>
/// <param name="pdesc">type descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>A2C_ERROR_Success on success, A2C_ERROR on failure.</return>
--- */


A2C_ERROR A2C_OPEN_TYPE_print(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;
    A2C_OPEN_TYPE const * pot = (A2C_OPEN_TYPE const *) pv;

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

int A2C_OPEN_TYPE_compare(PCVOID pvLeft, PCVOID pvRight, PC_A2C_DESCRIPTOR pdesc)
{
    int  i;
    A2C_OPEN_TYPE const * pLeft = (A2C_OPEN_TYPE const *) pvLeft;
    A2C_OPEN_TYPE const * pRight = (A2C_OPEN_TYPE const *) pvRight;

    unreferenced(pdesc);
    
    /*  Are they both encoded data? */
    /*  If so do the standard comparision */

    if (pLeft->data != NULL) {
        if (pRight->data == NULL) return -1;

        i = pRight->length - pLeft->length;
        if (i != 0) return i;

        if (i > 0) {
            i = memcmp(pRight->data, pLeft->data, pLeft->length);
            if (i == 0) {
                i = 1;
            }
            return i;
        }
        
        i = memcmp(pRight->data, pLeft->data, pRight->length);
        if (i == 0) {
            i = -1;
        }
        return i;
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

A2C_ERROR A2C_OPEN_TYPE_copy(PVOID pvLeft, PCVOID pvRight, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_ERROR   err;
    A2C_OPEN_TYPE * pLeft = (A2C_OPEN_TYPE *) pvLeft;
    A2C_OPEN_TYPE const * pRight = (A2C_OPEN_TYPE const *) pvRight;

    unreferenced(pdesc);
    
    /*  If source is encoded, copy data in encoded format */

    if (pRight->data != NULL) {
        pLeft->data = malloc(pRight->length);
        if (pLeft->data == NULL) return A2C_ERROR_outOfMemory;

        memcpy(pLeft->data, pRight->data, pRight->length);
        pLeft->length = pRight->length;

        pLeft->functionPtr = NULL;
        pLeft->objectPtr = NULL;

        return A2C_ERROR_Success;
    }

    A2C_Alloc(&pLeft->objectPtr, pRight->functionPtr);
    if (pLeft->objectPtr == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    err = pRight->functionPtr->pfnCopy(pLeft->objectPtr, pRight->objectPtr, pRight->functionPtr);
    if (err >= A2C_ERROR_Success) {
        pLeft->functionPtr = pRight->functionPtr;
    }

    pLeft->data = NULL;
    pLeft->length = 0;

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

A2C_ERROR A2C_OPEN_TYPE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    unreferenced(pv);
    unreferenced(pdesc);
    unreferenced(pcxt);
    unreferenced(pstm);
    
    *pf = TRUE;
    return A2C_ERROR_Success;
}

/****************************************** UTILITY FUNCTIONS ********************************************************/

/* ---
/// <summary>
/// </sumarry>
/// <param name="pbItem">points to current field being decoded</param>
/// <param name="pcr">component relation table to evaluate</param>
/// 
--- */

const A2C_DESCRIPTOR * A2C_LookupType(const A2C_COMPONENT_RELATIONS * pcr, const A2C_CONTEXT * pcxt)
{
    int                 cComponents = pcr->c;
    int                 cItems = pcr->pos->pdesc->cChildren;
    int                 cObjects = pcr->pos->c;
    int                 fUnique = TRUE;
    int                 i;
    int		        i1;
    int		        iReturn = 0;
    const A2C_ITEM *    rgItems = pcr->pos->pdesc->rgChildren;
    struct {
        const A2C_DESCRIPTOR *  pfn;
        int                     cbOffComp;
        int                     cbOffObj;
        int			iLayer;
        PBYTE			pb;
    }                   rgComponents[10];

    /*  Walk the array of components and do the setup */

    for (i=0; i<cComponents; i++) {
        /*  Look for this component in the list of object fields */
        
        for (i1=0; i1<cItems; i1++) {
            if (strcmp(pcr->rg[i].szName, rgItems[i1].szFieldName) == 0) {
                /* We found the component - grab the data we care about */
                
                rgComponents[i].pfn = rgItems[i1].pdu;
                rgComponents[i].cbOffComp = pcr->rg[i].cbRelOffset;
                rgComponents[i].cbOffObj = rgItems[i1].cbOffset;
                rgComponents[i].iLayer = pcxt->iRoot - 1 - pcr->rg[i].iLayer;

                rgComponents[i].pb = pcxt->rgpbRoots[pcxt->iRoot - 1 - pcr->rg[i].iLayer]+pcr->rg[i].cbRelOffset;
                
                if ((rgItems[i1].flags & A2C_FLAGS_UNIQUE) == 0) {
                    fUnique = 0;
                }
                break;
            }
        }

        /*  We did not find the component in the field list - return failure */
        
        if (i1 == cItems) {
            return (A2C_DESCRIPTOR *) NULL;
        }
    }

    /*  Walk to find the returned item */

    for (i=0; i<cItems; i++) {
        if (strcmp(pcr->szField, rgItems[i].szFieldName) == 0) {
            iReturn = i;
            break;
        }
    }

    if (i == cItems) {
        return NULL;
    }

    /*  Now walk through the object set looking for this item */

    for (i=0; i<cObjects; i++) {
        for (i1=0; i1<cComponents; i1++) {
            if (rgComponents[i1].pfn->pfnCompare(rgComponents[i1].pb,
                                                  *((PBYTE *)(((PBYTE) pcr->pos->rg[i])+rgComponents[i1].cbOffObj)),
                                                  rgComponents[i1].pfn) != 0) {
                break;
            }
        }
        if (i1 == cComponents) {
            /*  Matched */

            if (fUnique) {
                return (const A2C_DESCRIPTOR *) (*(PBYTE *)(((PBYTE) pcr->pos->rg[i])+rgItems[iReturn].cbOffset));
            }
        }
    }

    /*  No matching item was found */

    return NULL;
}

A2C_ERROR A2C_RELATION_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                              A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR * pfn;

    if (flags & A2C_FLAGS_MORE_DATA) {
        /*
         *  If we don't have a function pointer - then we must not understand the
         *      underlying type - treat as binary data
         */

        if (pos->functionPtr == NULL) {
            return A2C_OPEN_TYPE_decode_der(pos, pdesc, flags, pcxt, ptag, pstm);
        }

        
    }
    else {
        /*
         * Can't have a tag passed in here - only explcit tags are allowed on
         *   open types
         */

        if (ptag != NULL) {
            return A2C_ERROR_ICE;
        }

        /*
         *  Check to see if we understand the underlying type
         */
    
        pfn = A2C_LookupType(pInfo, pcxt);

        /*
         * Use the OPEN TYPE if we don't know what it should be
         */

        if (pfn == NULL) {
            return A2C_OPEN_TYPE_decode_der(pos, pdesc, flags, pcxt, ptag, pstm);
        }

        /*
         * Allocate and init
         *
         *  There is a chance that we are coming back in - since these should have been
         *      zeroed out to start with - assume that we have set them if the are 
         *      now non-zero.
         */

        if (pos->functionPtr == NULL) {
            A2C_Alloc(&pos->objectPtr, pfn);
            if (pos->objectPtr == NULL) {
                return A2C_ERROR_outOfMemory;
            }

            /*  Now call the decoder for the item */

            pos->functionPtr = pfn;
        }
    }

    /*
     *  pass on to the next level for decoding
     */
    
    return pos->functionPtr->pfnDerDecoder(pos->objectPtr, pos->functionPtr, flags, pcxt, NULL, pstm);
}

A2C_ERROR A2C_RELATION_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                              A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    const A2C_DESCRIPTOR * pfn;

    if (flags & A2C_FLAGS_MORE_DATA) {
        /*
         *  If we don't have a function pointer - then we must not understand the
         *      underlying type - treat as binary data
         */

        if (pos->functionPtr == NULL) {
            return A2C_OPEN_TYPE_decode_ber(pos, pdesc, flags, pcxt, ptag, pstm);
        }

        
    }
    else {
        /*
         * Can't have a tag passed in here - only explcit tags are allowed on
         *   open types
         */

        if (ptag != NULL) {
            return A2C_ERROR_ICE;
        }

        /*
         *  Check to see if we understand the underlying type
         */
    
        pfn = A2C_LookupType(pInfo, pcxt);

        /*  Use the OPEN TYPE if we don't know what it should be */

        if (pfn == NULL) {
            return A2C_OPEN_TYPE_decode_ber(pos, pdesc, flags, pcxt, ptag, pstm);
        }

        /*
         * Allocate and init
         *
         *  There is a chance that we are coming back in - since these should have been
         *      zeroed out to start with - assume that we have set them if the are 
         *      now non-zero.
         */

        if (pos->functionPtr == NULL) {
            A2C_Alloc(&pos->objectPtr, pfn);
            if (pos->objectPtr == NULL) {
                return A2C_ERROR_outOfMemory;
            }

            /*  Now call the decoder for the item */

            pos->functionPtr = pfn;
        }
    }
    
    /*
     *  pass on to the next level for decoding
     */
    
    return pos->functionPtr->pfnBerDecoder(pos->objectPtr, pos->functionPtr, flags, pcxt, NULL, pstm);
}


/* ---
/// <summary>
/// release any data allocated
/// </summary>
/// <param name="pv">pointer to the open type value</param>
/// <param name="pdesc">pointr to the descriptor table</param>
/// <returns>A2C_ERROR_Sucess</returns>
--- */

A2C_ERROR A2C_OCTET_OPEN_TYPE_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_OPEN_TYPE_release(pv, pdesc);
}

/* ---
/// <summary>
/// Decode the data - it has an octet string wrapper on it
/// </summary>
/// <param name="pv">pointer to the data object</param>
/// <param name="pdesc">pointer to object descriptor</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">source bytes for the decoder</param>
/// <returns>A2C_ERROR_Success on success, else error code</returns>
--- */

A2C_ERROR A2C_OCTET_OPEN_TYPE_decode_der(PVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags,
                                         A2C_CONTEXT * pcxt, A2C_TAG const * ptag,
                                         A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR   err;
    A2C_OCTET_OPEN_TYPE *     pobj = (A2C_OCTET_OPEN_TYPE *) pv;

    /*
     *  Pass it off to the octet string parser - it is an octet string containing the blob
     */

    err = A2C_OCTET_STRING_decode_der((PVOID) &pobj->length, pdesc, flags, pcxt, ptag, pstm);

    /*
     *  We don't now how to do any further processing - so just return the result
     */

    return err;
}

/* ---
/// <summary>
/// Encode this data structure and then wrap it in an OCTET STRING.
/// </summary>
/// <param name="pf">pointer to boolean value</param>
/// <param name="pdesc">pointer to descriptor for item</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">stream to write the encoded bytes on</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_OCTET_OPEN_TYPE_encode_der(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                         A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;
    A2C_OCTET_OPEN_TYPE const * pot = (A2C_OCTET_OPEN_TYPE const *) pv;

    unreferenced(pdesc);
    
    /*
     *  If it is a structure then encoded it
     */

    if (pot->objectPtr != NULL) {
        A2C_ERROR               err;
        A2C_STREAM_MEMORY       stm;

        /*
         *  Someplace to put the data
         */
        
        _A2C_Memory_Init(&stm);

        /*
         *  Do the encode of the embedded content
         */

        err = A2C_OCTET_STRING_encode_der((PVOID) &pot->length, &A2C_OCTET_STRING_descriptor, flags, pcxt,
                                          ptag, (PA2C_STREAM) &stm);
        if (err != A2C_ERROR_Success) {
            _A2C_Memory_Cleanup(&stm);
            return err;
        }

        err = _A2C_Memory_GetData(&stm, &os.data, &os.length);
        if (err != A2C_ERROR_Success) {
            _A2C_Memory_Cleanup(&stm);
            return err;
        }

        _A2C_Memory_Cleanup(&stm);
    }
    
    /*
     *  Otherwise the the provided value
     */
    
    else {
        os.length = pot->length;
        os.data = pot->data;
    }

    /*
     *  Now encode the actual data in an octet string and push it out onto the stream
     */
    
    return A2C_OCTET_STRING_encode_der(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// Decode the data - it has an octet string wrapper on it
/// </summary>
/// <param name="pv">pointer to the data object</param>
/// <param name="pdesc">pointer to object descriptor</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">source bytes for the decoder</param>
/// <returns>A2C_ERROR_Success on success, else error code</returns>
--- */

A2C_ERROR A2C_OCTET_OPEN_TYPE_decode_ber(PVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags,
                                         A2C_CONTEXT * pcxt, A2C_TAG const * ptag,
                                         A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR   err;
    A2C_OCTET_OPEN_TYPE *     pobj = (A2C_OCTET_OPEN_TYPE *) pv;

    /*
     *  Pass it off to the octet string parser - it is an octet string containing the blob
     */

    err = A2C_OCTET_STRING_decode_ber((PVOID) &pobj->length, pdesc, flags, pcxt, ptag, pstm);

    /*
     *  We don't now how to do any further processing - so just return the result
     */

    return err;
}

/* ---
/// <summary>
/// Encode this data structure and then wrap it in an OCTET STRING.
/// </summary>
/// <param name="pf">pointer to boolean value</param>
/// <param name="pdesc">pointer to descriptor for item</param>
/// <param name="flags">control flags for the decoder</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">correct tag information for this type.  Use defaults if NULL</param>
/// <param name="pstm">stream to write the encoded bytes on</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_OCTET_OPEN_TYPE_encode_ber(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                         A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;
    A2C_OCTET_OPEN_TYPE const * pot = (A2C_OCTET_OPEN_TYPE const *) pv;

    unreferenced(pdesc);
    
    /*
     *  If it is a structure then encoded it
     */

    if (pot->objectPtr != NULL) {
        A2C_ERROR               err;
        A2C_STREAM_MEMORY       stm;

        /*
         *  Someplace to put the data
         */
        
        _A2C_Memory_Init(&stm);

        /*
         *  Do the encode of the embedded content
         */

        err = A2C_OCTET_STRING_encode_ber((PVOID) &pot->length, &A2C_OCTET_STRING_descriptor, flags, pcxt,
                                          ptag, (PA2C_STREAM) &stm);
        if (err != A2C_ERROR_Success) {
            _A2C_Memory_Cleanup(&stm);
            return err;
        }

        err = _A2C_Memory_GetData(&stm, &os.data, &os.length);
        if (err != A2C_ERROR_Success) {
            _A2C_Memory_Cleanup(&stm);
            return err;
        }

        _A2C_Memory_Cleanup(&stm);
    }
    
    /*
     *  Otherwise the the provided value
     */
    
    else {
        os.length = pot->length;
        os.data = pot->data;
    }

    /*
     *  Now encode the actual data in an octet string and push it out onto the stream
     */
    
    return A2C_OCTET_STRING_encode_ber(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
}


/* ---
/// <summary>
/// A2C_OPEN_TYPE_print is the default open type value print routine.
/// <para>If the type is fully defined, then it is recursively called in order to
/// do the print out.  If the system only has the bytes then they are dumped.</para>
/// </summary>
/// <param name="pf">pointer to the open type value</param>
/// <param name="pdesc">type descriptor</param>
/// <param name="iLevel">indentation level</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>A2C_ERROR_Success on success</return>
--- */


int A2C_OCTET_OPEN_TYPE_print(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return A2C_OPEN_TYPE_print(pv, pdesc, iLevel, pstm);
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

int A2C_OCTET_OPEN_TYPE_compare(PCVOID pvLeft, PCVOID pvRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_OPEN_TYPE_compare(pvLeft, pvRight, pdesc);
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

A2C_ERROR A2C_OCTET_OPEN_TYPE_copy(PVOID pvLeft, PCVOID pvRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_OPEN_TYPE_copy(pvLeft, pvRight, pdesc);
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

A2C_ERROR A2C_OCTET_OPEN_TYPE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    return A2C_OPEN_TYPE_validate(pv, pdesc, pcxt, pf, pstm);
}

A2C_ERROR A2C_RELATION_OCTET_decode_der(A2C_COMPONENT_RELATIONS * pInfo, A2C_OCTET_OPEN_TYPE * pot, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                              A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR           err;
    const A2C_DESCRIPTOR * pfn;
    A2C_OCTET_STRING    os = {0};
    A2C_STREAM_MEMORY   stm;

    unreferenced(pdesc);

    /*
     *  Start by doing the decode operation until it is completed
     */

    err = A2C_OCTET_STRING_decode_der(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Look for the decode type
     */

    pfn = A2C_LookupType(pInfo, pcxt);

    /*  Use the OPEN TYPE if we don't know what it should be */

    if (pfn == NULL) {
        pot->length = os.length;
        pot->data = os.data;

        return A2C_ERROR_Success;
    }

    /*  Allocate and init */

    A2C_Alloc(&pot->objectPtr, pfn);
    if (pot->objectPtr == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    /*  Now call the decoder for the item */

    _A2C_Memory_Init_With_Data(&stm, os.data, os.length);
    
    pot->functionPtr = pfn;
    err = pfn->pfnDerDecoder(pot->objectPtr, pfn, flags, pcxt, NULL, &stm);

    A2C_OCTET_STRING_release(&os, &A2C_OCTET_STRING_descriptor);

    return err;
}

A2C_ERROR A2C_RELATION_OCTET_decode_ber(A2C_COMPONENT_RELATIONS * pInfo, A2C_OCTET_OPEN_TYPE * pot, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, 
                                              A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_ERROR           err;
    const A2C_DESCRIPTOR * pfn;
    A2C_OCTET_STRING    os = {0};
    A2C_STREAM_MEMORY   stm;

    unreferenced(pdesc);

    /*
     *  Start by doing the decode operation until it is completed
     */

    err = A2C_OCTET_STRING_decode_der(&os, &A2C_OCTET_STRING_descriptor, flags, pcxt, ptag, pstm);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Look for the decode type
     */

    pfn = A2C_LookupType(pInfo, pcxt);

    /*  Use the OPEN TYPE if we don't know what it should be */

    if (pfn == NULL) {
        pot->length = os.length;
        pot->data = os.data;

        return A2C_ERROR_Success;
    }

    /*  Allocate and init */

    A2C_Alloc(&pot->objectPtr, pfn);
    if (pot->objectPtr == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    /*  Now call the decoder for the item */

    _A2C_Memory_Init_With_Data(&stm, os.data, os.length);
    
    pot->functionPtr = pfn;
    err = pfn->pfnBerDecoder(pot->objectPtr, pfn, flags, pcxt, NULL, &stm);

    A2C_OCTET_STRING_release(&os, &A2C_OCTET_STRING_descriptor);

    return err;
}

