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

#ifndef __A2C_STREAM_H__
#define __A2C_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "A2C_System.h"
#include "A2C_Error.h"

extern int A2C_Indent_Size;


typedef struct _A2C_STREAM * PA2C_STREAM;

typedef A2C_ERROR (* A2C_stream_write_f) (PA2C_STREAM pstm, PCBYTE pb, A2C_LENGTH cb);
typedef A2C_ERROR (* A2C_stream_read_f)  (PA2C_STREAM pstm, PBYTE pb, A2C_LENGTH cb);
typedef A2C_ERROR (* A2C_stream_peek_f)  (PA2C_STREAM pstm, PBYTE pb, A2C_LENGTH cb);
typedef A2C_ERROR (* A2C_stream_get_f)   (PA2C_STREAM pstm, PBYTE * ppb, A2C_LENGTH * pcb);
typedef A2C_ERROR (* A2C_stream_free_f)  (PA2C_STREAM pstm);

typedef struct _A2C_STREAM {
    A2C_stream_write_f            pfnWrite;
    A2C_stream_read_f             pfnRead;
    A2C_stream_peek_f             pfnPeek;
    A2C_stream_get_f              pfnGetData;
    A2C_stream_free_f             pfnFree;
} A2C_STREAM;

/*
 *  Start with the memory based stream object.
 *
 *  Memory structure is:
 */

typedef struct {
    A2C_STREAM                  pfns;           /* functions to manipulate stream */
    int                         fAllocated;     /* buffer created with malloc? */
    PBYTE                       pbBase;         /* Pointer to start of buffer */
    PBYTE                       pb;             /* Pointer to current location */
    PBYTE                       pbLast;         /* Pointer to end of buffer */
} A2C_STREAM_MEMORY;

typedef A2C_STREAM_MEMORY * PA2C_STREAM_MEMORY;

A2C_ERROR A2C_GetDataFromStream(A2C_STREAM *, PBYTE * ppb, A2C_LENGTH * pcb);
A2C_ERROR A2C_GetStringFromStream(A2C_STREAM *, char ** psz);
A2C_ERROR A2C_FreeStream(A2C_STREAM * pstm);

/*
 *  Create a memory stream from no content
 */

A2C_ERROR A2C_CreateMemoryStream(PA2C_STREAM *);

/*
 *  Create a memory stream from a byte array
 */

A2C_ERROR A2C_CreateMemoryStreamFromBytes(PA2C_STREAM *, PBYTE pb, A2C_LENGTH cb);


#ifdef __cplusplus
}
#endif
#endif /* __A2C_STREAM_H__ */
