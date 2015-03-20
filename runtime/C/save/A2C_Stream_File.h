/* 
 * Copyright 2007-8 Jim Schaad and Paul Hoffman. All rights reserved.
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

#ifndef __A2C_STREAM_FILE_H__
#define __A2C_STREAM_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

    /*
     *  Create a stream that has a file as the destination.
     *
     *  The file will be:
     *          1. created (not openned)
     *          2. use binary mode
     *
     *  This stream type will return an error for the Get Data functionality
     */
    
    A2C_ERROR A2C_CreateFileStream(PA2C_STREAM *, const char * szFileName);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __A2C_STREAM_FILE_H__ */
