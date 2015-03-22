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

#include <stdio.h>
#include <string.h>


extern A2C_CONST A2C_ITEM_HEADER A2C_INTEGER_HUGE_description;

A2C_CONST A2C_DESCRIPTOR A2C_ENUMERATED_HUGE_descriptor = {
    A2C_INTEGER_HUGE_init,
    A2C_INTEGER_HUGE_release,
    (A2C_decoder_f) A2C_ENUMERATED_HUGE_decode_der,
    (A2C_encoder_f) A2C_ENUMERATED_HUGE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_HUGE_decode_ber,
    (A2C_encoder_f) A2C_ENUMERATED_HUGE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_HUGE_compare,
    (A2C_copy_f) A2C_ENUMERATED_HUGE_copy,
    (A2C_print_f) A2C_ENUMERATED_HUGE_print,
    A2C_ENUMERATED_HUGE_validate,
    &A2C_INTEGER_HUGE_description
};

extern A2C_CONST A2C_ITEM_HEADER A2C_INTEGER_NATIVE_description;

A2C_CONST A2C_DESCRIPTOR A2C_ENUMERATED_NATIVE_descriptor = {
    A2C_zero,
    A2C_nop,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der,
    (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber,
    (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    A2C_ENUMERATED_NATIVE_validate,
    &A2C_INTEGER_NATIVE_description
};

static A2C_TAG     TagEnumerated = {A2C_TAG_CLASS_UNIVERSAL, 10};

/*
 *  Decode a DER encoded enumerated value
 */

A2C_ERROR A2C_ENUMERATED_HUGE_decode_der(A2C_ENUMERATED_HUGE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If no override tag is given, use the default tag
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    /*
     *  Pass the job onto the integer code
     */

    return A2C_INTEGER_HUGE_decode_der(pi, pdesc, 0, pcxt, ptag, pstm);
}


/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_ENUMERATED_HUGE_encode_der(A2C_ENUMERATED_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, 
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    unreferenced(flags);
    
    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If not override tag was passed in, then use the default one
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    return A2C_INTEGER_HUGE_encode_der(pi, pdesc, 0, pcxt, ptag, pstm);
}
                               
/*
 *  Decode a DER encoded enumerated value
 */

A2C_ERROR A2C_ENUMERATED_HUGE_decode_ber(A2C_ENUMERATED_HUGE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If no override tag is given, use the default tag
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    /*
     *  Pass the job onto the integer code
     */

    return A2C_INTEGER_HUGE_decode_ber(pi, pdesc, 0, pcxt, ptag, pstm);
}


/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_ENUMERATED_HUGE_encode_ber(A2C_ENUMERATED_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, 
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If not override tag was passed in, then use the default one
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    return A2C_INTEGER_HUGE_encode_ber(pi, pdesc, 0, pcxt, ptag, pstm);
}

/* ---
/// <summary>
/// A2C_ENUMERATED_HUGE_print is the default enumerated value print routine.
/// <para>This function is supplied for default purposes, however it should never be called.
/// The compiler should emit a print function for all enumerations that display the name and value
/// of the enumeration.  This function only gets called for compiler failures. </para>
/// </summary>
/// <param name="pf">pointer to the boolean value</param>
/// <param name="pdesc">enumeration descriptor - ignored</param>
/// <param name="iLevel">indentation level - ignored </param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_ENUMERATED_HUGE_print(A2C_ENUMERATED_HUGE const * pi, PC_A2C_DESCRIPTOR pdesc,
                               int iLevel, PA2C_STREAM pstm)
{
    /*
     *  We should not have arrived here, but now we don't know what to do.
     *
     *  Default to the normal integer processing
     */

    return A2C_INTEGER_HUGE_print(pi, pdesc, iLevel, pstm);
}


int A2C_ENUMERATED_HUGE_compare(A2C_ENUMERATED_HUGE const * piLeft, A2C_ENUMERATED_HUGE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_INTEGER_HUGE_compare(piLeft, piRight, pdesc);
}

int A2C_ENUMERATED_HUGE_copy(A2C_ENUMERATED_HUGE * piLeft, A2C_ENUMERATED_HUGE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    return A2C_INTEGER_HUGE_copy(piLeft, piRight, pdesc);
}

/*
/// <summary>
/// Enumerated validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
*/

A2C_ERROR A2C_ENUMERATED_HUGE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
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

/*****************************************************************************************/

A2C_ERROR A2C_ENUMERATED_NATIVE_decode_der(A2C_ENUMERATED_NATIVE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If no override tag is given, use the default tag
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    /*
     *  Pass the job onto the integer code
     */

    return A2C_INTEGER_NATIVE_decode_der(pi, pdesc, 0, pcxt, ptag, pstm);
}


/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_ENUMERATED_NATIVE_encode_der(A2C_ENUMERATED_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, 
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If not override tag was passed in, then use the default one
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    return A2C_INTEGER_NATIVE_encode_der(pi, pdesc, 0, pcxt, ptag, pstm);
}
                               
/*
 *  Decode a DER encoded enumerated value
 */

A2C_ERROR A2C_ENUMERATED_NATIVE_decode_ber(A2C_ENUMERATED_NATIVE * pi, PC_A2C_DESCRIPTOR pdesc, int flags,
                                    A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    unreferenced(flags);
    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If no override tag is given, use the default tag
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    /*
     *  Pass the job onto the integer code
     */

    return A2C_INTEGER_NATIVE_decode_ber(pi, pdesc, 0, pcxt, ptag, pstm);
}


/*
 *  Write out a DER encoded integer
 */

A2C_ERROR A2C_ENUMERATED_NATIVE_encode_ber(A2C_ENUMERATED_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc, int flags, 
                              A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm)
{
    unreferenced(flags);

    /*
     *  This is encoded just like an integer, but change the tag
     *
     *  If not override tag was passed in, then use the default one
     */

    if (ptag == NULL) {
        ptag = &TagEnumerated;
    }

    return A2C_INTEGER_NATIVE_encode_ber(pi, pdesc, 0, pcxt, ptag, pstm);
}
                               
/* ---
/// <summary>
/// A2C_ENUMERATED_NATIVE_print is the default enumerated value print routine.
/// <para>This function is supplied for default purposes, however it should never be called.
/// The compiler should emit a print function for all enumerations that display the name and value
/// of the enumeration.  This function only gets called for compiler failures. </para>
/// </summary>
/// <param name="pf">pointer to the boolean value</param>
/// <param name="pdesc">enumeration descriptor - ignored</param>
/// <param name="iLevel">indentation level - ignored </param>
/// <param name="pstm">A2C_STREAM object to print into</param>
/// <return>1 on success, A2C_ERROR on failure.
--- */

A2C_ERROR A2C_ENUMERATED_NATIVE_print(A2C_ENUMERATED_NATIVE const * pi, PC_A2C_DESCRIPTOR pdesc,
                               int iLevel, PA2C_STREAM pstm)
{
    int                 i;
    A2C_NAMES const *   pNames = &pdesc->pItemDescription->names;
    char        rgch[128];

    /*
     *  We don't use these parameters
     */

    unreferenced(pdesc);
    unreferenced(iLevel);

    /*
     *  Walk the list of enumerated names looking for a good value
     */

    for (i=0; i<pNames->count; i++) {
        if (pNames->listArrayPtr[i].number == pi->nData) {
            return pstm->pfnWrite(pstm, (PBYTE) pNames->listArrayPtr[i].name, (A2C_LENGTH) strlen(pNames->listArrayPtr[i].name));
        }
    }

    /*
     *  We should never have gotten here.
     *  The enumeration is known at a higher level - try your best
     */

    snprintf(rgch, sizeof(rgch), "Unknown Enumeration Value %d", pi->nData);

    /*
     *  Force termination
     */

    rgch[sizeof(rgch)-1] = 0;

    return pstm->pfnWrite(pstm, (PBYTE) rgch, (A2C_LENGTH) strlen(rgch));
}


int A2C_ENUMERATED_NATIVE_compare(A2C_ENUMERATED_NATIVE const * piLeft, A2C_ENUMERATED_NATIVE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);

    return piLeft->nData - piRight->nData;
}

int A2C_ENUMERATED_NATIVE_copy(A2C_ENUMERATED_NATIVE * piLeft, A2C_ENUMERATED_NATIVE const * piRight, PC_A2C_DESCRIPTOR pdesc)
{
    unreferenced(pdesc);
    
    *piLeft = *piRight;
    
    return A2C_ERROR_Success;
}


/*
/// <summary>
/// Enumerated validation always succeeds.  We never have any issues here.
/// </summary>
/// <param name="pv">pointer to data</param>
/// <param name="pdesc">pointer to data descriptor </param>
/// <param name="pcxt">pointer to context of validate</param>
/// <param name="pstm">location of output data</param>
/// <returns>A2C_ERROR_Success</param>
*/

A2C_ERROR A2C_ENUMERATED_NATIVE_validate(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, A2C_CONTEXT * pcxt, int * pf, A2C_STREAM * pstm)
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



/***********************************************************************************************************/


A2C_ERROR A2C_ENUMERATED_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames)
{
    *pNames = &pdesc->pItemDescription->names;

    return A2C_ERROR_Success;
}

A2C_ERROR A2C_ENUMERATED_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi)
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

A2C_ERROR A2C_ENUMERATED_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz)
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
