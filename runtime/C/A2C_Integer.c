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
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "A2C_Stream_Internal.h"

/*
 *
 */

A2C_CONST A2C_ITEM_HEADER A2C_INTEGER_HUGE_description = {
    sizeof(A2C_INTEGER_HUGE), 0
};

A2C_CONST A2C_ITEM_HEADER A2C_INTEGER_NATIVE_description = {
    sizeof(A2C_INTEGER_NATIVE), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_INTEGER_HUGE_descriptor = {
    A2C_INTEGER_HUGE_init,
    A2C_INTEGER_HUGE_release,
    (A2C_decoder_f) A2C_INTEGER_HUGE_decode_der,
    (A2C_encoder_f) A2C_INTEGER_HUGE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_HUGE_decode_ber,
    (A2C_encoder_f) A2C_INTEGER_HUGE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_HUGE_compare,
    (A2C_copy_f) A2C_INTEGER_HUGE_copy,
    (A2C_print_f) A2C_INTEGER_HUGE_print,
    A2C_INTEGER_HUGE_validate,
    &A2C_INTEGER_HUGE_description
};

A2C_CONST A2C_DESCRIPTOR A2C_INTEGER_NATIVE_descriptor = {
    A2C_zero,
    A2C_nop,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der,
    (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber,
    (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    A2C_INTEGER_NATIVE_validate,
    &A2C_INTEGER_NATIVE_description
};

/*
 *  IntToRgb
 *
 *  Description:
 *      Convert a native integer to a network ordered byte array of data.
 *      This function assumes that the data is SIGNED.
 */

static void IntToRgb(int iData, BYTE rgb[])
{
    int i;

    for (i=sizeof(iData)-1; i>=0; i--) {
        rgb[i] = (BYTE) (iData & 0xff);
        iData >>= 8;
    }
    return;
}

/*
 *  RgbToInt
 *
 *  Description:
 *      Convert a network ordered byte array into a native integer
 *      Assume that array is unsigned.
 */

static int RgbToInt(BYTE rgb[], int cb, int * piData)
{
    int         i;
    int         v = 0;

    /*
     *  Do a sign extension copy to a temp buffer
     */
    
    if (rgb[0] & 0x80) {
        v = -1;
    }

    /*
     *  Now put the bytes into the integer
     */

    for (i=0; i< cb; i++) {
        v <<= 8;
        v |= rgb[i];
    }
    
    *piData = v;
    return 1;
}

/* ---
/// <summary>
/// Set to a zero value
/// </summary>
/// <param name="pv">pointer to structure</param>
/// <param name="pdesc">pointer to object descriptor</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_INTEGER_HUGE_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_INTEGER_HUGE *  pi = (A2C_INTEGER_HUGE *) pv;

    unreferenced(pdesc);

#ifndef NO_INIT
    pi->hData = calloc(0, 1);
    if (pi->hData != NULL) pi->hLength = 1;
#endif 
    
    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// Free the buffer if necessary
/// </summary>
/// <param name="pv">pointer to structure</param>
/// <param name="pdesc">pointer to object descriptor</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_INTEGER_HUGE_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_INTEGER_HUGE *  pi = (A2C_INTEGER_HUGE *) pv;

    unreferenced(pdesc);

    if (pi->hData != NULL) {
        free(pi->hData);
    }
    return A2C_ERROR_Success;
}
    

/* ---
/// <summary>
/// A2C_INTEGER_HUGE_decode_der is the default der integer decoder routine.  This uses the
/// long form of the integer.
/// <para>This function does not allow restarting of scanning.</para>
/// </summary>
/// <param name="pi">pointer to integer data</param>
/// <param name="pdesc">descriptor for the data</param>
/// <param name="flags">control flags</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">override implicit tag</param>
/// <param name="pstm">source bytes to decode from</param>
--- */

A2C_ERROR A2C_INTEGER_HUGE_decode_der(A2C_INTEGER_HUGE * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                 A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    A2C_ERROR   err;
    int         iClass;
    A2C_TAG_VALUE  iValue;
    int         fConstructed;

    /*
     *  We don't use these parameter
     */

    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);

    /*
     *  Grab the type and length of this item.  This function will without advancing
     *  the pointer if there are not sufficent bytes for the value in the buffer.
     */

    err = _A2C_get_tag_and_length(pstm, &iClass, &fConstructed, &iValue, &cbLength, &cbTL);
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Compare the acutal and expected tags.  It may be passed in or uses the default ones.
     */

    if (ptag != NULL) {
        if ((ptag->iClass != iClass) || (ptag->iValue != iValue)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }
    else {
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 2)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }

    /*
     *  We don't do either construted or indefinite lenght encoding for this type
     */
    
    if (fConstructed || (cbLength == -1)) {
        err = A2C_ERROR_malformedEncoding;
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

    pi->hData = malloc(cbLength);
    if (pi->hData == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
    
    pi->hLength = cbLength;
    err = _A2C_Memory_Read(pstm, pi->hData, cbLength);

    /*
     *  Return success
     */

    if (err >= A2C_ERROR_Success) return err;

    /*
     *  Cleanup and return the error
     */

ErrorExit:
    if (pi->hData != NULL) {
        free(pi->hData);
        pi->hData = NULL;
    }
                            
    return err;
}


/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_INTEGER_HUGE_encode_der(A2C_INTEGER_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int         cb;
    int         cbData = pi->hLength;
    A2C_ERROR   err;
    BYTE *      pbData = pi->hData;

    /*
     *  We don't use these parameters
     */

    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);

    /*
     *  Must have a buffer and must have a positive length
     */

    if (cbData < 1) return A2C_ERROR_invalidParameter;
    if (pbData == NULL) return A2C_ERROR_invalidParameter;

    /*
     *  Per claus 8.3.2 - we may need to shorten the output string
     *
     *  If the first byte is 0xff and the high bit on the second byte is set - skip the first byte
     *  If the first byte is 0x00 and the high bit on the second byte is clear - skip the first byte
     */

    while ((cbData >= 2) && (pbData[0] == 0xff) && ((pbData[1] & 0x80) == 0x80)) {
        cbData--;
        pbData++;

    }

    while ((cbData >= 2) && (pbData[0] == 0) && ((pbData[1] & 0x80) == 0)) {
        cbData --;
        pbData ++;
    }
    
    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 1
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 2, pstm, &cb);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cb);
    }
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the length
     */

    err = _A2C_write_length(cbData, pstm, &cb);
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the bytes
     */

    err = _A2C_write_bytes(pbData, cbData, pstm);

    /*
     *  Clean up and return error code
     */
ErrorExit:
    return err;
}

