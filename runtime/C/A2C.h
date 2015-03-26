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

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#ifndef _MCS_VER
#define __cdecl __attibute__((__cdecl__))
#endif

#include "A2C_Base.h"
#include "A2C_Binary.h"
#include "A2C_BitString.h"
#include "A2C_Choice.h"
#include "A2C_Enumerated.h"
#include "A2C_Der.h"
#include "A2C_Boolean.h"
#include "A2C_Integer.h"
#include "A2C_Null.h"
#include "A2C_ObjectDescriptor.h"
#include "A2C_ObjectIdentifier.h"
#include "A2C_Object_Set.h"
#include "A2C_OctetString.h"
#include "A2C_OpenType.h"
#include "A2C_Sequence.h"
#include "A2C_Set.h"
#include "A2C_Seq_Of.h"
#include "A2C_Set_Of.h"
#include "A2C_String.h"
#include "A2C_Stream.h"
#include "A2C_Tag.h"
#include "A2C_Time.h"
