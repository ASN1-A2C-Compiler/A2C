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
#include <string.h>

#include "A2C_Stream_Internal.h"

typedef unsigned char uchar;


static uint32 Zero = 0;

/*  We want the tags all to be up front so we don't ever re-initialize */

const A2C_TAG TagBMPString = {A2C_TAG_CLASS_UNIVERSAL, 30};
const A2C_TAG TagGeneralString = {A2C_TAG_CLASS_UNIVERSAL, 27};
const A2C_TAG TagGraphicString = {A2C_TAG_CLASS_UNIVERSAL, 25};
const A2C_TAG TagIA5String = {A2C_TAG_CLASS_UNIVERSAL, 22};
const A2C_TAG TagNumericString = {A2C_TAG_CLASS_UNIVERSAL, 18};
const A2C_TAG TagPrintableString = {A2C_TAG_CLASS_UNIVERSAL, 19};
const A2C_TAG TagTeletexString = {A2C_TAG_CLASS_UNIVERSAL, 20};
const A2C_TAG TagUniversalString = {A2C_TAG_CLASS_UNIVERSAL, 28};
const A2C_TAG TagUTF8String = {A2C_TAG_CLASS_UNIVERSAL, 12};
const A2C_TAG TagVideotexString = {A2C_TAG_CLASS_UNIVERSAL, 21};
const A2C_TAG TagVisibleString = {A2C_TAG_CLASS_UNIVERSAL, 26};

char Rgf[] = {
/*  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 0x00-0x0f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 0x10-0x1f */
    7, 4, 4, 4, 4, 4, 4, 5, 5, 5, 4, 5, 5, 5, 5, 5,  /* 0x20-0x2f */
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 4, 4, 5, 4, 5,  /* 0x30-0x3f */
    4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  /* 0x40-0x4f */
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4,  /* 0x50-0x5f */
    4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  /* 0x60-0x6f */
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 0,  /* 0x70-0x7f */

    /*  1 = in the PrintableString set */
    /*  2 = in the NumericString set */
    /*  4 = in the VisibleString set */
};


/*
 *  Set of illegal characters are as follows
 */
static int unicode_is_valid_char(uint32 current_char)
{
    if (current_char > 0x10FFFF) return 0;
    if ((current_char & 0xFFFF) == 0xFFFE) return 0;
    if ((current_char & 0xFFFF) == 0xFFFF) return 0;
    if ((0xD800 <= current_char) && (current_char <= 0xDBFF)) return 0;
    if ((0xFDD0 <= current_char) && (current_char <= 0xFDEF)) return 0;
    return 1;
}


static int process_continuation_byte(uchar** s, uint32* return_char)
{
    
    if ((**s & 0xC0) != 0x80) {
        return 0;
    }

    *return_char <<= 6;
    *return_char |= (**s & 0x3F);

    ++*s;

    return 1;
}

/*
 */

int utf8_next_char(uchar **s, uint32* return_char)
{
    int return_code = 1;
    unsigned int number_continuation_bytes = 0;
    uchar mask = 0x0FF;
    unsigned int counter = 0;
    unsigned int smallest = 0;

    /*
     *  If no string passed in - return 0
     */
    
    if (s == NULL) {
        return return_code;
    }

    /*
     *  Compute the number of continuation bytes based on
     *  the top bits of this byte.
     *
     *  000000-00007f   0zzz zzzz
     *  000080-0007ff   110y yyyy 10zz zzzz
     *  000800-00ffff   1110 xxxx 10yy yyyy 10zz zzzz
     *  010000-10ffff   1111 0www 10xx xxxx 10yy yyyy 10zz zzzz
     */
    
    if ((**s & 0x80) == 0) {
        number_continuation_bytes = 0;
        mask = 0x7F;
    }
    else if ((**s & 0xE0) == 0xC0) {
        mask = 0x1F;
        number_continuation_bytes = 1;
        /*  110y yyyy 10zz zzzz */
        /*          1   11 1111 */
        /*  Smallest lead is  1100 0010 */
        smallest = 1;
    }
    else if ((**s & 0xF0) == 0xE0) {
        mask = 0x0F;
        number_continuation_bytes = 2;
        /*  1110 xxxx 10yy yyyy 10zz zzzz */
        /*               1 1111   11 1111 */
        smallest = 0;
    }
    else if ((**s & 0xF8) == 0xF0) {
        mask = 0x07;
        number_continuation_bytes = 3;
        smallest = 0;
    }
    else {
        /* Some encoding that would make us sad */
        return 0;
    }

    /*
     *  Grab real bits from this byte
     */

    *return_char = **s & mask;

    /*
     *  If the value is zero - then it was not encode in it's shortest form
     */

    if ((*return_char <= smallest) && (number_continuation_bytes != 0)) {
        /* Overlong */
        return 0;
    }

    /*
     *  Next character
     */
    
    ++*s;

    /*
     *
     */
    
    for (counter = 0; counter < number_continuation_bytes; ++counter) {
        return_code = process_continuation_byte(s, return_char);
        if (!return_code) {
            return return_code;
        }
    }

    return 1;
}

/* ---
//
//
--- */

static A2C_ERROR string_print(const char * psz, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    unreferenced(pdesc);
    unreferenced(iLevel);
    
    return pstm->pfnWrite(pstm, (PBYTE) psz, strlen(psz));
}