A2C_ERROR A2C_INTEGER_HUGE_decode_ber(A2C_INTEGER_HUGE * pi, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
                                 A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*
     *  No differences between DER and BER for integer
     */

    return A2C_INTEGER_HUGE_decode_der(pi, pdesc, flags, pcxt, ptag, pstm);
}


A2C_ERROR A2C_INTEGER_HUGE_encode_ber(A2C_INTEGER_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                 A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*
     *  No significant differences between DER and BER
     */

    return A2C_INTEGER_HUGE_encode_der(pi, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_INTEGER_HUGE_print creates a UTF8 character string that describes the contents of the structure
/// </summary>
/// <param name="pv"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <param name="pstm"> stream object that recieves the description </param>
/// <returns> error code value </returns>
--- */

int A2C_INTEGER_HUGE_print(A2C_INTEGER_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;

    /*
     *  Turn this into an octet string and use the printing routine over there
     */

    os.length = pi->hLength;
    os.data = pi->hData;

    return A2C_OCTET_STRING_print(&os, pdesc, iLevel, pstm);
}

/* ---
/// <summary>
/// </summary>
/// <param name="piLeft">compare value #1</param>
/// <param name="piRight">compare value #2</param>
/// <param name="pdesc">data descriptor structure for comparison</param>
/// <returns>zero if the same, non-zero if different</returns>
--- */

int A2C_INTEGER_HUGE_compare(A2C_INTEGER_HUGE const * piLeft, A2C_INTEGER_HUGE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    int         cbLeft = piLeft->hLength;
    int         cbRight = piRight->hLength;
    PBYTE       pbLeft = piLeft->hData;
    PBYTE       pbRight = piRight->hData;
    
    unreferenced(pdesc);

    /*
     *  Compare the lead bytes in both items
     */

    while ((cbLeft > 0) && (cbRight > 0)) {
        if (*pbLeft != *pbRight) {
            return *pbLeft - *pbRight;
        }
        cbLeft--;
        cbRight--;
        pbLeft--;
        pbRight--;
    }

    /*
     *  Now compare lengths
     */

    return cbLeft - cbRight;
}

/* ---
/// <summary>
/// </summary>
/// <param name="piDest">pointer to destination value</param>
/// <param name="piSrc">pointer to source value</param>
/// <param name="pdesc">pointer to data descriptor information</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_INTEGER_HUGE_copy(A2C_INTEGER_HUGE * piLeft, A2C_INTEGER_HUGE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    /*
     *  Allocate a new buffer
     */

    piLeft->hData = malloc(piRight->hLength);
    if (piLeft->hData == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    /*
     *  Copy over the acutal data
     */
    
    piLeft->hLength = piRight->hLength;
    memcpy(piLeft->hData, piRight->hData, piRight->hLength);

    return A2C_ERROR_Success;
}

/*
/// <summary>
/// Integer validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
*/

A2C_ERROR A2C_INTEGER_HUGE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    A2C_INTEGER_HUGE *  pi = (A2C_INTEGER_HUGE *) pv;

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
     *  Check buffer length > 0
     */
    
    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if (pi->hLength <= 0) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            return A2C_Validate_Print(pdesc, pcxt, "INTEGER", "Cannot have zero length INTEGER", pstm);
        }
        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check that there is a buffer
     */
    
    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        
        if  (pi->hData == NULL) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            return A2C_Validate_Print(pdesc, pcxt, "INTEGER", "Must have data buffer", pstm);
        }
    }

    /*
     *  No validate is done - return success
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}


/*****************************************/

