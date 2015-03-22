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

A2C_CONST A2C_ITEM_HEADER A2C_OBJECT_IDENTIFIER_description = {
    sizeof(A2C_OBJECT_IDENTIFIER), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_OBJECT_IDENTIFIER_descriptor = {
    A2C_string_init,
    A2C_string_release,
    (A2C_decoder_f) A2C_OBJECT_IDENTIFIER_decode_der,
    (A2C_encoder_f) A2C_OBJECT_IDENTIFIER_encode_der,
    (A2C_decoder_f) A2C_OBJECT_IDENTIFIER_decode_ber,
    (A2C_encoder_f) A2C_OBJECT_IDENTIFIER_encode_ber,
    (A2C_compare_f) A2C_OBJECT_IDENTIFIER_compare,
    (A2C_copy_f) A2C_OBJECT_IDENTIFIER_copy,
    (A2C_print_f) A2C_OBJECT_IDENTIFIER_print,
    A2C_OBJECT_IDENTIFIER_validate,
    &A2C_OBJECT_IDENTIFIER_description
};

/*
 *  Utility Functions
 */

/*  SzToRgui
 *
 *  Convert a string to an array of integers. 
 *  We assume that 
 *      1. no single value is greater than an unsigned integer
 *      2. 
 *
 *  Returns # of items in the array used.
 *  Returns -# of items if insufficent space.
 */

static int _A2C_SzToRgui(const char * szOID, unsigned int * rgui, int cui)
{
    int         cch = (A2C_LENGTH) strlen(szOID);
    int         cX;
    int         i;

    /*
     *  Count the # of '.' characters
     */

    for (i=0, cX=1; i<cch; i++) if (szOID[i] == '.') cX += 1;
    if (cX > cui) return -cX;

    /*
     *  Walk the list converting each item,
     *  assume each item fits in an unsigned int.
     */

    for (i=0; i<cX; i++) {
        rgui[i] = atoi(szOID);
        if (i<cX-1) {
            while (*szOID != '.') szOID++;
            szOID++;                    /* Skip over the '.' */
        }
    }
    
    return cX;
}


/*
 *  Read in a DER encoded boolean
 */

A2C_ERROR A2C_OBJECT_IDENTIFIER_decode_der(A2C_OBJECT_IDENTIFIER * pobj, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    int         cbOut = 0;
    A2C_LENGTH  cbTL;
    int         cch;
    int         err;
    int		fConstructed;
    int         i;
    int         iClass;
    int         iItem;
    A2C_TAG_VALUE iValue;
    BYTE *      pbData = NULL;
    BYTE        rgbData[256];
    char        rgch[256];
    char	rgchT[20];
    char *      sz;
    unsigned int ui;

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
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 6)) {
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
     *  Do we have enough data?
     */

    if (_A2C_Memory_Left(pstm) < cbTL + cbLength) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Skip over the tag and length
     */

    _A2C_Memory_Skip(pstm, cbTL);

    /*
     *  Read in the length.  We may need to allocate a new buffer to hold it
     */

    if (cbLength > sizeof(rgbData)) {
        pbData = malloc(cbLength);
        if (pbData == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
    }
    else {
        pbData = rgbData;
    }

    err = _A2C_Memory_Read(pstm, pbData, cbLength);
    if (err < 0) {
        goto ErrorExit;
    }
    cbOut += cbLength;
    
    /*
     *  Convert from byte array to int to string
     */

    iItem = 0;
    ui = 0;
    sz = rgch;
    *sz = 0;
    cch = 0;
    for (i=0; i<cbLength; i++) {
        ui = (ui << 7) + (pbData[i] & 0x7f);
        if ((pbData[i] & 0x80) == 0) {
            if (iItem == 0) {
                snprintf(sz, sizeof(sz), "%d.", ui/40);
                ui = ui % 40;
                iItem = 1;
                cch = (A2C_LENGTH) strlen(sz);
            }
            snprintf(rgchT, sizeof(rgchT), "%d.", ui);

            cch += (A2C_LENGTH) strlen(rgchT);
            if (cch > sizeof(rgch)-1) {
                err = A2C_ERROR_ICE;
                goto ErrorExit;
            }
            strcat(sz, rgchT);
            ui = 0;
        }
    }

    sz[strlen(sz)-1] = 0;

    pobj->oid = strdup(sz);
    if (pobj->oid == 0) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
    
    return A2C_ERROR_Success;

ErrorExit:
    if (pbData != NULL) free(pbData);
    return err;
}

/*
 *  Write out a DER encoded boolean
 */

A2C_ERROR A2C_OBJECT_IDENTIFIER_encode_der(A2C_OBJECT_IDENTIFIER const * pobj, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int                 cb;
    int                 cbAlloc;
    int                 cbOut;
    int                 cItems;
    int                 cbLength;
    A2C_ERROR           err;
    int                 i;
    int                 j;
    BYTE *              pbData = NULL;
    unsigned int *      pui = NULL;
    BYTE                rgb[6];
    BYTE                rgbData[256];
    unsigned int        rgui[50];

    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    
    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 6
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 6, pstm, &cbOut);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cbOut);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Convert the string to an array of ints
     */

    cItems = _A2C_SzToRgui(pobj->oid, rgui, countof(rgui));
    if (cItems < 0) {
        pui = malloc(-cItems * sizeof(unsigned int));
        if (pui == 0) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
        
        cItems = _A2C_SzToRgui(pobj->oid, pui, -cItems);
        if (cItems < 0) {
            err = A2C_ERROR_ICE;
            goto ErrorExit;
        }
    }
    else {
        pui = rgui;
    }

    if (cItems == 1) {
        pui[1] = 0;
        cItems = 2;
    }

    /*
     *  Convert the array of ints to an array of bytes
     *
     *  First two items are combined as X*40+Y
     */

    pui[1] = pui[0]*40 + pui[1];

    pbData = rgbData;
    cbAlloc = sizeof(rgbData);
    cbLength = 0;
    
    for (i=1; i<cItems; i++) {
        /*
         *  Convert a single number
         */
        
        for (j=5; (j>=0) && (pui[i] != 0); j--) {
            rgb[j] = (BYTE) (pui[i] & 0x7f);
            pui[i] >>= 7;
            if (j != 5) rgb[j] |= 0x80;
        }
        if (pui[i] != 0) {
            err = A2C_ERROR_ICE;
            goto ErrorExit;
        }

        /*
         *  Now add to the encoded string
         */

        cbLength += (5-j);
        if (cbLength > cbAlloc) {
            BYTE * pbNew;
            if (pbData == rgbData) {
                pbNew = malloc(cbAlloc+128);
                if (pbNew == NULL) {
                    err = A2C_ERROR_outOfMemory;
                    goto ErrorExit;
                }
                memcpy(pbNew, rgbData, sizeof(rgbData));
            }
            else {
                pbNew = realloc(pbData, cbAlloc + 128);
                if (pbNew == NULL) {
                    err = A2C_ERROR_outOfMemory;
                    goto ErrorExit;
                }
            }
            
            pbData = pbNew;
            cbAlloc += 128;
        }
        
        memcpy(pbData+cbLength-(5-j), &rgb[j+1], (6-j));
    }
        
    /*
     *  Write out the length byte
     */

    err = _A2C_write_length(cbLength, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the bytes
     */

    err = _A2C_write_bytes(pbData, cbLength, pstm);

ErrorExit:
    if (pui != rgui) free(pui);
    if (pbData != rgbData) free(pbData);

    return err;
}

