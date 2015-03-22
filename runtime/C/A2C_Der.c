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

#include "A2C_Stream_Internal.h"

/*
 *  RgbToUInt
 *
 *  Description:
 *      Convert a network ordered byte array into a native integer
 *      Assume that array is unsigned.
 */

static int RgbToUInt(BYTE rgb[], unsigned int * piData)
{
    int i;
    int v = 0;

    for (i=0; i< 4; i++) {
        v <<= 8;
        v |= rgb[i];
    }
    
    *piData = v;
    return 1;
}


/* ---
/// <summary>
/// Internal utility function to retrieve the type and length from the stream as well as
/// returning other data.
/// </summary>
/// <param name="pstm">input memory buffer</param>
/// <param name="piClass">returns class of the tag</param>
/// <param name="pfConstructed">returns if tag is constructed</param>
/// <param name="piValue">returns tag value</param>
/// <param name="pcbLength">returns value length</param>
/// <param name="pcbTL">returns length of Tag and Length</param>
--- */

A2C_ERROR _A2C_get_tag_and_length(A2C_STREAM_MEMORY * pstm, int *piClass, int *pfConstructed, A2C_TAG_VALUE *piValue, A2C_LENGTH *pcbLength, A2C_LENGTH *pcbTL)
{
    A2C_ERROR   err;
    PBYTE       pb = pstm->pb;
    PBYTE       pbLast = pstm->pbLast;

    if (pb == pbLast) {
        return A2C_ERROR_needMoreData;
    }

    /*
     *  Grab the class to return
     */

    *piClass = (pb[0] >> 6);

    /*
     *  Grab the constructed flag
     */

    *pfConstructed = (pb[0] >> 5) & 0x1;

    /*
     *  Is this a long or short form?
     */

    if ((pb[0] & 0x1f) < 0x1f) {
        /*
         *  Short form - set the tag value
         */

        *piValue = (pb[0] & 0x1f);
    }
    else {
        A2C_TAG_VALUE   tag;
        
        /*
         *  Long form - need to build out the actual tag value
         */

        tag = 0;
        do {
            pb++;
            if (pb == pbLast) {
                err = A2C_ERROR_needMoreData;
                goto ErrorExit;
            }

            /*
             *  Check for overflow
             *
             *  sizeof(tag)     Mask
             *     1           0xfe<<0          0xfe
             *     2           0xfe<<8          0xfe00
             *     4           0xfe<<24         0xfe000000
             */

            if (tag & (0x00fe << (sizeof(tag)*8-8))) {
                err = A2C_ERROR_bufferOverRun;
                goto ErrorExit;
            }
            
            tag <<= 7;
            tag += (*pb) & 0x7f;
        } while ((*pb) & 0x80);

        *piValue = tag;
    }

    pb++;
    if (pb == pbLast) {
        err = A2C_ERROR_needMoreData;
        goto ErrorExit;
    }

    /*
     *  Check if this is a long or short version
     */

    if (*pb & 0x80) {
        BYTE    b;
        BYTE    rgb[sizeof(int)] = {0};

        if (*pb == 0x80) {
            *pcbLength = -1;
            pb++;
        }
        else {
        
            /*
             *  Long version
             */

            b = (*pb) & 0x7f;

            if (b > sizeof(int)) {
                err = A2C_ERROR_dataTooLarge;
                goto ErrorExit;
            }

            pb++;
            if (pb + b >= pbLast) {
                err = A2C_ERROR_needMoreData;
                goto ErrorExit;
            }
        
            memcpy(&rgb[sizeof(int)-b], pb, b);

            if (!RgbToUInt(rgb, (unsigned int *) pcbLength)) {
                err = A2C_ERROR_dataTooLarge;
                goto ErrorExit;
            }
        
            pb += b;
        }
    }
    else {
        /*
         *  Short Version
         */

        *pcbLength = *pb;
        pb++;
    }

    *pcbTL = (A2C_LENGTH) (pb - pstm->pb);

    /*
     *  M00BUG - Check for overflow
     */

    err = A2C_ERROR_Success;

ErrorExit:
    return err;
}


/*
 *  Write byte to the stream
 */

