/* 
 * Copyright 2007 Jim Schaad and Paul Hoffman. All rights reserved.
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

#include <stdio.h>
#include "A2C_Stream.h"
#include "A2C_Stream_Internal.h"

typedef struct {
    A2C_STREAM          pfns;           /* functions to manipulate the stream */
    FILE *              fp;             /* file pointer */
} A2C_STREAM_FILE;

/* ---
/// <summary>
/// Write data to the stream.
/// </summary>
/// <param name="pstm">the stream object to use</param>
/// <param name="pbIn">buffer to write from</param>
/// <param name="cbIn">number of bytes to be written</param>
/// <returns>A2C Error code</returns>
--- */

A2C_ERROR _A2C_File_Write(PA2C_STREAM pstm, PCBYTE pbIn, A2C_LENGTH cbIn)
{
    A2C_LENGTH          cbOut;
    A2C_STREAM_FILE *   p = (A2C_STREAM_FILE *) pstm;
    
    /*
     *  Write to the file and return success if all bytes went out
     */

    cbOut = fwrite(pbIn, 1, cbIn, p->fp);

    if (cbIn != cbOut) {
        return A2C_ERROR_fail;
    }
    return A2C_ERROR_Success;
}

/* ---
/// <summary>
/// read opertion is not supported for this stream
/// </summary>
/// <returns>A2C_ERROR_notImplemented</returns>
--- */

A2C_ERROR _A2C_File_Read(PA2C_STREAM pstm, PBYTE pbIn, A2C_LENGTH cbIn)
{
    unreferenced(pstm);
    unreferenced(pbIn);
    unreferenced(cbIn);

    return A2C_ERROR_notImplemented;
}

/* ---
/// <summary>
/// peek operation is not supported for this stream type
/// </summary>
/// <returns>A2C_ERROR_notImplemented</returns>
--- */

A2C_ERROR _A2C_File_Peek(PA2C_STREAM pstm, PBYTE pbIn, A2C_LENGTH cbIn)
{
    unreferenced(pstm);
    unreferenced(pbIn);
    unreferenced(cbIn);

    return A2C_ERROR_notImplemented;
}

/* ---
/// <summary>
/// Get data operation is not supported
/// </summary>
/// <returns>A2C_ERROR_notImplemented</returns>
--- */

A2C_ERROR _A2C_File_GetData(PA2C_STREAM pstm, PBYTE * ppb, A2C_LENGTH * pcb)
{
    unreferenced(pstm);
    unreferenced(ppb);
    unreferenced(pcb);

    return A2C_ERROR_notImplemented;
}

/* ---
/// <summary>
/// Free all items and close the associated file
/// </summary>
/// <param name="pstm">stream to be freed</param>
/// <returns>A2C Error code</returns>
--- */

A2C_ERROR _A2C_File_Free(PA2C_STREAM pstm)
{
    A2C_STREAM_FILE *   p = (A2C_STREAM_FILE *) pstm;
    
    fclose(p->fp);

    free(p);

    return A2C_ERROR_Success;
}


/* ---
/// <summary>
/// Create an output only stream attached to a file
/// </summary>
/// <param name="ppstm">location to return the stream pointer</param>
/// <param name="szFileName">file name to use for the stream</param>
/// <returns>A2C_ERROR code</returns>
--- */

A2C_ERROR A2C_CreateFileStream(PA2C_STREAM * ppstm, const char * szFileName)
{
    A2C_ERROR           err;
    FILE *              fp = NULL;
    A2C_STREAM_FILE *   pstm;

    /*
     *  Allocate the space to hold me
     */

    pstm = (A2C_STREAM_FILE *) malloc(sizeof(A2C_STREAM_FILE));
    if (pstm == NULL) {
        err = A2C_ERROR_outOfMemory;
        goto Exit;
    }

    /*
     *  Try and open the file in question
     */

    fp = fopen(szFileName, "wb");
    if (fp == NULL) {
        err = A2C_ERROR_invalidParameter;
        goto Exit;
    }

    /*
     *  Finish all of the setup
     */

    pstm->pfns.pfnWrite = _A2C_File_Write;
    pstm->pfns.pfnRead = _A2C_File_Read;
    pstm->pfns.pfnPeek = _A2C_File_Peek;
    pstm->pfns.pfnGetData = _A2C_File_GetData;
    pstm->pfns.pfnFree = _A2C_File_Free;
    pstm->fp = fp;

    fp = NULL;
    err = A2C_ERROR_Success;
    *ppstm = (PA2C_STREAM) pstm;

Exit:
    if (fp != NULL) fclose(fp);
    return err;
}