/* ---
/// <summary>
/// A2C_INTEGER_NATIVE_decode_der is the default der integer decoder routine.  This uses
/// the native integer for the data structure.
/// <para>This function does not allow restarting of scanning.</para>
/// </summary>
/// <param name="pi">pointer to integer data</param>
/// <param name="pdesc">descriptor for the data</param>
/// <param name="flags">control flags</param>
/// <param name="pcxt">restart context information</param>
/// <param name="ptag">override implicit tag</param>
/// <param name="pstm">source bytes to decode from</param>
--- */

A2C_ERROR A2C_INTEGER_NATIVE_decode_der(A2C_INTEGER_NATIVE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                        A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    int                 cb;
    A2C_INTEGER_HUGE    data = {0};
    A2C_ERROR           err;
    PBYTE               pb;

    /*
     *  Start by using the long form integer decode to get the data
     */
    
    err = A2C_INTEGER_HUGE_decode_der(&data, pdesc, flags, pcxt, ptag, pstm);
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  If it is too long - then we have a format error
     */

    pb = data.hData;
    cb = data.hLength;

    if (cb > sizeof(int)) {

        /*
         *   See if we can remove sign extensions
         */

        while ((cb > sizeof(int)) && (pb[0] == 0) && (pb[1] < 0x80)) {
            cb--;
            pb++;
        }

        while ((cb > sizeof(int)) && (pb[0] == 0xff) && ((pb[1] & 0x80) != 0)) {
            cb--;
            pb++;
        }

        if (cb > sizeof(int)) {
            err = A2C_ERROR_dataRange;
            goto ErrorExit;
        }
    }

    /*
     *  Convert the buffer into an integer
     */

    RgbToInt(pb, cb, &pi->nData);

    /*
     *  Cleanup and exit
     */
    
ErrorExit:
    if (data.hData != NULL) free(data.hData);
    return err;
}

