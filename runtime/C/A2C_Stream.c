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

#include "A2C_Stream_Internal.h"

/*
 *  Utility functions defined
 */

int A2C_Indent_Size = 2;

/*
 *
 */

A2C_ERROR _A2C_Memory_Write(A2C_STREAM_MEMORY * pstm, PCBYTE pbIn, size_t cbIn)
{
    int                 cbAlloc;
    PBYTE               pb;

    /*
     *  Are we going to overflow the buffer?  If so re-allocate it
     */
    
    if ((pstm->pbBase == NULL) || ((pstm->pb + cbIn) >= pstm->pbLast)) {
        if ((pstm->pbBase != NULL) && !pstm->fAllocated) {
            return A2C_ERROR_bufferOverRun;
        }

        /*
         *  Round up to the nearest 1K boundary
         */

        cbAlloc = (cbIn + 1023) & ~1023;

        /*
         *  Allocate new buffers in 1K chunks
         */
        
        pstm->fAllocated = 1;
        pb = realloc(pstm->pbBase, (pstm->pbLast- pstm->pbBase) + cbAlloc);
        if (pb == NULL) {
            return A2C_ERROR_outOfMemory;
        }

        /*
         *  Setup the new pointers
         */
        
        pstm->pbLast = (pstm->pbLast - pstm->pbBase) + cbAlloc + pb;
        pstm->pb = (pstm->pb - pstm->pbBase) + pb;
        pstm->pbBase = pb;
    }

    /*
     *  Copy the bytes to the buffer
     */

    memcpy(pstm->pb, pbIn, cbIn);
    pstm->pb += cbIn;

    /*
     *  Return the number of bytes copied
     */

    return A2C_ERROR_Success;
}

A2C_ERROR _A2C_Memory_Read(A2C_STREAM_MEMORY * pstm, PBYTE pbIn, size_t cbIn)
{
    /*
     *  Do we have enough bytes to return the data?
     */

    if ((pstm == NULL) || (pstm->pb+cbIn) > pstm->pbLast) {
        return A2C_ERROR_bufferOverRun;
    }

    /*
     *  Copy over the bytes and return
     */

    memcpy(pbIn, pstm->pb, cbIn);
    pstm->pb += cbIn;

    return A2C_ERROR_Success;
}

A2C_ERROR _A2C_Memory_Peek(A2C_STREAM_MEMORY * pstm, PBYTE pbIn, size_t cbIn)
{
    /*
     *  Do we have enough bytes to return the data?
     */

    if ((pstm == NULL) || (pstm->pb+cbIn) >= pstm->pbLast) {
        return A2C_ERROR_bufferOverRun;
    }

    /*
     *  Copy over the bytes and return
     */

    memcpy(pbIn, pstm->pb, cbIn);

    return A2C_ERROR_Success;
}

A2C_ERROR _A2C_Indent(A2C_STREAM * pstmIn, int iLevel)
{
    int                 err;
    static const BYTE   rgb[65] = "                                                                ";

    iLevel *= A2C_Indent_Size;

    while (iLevel > sizeof(rgb)) {
        err = pstmIn->pfnWrite(pstmIn, rgb, sizeof(rgb)-1);
        if (err < 0) return err;
        
        iLevel -= sizeof(rgb);
    }

    return pstmIn->pfnWrite(pstmIn, rgb, iLevel);
}

A2C_ERROR _A2C_Memory_GetData(A2C_STREAM_MEMORY * pstm, PBYTE * ppb, A2C_LENGTH * pcb)
{
    /*
     *  Setup the return variables
     */
    
    *ppb  = pstm->pbBase;
    *pcb = (int) (pstm->pb - pstm->pbBase);

    /*
     * We no longer own the data
     */

    pstm->fAllocated = 0;
    pstm->pbBase = 0;
    pstm->pb = 0;
    pstm->pbLast = 0;

    return A2C_ERROR_Success;
}

A2C_ERROR _A2C_Memory_Free(A2C_STREAM_MEMORY * pstm)
{
    if (pstm != NULL) {
        if (pstm->fAllocated) {
            free(pstm->pbBase);
        }

        free(pstm);
    }

    return A2C_ERROR_Success;
}

void _A2C_Memory_Cleanup(A2C_STREAM_MEMORY * pstm)
{
    if (pstm->fAllocated) {
        free(pstm->pbBase);
    }
}


void _A2C_Memory_Init_With_Data(A2C_STREAM_MEMORY * pstm, PCBYTE pb, size_t cb)
{
    /*
     *  Set up the pointers to the passed in values
     */

    pstm->fAllocated = 0;
    pstm->pbBase = (PBYTE) pb;
    pstm->pb = (PBYTE) pb;
    pstm->pbLast = (PBYTE) pb + cb;

    /*
     *  Initialize function pointers
     */
    
    pstm->pfns.pfnWrite = (A2C_stream_write_f) _A2C_Memory_Write;
    pstm->pfns.pfnRead = (A2C_stream_read_f) _A2C_Memory_Read;
    pstm->pfns.pfnPeek = (A2C_stream_peek_f) _A2C_Memory_Peek;
    pstm->pfns.pfnGetData = (A2C_stream_get_f) _A2C_Memory_GetData;
    pstm->pfns.pfnFree = (A2C_stream_free_f) _A2C_Memory_Free;

}    

void _A2C_Memory_Init(A2C_STREAM_MEMORY * pstm)
{
    /*
     *  Initialize function pointers
     */
    
    pstm->pfns.pfnWrite = (A2C_stream_write_f) _A2C_Memory_Write;
    pstm->pfns.pfnRead = (A2C_stream_read_f) _A2C_Memory_Read;
    pstm->pfns.pfnPeek = (A2C_stream_peek_f) _A2C_Memory_Peek;
    pstm->pfns.pfnGetData = (A2C_stream_get_f) _A2C_Memory_GetData;
    pstm->pfns.pfnFree = (A2C_stream_free_f) _A2C_Memory_Free;

    /*
     *  Set up the pointers to the passed in values
     */

    pstm->fAllocated = 0;
    pstm->pbBase = 0;
    pstm->pb = 0;
    pstm->pbLast = 0;
}    