A2C_ERROR A2C_OBJECT_IDENTIFIER_decode_ber(A2C_OBJECT_IDENTIFIER * psz, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*  No differences between DER and BER encoding */
    
    return A2C_OBJECT_IDENTIFIER_decode_der(psz, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_OBJECT_IDENTIFIER_encode_ber(A2C_OBJECT_IDENTIFIER const * psz, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*  No differences between DER and BER encoding */

    return A2C_OBJECT_IDENTIFIER_encode_der(psz, pdesc, flags, pcxt, ptag, pstm);
}


/* ---
/// <summary>
/// A2C_OBJECT_IDENTIFIER_print is the default object identifier value print routine.
/// </summary>
/// <param name="pf">pointer to the OID value</param>
/// <param name="pdesc">type descriptor - ignored</param>
/// <param name="iLevel">indentation level - ignroed</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_OBJECT_IDENTIFIER_print(A2C_OBJECT_IDENTIFIER const * psz, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    /*
     *  We don't use these parameters
     */
    
    unreferenced(pdesc);
    unreferenced(iLevel);

    /*
     *  Copy the string to the output buffer
     */

    return pstm->pfnWrite(pstm, (PCBYTE) psz->oid, (A2C_LENGTH) strlen(psz->oid));
}

int A2C_OBJECT_IDENTIFIER_compare(A2C_OBJECT_IDENTIFIER const * pfLeft, A2C_OBJECT_IDENTIFIER const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */

    unreferenced(pdesc);

    /*
     *  Simply compare the strings
     */
    
    return strcmp(pfLeft->oid, pfRight->oid);
}

A2C_ERROR A2C_OBJECT_IDENTIFIER_copy(A2C_OBJECT_IDENTIFIER * pLeft, A2C_OBJECT_IDENTIFIER const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    /*
     *  We don't use this parameter
     */

    unreferenced(pdesc);

    /*
     *  Allocate the memory and copy over
     */

    pLeft->oid = strdup(pRight->oid);
    if (pLeft->oid == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check that the character set is correct 0-9 and '.'
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_OBJECT_IDENTIFIER_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    char *      sz = ((A2C_OBJECT_IDENTIFIER *) pv)->oid;

    unreferenced(pdesc);

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
     *  Check that it is a string
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if (sz == NULL) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            /*
             *  Print error
             */

            return A2C_Validate_Print(pdesc, pcxt, "OBJECT IDENTIFIER", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check for legal characters
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        /*
         *  M00SECURITY: Can overflow to fault page
         *  M00BUG - check that first and last characters are not .
         *  M00BUG - check that no two characters in a row are .
         */
        
        for (; *sz != 0; sz++) {
            if ((*sz != '.') && !(('0' <= *sz) && (*sz <= '9'))) {
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                return A2C_Validate_Print(pdesc, pcxt, "OBJECT IDENTIFIER", "Invalid character", pstm);
            }
        }

        /*
         *  Successful
         */
    }

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}

/***************************************************************************/

A2C_CONST A2C_ITEM_HEADER A2C_RELATIVE_OBJECT_IDENTIFIER_description = {
    sizeof(A2C_RELATIVE_OBJECT_IDENTIFIER), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_RELATIVE_OBJECT_IDENTIFIER_descriptor = {
    A2C_string_init,
    A2C_string_release,
    (A2C_decoder_f) A2C_RELATIVE_OBJECT_IDENTIFIER_decode_der,
    (A2C_encoder_f) A2C_RELATIVE_OBJECT_IDENTIFIER_encode_der,
    (A2C_decoder_f) A2C_RELATIVE_OBJECT_IDENTIFIER_decode_ber,
    (A2C_encoder_f) A2C_RELATIVE_OBJECT_IDENTIFIER_encode_ber,
    (A2C_compare_f) A2C_RELATIVE_OBJECT_IDENTIFIER_compare,
    (A2C_copy_f) A2C_RELATIVE_OBJECT_IDENTIFIER_copy,
    (A2C_print_f) A2C_RELATIVE_OBJECT_IDENTIFIER_print,
    A2C_RELATIVE_OBJECT_IDENTIFIER_validate,
    &A2C_RELATIVE_OBJECT_IDENTIFIER_description
};

/*
 *  Read in a DER encoded boolean
 */

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_decode_der(A2C_RELATIVE_OBJECT_IDENTIFIER * pobj, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    A2C_LENGTH  cbLength;
    A2C_LENGTH  cbTL;
    size_t      cch;
    int         err;
    int         fConstructed = 0;   /* M00BUG - set from flags */
    int         i;
    int         iClass;
    int         iItem;
    A2C_TAG_VALUE  iValue;
    BYTE *      pbData = NULL;
    BYTE        rgbData[256];
    char        rgch[256];
    char	rgchT[20];
    char *      sz;
    unsigned int ui;

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
        if ((iClass != A2C_TAG_CLASS_UNIVERSAL) || (iValue != 13)) {
            err = A2C_ERROR_tagMismatch;
            goto ErrorExit;
        }
    }

    /*
     *  If this is a constructed value, then fail we don't have the
     *  correct code to deal with that in DER world.
     */
    
    if (fConstructed) {
        err = A2C_ERROR_malformedDer;
        goto ErrorExit;
    }

    /*
     *  Do we have enough data?
     */

    if (_A2C_Memory_Left(pstm) < cbTL + cbLength) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Skip over the tag and length
     */

    _A2C_Memory_Skip(pstm, cbTL);

    /*
     *  Read in the length.  We may need to allocate a new buffer to hold it
     */

    if (cbLength > sizeof(rgbData)) {
        pbData = malloc(cbLength);
        if (pbData == NULL) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
    }
    else {
        pbData = rgbData;
    }

    err = _A2C_Memory_Read(pstm, pbData, cbLength);
    if (err < 0) {
        goto ErrorExit;
    }
    
    /*
     *  Convert from byte array to int to string
     */

    iItem = 0;
    ui = 0;
    sz = rgch;
    *sz = 0;
    cch = 0;
    for (i=0; i<cbLength; i++) {
        ui = (ui << 7) + (pbData[i] & 0x7f);
        if ((pbData[i] & 0x80) == 0) {
            snprintf(rgchT, sizeof(rgchT), "%d.", ui);
            cch += strlen(sz);
            if (cch > sizeof(rgch)-1) {
                err = A2C_ERROR_ICE;
                goto ErrorExit;
            }
            strcat(sz, rgchT);
            ui = 0;
        }
    }

    /*  Remove trailing '.' */
    
    sz[strlen(sz)-1] = 0;

    /*
     *  Move the information to thr return location
     */

    pobj->oid = strdup(sz);
    if (pobj->oid == 0) {
        err = A2C_ERROR_outOfMemory;
        goto ErrorExit;
    }
    
    return A2C_ERROR_Success;

ErrorExit:
    if (pbData != NULL) free(pbData);
    return err;
}

/*
 *  Write out a DER encoded boolean
 */

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_encode_der(A2C_RELATIVE_OBJECT_IDENTIFIER const * pobj, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    int         cb;
    int         cbAlloc;
    int         cbOut = 0;
    int         cItems;
    int                 cbLength;
    A2C_ERROR           err;
    int                 i;
    int                 j;
    BYTE *              pbData = NULL;
    unsigned int *      pui = NULL;
    BYTE                rgb[6];
    BYTE                rgbData[256];
    unsigned int        rgui[50];

    unreferenced(pdesc);
    unreferenced(flags);
    unreferenced(pcxt);
    
    /*
     *  Were we told not to write out the tag?
     *  if not - then tag = UNIVERSAL, 13
     */

    if (ptag == NULL) {
        err = _A2C_write_tag(A2C_TAG_CLASS_UNIVERSAL, 0, 13, pstm, &cbOut);
    }
    else {
        err = _A2C_write_tag(ptag->iClass, 0, ptag->iValue, pstm, &cbOut);
    }
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Convert the string to an array of ints
     */

    cItems = _A2C_SzToRgui(pobj->oid, rgui, countof(rgui));
    if (cItems < 0) {
        pui = malloc(-cItems * sizeof(unsigned int));
        if (pui == 0) {
            err = A2C_ERROR_outOfMemory;
            goto ErrorExit;
        }
        cItems = _A2C_SzToRgui(pobj->oid, pui, -cItems);
        if (cItems < 0) {
            err = A2C_ERROR_ICE;
            goto ErrorExit;
        }
    }
    else {
        pui = rgui;
    }

    /*
     *  Convert the array of ints to an array of bytes
     */

    pbData = rgbData;
    cbAlloc = sizeof(rgbData);
    cbLength = 0;
    
    for (i=0; i<cItems; i++) {
        /*
         *  Convert a single number
         */
        
        for (j=5; (j>=0) && (pui[i] != 0); j--) {
            rgb[j] = (BYTE) (pui[i] & 0x7f);
            pui[i] >>= 7;
            if (j != 5) rgb[j] |= 0x80;
        }
        if (pui[i] != 0) {
            err = A2C_ERROR_ICE;
            goto ErrorExit;
        }

        /*
         *  Now add to the encoded string
         */

        cbLength += (5-j);
        if (cbLength > cbAlloc) {
            BYTE * pbNew;
            if (pbData == rgbData) {
                pbNew = malloc(cbAlloc+128);
                if (pbNew == NULL) {
                    err = A2C_ERROR_outOfMemory;
                    goto ErrorExit;
                }
                memcpy(pbNew, rgbData, sizeof(rgbData));
            }
            else {
                pbNew = realloc(pbData, cbAlloc + 128);
                if (pbNew == NULL) {
                    err = A2C_ERROR_outOfMemory;
                    goto ErrorExit;
                }
            }
            
            pbData = pbNew;
            cbAlloc += 128;
        }
        
        memcpy(pbData+cbLength-(5-j), &rgb[j+1], (6-j));
    }
        
    /*
     *  Write out the length byte
     */

    err = _A2C_write_length(cbLength, pstm, &cb);
    if (err < A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Write out the bytes
     */

    err = _A2C_write_bytes(pbData, cbLength, pstm);

ErrorExit:
    if (pui != rgui) free(pui);
    if (pbData != rgbData) free(pbData);

    return err;
}

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_decode_ber(A2C_RELATIVE_OBJECT_IDENTIFIER * psz, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    /*  No differences between DER and BER encoding */
    
    return A2C_RELATIVE_OBJECT_IDENTIFIER_decode_der(psz, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_encode_ber(A2C_RELATIVE_OBJECT_IDENTIFIER const * psz, PC_A2C_DESCRIPTOR pdesc, int flags, 
                                           A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    /*  No differences between DER and BER encoding */

    return A2C_RELATIVE_OBJECT_IDENTIFIER_encode_der(psz, pdesc, flags, pcxt, ptag, pstm);
}


/* ---
/// <summary>
/// A2C_RELATIVE_OBJECT_IDENTIFIER_print is the default object identifier value print routine.
/// </summary>
/// <param name="pf">pointer to the OID value</param>
/// <param name="pdesc">type descriptor - ignored</param>
/// <param name="iLevel">indentation level - ignroed</param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_print(A2C_RELATIVE_OBJECT_IDENTIFIER const * psz, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    unreferenced(pdesc);
    unreferenced(iLevel);

    /*
     *  Copy the string to the output buffer
     */

    return pstm->pfnWrite(pstm, (PCBYTE) psz->oid, (A2C_LENGTH) strlen(psz->oid));
}

int A2C_RELATIVE_OBJECT_IDENTIFIER_compare(A2C_RELATIVE_OBJECT_IDENTIFIER const * pfLeft, A2C_RELATIVE_OBJECT_IDENTIFIER const * pfRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    /*
     *  Simply compare the strings
     */
    
    return strcmp(pfLeft->oid, pfRight->oid);
}

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_copy(A2C_RELATIVE_OBJECT_IDENTIFIER * pLeft, A2C_RELATIVE_OBJECT_IDENTIFIER const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    /*
     *  Allocate the memory and copy over
     */

    pLeft->oid = strdup(pRight->oid);
    if (pLeft->oid == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check that the character set is correct 0-9 and '.'
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_RELATIVE_OBJECT_IDENTIFIER_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    char *      sz = ((A2C_RELATIVE_OBJECT_IDENTIFIER *) pv)->oid;

    unreferenced(pdesc);

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
     *  Check that it is a string
     */

    if (A2C_ContextNum(pcxt, iFrame) == 0) {
        if (sz == NULL) {
            /*
             *  Mark as finished
             */
            
            A2C_SetContextNum(pcxt, iFrame, -1);

            /*
             *  Print error
             */

            return A2C_Validate_Print(pdesc, pcxt, "OBJECT IDENTIFIER", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check for legal characters
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        /*
         *  M00SECURITY: Can overflow to fault page
         *  M00BUG - check that first and last characters are not .
         *  M00BUG - check that no two characters in a row are .
         */
        
        for (; *sz != 0; sz++) {
            if ((*sz != '.') && !(('0' <= *sz) && (*sz <= '9'))) {
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                return A2C_Validate_Print(pdesc, pcxt, "OBJECT IDENTIFIER", "Invalid character", pstm);
            }
        }

        /*
         *  Successful
         */
    }

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}
