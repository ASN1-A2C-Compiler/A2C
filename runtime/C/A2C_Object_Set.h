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

#ifndef __A2C_OBJECT_SET_H__
#define __A2C_OBJECT_SET_H__

#include "A2C_Base.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct {
    int c;      /* Count of items */
    const A2C_ITEM_HEADER * pdesc;
    const PCVOID * rg;
} A2C_OBJECT_SET;

typedef struct {
    char *      szName;
    int         iLayer;
    int         cbRelOffset;
} A2C_COMPONENT_RELATIONS_TABLE;

typedef struct {
    const A2C_OBJECT_SET *    pos;
    char *              szField;
    int                 c;
    A2C_COMPONENT_RELATIONS_TABLE * rg;
} A2C_COMPONENT_RELATIONS;


#define A2C_CLASS_init 0
#define A2C_CLASS_decode_der 0
#define A2C_CLASS_encode_der 0
#define A2C_CLASS_decode_ber 0
#define A2C_CLASS_encode_ber 0
#define A2C_CLASS_print 0
#define A2C_CLASS_compare 0
#define A2C_CLASS_copy 0
#define A2C_CLASS_validate 0

#ifdef __cplusplus
}
#endif
#endif /* __A2C_OBJECT_SET_H__ */