A2C_ERROR _A2C_Memory_Copy(A2C_STREAM_MEMORY * pstmSrc, A2C_STREAM_MEMORY * pstmDst, size_t cb)
{
    A2C_ERROR           err;
    
    /*
     *  Are there enough bytes in the source?
     */

    if ((pstmSrc == NULL) || ((pstmSrc->pb + cb) > pstmSrc->pbLast)) {
        return A2C_ERROR_bufferOverRun;
    }

    /*
     *  Write the bytes to the destination
     */

    err = _A2C_Memory_Write(pstmDst, pstmSrc->pb, cb);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Skip past the data we just "read"
     */

    pstmSrc->pb += cb;

    return A2C_ERROR_Success;
}

/**********************************************************************************/

/* ---
/// <summary>
/// Create an expandable buffer which can be used as a stream target.
/// </summary>
/// <param name="ppstm">location to return the new stream pointer</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_CreateMemoryStream(PA2C_STREAM * ppstm)
{
    A2C_STREAM_MEMORY * pstm;

    /*
     *  Allocate the buffer
     *
     *  Calloc zero inits the buffers
     */

    pstm = calloc(sizeof(A2C_STREAM_MEMORY), 1);
    if (pstm == NULL) {
        return A2C_ERROR_outOfMemory;
    }

    pstm->fAllocated = 1;
    
    /*
     *  Initialize function pointers
     */
    
    pstm->pfns.pfnWrite = (A2C_stream_write_f) _A2C_Memory_Write;
    pstm->pfns.pfnRead = (A2C_stream_read_f) _A2C_Memory_Read;
    pstm->pfns.pfnPeek = (A2C_stream_peek_f) _A2C_Memory_Peek;
    pstm->pfns.pfnGetData = (A2C_stream_get_f) _A2C_Memory_GetData;
    pstm->pfns.pfnFree = (A2C_stream_free_f) _A2C_Memory_Free;

    /*
     *  Return the result
     */

    *ppstm = (PA2C_STREAM) pstm;
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// Create a stream object and initialize it to point to the provided buffer.
/// The buffer contents must not change during the use of the stream or the
/// contents of the stream will change as well
/// </summary>
/// <param name="ppstm">return location for the created stream</param>
/// <param name="pb">pointer to byte array for data</param>
/// <param name="cb">count of bytes</param>
/// <returns>A2C_ERROR</returns>
--- */

A2C_ERROR A2C_CreateMemoryStreamFromBytes(PA2C_STREAM * ppstm, PBYTE pb, size_t cb)
{
    A2C_ERROR           err;
    A2C_STREAM_MEMORY * pstm;

    /*
     *   Allocate the object - check for a possible error
     */
    
    err = A2C_CreateMemoryStream((PA2C_STREAM *) &pstm);
    if (err < A2C_ERROR_Success) {
        return err;
    }

    /*
     *  Buffer is not allocated
     */
    
    pstm->fAllocated = 0;

    /*
     *  Set up the pointers to the passed in values
     */

    pstm->pbBase = pb;
    pstm->pb = pb;
    pstm->pbLast = pb + cb;

    /*
     *  Return the allocated stream
     */

    *ppstm = (PA2C_STREAM) pstm;
    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// Allow the stream to free itself and go away.  Code depends on the implementation
/// of the stream.
/// </summary>
/// <params name="pstm">pointer to the stream to be freed</params>
/// <returns>A2C Error code</returns>
--- */

A2C_ERROR A2C_FreeStream(PA2C_STREAM pstm)
{
    return pstm->pfnFree(pstm);
}

/* ---
/// <summary>
/// Extract data from the stream - once you have done so you are reponsible to
/// free the buffer returned.  The stream will no longer have any data after this
/// function call. Data is allocated with malloc.
/// </summary>
/// <params name="pstm">pointer to the stream to be freed</params>
/// <params name="ppb">return location of buffer pointer</params>
/// <params name="pcb">return locaion of buffer length</params>
/// <returns>A2C Error code</returns>
--- */

A2C_ERROR A2C_GetDataFromStream(PA2C_STREAM pstm, PBYTE * ppb, int * pcb)
{
    size_t		cb;
    A2C_ERROR	err;
    
    err = pstm->pfnGetData(pstm, ppb, &cb);
    
    if (cb & 0xffff0000) return A2C_ERROR_bufferOverRun;
    
    *pcb = (int) cb;
    return err;
}

/* ---
/// <summary>
/// Extract data from the stream - once you have done so you are reponsible to
/// free the buffer returned.  The stream will no longer have any data after this
/// function call. Data is allocated with malloc.  Data is guarenteed to be zero
/// terminated
/// </summary>
/// <params name="pstm">pointer to the stream to be freed</params>
/// <params name="ppb">return location of buffer pointer</params>
/// <params name="pcb">return locaion of buffer length</params>
/// <returns>A2C Error code</returns>
--- */

A2C_ERROR A2C_GetStringFromStream(PA2C_STREAM pstm, char ** psz)
{
    BYTE        b = 0;
    size_t      cb;
    A2C_ERROR   err;
    
    /*
     *  Make the buffer zero termianted
     */

    err = pstm->pfnWrite(pstm, &b, 1);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Retrieve the buffer
     */
    
    return pstm->pfnGetData(pstm, (PBYTE *) psz, &cb);
}

