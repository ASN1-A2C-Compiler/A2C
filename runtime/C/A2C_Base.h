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

#ifndef __A2C_BASE_H__
#define __A2C_BASE_H__

#include "A2C_System.h"
#include "A2C_Error.h"
#include "A2C_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _A2C_DESCRIPTOR * P_A2C_DESCRIPTOR;
typedef struct _A2C_DESCRIPTOR const * PC_A2C_DESCRIPTOR;

/*
 *  Internal structures for decode/encode operations
 */

typedef struct {
    int             iItem;                  /* Item # of some type */
    A2C_LENGTH      cbLeft;                 /* # bytes left to process here */
    PBYTE           pbData;                 /* Unknown type - saved data */
    int             cbData;                 /* length of the data */
    int             rgf[4];                 /* Set flags */
    A2C_STREAM_MEMORY * pstm;               /* memory stream object */
} A2C_CXT_ITEM;
    
typedef struct {
    int             cUsed;                  /* # of contexts used */
    int             iCurrent;               /* Current context # */
    int             cAlloc;                 /* # of contexts allocted */
    A2C_CXT_ITEM *  rgCxts;                 /* context stack */

    int             cbBuffer;               /* # of bytes cached */
    PBYTE           pbBuffer;               /* bytes cached */

    int             cRoots;                 /* # of decoder roots allocated */
    int             iRoot;                  /* Index of current top decoder root */
    PBYTE *         rgpbRoots;              /* Array of decoder roots */
} A2C_CONTEXT;

/*
 *  This structure is used for holding an ASN.1 TAG.  The tag
 *  consists of the class and the value.  The value is not restricted
 *  to a specific range.
 */
    
#define A2C_TAG_CLASS_UNIVERSAL 0
#define A2C_TAG_CLASS_APPLICATION 1
#define A2C_TAG_CLASS_CONTEXT 2
#define A2C_TAG_CLASS_PRIVATE 3

typedef struct _A2C_TAG {
    int             iClass;                  /* class of tag; */
    A2C_TAG_VALUE   iValue;                  /* value of tag; */
} A2C_TAG;

/*
 *  This structure is used to describe each element in 
 *      a CHOICE, SEQUENCE or SET.  
 */

typedef struct _A2C_ITEM {
    char const *    szFieldName;            /* Name of the field */
    int             cbOffset;               /* Offset of the field */
    A2C_TAG         tag;                    /* Expected/required tag for field */
    PC_A2C_DESCRIPTOR pdu;                  /* Field descriptor table */
    BYTE const *    pvDefault;              /* Default value if one exits */
    int             flags;                  /* Controlling flags */
    int             iVersion;               /* Version # for the field */
} A2C_ITEM;

/*
 *  Describe the names in an ENUMATED, INTEGER or BIT STRING 
 */

    typedef struct {
        int             cb;
        BYTE *          pb;
    } A2C_VALUE;
    
typedef struct {
    int                         number;   /* Integer value */
    A2C_VALUE const *           pValue;   /* if != 0 huge value */
    char const *                name;     /* Name of associated value */
} A2C_NAME_ITEM;

typedef struct {
    int                 count;
    A2C_NAME_ITEM const * listArrayPtr;
} A2C_NAMES;

#define A2C_ITEM_FLAGS_EXTENDABLE               1       /* ASN.1 type is expandable */

typedef struct {
    int             cbStruct;               /* Size of the data structure */
                                            /*     (real data structure not me) */
    int             flags;
    int             cChildren;              /* Count of children */
    A2C_ITEM const* rgChildren;             /* Description of each child field */
    A2C_NAMES       names;                  /* Names for named items */
} A2C_ITEM_HEADER;

/*
 *  Prototypes for C++ style function calls
 */

typedef A2C_ERROR   (* A2C_init_f)  (PVOID pv, PC_A2C_DESCRIPTOR);
typedef A2C_ERROR   (* A2C_release_f)  (PVOID pv, PC_A2C_DESCRIPTOR);

typedef A2C_ERROR   (* A2C_decoder_f) (PVOID pvData,  PC_A2C_DESCRIPTOR, int flags,
                                       A2C_CONTEXT * pcxt, A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm);
typedef A2C_ERROR   (* A2C_encoder_f) (PCVOID pvData, PC_A2C_DESCRIPTOR, int flags,
                                       A2C_CONTEXT * pcxt, A2C_TAG const * ptag, PA2C_STREAM pstm);

typedef int         (* A2C_compare_f) (PCVOID pvData1, PCVOID pvData2, PC_A2C_DESCRIPTOR);
typedef A2C_ERROR   (* A2C_copy_f)    (PVOID pvDest, PCVOID pvSrc, PC_A2C_DESCRIPTOR);

