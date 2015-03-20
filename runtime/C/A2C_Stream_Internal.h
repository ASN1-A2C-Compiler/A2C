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

#include "A2C_System.h"

#ifndef __A2C_STREAM_INTERNAL_H__
#define __A2C_STREAM_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  These are the supporting functions for the memory stream object.
 *  We use these calls directly internally.
 */

A2C_ERROR _A2C_Memory_Write(A2C_STREAM_MEMORY * pstmIn, PCBYTE pbIn, size_t cbIn);
A2C_ERROR _A2C_Memory_Read(A2C_STREAM_MEMORY * pstmIn, PBYTE pbIn, size_t cbIn);
A2C_ERROR _A2C_Memory_Peek(A2C_STREAM_MEMORY * pstmIn, PBYTE pbIn, size_t cbIn);
A2C_ERROR _A2C_Memory_Indent(A2C_STREAM_MEMORY * pstmIn, int iLevel);
A2C_ERROR _A2C_Memory_GetData(A2C_STREAM_MEMORY * pstmIn, PBYTE * ppb, A2C_LENGTH * pcb);
A2C_ERROR _A2C_Memory_Free(A2C_STREAM_MEMORY * pstmIn);
A2C_ERROR _A2C_Memory_Copy(A2C_STREAM_MEMORY * pstmIn, A2C_STREAM_MEMORY * pstmOut, size_t cb);
void _A2C_Memory_Init(A2C_STREAM_MEMORY * pstm);
void _A2C_Memory_Init_With_Data(A2C_STREAM_MEMORY * pstm, PCBYTE pb, size_t cb);
void _A2C_Memory_Cleanup(A2C_STREAM_MEMORY * pstm);
A2C_ERROR _A2C_Indent(A2C_STREAM * pstm, int iLevel);

/*
 *  These should be inlines, but I don't know how to do it correctly
 *  for non windows compilers
 */

A2C_INLINE void _A2C_Memory_Skip(A2C_STREAM_MEMORY * pstm, size_t cbSkip) { (pstm)->pb += cbSkip; }
A2C_INLINE A2C_LENGTH _A2C_Memory_Length(A2C_STREAM_MEMORY * pstm) { return (A2C_LENGTH) ((pstm)->pb - (pstm)->pbBase); }
A2C_INLINE A2C_LENGTH _A2C_Memory_Left(A2C_STREAM_MEMORY * pstm) { return (A2C_LENGTH) ((pstm)->pbLast - (pstm)->pb); }

#ifdef __cplusplus
}
#endif
#endif /* __A2C_STREAM_INTERNAL_H__ */