A2C_ERROR A2C_string_init(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_UNIVERSAL_STRING * ps = (A2C_UNIVERSAL_STRING *) pv;

    unreferenced(pdesc);

#ifndef NO_INIT
    ps->string = &Zero;
#endif // NO_INIT

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_string_release(PVOID pv, PC_A2C_DESCRIPTOR pdesc)
{
    A2C_UNIVERSAL_STRING * ps = (A2C_UNIVERSAL_STRING *) pv;

    unreferenced(pdesc);

    if (ps->string != &Zero) free(ps->string);

    return A2C_ERROR_Success;
}

static int _A2C_wcslen(uint16 * psz)
{
    int i;
    
    for (i=0; *psz != 0; i++, psz++);

    return i;
}

static uint16 *  _A2C_wcsdup(uint16 * pszIn)
{
    int         cb;
    uint16 *     pszOut;
    
    cb = _A2C_wcslen(pszIn)*2 + 2;
    pszOut = malloc(cb);
    if (pszOut != NULL) {
        memcpy(pszOut, pszIn, cb);
    }
    return pszOut;
}

static int _A2C_c4len(uint32 * psz)
{
    int i;

    for (i=0; *psz!= 0; i++, psz++);

    return i;
}

static uint32 * _A2C_c4dup(uint32 * pszIn)
{
    int         cb;
    uint32 *     pszOut;
    
    cb = _A2C_c4len(pszIn)*4 + 4;
    pszOut = malloc(cb);
    if (pszOut != NULL) {
        memcpy(pszOut, pszIn, cb);
    }
    return pszOut;
}

static void _A2C_wcsswap(uint16 * psz)
{
    BYTE b1;
    BYTE b2;
    
    for (; *psz != 0; psz++) {
        b1 = (BYTE) ((*psz) & 0xff);
        b2 = (BYTE) (((*psz) & 0xff00) >> 8);

        *psz = (b1 << 8) | b2;
    }
}

static void _A2C_c4swap(uint32 * psz)
{
    BYTE b1;
    BYTE b2;
    BYTE b3;
    BYTE b4;

    for (; *psz != 0; psz++) {
        b1 = (BYTE) (((*psz) & 0x000000ff));
        b2 = (BYTE) (((*psz) & 0x0000ff00) >> 8);
        b3 = (BYTE) (((*psz) & 0x00ff0000) >> 16);
        b4 = (BYTE) (((*psz) & 0xff000000) >> 24);

        *psz = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    }
}

/**************************** A2C_BMP_STRING *******************************************/

/* ---
/// <summary>
/// A2C_BMP_STRING_decriptor contains the dispatch table for the string type BMP.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_BMP_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_BMP_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_BMP_STRING_decode_der,
    (A2C_encoder_f) A2C_BMP_STRING_encode_der,
    (A2C_decoder_f) A2C_BMP_STRING_decode_ber,
    (A2C_encoder_f) A2C_BMP_STRING_encode_ber,
    (A2C_compare_f) A2C_BMP_STRING_compare,
    (A2C_copy_f) A2C_BMP_STRING_copy,
    (A2C_print_f) A2C_BMP_STRING_print,
    A2C_BMP_STRING_validate,
    &A2C_BMP_STRING_description
};


/* ---
/// <summary>
/// A2C_BMP_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [30] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_BMP_STRING_decode_der(A2C_BMP_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    int                 i;
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    uint16 *             pw = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagBMPString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        /*  Look for the null - is it in the correct place? */

        for (pw = (uint16 *) os.data, i = os.length; i > 0; pw++, i-=2) {
            if (*pw == 0) {
                free(os.data);
                return A2C_ERROR_dataFormat;
            }
        }

        /*  Duplicate the buffer to add the zero terminator */

        pw = malloc(os.length + 2);
        if (pw == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pw, os.data, os.length);
        pw[os.length/2] = 0;

        /*  Swap the order of the bytes to int order */
        
        _A2C_wcsswap(pw);

        /*  Save the answer */
        
        pv->string = pw;

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_BMP_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [30] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_BMP_STRING_encode_der(A2C_BMP_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os;

    /*  Create a copy of the string */

    os.data = (BYTE *) _A2C_wcsdup(pv->string);
    if (os.data == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    os.length = _A2C_wcslen(pv->string) * 2;

    /* Byte swap the string */

    _A2C_wcsswap((uint16 *) os.data);

    /*  Do the encode */

    err = A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                      ptag == NULL ? &TagBMPString : ptag, pstm);

    /* Cleanup */

    free(os.data);
    return err;
}

/* ---
/// <summary>
/// A2C_BMP_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [30] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_BMP_STRING_decode_ber(A2C_BMP_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    int                 i;
    A2C_OCTET_STRING    os = {0};
    uint16 *             pw = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagBMPString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        /*  Look for the null - is it in the correct place? */

        for (pw = (uint16 *) os.data, i = os.length; i > 0; pw++, i-=2) {
            if (*pw == 0) {
                free(os.data);
                return A2C_ERROR_dataFormat;
            }
        }

        /*  Duplicate the buffer to add the zero terminator */

        pw = malloc(os.length + 2);
        if (pw == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pw, os.data, os.length);
        pw[os.length/2] = 0;

        /*  Swap the order of the bytes to int order */
        
        _A2C_wcsswap(pw);

        /*  Save the answer */
        
        pv->string = pw;

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_BMP_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [30] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_BMP_STRING_encode_ber(A2C_BMP_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os;

    /*  Create a copy of the string */

    os.data = (BYTE *) _A2C_wcsdup(pv->string);
    if (os.data == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    os.length = _A2C_wcslen(pv->string) * 2;

    /* Byte swap the string */

    _A2C_wcsswap((uint16 *) os.data);

    /*  Do the encode */

    err = A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                      ptag == NULL ? &TagBMPString : ptag, pstm);

    /* Cleanup */

    free(os.data);
    return err;
}

A2C_ERROR A2C_BMP_STRING_copy(A2C_BMP_STRING * pLeft, A2C_BMP_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    pLeft->string = _A2C_wcsdup(pRight->string);

    if (pLeft->string == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    return A2C_ERROR_Success;
}

int A2C_BMP_STRING_compare(A2C_BMP_STRING const * pLeft, A2C_BMP_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    uint16 *     piLeft = pLeft->string;
    uint16 *     piRight = pRight->string;
    
    unreferenced(pdesc);

    /*
     *  Walk the list until the first string hits a zero or
     *  a character is different
     */

    for (;*piLeft != 0; piLeft++, piRight++) {
        if (*piLeft != *piRight) return *piLeft - *piRight;
    }

    /*
     *  Termianted the left string - compute the result
     */
    
    return (*piLeft)-(*piRight);
}

/* ---
/// <summary>
/// A2C_BMP_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_BMP_STRING_print(A2C_BMP_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;

    unreferenced(pdesc);

    /*
     *  Treat as an octet string
     */

    os.data = (PBYTE) pv->string;
    os.length = _A2C_wcslen(pv->string)*2;
    
    return A2C_OCTET_STRING_print(&os, pdesc, iLevel, pstm);
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

A2C_ERROR A2C_BMP_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uint16 *    sz = ((A2C_BMP_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "BMPString", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  For each character - check that the character is in bounds.
     *  We only report one invalid character
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        for (;*sz != 0; sz++) {
            uint16      ch = *sz;
            
            if (((ch & 0xfffe) == 0xfffe) ||  /* 0xfffe and 0xffff */
                ((0xd800 <= ch) && (ch <= 0xdbff)) ||
                ((0xfdd0 <= ch) && (ch <= 0xfdef))) {
                char    rgch[100];
                
                A2C_SetContextNum(pcxt, iFrame, -11);

                /*
                 *  Print error
                 */

                snprintf(rgch, sizeof(rgch), "Character 0x%x is not a valid BMP character", ch);
                return A2C_Validate_Print(pdesc, pcxt, "BMPString", rgch, pstm);
            }
        }

        /*
         *  We succeeded - exit
         */
    }

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}

/**************************** A2C_GENERAL_STRING *******************************************/

/* ---
/// <summary>
/// A2C_GENERAL_STRING_decriptor contains the dispatch table for the string type GENERAL.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_GENERAL_STRING_description = {
    sizeof(A2C_GENERAL_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_GENERAL_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_GENERAL_STRING_decode_der,
    (A2C_encoder_f) A2C_GENERAL_STRING_encode_der,
    (A2C_decoder_f) A2C_GENERAL_STRING_decode_ber,
    (A2C_encoder_f) A2C_GENERAL_STRING_encode_ber,
    (A2C_compare_f) A2C_GENERAL_STRING_compare,
    (A2C_copy_f) A2C_GENERAL_STRING_copy,
    (A2C_print_f) A2C_GENERAL_STRING_print,
    A2C_GENERAL_STRING_validate,
    &A2C_GENERAL_STRING_description
};


/* ---
/// <summary>
/// A2C_GENERAL_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GENERAL_STRING_decode_der(A2C_GENERAL_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagGeneralString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_GENERAL_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GENERAL_STRING_encode_der(A2C_GENERAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagGeneralString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_GENERAL_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GENERAL_STRING_decode_ber(A2C_GENERAL_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagGeneralString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_GENERAL_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GENERAL_STRING_encode_ber(A2C_GENERAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagGeneralString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_GENERAL_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_GENERAL_STRING_print(A2C_GENERAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_GENERAL_STRING_compare(A2C_GENERAL_STRING const * pLeft, A2C_GENERAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_GENERAL_STRING_copy(A2C_GENERAL_STRING * pLeft, A2C_GENERAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;
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

A2C_ERROR A2C_GENERAL_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uchar * sz = (uchar *) ((A2C_GENERAL_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "GeneralString", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Legal characters are "All G and all C sets + SPACE + DELETE"
     *
     *  We are not checking this.
     */

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}

/**************************** A2C_GRAPHIC_STRING *******************************************/

/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_decriptor contains the dispatch table for the string type GRAPHIC.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_GRAPHIC_STRING_description = {
    sizeof(A2C_GRAPHIC_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_GRAPHIC_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_GRAPHIC_STRING_decode_der,
    (A2C_encoder_f) A2C_GRAPHIC_STRING_encode_der,
    (A2C_decoder_f) A2C_GRAPHIC_STRING_decode_ber,
    (A2C_encoder_f) A2C_GRAPHIC_STRING_encode_ber,
    (A2C_compare_f) A2C_GRAPHIC_STRING_compare,
    (A2C_copy_f) A2C_GRAPHIC_STRING_copy,
    (A2C_print_f) A2C_GRAPHIC_STRING_print,
    A2C_GRAPHIC_STRING_validate,
    &A2C_GRAPHIC_STRING_description
};


/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_decode_der(A2C_GRAPHIC_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagGraphicString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_encode_der(A2C_GRAPHIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagGraphicString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_decode_ber(A2C_GRAPHIC_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagGraphicString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_encode_ber(A2C_GRAPHIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagGraphicString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_GRAPHIC_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_print(A2C_GRAPHIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_GRAPHIC_STRING_compare(A2C_GRAPHIC_STRING const * pLeft, A2C_GRAPHIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_GRAPHIC_STRING_copy(A2C_GRAPHIC_STRING * pLeft, A2C_GRAPHIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check the string is real.
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_GRAPHIC_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uchar * sz = (uchar *) ((A2C_GRAPHIC_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "UTF8String", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Legal characters are "All G sets + SPACE"
     *
     *  We are not checking this.
     */

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);
    *pf = TRUE;
    return A2C_ERROR_Success;
}


/**************************** A2C_IA5_STRING *******************************************/

/* ---
/// <summary>
/// A2C_IA5_STRING_decriptor contains the dispatch table for the string type IA5.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_IA5_STRING_description = {
    sizeof(A2C_IA5_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_IA5_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_IA5_STRING_decode_der,
    (A2C_encoder_f) A2C_IA5_STRING_encode_der,
    (A2C_decoder_f) A2C_IA5_STRING_decode_ber,
    (A2C_encoder_f) A2C_IA5_STRING_encode_ber,
    (A2C_compare_f) A2C_IA5_STRING_compare,
    (A2C_copy_f) A2C_IA5_STRING_copy,
    (A2C_print_f) A2C_IA5_STRING_print,
    A2C_IA5_STRING_validate,
    &A2C_IA5_STRING_description
};


/* ---
/// <summary>
/// A2C_IA5_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_IA5_STRING_decode_der(A2C_IA5_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagIA5String : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;
        pv->string = pb;

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_IA5_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_IA5_STRING_encode_der(A2C_IA5_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagIA5String : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_IA5_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_IA5_STRING_decode_ber(A2C_IA5_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagIA5String : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_IA5_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [22] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_IA5_STRING_encode_ber(A2C_IA5_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagIA5String : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_IA5_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_IA5_STRING_print(A2C_IA5_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_IA5_STRING_compare(A2C_IA5_STRING const * pLeft, A2C_IA5_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_IA5_STRING_copy(A2C_IA5_STRING * pLeft, A2C_IA5_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check that charactes are in the legal set - 0-9 and space
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_IA5_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uchar * sz = (uchar*) (((A2C_IA5_STRING *) pv)->string);

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

            return A2C_Validate_Print(pdesc, pcxt, "IA5String", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check for legal characters
     *
     *  Reg# 1 & 6, + SPACE + DELETE
     *
     *  Maps to 0 to 31, 33 to 126, 32 , 127
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        /*
         *  M00SECURITY: Can overflow to fault page
         *  M00BUG: Technically 0 is a legal character to encode.
         *  M00NOTE: Character 0 can be removed w/o changing the string, thus shortest encoding
         *      says that we should remove 0's from the encoded sequence.
         */
        
        for (; *sz != 0; sz++) {
            if (*sz > 127) {
                char rgch[100];
                
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Invalid character %x", *sz);

                return A2C_Validate_Print(pdesc, pcxt, "IA5String", rgch, pstm);
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



/**************************** A2C_NUMERIC_STRING *******************************************/

/* ---
/// <summary>
/// A2C_NUMERIC_STRING_decriptor contains the dispatch table for the string type NUMERIC.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_NUMERIC_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_NUMERIC_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_NUMERIC_STRING_decode_der,
    (A2C_encoder_f) A2C_NUMERIC_STRING_encode_der,
    (A2C_decoder_f) A2C_NUMERIC_STRING_decode_ber,
    (A2C_encoder_f) A2C_NUMERIC_STRING_encode_ber,
    (A2C_compare_f) A2C_NUMERIC_STRING_compare,
    (A2C_copy_f) A2C_NUMERIC_STRING_copy,
    (A2C_print_f) A2C_NUMERIC_STRING_print,
    A2C_NUMERIC_STRING_validate,
    &A2C_NUMERIC_STRING_description
};


/* ---
/// <summary>
/// A2C_NUMERIC_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [18] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_decode_der(A2C_NUMERIC_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagNumericString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_NUMERIC_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [18] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_encode_der(A2C_NUMERIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagNumericString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_NUMERIC_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [18] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_decode_ber(A2C_NUMERIC_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagNumericString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;
        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_NUMERIC_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [18] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_encode_ber(A2C_NUMERIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagNumericString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_NUMERIC_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_print(A2C_NUMERIC_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_NUMERIC_STRING_compare(A2C_NUMERIC_STRING const * pLeft, A2C_NUMERIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_NUMERIC_STRING_copy(A2C_NUMERIC_STRING * pLeft, A2C_NUMERIC_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check that charactes are in the legal set - 0-9 and space
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_NUMERIC_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    uchar *     sz = (uchar *) ((A2C_NUMERIC_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "NumericString", "String is NULL", pstm);
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
         */
        
        for (; *sz != 0; sz++) {
            if ((*sz > 126) || ((Rgf[*sz] & 2) == 0)) {
                char    rgch[100];
                
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Invalid character %s", *sz);
                return A2C_Validate_Print(pdesc, pcxt, "NumericString", rgch, pstm);
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

/**************************** A2C_PRINTABLE_STRING *******************************************/

/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_decriptor contains the dispatch table for the string type PRINTABLE.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_PRINTABLE_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_PRINTABLE_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_PRINTABLE_STRING_decode_der,
    (A2C_encoder_f) A2C_PRINTABLE_STRING_encode_der,
    (A2C_decoder_f) A2C_PRINTABLE_STRING_decode_ber,
    (A2C_encoder_f) A2C_PRINTABLE_STRING_encode_ber,
    (A2C_compare_f) A2C_PRINTABLE_STRING_compare,
    (A2C_copy_f) A2C_PRINTABLE_STRING_copy,
    (A2C_print_f) A2C_PRINTABLE_STRING_print,
    A2C_PRINTABLE_STRING_validate,
    &A2C_PRINTABLE_STRING_description
};


/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [19] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_decode_der(A2C_PRINTABLE_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagPrintableString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [19] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_encode_der(A2C_PRINTABLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING	os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagPrintableString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [19] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_decode_ber(A2C_PRINTABLE_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagPrintableString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [19] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_encode_ber(A2C_PRINTABLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagPrintableString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_PRINTABLE_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_print(A2C_PRINTABLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_PRINTABLE_STRING_compare(A2C_PRINTABLE_STRING const * pLeft, A2C_PRINTABLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_PRINTABLE_STRING_copy(A2C_PRINTABLE_STRING * pLeft, A2C_PRINTABLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    
    pLeft->string = strdup(pRight->string);
    
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check that charactes are in the legal set - 0-9 and space
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_PRINTABLE_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uchar *      sz = (uchar*) (((A2C_IA5_STRING *) pv)->string);

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

            return A2C_Validate_Print(pdesc, pcxt, "PrintablString", "String is NULL", pstm);
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
         */
        
        for (; *sz != 0; sz++) {
            if ((*sz > 126) || ((Rgf[*sz] & 1) == 0)) {
                char    rgch[100];
                
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Invalid character %x", *sz);
                return A2C_Validate_Print(pdesc, pcxt, "PrintableString", rgch, pstm);
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


/**************************** A2C_TELETEX_STRING *******************************************/

/* ---
/// <summary>
/// A2C_TELETEX_STRING_decriptor contains the dispatch table for the string type TELETEX.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_TELETEX_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_TELETEX_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_TELETEX_STRING_decode_der,
    (A2C_encoder_f) A2C_TELETEX_STRING_encode_der,
    (A2C_decoder_f) A2C_TELETEX_STRING_decode_ber,
    (A2C_encoder_f) A2C_TELETEX_STRING_encode_ber,
    (A2C_compare_f) A2C_TELETEX_STRING_compare,
    (A2C_copy_f) A2C_TELETEX_STRING_copy,
    (A2C_print_f) A2C_TELETEX_STRING_print,
    A2C_TELETEX_STRING_validate,
    &A2C_TELETEX_STRING_description
};


/* ---
/// <summary>
/// A2C_TELETEX_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_decode_der(A2C_TELETEX_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagTeletexString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_TELETEX_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_encode_der(A2C_TELETEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING	os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagTeletexString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_TELETEX_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_decode_ber(A2C_TELETEX_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagTeletexString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_TELETEX_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_encode_ber(A2C_TELETEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagTeletexString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_TELETEX_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_print(A2C_TELETEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_TELETEX_STRING_compare(A2C_TELETEX_STRING const * pLeft, A2C_TELETEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_TELETEX_STRING_copy(A2C_TELETEX_STRING * pLeft, A2C_TELETEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// We don't do anything to validate teletex strings.  The specification of what is allowable as
/// characters and what encoding formats are allowed.
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_TELETEX_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    uchar *     sz = (uchar *)(((A2C_TELETEX_STRING *) pv)->string);
    
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

            return A2C_Validate_Print(pdesc, pcxt, "TeletexString", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  We don't check the actual set of characters - should be from the reg. no.
     *
     *  6, 87, 102, 103, 106, 107, 126, 144, 150, 153, 156, 164, 165, 168 + SPACE + DELETE
     *
     *  Nobody really knows what this means any more so we are just ignoring it.
     */

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}


/**************************** A2C_UNIVERSAL_STRING *******************************************/

/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_decriptor contains the dispatch table for the string type UNIVERSAL.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_UNIVERSAL_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_UNIVERSAL_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_UNIVERSAL_STRING_decode_der,
    (A2C_encoder_f) A2C_UNIVERSAL_STRING_encode_der,
    (A2C_decoder_f) A2C_UNIVERSAL_STRING_decode_ber,
    (A2C_encoder_f) A2C_UNIVERSAL_STRING_encode_ber,
    (A2C_compare_f) A2C_UNIVERSAL_STRING_compare,
    (A2C_copy_f) A2C_UNIVERSAL_STRING_copy,
    (A2C_print_f) A2C_UNIVERSAL_STRING_print,
    A2C_UNIVERSAL_STRING_validate,
    &A2C_UNIVERSAL_STRING_description
};


/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [28] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_decode_der(A2C_UNIVERSAL_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    int                 i;
    A2C_OCTET_STRING    os = {0};
    uint32 *             pi;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagUniversalString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        /*  Look for the null - is it in the correct place? */

        for (pi = (uint32 *) os.data, i = os.length; i > 0; pi++, i-=4) {
            if (*pi == 0) {
                free(os.data);
                return A2C_ERROR_dataFormat;
            }
        }

        /*  Duplicate the buffer to add the zero terminator */

        pi = malloc(os.length+4);
        if (pi == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(pi, os.data, os.length);
            pi[os.length/4] = 0;

            pv->string = pi;

            /*  Swap the order of the bytes to int order */
        
            _A2C_c4swap(pi);
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [28] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_encode_der(A2C_UNIVERSAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING	os;

    /*  Create a copy of the string */

    os.data = (BYTE *) _A2C_c4dup(pv->string);
    if (os.data == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    os.length = _A2C_c4len(pv->string)*4;

    /* Byte swap the string */

    _A2C_c4swap((uint32 *) os.data);

    /*  Do the encode */

    err = A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                      ptag == NULL ? &TagUniversalString : ptag, pstm);
    /* Cleanup */

    free(os.data);
    return err;
}

/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [28] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_decode_ber(A2C_UNIVERSAL_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    int                 i;
    A2C_OCTET_STRING    os = {0};
    uint32 *             pi;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagUniversalString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        /*  Look for the null - is it in the correct place? */

        for (pi = (uint32 *) os.data, i = os.length; i > 0; pi++, i-=4) {
            if (*pi == 0) {
                free(os.data);
                return A2C_ERROR_dataFormat;
            }
        }

        /*  Duplicate the buffer to add the zero terminator */

        pi = malloc(os.length+4);
        if (pi == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(pi, os.data, os.length);
            pi[os.length*4] = 0;
            pv->string = pi;

            /*  Swap the order of the bytes to int order */
        
            _A2C_c4swap(pi);
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [28] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_encode_ber(A2C_UNIVERSAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING	os;

    /*  Create a copy of the string */

    os.data = (BYTE *) _A2C_c4dup(pv->string);
    if (os.data == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    os.length = _A2C_c4len(pv->string)*4;

    /* Byte swap the string */

    _A2C_c4swap((uint32 *) os.data);

    /*  Do the encode */

    err = A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagUniversalString : ptag, pstm);

    /* Cleanup */

    free(os.data);
    return err;
}

A2C_ERROR A2C_UNIVERSAL_STRING_compare(A2C_UNIVERSAL_STRING const * pLeft, A2C_UNIVERSAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    uint32 *     piLeft = pLeft->string;
    uint32 *     piRight = pRight->string;
    
    unreferenced(pdesc);

    /*
     *  Walk the list until the first string hits a zero or
     *  a character is different
     */

    for (;*piLeft != 0; piLeft++, piRight++) {
        if (*piLeft != *piRight) return *piLeft - *piRight;
    }

    /*
     *  Termianted the left string - compute the result
     */
    
    return (*piLeft)-(*piRight);
}

A2C_ERROR A2C_UNIVERSAL_STRING_copy(A2C_UNIVERSAL_STRING * pLeft, A2C_UNIVERSAL_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    
    pLeft->string = _A2C_c4dup(pRight->string);

    if (pLeft->string == NULL) {
        return A2C_ERROR_outOfMemory;
    }
    
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// A2C_UNIVERSAL_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_print(A2C_UNIVERSAL_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING    os;

    unreferenced(pdesc);

    /*
     *  Treat as an octet string
     */

    os.data = (PBYTE) pv->string;
    os.length = _A2C_c4len(pv->string)*4;
    
    return A2C_OCTET_STRING_print(&os, pdesc, iLevel, pstm);
}

/* ---
/// <summary>
/// No checks except for presence on universal strings
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_UNIVERSAL_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uint32 *      sz = ((A2C_UNIVERSAL_STRING *) pv)->string;

    unreferenced(pdesc);

    /*
     *  If not already done - push our self on the stack
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

            return A2C_Validate_Print(pdesc, pcxt, "UniversalString", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  Check for validity of characters
     */

    if (A2C_ContextNum(pcxt, iFrame) == 1) {
        /*
         *  M00SECURITY: can overflow to fault page
         */

        for (; *sz != 0; sz++) {
            if (!unicode_is_valid_char(*sz)) {
                char rgch[100];

                /*
                 *  Mark as finished
                 */

                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Illegal character U+%x", *sz);

                return A2C_Validate_Print(pdesc, pcxt, "UniversalString", rgch, pstm);
            }
        }
    }   

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}



/**************************** A2C_UTF8_STRING *******************************************/

/* ---
/// <summary>
/// A2C_UTF8_STRING_decriptor contains the dispatch table for the string type UTF8.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_UTF8_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_UTF8_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_UTF8_STRING_decode_der,
    (A2C_encoder_f) A2C_UTF8_STRING_encode_der,
    (A2C_decoder_f) A2C_UTF8_STRING_decode_ber,
    (A2C_encoder_f) A2C_UTF8_STRING_encode_ber,
    (A2C_compare_f) A2C_UTF8_STRING_compare,
    (A2C_copy_f) A2C_UTF8_STRING_copy,
    (A2C_print_f) A2C_UTF8_STRING_print,
    A2C_UTF8_STRING_validate,
    &A2C_UTF8_STRING_description
};


/* ---
/// <summary>
/// A2C_UTF8_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [12] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_decode_der(A2C_UTF8_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagUTF8String : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_UTF8_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [12] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_encode_der(A2C_UTF8_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING	os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagUTF8String : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_UTF8_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [12] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_decode_ber(A2C_UTF8_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagUTF8String : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_UTF8_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [12] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_encode_ber(A2C_UTF8_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING	os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagUTF8String : ptag, pstm);
}

A2C_ERROR A2C_UTF8_STRING_compare(A2C_UTF8_STRING const * pLeft, A2C_UTF8_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_UTF8_STRING_copy(A2C_UTF8_STRING * pLeft, A2C_UTF8_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// A2C_UTF8_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_print(A2C_UTF8_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

/* ---
/// <summary>
/// Check ???
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_UTF8_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    uint32      ch;
    A2C_ERROR   err;
    int         iFrame;
    uchar * sz = (uchar *) ((A2C_UTF8_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "UTF8String", "String is NULL", pstm);
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
         */
        
        for (; *sz != 0;) {
            /*
             *  Get the character
             */

            if (!utf8_next_char(&sz, &ch)) {
                A2C_SetContextNum(pcxt, iFrame, -1);
                return A2C_Validate_Print(pdesc, pcxt, "UTF8String", "Malformed UTF8 character string", pstm);
            }

            if (!unicode_is_valid_char(ch)) {
                char    rgch[100];
                
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Invalid character %x", ch);

                return A2C_Validate_Print(pdesc, pcxt, "UTF8String", rgch, pstm);
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


/**************************** A2C_VIDEOTEX_STRING *******************************************/

/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_decriptor contains the dispatch table for the string type VIDEOTEX.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_VIDEOTEX_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_VIDEOTEX_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_VIDEOTEX_STRING_decode_der,
    (A2C_encoder_f) A2C_VIDEOTEX_STRING_encode_der,
    (A2C_decoder_f) A2C_VIDEOTEX_STRING_decode_ber,
    (A2C_encoder_f) A2C_VIDEOTEX_STRING_encode_ber,
    (A2C_compare_f) A2C_VIDEOTEX_STRING_compare,
    (A2C_copy_f) A2C_VIDEOTEX_STRING_copy,
    (A2C_print_f) A2C_VIDEOTEX_STRING_print,
    A2C_VIDEOTEX_STRING_validate,
    &A2C_VIDEOTEX_STRING_description
};


/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_decode_der(A2C_VIDEOTEX_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagVideotexString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_encode_der(A2C_VIDEOTEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING	os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagVideotexString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_decode_ber(A2C_VIDEOTEX_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_ERROR           err;
    A2C_OCTET_STRING    os = {0};
    char *              sz;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagVideotexString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        sz = malloc(os.length+1);
        if (sz == NULL) {
            err = A2C_ERROR_outOfMemory;
        }
        else {
            memcpy(sz, os.data, os.length);
            sz[os.length] = 0;

            if (strlen(sz) != (size_t) os.length) {
                err = A2C_ERROR_dataFormat;
                free(sz);
            }
            else {
                pv->string = sz;
            }
        }
        
        free(os.data);
    }
    return err;
}

/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [20] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_encode_ber(A2C_VIDEOTEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = (PBYTE) pv->string;
    os.length = (A2C_LENGTH) strlen(pv->string);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagVideotexString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_VIDEOTEX_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_print(A2C_VIDEOTEX_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_VIDEOTEX_STRING_compare(A2C_VIDEOTEX_STRING const * pLeft, A2C_VIDEOTEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_VIDEOTEX_STRING_copy(A2C_VIDEOTEX_STRING * pLeft, A2C_VIDEOTEX_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    pLeft->string = strdup(pRight->string);
    
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// We don't do anything to validate videotex strings.  The specification of what is allowable as
/// characters and what encoding formats are allowed.
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_VIDEOTEX_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR           err;
    int                 iFrame;
    uchar *     sz = (uchar *)(((A2C_VIDEOTEX_STRING *) pv)->string);
    
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

            return A2C_Validate_Print(pdesc, pcxt, "VideotxString", "String is NULL", pstm);
        }

        /*
         *  Goto next check
         */

        A2C_SetContextNum(pcxt, iFrame, 1);
    }

    /*
     *  We don't check the actual set of characters - should be from the reg. no.
     *
     *
     *  Nobody really knows what this means any more so we are just ignoring it.
     */

    /*
     *  Pop our context - we are done
     */

    A2C_PopContext(pcxt, iFrame);

    *pf = TRUE;
    return A2C_ERROR_Success;
}


/**************************** A2C_VISIBLE_STRING *******************************************/

/* ---
/// <summary>
/// A2C_VISIBLE_STRING_decriptor contains the dispatch table for the string type VISIBLE.
/// </summary>
--- */

A2C_CONST A2C_ITEM_HEADER A2C_VISIBLE_STRING_description = {
    sizeof(A2C_BMP_STRING), 0
};

A2C_CONST A2C_DESCRIPTOR A2C_VISIBLE_STRING_descriptor = {
    A2C_string_init, A2C_string_release,
    (A2C_decoder_f) A2C_VISIBLE_STRING_decode_der,
    (A2C_encoder_f) A2C_VISIBLE_STRING_encode_der,
    (A2C_decoder_f) A2C_VISIBLE_STRING_decode_ber,
    (A2C_encoder_f) A2C_VISIBLE_STRING_encode_ber,
    (A2C_compare_f) A2C_VISIBLE_STRING_compare,
    (A2C_copy_f) A2C_VISIBLE_STRING_copy,
    (A2C_print_f) A2C_VISIBLE_STRING_print,
    A2C_VISIBLE_STRING_validate,
    &A2C_VISIBLE_STRING_description
};


/* ---
/// <summary>
/// A2C_VISIBLE_STRING_decode_der implements the DER decode functionality.  This type is implmeented
/// as [26] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_decode_der(A2C_VISIBLE_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_der(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagVisibleString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_VISIBLE_STRING_encode_der implements the DER encode functionality.  This type is implmeented
/// as [26] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_encode_der(A2C_VISIBLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = *(PBYTE *) pv;
    os.length = (A2C_LENGTH) strlen((char *) os.data);

    return A2C_OCTET_STRING_encode_der(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagVisibleString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_VISIBLE_STRING_decode_ber implements the BER decode functionality.  This type is implmeented
/// as [26] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of the decoded data</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">source of bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_decode_ber(A2C_VISIBLE_STRING * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM_MEMORY pstm)
{
    A2C_OCTET_STRING    os = {0};
    A2C_ERROR           err;
    char *              pb = NULL;

    err = A2C_OCTET_STRING_decode_ber(&os, pdesc, flags, pcxt,
                                      ptag == NULL ? &TagVisibleString : ptag, pstm);
    if (err >= A2C_ERROR_Success) {
        pb = malloc(os.length + 1);
        if (pb == NULL) {
            free(os.data);
            return A2C_ERROR_outOfMemory;
        }
        memcpy(pb, os.data, os.length);
        pb[os.length] = 0;

        if (strlen(pb) != (size_t) os.length) {
            err = A2C_ERROR_dataFormat;
            free(pb);
        }
        else {
            pv->string = pb;
        }

        free(os.data);
    }
    
    return err;
}

/* ---
/// <summary>
/// A2C_VISIBLE_STRING_encode_ber implements the BER encode functionality.  This type is implmeented
/// as [26] OCTET STRING so most of the work is passed off to OCTET_STRING in order to implement
/// </summary>
/// <param name="pv">pointer to the destination of data to encoded</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="flags">control decode operation</param>
/// <param name="pcxt">re-start context information</param>
/// <param name="ptag">over ride of default tagging if not NULL</param>
/// <param name="pstm">destination  of encoded bytes to decode</param>
/// <returns>A2C_ERROR code for operation</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_encode_ber(A2C_VISIBLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    A2C_OCTET_STRING os;

    os.data = *(PBYTE *) pv;
    os.length = (A2C_LENGTH) strlen((char *) os.data);

    return A2C_OCTET_STRING_encode_ber(&os, pdesc, flags, pcxt, 
                                       ptag == NULL ? &TagVisibleString : ptag, pstm);
}

/* ---
/// <summary>
/// A2C_VISIBLE_STRING_print creates a UTF8 encoded output stream containing the data.  This
/// functionality is common to the string routines so a common print routine is used.
/// </summary>
/// <param name="pv">pointer to the destination of data to print</param>
/// <param name="pdesc">descriptor for the data type</param>
/// <param name="iLevel">indentation level for the data</param>
/// <param name="pstm">destination of outputed bytes to decode</param>
/// <returns>A2C_ERROR code for the operation</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_print(A2C_VISIBLE_STRING const * pv, PC_A2C_DESCRIPTOR pdesc, int iLevel, PA2C_STREAM pstm)
{
    return string_print(pv->string, pdesc, iLevel, pstm);
}

A2C_ERROR A2C_VISIBLE_STRING_compare(A2C_VISIBLE_STRING const * pLeft, A2C_VISIBLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    return strcmp(pLeft->string, pRight->string);
}

A2C_ERROR A2C_VISIBLE_STRING_copy(A2C_VISIBLE_STRING * pLeft, A2C_VISIBLE_STRING const * pRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    pLeft->string = strdup(pRight->string);
    if (pLeft->string == NULL) return A2C_ERROR_outOfMemory;

    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Check 
/// </summary>
/// <param name="pv">pointer to the string data</param>
/// <param name="pdesc">descriptor for the string</param>
/// <param name="pcxt">restart context information</param>
/// <param name="pstm">location to dump data</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_VISIBLE_STRING_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, PA2C_STREAM pstm)
{
    A2C_ERROR   err;
    int         iFrame;
    uchar * sz = (uchar *) ((A2C_IA5_STRING *) pv)->string;

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

            return A2C_Validate_Print(pdesc, pcxt, "VisibleString", "String is NULL", pstm);
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
         */
        
        for (; *sz != 0; sz++) {
            if ((*sz > 127) || ((Rgf[*sz] & 4) == 0)) {
                char    rgch[100];
                
                /*
                 *  Print out error
                 */
                
                A2C_SetContextNum(pcxt, iFrame, -1);

                snprintf(rgch, sizeof(rgch), "Invalid character %s", *sz);
                return A2C_Validate_Print(pdesc, pcxt, "VisibleString", rgch, pstm);
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
