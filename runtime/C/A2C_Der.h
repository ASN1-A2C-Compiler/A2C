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

#ifndef __A2C_DER_H__
#define __A2C_DER_H__

#include "A2C_Base.h"
#include "A2C_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    void IntToRgb(int iData, BYTE rgb[]);
    int RgbToUInt(BYTE rgb[], unsigned int * piData);
    int RgbToInt(BYTE rgb[], int cb, int * piData);
*/

#define A2C_FLAGS_TAG_IMPLICIT 0x00000001
#define A2C_FLAGS_TAG_EXPLICIT 0x00000002
#define A2C_FLAGS_OPTIONAL     0x00000010      /* Field is an optional field */
#define A2C_FLAGS_DEFAULT      0x00000020      /* Field is a defaulted field - data is in the record */
#define A2C_FLAGS_DEFAULT_PTR  0x00000040      /* Field is a defaulted field - data is pointed to */
#define A2C_FLAGS_UNIQUE       0x00000100      /* Field is tagged UNIQUE */
#define A2C_FLAGS_DATA_PTR     0x00000200      /* Data points to field value */
#define A2C_FLAGS_EXTENSION_END 0x00010000     /* End of possible extension marker */


    A2C_ERROR _A2C_write_tag(int tag_type, int fConstructed, int tag_value, PA2C_STREAM pstm, int * pcbOut);
    A2C_ERROR _A2C_write_length(size_t cbLength, PA2C_STREAM pstm, int * pcb);
    A2C_ERROR _A2C_write_bytes(PCBYTE pbData, size_t cbData, PA2C_STREAM pstm);
    A2C_ERROR _A2C_get_tag_and_length(A2C_STREAM_MEMORY * pstm, int * piClass, int * pfConstructed,
                                      A2C_TAG_VALUE * piValue, A2C_LENGTH * pcbLength, A2C_LENGTH * pcbTL);


#ifdef __cplusplus
};
#endif

#endif /* __A2C_DER_H__ */