typedef A2C_ERROR   (* A2C_print_f) (PCVOID pvData, PC_A2C_DESCRIPTOR, int iLevel, PA2C_STREAM pstm);

typedef A2C_ERROR   (* A2C_validate_f) (PCVOID pvData, PC_A2C_DESCRIPTOR,
                                        A2C_CONTEXT * pdxt, int * pf, A2C_STREAM *);

typedef struct _A2C_DESCRIPTOR {
    A2C_init_f                      pfnInit;
    A2C_release_f                   pfnRelease;
    A2C_decoder_f                   pfnDerDecoder;
    A2C_encoder_f                   pfnDerEncoder;
    A2C_decoder_f                   pfnBerDecoder;
    A2C_encoder_f                   pfnBerEncoder;
    A2C_compare_f                   pfnCompare;
    A2C_copy_f                      pfnCopy;
    A2C_print_f                     pfnPrint;
    A2C_validate_f                  pfnValidate;
    A2C_ITEM_HEADER const *         pItemDescription;
} A2C_DESCRIPTOR;

/*
 *  Flag parameters for A2C_EncodeDer, A2C_DecodeDer, A2C_EncodeBer and A2C_DecodeDer
 */

#define A2C_FLAGS_MORE_DATA 0x00000001              /* Continuation call for A2C_DerDecode and A2C_BerDecode */
#define A2C_FLAGS_STRICT_DER 0x00000002             /* Force strict DER on decode operation */
#define A2C_FLAGS_BUFFER_PROVIDED 0x00000004        /* Data buffer is provided not allocated */

A2C_ERROR A2C_Alloc(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_Free(PVOID pv, PC_A2C_DESCRIPTOR desc);

A2C_ERROR A2C_Init(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_Release(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);

A2C_ERROR A2C_EncodeDer(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags,
                        A2C_CONTEXT ** pcxt, PA2C_STREAM pstm);
A2C_ERROR A2C_DecodeDer(void **, PC_A2C_DESCRIPTOR, int, A2C_CONTEXT ** ppcxt,
                        PCBYTE pb, size_t cb);

A2C_ERROR A2C_EncodeBer(PCVOID pvData, PC_A2C_DESCRIPTOR pdesc, int flags,
                        A2C_CONTEXT ** pcxt, PA2C_STREAM pstm);
A2C_ERROR A2C_DecodeBer(PVOID *, PC_A2C_DESCRIPTOR, int, A2C_CONTEXT ** ppcxt,
                        PCBYTE pb, size_t cb);

int A2C_Compare(PCVOID, PCVOID, PC_A2C_DESCRIPTOR);
A2C_ERROR A2C_Copy(PVOID, PCVOID, PC_A2C_DESCRIPTOR);

A2C_ERROR A2C_Print(PCVOID, PC_A2C_DESCRIPTOR, PA2C_STREAM pstm);


A2C_ERROR A2C_Validate(PCVOID, PC_A2C_DESCRIPTOR, int * pf);
A2C_ERROR A2C_Validate2(PCVOID, PC_A2C_DESCRIPTOR, A2C_CONTEXT **, int * pf, A2C_STREAM *);


A2C_ERROR A2C_zero(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_nop(PVOID pvData, PC_A2C_DESCRIPTOR pdesc);
A2C_ERROR A2C_FreeContext(A2C_CONTEXT *);   /* clean up a context structure */

A2C_ERROR A2C_Version(int * piVersion);

A2C_ERROR A2C_Validate_Print(PC_A2C_DESCRIPTOR, A2C_CONTEXT *, char const *, char const *, A2C_STREAM *);

A2C_ERROR A2C_List(PC_A2C_DESCRIPTOR pdesc, A2C_NAMES const * * pNames);
A2C_ERROR A2C_NameToInt(PC_A2C_DESCRIPTOR pdesc, char const * szName, int * pi);
A2C_ERROR A2C_IntToName(PC_A2C_DESCRIPTOR pdesc, int iValue, char const ** psz);



/*
 *  Are these all internal?
 */

int             A2C_ContextFrame(A2C_CONTEXT * pcxt);

A2C_ERROR       A2C_PushContext(A2C_CONTEXT *, int *);
A2C_ERROR       A2C_PushContext2(A2C_CONTEXT *, int *, char const *, int);
A2C_ERROR       A2C_PopContext(A2C_CONTEXT *, int);

int             A2C_ContextTOS(A2C_CONTEXT *);

int             A2C_ContextNum(A2C_CONTEXT *, int);
int             A2C_SetContextNum(A2C_CONTEXT *, int, int);
A2C_ERROR       A2C_SetContextName(A2C_CONTEXT *, int, char const *);

#ifdef __cplusplus
}
#endif
#endif /* __A2C_BASE_H__ */


