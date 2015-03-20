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

extern A2C_ERROR A2C_SEQUENCE_OF_decode_common(int fDER, A2C_SEQUENCE_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                               A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm);


#define SORT(a, b, c) qsort(a, b, sizeof(a[0]), c);

typedef struct {
    BYTE *      pb;
    A2C_LENGTH  cb;
} Element;

const A2C_TAG tagSet = {A2C_TAG_CLASS_UNIVERSAL, 17};

#define TRUE 1
#define FALSE 0

/*
 *  compare - comparison sort function
 */

int __cdecl _compare_set_of(const void * a, const void * b)
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

/* ---
/// <summary>
/// </summary>
--- */

A2C_ERROR A2C_SET_OF_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_SET_OF * pData = (A2C_SET_OF *) pv;

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

A2C_ERROR A2C_SET_OF_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;    
    int                 iItem;
    PBYTE               pb;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;
    A2C_SET_OF *        pData = (A2C_SET_OF *) pv;


    pb = (PBYTE) pData->array;
    
    for (iItem=0; iItem<pData->count; iItem++, pb += cbStruct) {

        rgItems[0].pdu->pfnRelease(pb, rgItems[0].pdu);
    }

    free(pData->array);

    return A2C_ERROR_Success;
}
        

/*
 *  Fun
 */
   
A2C_ERROR A2C_SET_OF_decode_der(A2C_SET_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &tagSet;
    }

    return A2C_SEQUENCE_OF_decode_common(TRUE, pData, pdesc, flags, pcxt, ptag, pstm);
}

/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_SET_OF_encode_der(A2C_SET_OF const * data, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int		        cb;
    int                 cbSet = 0;
    int                 cbStruct = pdesc->pItemDescription->rgChildren[0].pdu->pItemDescription->cbStruct;
    int                 cbTag = 0;
    A2C_ERROR           err = A2C_ERROR_Success;
    int		        iItem;
    BYTE *	        pbData;
    A2C_STREAM_MEMORY   stm = {0};
    A2C_TAG const *     ptagLocal;
    Element *           rgdata = 0;
    A2C_ITEM const *    rgItems = pdesc->pItemDescription->rgChildren;;

    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 17 & constucted
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
     *  Allocate the results array
     */

    rgdata = (Element *) calloc(data->count, sizeof(Element));
    if (rgdata == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
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
        
    _A2C_Memory_Init(&stm);

    for (iItem = 0; iItem<data->count; iItem++, pbData+= cbStruct) {

        /*
         *  Call the encoder for this item
         */

        err = rgItems[0].pdu->pfnDerEncoder(pbData, rgItems[0].pdu, flags, pcxt, ptagLocal, (PA2C_STREAM) &stm);
        if (err < 0) {
            goto ErrorExit;
        }

        /*
         *  Get the acutal bytes for a later sort
         */

        _A2C_Memory_GetData(&stm, &rgdata[iItem].pb, &rgdata[iItem].cb);


        /*
         *  Running total of bytes
         */
        
        cbSet += rgdata[iItem].cb;
    }

    /*
     *  Now we know the final size - emit that
     */

    err = _A2C_write_length(cbSet, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Sort the arrays of data
     */

    SORT(rgdata, data->count, _compare_set_of);

    /*
     *  Now copy the data out
     */
    
    for (iItem = 0; iItem < data->count; iItem++) {
        err = pstm->pfnWrite(pstm, rgdata[iItem].pb, rgdata[iItem].cb);
        if (err < 0) {
            goto ErrorExit;
        }
    }
    

    /*
     *  Return count of bytes encoded
     */
    
ErrorExit:
    _A2C_Memory_Cleanup(&stm);
    if (rgdata != NULL) {
        for (iItem=0; iItem<data->count; iItem++) {
            if (rgdata[iItem].pb != NULL) free(rgdata[iItem].pb);
        }
        free(rgdata);
    }
            
    return err;
}

A2C_ERROR A2C_SET_OF_decode_ber(A2C_SET_OF * pData, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                A2C_CONTEXT * pcxt, A2C_TAG const *ptag, A2C_STREAM_MEMORY * pstm)
{
    if (ptag == NULL) {
        ptag = &tagSet;
    }
    
    return A2C_SEQUENCE_OF_decode_common(FALSE, pData, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_SET_OF_encode_ber(A2C_SET_OF const * data, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*
     *  Since we don't sort, SET and SEQ are the same
     */

    if (ptag == NULL) {
        ptag = &tagSet;
    }
    
    return A2C_SEQUENCE_OF_encode_ber(data, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_SET_OF_print(A2C_SET_OF const * pdata, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm) 
{
    return A2C_SEQUENCE_OF_print(pdata, pdesc, iLevel, pstm);
}

int A2C_SET_OF_compare(A2C_SET_OF const * pfLeft, A2C_SET_OF const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_SEQUENCE_OF_compare(pfLeft, pfRight, pdesc);
}

A2C_ERROR A2C_SET_OF_copy(A2C_SET_OF * pseqLeft, A2C_SET_OF const * pseqRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_SEQUENCE_OF_copy(pseqLeft, pseqRight, pdesc);
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

A2C_ERROR A2C_SET_OF_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    return A2C_SEQUENCE_OF_validate(pv, pdesc, pcxt, pf, pstm);
}