int _A2C_write_bytes(PCBYTE pbData, size_t cbData, PA2C_STREAM pstm)
{
    if (pstm == 0) {
        return A2C_ERROR_ICE;
    }

    return pstm->pfnWrite(pstm, pbData,(A2C_LENGTH)  cbData);
}

/*
 *  Write a DER encoded length into a stream
 */

A2C_ERROR _A2C_write_length(size_t cbLength, PA2C_STREAM pstm, int * pcb)
{
    unsigned int        cbOut;
    A2C_ERROR           err;
    int                 i;

    /*
     *  Are we doing an indefinite length encode?
     */

    if (cbLength == -1) {
        BYTE b = 0x80;
        
        err = pstm->pfnWrite(pstm, &b, 1);
        return err;
    }

	if (cbLength & 0xffff0000) return A2C_ERROR_bufferOverRun;
    
    /*
     *  size = 1 if length <= 0x7f
     *  else size = minumum # of bytes to hold the value
     */

    if (cbLength < 0x80) cbOut = 1;
    else  {
        unsigned int cb = (A2C_LENGTH) cbLength;
        for (i=1; cb != 0; i++, cb = cb >> 8);
        cbOut = i;
    }

    /*
     * If just getting the length - return it
     */

    if (pstm == 0) {
        *pcb = cbOut;
        return A2C_ERROR_Success;
    }

    /*
     *  Output the contents
     */

    if (cbOut == 1) {
        BYTE b = (BYTE) (cbLength & 0xff);
        
        err = pstm->pfnWrite(pstm, &b, 1);
        if (err < A2C_ERROR_Success) {
            return err;
        }
        cbOut = 1;
    }
    else {
        BYTE    rgb[4];
        
        rgb[0] = (BYTE) ((cbOut-1) | 0x80);
        for (i=cbOut-1; cbLength != 0; i--) {
            rgb[i] = (BYTE) (cbLength & 0xff);
            cbLength = cbLength >> 8;
        }
        err = pstm->pfnWrite(pstm, rgb, cbOut);
        if (err < A2C_ERROR_Success) {
            return err;
        }
    }

    *pcb = cbOut;
    return A2C_ERROR_Success;
}

/*
 *  Write a DER encoded tag into a stream
 */

A2C_ERROR _A2C_write_tag(int tag_type, int fConstructed, int tag_value, PA2C_STREAM pstm, int * pcb)
{
    int cbTag;
    
    /*
     *  Start by computing the size of the tag emited.
     *
     *  if the tag value is less than 31 - stores into a single byte
     *  if the tag value is greater - 1 byte plus # of 7 bit blocks in the value.
     */

    if (tag_value < 31) cbTag = 1;
    else {
        int val;
        
        for (cbTag=2, val=tag_value; val != 0; cbTag++, val = val >> 7);
    }

    /* Are we just getting the size? */

    if (pstm == 0) {
        *pcb = cbTag;
        return A2C_ERROR_Success;
    }

    /*  Place into the destination array */

    if (tag_value < 31) {
        BYTE b = (char) ((tag_type << 6) | (!!fConstructed << 5) | tag_value);
        *pcb = 1;
        return pstm->pfnWrite(pstm, &b, 1);
    }
    else {
        /*
         *  Compute the array of bytes, then copy the significant ones to the
         *      destination buffer
         */
        
        BYTE rgb[6] = {0};
        int i;
        char f = 0;
        
        for (i=5; i >= 0; i--) {
            
            /*
             * Save the value, and or in the continuation bit
             */
            
            rgb[i] = (char) ((tag_value & 0x7f) | f);

            /*
             *  shift over the tag value
             */

            tag_value = tag_value >> 7;

            /*
             *  If zero - we can exit and do the copy now
             */
            
            if (tag_value == 0) {
                break;
            }

            /*
             *  Set the continuation bit after the first item 
             */
            
            f = (unsigned char) 0x80;
        }
        rgb[i-1] = (char) ((tag_type << 6) | (!!fConstructed << 5) | 31);

        *pcb = 6-i;
        return pstm->pfnWrite(pstm, &rgb[i-1], 6-i);
    }
}