/*
 *  Write out a DER encoded integer from a native integer
 */

int A2C_INTEGER_NATIVE_encode_der(A2C_INTEGER_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    BYTE        rgb[sizeof(int)];
    A2C_INTEGER_HUGE data;

    /*
     *  Convert into a huge integer and encode
     */

    data.hLength = sizeof(rgb);
    data.hData = rgb;

    IntToRgb(pi->nData, rgb);

    return A2C_INTEGER_HUGE_encode_der(&data, pdesc, flags, pcxt, ptag, pstm);
}

int A2C_INTEGER_NATIVE_encode_ber(A2C_INTEGER_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                  A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    BYTE        rgb[sizeof(int)];
    A2C_INTEGER_HUGE data;

    /*
     *  Convert into a huge integer and encode
     */

    data.hLength = sizeof(rgb);
    data.hData = rgb;

    IntToRgb(pi->nData, rgb);

    return A2C_INTEGER_HUGE_encode_der(&data, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_INTEGER_NATIVE_decode_ber(A2C_INTEGER_NATIVE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                        A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*
     *  No difference in processing for DER and BER on integers
     */
    
    return A2C_INTEGER_NATIVE_decode_der(pi, pdesc, flags, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_INTEGER_NATIVE print creates a UTF8 character string that describes the contents of the structure
/// </summary>
/// <param name="pv"> pointer to the data structure</param>
/// <param name="pdesc"> descriptor about the data strcture</param>
/// <param name="pstm"> stream object that recieves the description </param>
/// <returns> error code value </returns>
--- */

int A2C_INTEGER_NATIVE_print(A2C_INTEGER_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    int                 i;
    A2C_NAMES const *   pNames = &pdesc->pItemDescription->names;
    char        rgch[128];

    unreferenced(pdesc);
    unreferenced(iLevel);

    /*
     *  See if it's named
     */

    for (i=0; i<pNames->count; i++) {
        if (pi->nData == pNames->listArrayPtr[i].number) {
            return pstm->pfnWrite(pstm, (PBYTE) pNames->listArrayPtr[i].name, (A2C_LENGTH) strlen(pNames->listArrayPtr[i].name));
        }
    }

    /*
     *  Format a string with the two values
     */

    snprintf(rgch, sizeof(rgch), "%d (%x)", pi->nData, pi->nData);

    /*
     *  Force termination of the string
     */

    rgch[sizeof(rgch)-1] = 0;

    /*
     *  Append it to the output buffer
     */

    return pstm->pfnWrite(pstm, (PBYTE) rgch, (A2C_LENGTH) strlen(rgch));
}

int A2C_INTEGER_NATIVE_compare(A2C_INTEGER_NATIVE const * piLeft, A2C_INTEGER_NATIVE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */
    
    unreferenced(pdesc);

    /*
     *  Simple comparison
     */

    return piLeft->nData - piRight->nData;
}

int A2C_INTEGER_NATIVE_copy(A2C_INTEGER_NATIVE * piLeft, A2C_INTEGER_NATIVE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */
    
    unreferenced(pdesc);

    /*
     *  Copy the data from the src to the dest
     */

    *piLeft = *piRight;

    return A2C_ERROR_Success;
}

/*
/// <summary>
/// Integer validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
*/

A2C_ERROR A2C_INTEGER_NATIVE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
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


/******************************************************************************************/

A2C_ERROR A2C_INTEGER_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames)
{
    *pNames = &pdesc->pItemDescription->names;

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_INTEGER_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi)
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

A2C_ERROR A2C_INTEGER_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz)
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
