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

#ifndef __A2C_ERROR_H__
#define __A2C_ERROR_H__

typedef enum {
    A2C_ERROR_Success=0,
    A2C_ERROR_True = 1,
    A2C_ERROR_False = 0,
    A2C_ERROR_fail = -1,                        /* Unknown/unspecified failure */
    A2C_ERROR_bufferOverRun = -2,
    A2C_ERROR_outOfMemory = -3,
    A2C_ERROR_ICE = -5,                                /* Internal compiler error */
    A2C_ERROR_needMoreData = -6,
    A2C_ERROR_tagMismatch = -7,
    A2C_ERROR_malformedDer = -8,                        /* Not correctly DER encoded */
    A2C_ERROR_dataFormat = -9,
    A2C_ERROR_dataTooLarge = -10,
    A2C_ERROR_malformedEncoding = -12,                 /* Input was incorrectly encoded */
    A2C_ERROR_notImplemented = -13,                    /* Code does not exist for something */
    A2C_ERROR_notExtensible = -14,
    A2C_ERROR_missingSetField = -15,
    A2C_ERROR_indexOutOfRange = -16,                    /* Index is out of range */
    A2C_ERROR_notFound = -17,                           /* Item not found */
    A2C_ERROR_dataRange = -18,                          /* Data value will not fit in buffer */
    A2C_ERROR_invalidParameter = -19                    /* Some data is incorrect */

}  A2C_ERROR;



#endif /* __A2C_ERROR_H__ */


