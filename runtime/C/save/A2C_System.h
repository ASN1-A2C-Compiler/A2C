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

#ifndef __A2C_SYSTEM_H__
#define __A2C_SYSTEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef unsigned char BYTE;
    typedef BYTE * PBYTE;
    typedef BYTE const * PCBYTE;
    typedef void * PVOID;
    typedef void const * PCVOID;

#define A2C_CONST /* Define to "const" if use -const=yes for compiler */

#ifdef _MSC_VER
#define unreferenced(a) a;
#else
#define unreferenced(a) a=a;
#endif

#define OFFSET_OF(type, item) ((size_t) (&(((type *) 0)->item)))
#ifndef countof
#define countof(a) (sizeof(a)/sizeof(a[0]))
#endif
#define REL_OFFSET_OF(type, item1, item2) (OFFSET_OF(type, item1)-OFFSET_OF(type, item2))
#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef _MSC_VER
#define A2C_INLINE __inline
#else
#define A2C_INLINE static __inline
#endif

/*
 *  This is used for type length part of the TLV encoder.  This MUST be a signed
 *      type or the code will not work.
 */
    
typedef int A2C_LENGTH;
typedef int A2C_TAG_VALUE;


/*
 * For some unknown reason the Microsoft compiler really does wants the underscore
 */

#ifdef _MSC_VER
#define snprintf _snprintf
#endif /* _MSC_VER */

#ifdef __cplusplus
}
#endif
#endif /* __A2C_SYSTEM_H__ */

