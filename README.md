# A2C
A2C Open Source Compiler

October 7, 2008

*a2c* is an ASN.1 compiler. It reads one or more ASN.1 syntax files (often called ASN.1 modules) and emits a C program that embodies that syntax. *a2c* is useful to programmers who are writing C programs that will read and/or write data that is expressed in an ASN.1 format, such as PKIX certificates and CMS (S/MIME) messages.

*a2c* handles nearly all of the ASN.1:2002 syntax (inputs in earlier versions can be converted to ASN.1:2002). In specific, the compiler can handle ITU-T Rec. X.680 (without the XML Value notation and a couple of other minor items), X.681, X.682, X.683, and DER and BER from X.690.

The C program created by *a2c* consists of many structures and functions that can then be used by another program. *a2c* comes with an optional runtime library that can be used to greatly speed parts of that integration.

# 1. Table of Contents

    1. Table of Contents
    2. Installing a2c
       2.1 Installing a2c on Windows
       2.2 Building a2c on Windows from Source
       2.3 Installing a2c on non-Windows Environments
       2.4 Building a2c on non-Windows Environments from Source
       2.5 Testing a2c
    3. Compiling ASN.1 Modules with a2c
       3.1 Command Line Options
    4. a2c Compiler Directives
       4.1 Namespaces
       4.2 Integer Sizes
       4.3 Leaving Objects as Binary
       4.4 Overriding Functions
       4.5 Using Pointer
       4.6 User Specified Types
       4.7 Extending Object Sets
       4.8 Emit Strings to Output Files
    5. Building C Programs with a2c
       5.1 Building the Runtime Library
       5.2 Built-in Routines
       5.3 Structure Members for ASN.1 Types
       5.4 Programming Example for the Routines
       5.5 Programming with Huge and Native Integers
       5.6 Programming with SEQUENCE
       5.7 Programming with SEQUENCE OF
    6. Known Limitations of a2c
    7. Installing .NET and Mono
       7.1 Installing the .NET Framework SDK
       7.2 Installing Mono
    A. Copyright for a2c


# 2. Installing *a2c*

*a2c* is available both as a compiled binary and as source code. The binary executable consists of one file, a2c.exe. This is executable under the .NET framework on Windows systems and under Mono on Linux, BSD, and other similar systems.

To obtain *a2c*, see the *a2c*'s project page at:
    https://github.com/ASN1-A2C-Compiler/A2C

*a2c* is written in C#, a language similar to C but with additional features. If you do not care about compiling *a2c* yourself, and instead use the distributed binary version, you do not need any C# tools. However, to execute a2c.exe, you will still need the .NET framework on Windows (which is built into most modern versions of Windows) or Mono (which is generally not built into Linux, BSD, or other systems).

Most programmers who use *a2c* will also want  to use *a2c*'s runtime library for gluing some of *a2c*'s functions into your C code. That library is platform-specific (and in some cases compiler-specific), and is only distributed in source code form. Thus, in order to use the output of *a2c* with a C program, you also need to build the runtime library; doing so requires the same tools as are needed to compile C programs for any particular system. The runtime library is written in C. Building the runtime library is described in Section 5. Note that, if you use the web service to compile your modules, you still need to compile the runtime library.

Of course, the *a2c* package also includes many other files, such as this document, a test system, and so on.

## 2.1 Installing *a2c* on Windows

*a2c* under Windows requires the .NET framework, version 2 or higher. After installing .NET on your system, unpack the distribution file (the .zip file) into a directory of your choice. You can copy the a2c.exe file to any desired directory, such as where your ASN.1 module resides, or to a directory in the executable search path.

See section 5 for a description of how to build and use the runtime library.

## 2.2 Building *a2c* on Windows from Source

Building *a2c* requires the .NET Framework SDK. After installing the .NET Framework SDK on your system, unpack the distribution file (the .zip file) into a directory of your choice.

Change to the "NET-files" directory:
    cd NET-files
Run "nmake" program:
    nmake
This installs the correct makefiles and batch programs for .NET. Next, return to the base directory of the installation and run the "nmake" program there:
    nmake
This step cleans out files that were built earlier, then builds all the tools needed to build *a2c*, then builds *a2c* itself, then builds the runtime library.

## 2.3 Installing *a2c* on non-Windows Environments

After installing Mono on your system, unpack the distribution file (the .zip file) into a directory of your choice. You can copy the a2c.exe file to any desired directory, such as where your ASN.1 module resides, or to a directory in the executable search path.

*a2c* requires Mono version 1.2. At the time of this writing, most Mono distributions are version 1.1, although you can find Mono version 1.2 distributions in many places, and you can also easily find Mono version 1.2 source code.

See section 5 for a description of how to build and use the runtime library.

## 2.4 Building a2c on non-Windows Environments from Source

After installing Mono on your system, unpack the distribution file (the .gz file) into a directory of your choice.

Change to the "Mono-files" directory:
    cd Mono-files
Run the "make" program:
    make
This installs the correct makefiles and batch programs for Mono. Next, return to the base directory of the installation and run the "make" program there:
    make
This step cleans out files that were built earlier, then builds all the tools needed to build *a2c*, then builds *a2c* itself, then builds the runtime library.

## 2.5 Testing *a2c*

The 'test' directory off of the main distribution directory contains many ASN.1 modules. The "rfc*" modules are examples of ASN.1 modules for IETF security standards. The "example*" modules are examples used in this document; the "program*.c" files are C programs associated with those modules. 

There are two example modules, example-1.asn and example-everything.asn. The first is the (trivial) module that goes with the example program later in this document; the second has an example of all relevant ASN.1 types. There are two programs, program-1.c and program-everything.c, that goes with the modules.

Before you compile the C programs, you must first assign the environment variable "PATH_TO_A2C" to be the directory that is the base of the distribution. This is used for the C compiler to find the library files. These tests also require that the runtime library is already built.

There are "make" targets for the two examples. These targets compile the example modules, compile the C programs associated with them, and execute the C programs. Give the command:
    nmake program-1 (.NET)
    nmake program-everything (.NET)
    make program-1 (Mono)
    make program-everything (Mono)


# 3. Compiling ASN.1 Modules with *a2c*

The *a2c* executable consists of a single file, a2c.exe. This is executable under the .NET framework on Windows systems and under Mono version 1.2 on Linux, FreeBSD, and other similar systems. A section near the end of this document describes how to install the .NET framework on Windows systems and Mono on non-Windows systems.

On Windows, the *a2c* command line looks something like:
    a2c sample1.asn sample2.asn
On Mono-based systems, the a2c command line looks something like:
    mono a2c.exe sample1.asn sample2.asn
*a2c* will emit the .c file and .h file based on the name of the first input file, unless the "-out" option is given.

## 3.1 Command Line Options

The command-line syntax is:
    a2c [options] infilenames

The options are:

-out=<basefilename>
    Changes the name of the output files to the specified name

-debug=<number> (default is 0)
    Causes debugging output. "1" means "some debugging"; other
    values may be defined in the future.

-format=<string> (default is "c")
    The only value available is "c". In the future, this might     have values such as "java", "perl", "python", "ruby", "cobol",      "fortran", "ada", "erlang", "pl1", "snobol", "forth", and so on.

-emit=<string> (default is "yes")
    Tells whether or not to emit the following output files. To cause *a2c* to just run through the input files looking for errors without emitting output, use the value "no". You can have more than one -emit option, telling *a2c* to emit some output files but not others. For example, "-emit=no mod1.asn -emit=yes mod2.asn" will cause only the output for mod1.asn to be suppressed.

-anonymous-union=<string> (default is "no")
    Tells whether or not to allow *a2c* to emit anonymous unions. Set to "yes" to allow them.

-inline=<string> (default is "yes")
    Tells *a2c* how to create inline functions. The three choices are "yes", "no", and "static". For the .NET compiler, the value should probably be "yes"; for gcc and compilers like gcc, the value should probably be "static".

-int-size=<integer> (default is 32)
    Tells the compiler what the size of a native integer is. This allows the compiler to do checking on constraints and runtime uses of the 'int' type for overflow conditions.

-eolhint=<string> (default is "no")
    Tells *a2c* that it can assume that all internal commands and all assignment statements will start on a new line.  If used this option must be placed prior to any file names.

-const=<string> (default is "no")
    The const keyword is emitted as part of the compiler output.

-lookup=<file name>
    Load a file which contains pointers to where modules can be found.  The file contains lines with the following formats:
    File: <OID> <filename>
    New: <OID-old> <OID-new>

For example, to compile sample.asn with debugging turned on and the output files called "new-sample", you would use one of the following command lines:
    a2c -inline=yes -out=new-sample -debug=1 sample.asn (.NET)
    mono a2c.exe -inline=static -out=new-sample -debug=1 sample.asn (Mono)


# 4. *a2c* Compiler Directives

*a2c* takes various directives that are outside the scope of the ASN.1 specification but that help developers control the output from *a2c*. This section describes those directives.

*a2c*'s compiler directives are given on a line of their own. They begin with "--#" and end with "#--", such as:
    --#NativeInteger#--

Most of the *a2c* compiler directives need to be placed between the BEGIN and END keywords.  Directives that do not reference a non-terminal may be placed before the BEGIN key word, but the END keyword is taken as the definitive end of the file.

## 4.1 Namespaces

The "Namespace" directive specifies the namespace used in the code that is output by *a2c*. The format of the directive is:
    Namespace: string

This directive affects the namespace for the current file and all subsequent files until a new directive is located. If multiple namespace directives are in a single file, the last one wins.

For example, if you want the namespace in the output to be "Handler1_", you would include the directive:
    --#Namespace: "Handler1_"#--

The namespace string is prefixed to the ASN.1 identifier to form a variable or function name.  Thus, the string should consist only of those characters that are legal for an identifier.

## 4.2 Integer Sizes

The "NativeInteger" directive tells *a2c* to emit the ASN.1 type INTEGER using the built-in integer type for C. The "HugeInteger" directive tells *a2c* to use a variable length buffer to hold the INTEGER value. The default is for *a2c* to use NativeInteger.

This directive can be used globally and locally. If you do not specify a field name, *a2c* uses the directive globally. If you use more than one instance of the global context version of these directives in a file, the first instance is used.

To use the directive in the local context, add a colon and the name of a type or of a specific field. This must be done after the definition of the type or field.

For example, assume that you have given the "NativeInteger" directive early in a module (or are just using this from *a2c*'s defaults), but want the "CertificateSerialNumber" type and the "distance" field of the "Measurements" type to be variable sized. This would look like:
    --#NativeInteger#--
       . . . other ASN.1 . . .
       . . . Definition of CertificateSerialNumber . . .
    --#HugeInteger: CertificateSerialNumber#--
       . . . other ASN.1 . . .
       . . . Definition of Measurements.distance . . .
    --#HugeInteger: Measurements.distance#--

Equivalent directives exist for the ASN.1 type ENUMERATED. These directives are NativeEnumerated and HugeEnumerated. The compiler defaults to using the native data types.

Directives may be placed in a separate file and reference the correct module by prefixing the type name with the module reference identifier. Thus, if you are using the PKIX module (whose module identifier is PKIX1Explicit88), you can use the command:
   --#HugeInteger:  PKIX1Explicit88.CertificateSerialNumber #--

## 4.3 Leaving Objects as Binary

You may have different libraries for dealing with objects that cross boundaries.  For example, S/MIME (CMS) uses PKIX certificates, but your system may separate the parsing and processing of certificates from that of CMS objects. The Binary directive informs the decoder that when you reach a particular type or field, that field is to be left as an encoded blob for later decoding. When you  reference a type in this manner, it is a good idea to tag the imported type name in the referencing module. For example, to have the decoder leave the Certificate type as a binary blob, you would use:

    --#Binary: Certificate#--

## 4.4 Overriding Functions

In some cases, you may want to override the function that *a2c* calls for a specific operation on a specific type in your module. For example, you might want to use a particular *a2c* encoding funciton with a particular type based on prior knowledge of use of the module.

The syntax for this directive is:
    --#UseFunction: <typeName> "<Tag>" "<functionName>"#--
<functionName> is the name of the function to be called. This can either be a function internal to *a2c* or one you have defined yourself.

Legal values for <Tag> are:
    Init         Initialization function
    Release      Cleanup function
    EncodeBER    BER encode function
    DecodeBER    BER decode function
    EncodeDER    DER encode function
    DecodeDER    DER decode function
    Copy         Function to copy structure into a blank
    Compare      Function to compare to structures
    Print        Function to print structure data
    Validate     Function to validate structure
Tag names are case sensitive.

For example:
    --#UseFunction: AuthAttributes "EncodeBER" "A2C_SET_OF_encode_der"#--
This is used in rfc3852.asn so that the SET OF authenticated attributes is encoded using the DER encoding rules rather than the BER encoding rules; this action is required in Section 9.1 in RFC 3852.

## 4.5 Pointers

The Pointer directive is used to mark a field as having a pointer to the type rather than the type itself.
  
For optional fields, the Pointer directive can be used to remove a field from the flags; this is useful to prevent an overflow if there are more optional fields that bits in the flag. For example, if the optional field sq3Int2 is in structure SQ3, you would normally check if it is present with code like the following in your C program: 
    if (SQ3_ptr->optionalFields & SQ3_sq3Int2_present) { some code };
To change this to a pointer, you would use the following in your ASN.1 module:
    --#Pointer: SQ3.sq3Int2#--
This changes the call in your C program to:
    if (SQ3_ptr->sq3Int2 != NULL) { some code };

If you have a cyclic structure (where structure A includes structure B, and structure B includes structure A, or where structure A is recursive), *a2c* will create a pointer when compiling. However, if you want to create the pointer yourself (such as to be sure that the pointer is used on A instead of B), you can also use the Pointer directive.

## 4.6  User Specified Types

In some cases, you may want to override the type that *a2c* would use in the emitted code.  For example, you might want to use a SystemTime rather the more complicated struture for the Time structure in RFC 5280. When the type is overridden, one generally needs to override all of the *a2c* functions at the same time.  (An exception might be when a field is added to a structure, in this case the default functions may be sufficent.)

The syntax for this command is:
    --# UseType: <typeName> "<userTypeName>" #--
    --# UseType: <fieldName> "<userTypeName>" #--
<userTypeName> is the name of the type that is to be used when emission is done.

For example:
    --#UserFunction: Time "SystemTime" #--
This is used in RFC 5280 to allow for a simpler time function.  The encoding functions would then be overriden to deal with the issue of encoding the time as either a UTC Time or a Generalized Time.

This command can also be used to override the type used for a field in a sequence, choice or set.  However in this case it needs to be noted that the entire type for the field is changed.  Thus if one does the following:

  ContentInfo ::= SEQUENCE {
     contentType    OBJECT IDENTIFIER,
     content        [0] EXPLICIT OCTET STRING
  }
  --# UseType: ContentInfo.content "OCONTENT" #--

Then the new type is responsible for emitting and parsing the tag for the content field.


## 4.7 Extending Object Sets

Many times new objects are created in new modules that one wishes to add to an object set defined in an existing modules.  For example the creation of a new CMS content type could be added to the CMS ContentSet so that the new content type could be automatically decoded.

The syntax for this directive is:
   --# AddToObjectSet: <objectSetName> <addItem> #--
<objectSetName> is a defined object set.
<addItem> is an object or an object set name to be added.

For example:
   --# AddToObjectSet: CryptographicMessageSyntax-2009.ContentSet
        ct-authEnvelopedData #--
This would be used in the updated ASN.1 module for RFC 5083 to add a the CONTENT-TYPE object for authenticated data into the updated module for RFC 3852.

## 4.8 Emit Strings to Output Files

As changes are made to the data types of ASN.1 types, it is some times necessary to be able to include additional header files or out declarations.  This directive allows one to do this.

The syntax for this directive is:
  --# EmitString: <moduleName> <headerFileTag> <string> #--
<moduleName> is an optional module name, if omitted it defaults to the current module.
<headerFileTag> is a string with the extension of the file to be emitted into.  Currently this must be "h" or "c" for the C language emitter.
<string> is the string to be emitted into the file.  This may be a multi-line string.

For example:
  --# EmitString: "h" "#include ""<NewHeader.h>""" #--
This would emit the include directive into the emitted .h file.

# 5. Building C Programs with *a2c*

After you have created a C program with *a2c*, you need to integrate it with the rest of your project. This normally means taking the .c and .h files generated by *a2c* for your ASN.1 module, and calling into them from an outside program.

When writing a C program that uses the output of *a2c*, you use the structures that are defined in the .h file that was produced. Basically, you have an #include statement that pulls in the produced .h file, and you compile your C code with the produced .c file.

In order to build C programs, you should have the makefiles for your type of system installed in the right places. This was already done for you if you built *a2c* from source. If you did not build from souce, take the following steps.

Programming with *a2c* under Windows requires the .NET framework, version 2 or higher. Change to the "NET-files" directory:
    cd NET-files
Run "nmake" program:
    nmake

Programming with *a2c* on non-Windows systems requires the normal C programming tools. Change to the "Mono-files" directory:
    cd Mono-files
Run the "make" program:
    make

There is an example program, "program-everything", in the "test" directory that has examples of using each ASN.1 type and the helper routines. You can build the program in the "test" directory with:
    nmake program-everything (.NET)
    make program-everything (Mono)

## 5.1 Building the Runtime Library

*a2c* includes an optional runtime library that can assist with this integration. The library comes in source form, and you need to compile all the objects to put together the library on the target system for your program.

To build the runtime library, change to the runtime/C directory and generate it from the makefile. The resulting library is called A2C_Runtime.lib.

For .NET:
    cd runtime\C
    nmake clean
    nmake
For Mono:
    cd runtime/C; make clean; make

## 5.2 Built-in Routines

The routines fall into two categories: those whose name is the same for all invocations, and those whose names are specific to the type of item that is being referred to. The type name appears at the end of the name, preceded by an underscore. For example, the name of "A2C_Alloc_sometype" changes with the type (in this case, "sometype" would be the actual name of the type, as seen in the .c file); "A2C_CreateMemoryStream" has the same name regardless of the type.

The *a2c* routines fall into a few logical groups:
- Memory management: A2C_Alloc_sometype, A2C_Free_sometype,
    A2C_Init_sometype, A2C_Release_sometype, A2C_CreateMemoryStream,
    A2C_FreeStream, A2C_AllocArray_sometype, A2C_FreeArray_sometype,
    A2C_ReallocArray_sometype, A2C_FreeContext
- Encoding: A2C_EncodeDer_sometype, A2C_EncodeBer_sometype
- Decoding: A2C_DerDecode_sometype, A2C_BerDecode_sometype
- Data validation: A2C_Validate, A2C_Validate2
- Housekeeping: A2C_Compare_sometype, A2C_Copy_sometype,
    A2C_Print_sometype, A2C_GetDataFromStream,
    A2C_GetStringFromStream, A2C_Version
- Type-specific: A2C_BIT_STRING_IsSet, A2C_BIT_STRING_Set,
    A2C_BIT_STRING_Clear, A2C_List_sometype, A2C_NameToInt_sometype,
    A2C_IntToName_sometype

Each routine returns an error code (all of which which are defined in in runtime/C/A2C_Error.h). A negative number means an error while 0 means success.

The memory management routines allocate and deallocate memory for an object's structure.  These routines use the default malloc memory manager. You do not have to use them: you can use your own memory manager or stack variables. However, they are handy because they know how much memory to allocate for the various structures.

A2C_Alloc_sometype(&some_objectptr)
    This routine allocates and initializes an object of the type given     in the name. The object is returned in the variable some_objectptr     on success.  A2C_Init_sometype is called by this routine for the initialization, so it is not necessary to call it again.

A2C_Free_sometype(some_objectptr)
    Frees the memory allocated by A2C_Alloc. This function will call A2C_Release to recursively free all referenced memory. Be careful not to free an object that was not allocated with A2C_Alloc.

A2C_Init_sometype(some_objectptr)
    Initializes the contents of the structure. If a structure contains other in it (such as if the structure is a SET or a SEQUENCE), A2C_Init_sometype initializes them recursively. A2C_Init is automatically called by A2C_Alloc, but you can use it for other objects. The processes of initialization will copy in the default value for any field that has a default, and will copy in some legal value for all other fields.

A2C_Release_sometype(some_objectptr)
    Releases the contents of the structure. If a structure contains other objects (such as if the structure is a SET or a SEQUENCE), A2C_Release_sometype recursively frees the memory used by them. A2C_Release_sometype is automatically called by A2C_Free, but you can use it if you are using stack based variables or your own memory management.

A2C_CreateMemoryStream(&some_streamptr)
    Create a memory stream object and return its pointer in some_streamptr.  A memory stream object is used to receive and hold data that of an unknown size.  The memory stream object will
    grow its internal buffers as needed.  In a2c, they are frequently used when encoding or printing an object.

A2C_FreeStream(some_streamptr)
    This routine causes the referenced stream object to free itself and any memory that it holds.

A2C_AllocArray_sometype(some_objectptr, count)
    Allocates an array for SEQUENCE OF and SET OF constructs. some_objectptr is the pointer to the field, and count is the number of elements to be allocated for the array.  This routine calls A2C_Init_sometype on each element allocated.

A2C_FreeArray_sometype(some_objectptr)
    Frees the memory allocated by  A2C_AllocArray. This function calls A2C_Release on each element allocated.

A2C_ReallocArray_sometype(some_objectptr, countNew)
    Reallocates an array that was already allocated by A2C_AllocArray_sometype. This function does not change the count field in the referenced object unless countNew is smaller than count. It will change the countAllocated field in the referenced object to countNew. This function calls A2C_Init_sometype on each added element (or A2C_Release_sometype on each deleted element).

A2C_FreeContext(contextptr);
    Contexts are used with some *a2c* runtime routines to continue an operation after stopping; the context keeps information between calls. Contexts have a type of A2C_CONTEXT and are initialized as NULL. The memory used by a context can be freed with A2C_FreeContext; some of the routines will automatically free the memory.

A2C_EncodeDer_sometype(some_objectptr, flags, &contextptr, some_streamptr)
    This function encodes an object using the DER encoding rules. some_objectptr indicates the object to be encoded, and flags is an integer (there are currently no flag values for encoding). contextptr is NULL. some_streamptr is a pointer to a stream object, the encoded bytes are written to the stream object. A2C_EncodeDer_sometype has the same syntax as A2C_EncodeDer_sometype but uses the BER encoding rules.

A2C_DecodeDer_sometype(&some_objectptr, flags, &contextptr, bufferptr, bufferlen)
    This function decodes an object using the DER encoding rules.  For most purposes, A2C_DecodeBer_sometype should be used instead because BER is a superset of the DER encoding rules. The decode object is returned in some_objectptr, which is allocated by the function. Flags is an integer whose values are described below. contextptr is a pointer that is used to hold state between multiple calls to A2C_DecodeDer_sometype for a single decode operation internally by *a2c*; the initial value should be NULL. bufferptr is a pointer to the buffer that holds the encoded object; bufferlen is the length of that buffer. A2C_DecodeBer_sometype has the same syntax as A2C_DecodeDer_sometype.

    For the flags, A2C_FLAGS_MORE_DATA indicates a continuation call for decoding. A2C_FLAGS_STRICT_DER forces strict DER decoding.

A2C_Validate_sometype(some_objectptr, &some_int)
    This routine recursively checks each field in object to see if it is valid.  If any field fails validation, the function exits and sets some_int to 0; otherwise, some_int is set to 1.

A2C_Validate2_sometype(some_objectptr, &contextptr, &some_int, some_streamptr)
    This routine recursively checks each field in object to see if it is valid.  If everything validates, the stream will not be used and some_int is set to 1.  If any part of the field does not validate, some_int is set to 0, and the stream is set to a string that contains a description of the error.  That text has three lines, each terminated with a linefeed: the path to the field that failed (field names separated by periods), the type name of the field that failed, and some human-readable text.

A2C_Compare_sometype(some_objectptr, other_objectptr)
    This routine compares the two objects and returns 0 if they are the same, or otherwise returns a non-zero result.

A2C_Copy_sometype(dest_objectptr, source_objectptr)
    This copies the contents of the second object into the first.

A2C_Print_sometype(some_objectptr, some_streamptr)
    This pretty-prints the object, storing the result in some_streamptr. Complex objects such as SEQUENCEs have their sub-objects printed one per line. Note that the stream is not null-terminated.

A2C_GetDataFromStream(some_streamptr, &some_buffptr, &some_bufflen)
    This routine gets data from the stream pointed to by    some_streamptr and puts it into the some_buffptr (unsigned char *). The length of that buffer is put into some_bufflen (int).
    After a call to this routine, the stream no longer has any data. The buffer must be freed by the caller.

A2C_GetStringFromStream(some_streamptr, &some_buffptr)
    This routine gets data from the stream pointed to by some_streamptr and puts it into the some_buffptr (char *), and the buffer is null-terminated. After a call to this routine, the stream no longer has any data. The buffer must be freed by the caller.

A2C_Version(&some_int)
    Puts the version number of *a2c* into some_int (int).

A2C_BIT_STRING_IsSet(some_objectptr, iBit)
    some_objectptr must point to a BIT STRING object. iBit (int) is a bit position in the bit string. The returned value is A2C_ERROR_False if the bit is 0, A2C_ERROR_True if the bit is 1, and less than 0 if the routine fails.

A2C_BIT_STRING_Set(some_objectptr, iBit)
    Causes the bit at position iBit to be set to 1 in the bit string.

A2C_BIT_STRING_Clear(some_objectptr, iBit)
    Causes the bit at position iBit to be set to 0 in the bit string.

A2C_List_sometype(&list_structptr)
    Returns a pointer to a structure, list_structptr (A2C_NAMES *) that represents the contents of the ENUMERATED, BIT STRING, or INTEGER object. The first member of that structure is 'count' (int), the number of elements in the second member, which is the array 'listArrayPtr' (A2C_NAME *). The array consists of structures; each structure has a member 'name' (char *) and 'number' (int) with the integer associated with that name.

A2C_NameToInt_sometype(thisName, &intval)
    For an ENUMERATED, BIT STRING, or INTEGER type, this routine looks up the name 'thisName' (char *) and sets the integer 'intval' (int) to the number associated with the name.

A2C_IntToName_sometype(thisInt, &nameptr)
    For an ENUMERATED, BIT STRING, or INTEGER type, this routine looks up the number 'thisInt' (int) and sets the string pointed to by nameptr (char *) to the name associated with the number.

5.3 Structure Members for ASN.1 Types

This section describes the members of the structures that are defined for the ASN.1 types. Some types have multiple members, but many have just one.

Many ASN.1 types allow tagging of values. These tags are used for encoding and decoding, but cannot be used in a program to identify objects.

BIT STRING
    The members are 'bitLength' (int), 'bufferLength' (int) and 'data' (BYTE *). The bitLength is the number of bits used in the BIT STRING. bufferLength is the size of data in bytes.

BOOLEAN
    The member, 'data' (int). 0 means FALSE, all other values mean TRUE.

Character strings
    The string types all have one member, 'string', that references an array holding the characters of the string.  The string is always zero-terminated. However, the different types of strings have different definitions for 'string':
    BMPString         uint16 *
    IA5String         char *
    GeneralString     char *
    GraphicString     char *
    ISO646String      char *
    NumericString     char *
    PrintableString   char *
    TeletexString     char *
    T61String         char *
    UniversalString   uint32 *
    UTF8String        char *
    VideotexString    char *
    VisibleString     char *

CHOICE
    The member 'index' (int) is the position of the item selected in the list of choices. *a2c* emits a #define for each item in the list in the generated .h file. The C emitter uses a nameless union to hold the CHOICE elements.

EMBEDDED
    EMBEDDED types are not yet handled by *a2c*.

ENUMERATED
    The members are identical to INTEGER.

EXTERNAL
    External is not yet handled by *a2c*.

GeneralizedTime
    This is a character string of type VisibleString. It has one member, 'string'.

INTEGER
    The members are different depending on whether the INTEGER is native or huge; see the compiler directives for more information. For native integers, 'nData' is the data (int). For huge integers, 'hData' is an array (BYTE *); the array has size 'hLength' (int). See below for a programming example of using integers.

NULL
     There is one member, 'dummy' (int), whose value is ignored.

ObjectDescriptor
    This is a character string of type GraphicString. It has one member, 'string'

OBJECT IDENTIFIER
    The member is 'oid' (char *). Note that it is a string, not an array of integers. The string consists of integers with periods between the integers.

OCTET STRING
    The members are 'length' (int) and 'data', which is an array (BYTE *).

OPEN TYPE

    The open type has for members; these members are used in pairs, and normally only one of the pairs is set at any time. When the object is in a decoded form, the object is referenced by 'objectPtr' and the object is described by the descriptor referenced in 'functionPtr'. The encoder functions will recurse into the referenced object.  If the object is in an encoded form, the encoded bytes are pointed to by ‘data’ (BYTE *) and length (int); in this case the encoders will copy the bytes directly into the encoded stream.

    During decode operations, the decode may place the object into either of the two sets of members depending on how it is operating and what objects are available to it. This will be described in more detail in a later version.

REAL
    Reals are not yet handled by *a2c*.

RELATIVE OID
    The member is 'oid' (char *). Note that it is a string, not an array of integers. The string consists of integers with periods between the integers.

SEQUENCE
    Unless you have an OPTIONAL field in your sequence, there are no a2c-specific generated fields; see below for information on accessing the fields in your sequence. If you have an OPTIONAL field, *a2c* inserts the member 'optionalFields' (int) which is a bit string indicating which optional fields are in use.

SEQUENCE OF
    The members are 'count' (int), 'countAllocated' (int), and 'array' (VOID *). Set 'array' to point to an array of the type that you are using. See below for more information on using SEQUENCE OF.

SET
    Unless you have an OPTIONAL field in your set, there are no a2c-specific generated fields; see below for information on accessing the fields in your set. If you have an OPTIONAL field, *a2c* inserts the member 'optionalFields' (int) which is a bit string indicating which optional fields are in use.

SET OF
    The members are 'count' (int), 'countAllocated' (int),  and 'array' (VOID *). Set 'array' to point to an array of the type that you are using. Programming SET OF is essentially the same as for SEQUENCE OF.

UTCTime
    This is a character string of type VisibleString. It has one member, 'string'.

## 5.4 Programming Example for the Routines

The following program shows how to use all the routines, using strings as the example.

The ASN.1 module that was used, example-1.asn, contains the following:

Example1

```ASN.1
DEFINITIONS ::=
BEGIN
Str1 ::= PrintableString
END
```

The program was compiled as:

Windows:
    INCLUDE=%include%;%PATH_TO_A2C%\runtime\C
    cl -Zi %PATH_TO_A2C%\runtime\C\A2C_Runtime.lib program-1.c example-1.c
Unix:
    gcc -I$PATH_TO_A2C/runtime/C program-1.c example-1.c $PATH_TO_A2C/runtime/C/A2C_Runtime.lib

program-1.c is:

```C
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "A2C.h"
#include "example-1.h"

int main(int argc, char * argv[])
{
    Str1 *              stra_ptr; /* Will allocate with A2C */
    Str1                strd; /* Allocated on our own */
    Str1 *              strDecode_ptr = 0;
    A2C_STREAM *        encodeStream_ptr = 0;
    A2C_STREAM *        printStream_ptr = 0;
    A2C_ERROR           err;
    int                 i;
    char *              string_buff_ptr;
    unsigned char *     data_buff_ptr;
    int                 buff_len;
    
    /*  Allocate the data structure */
    err = A2C_Alloc_Str1(&stra_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Allocation error */
    /*  Set the value of stra_ptr */
    stra_ptr->string = strdup("abc");
    /*  Set up the strd structure */
    A2C_Init_Str1(&strd);
    strd.string = "def";
    /* Print out the two strings */
    err = A2C_CreateMemoryStream(&printStream_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Allocation error */
    err = A2C_Print_Str1(stra_ptr, printStream_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Filling error */
    err = A2C_GetStringFromStream(printStream_ptr, &string_buff_ptr);
    printf("The value of stra_ptr is %s\n", string_buff_ptr);
    err = A2C_Print_Str1(&strd, printStream_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Filling error */
    err = A2C_GetStringFromStream(printStream_ptr, &string_buff_ptr);
    printf("The value of strd is %s\n", string_buff_ptr);

    /*  Compare the strings */
    printf("First compare: ");
    if (A2C_Compare_Str1(stra_ptr, &strd) == 0) {
        printf("The strings are the same\n");
    } else {
        printf("The strings are different\n");
    }
    /* Make them the same by copying stra into strd */
    err = A2C_Copy_Str1(&strd, stra_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Copying error */
    printf("Second compare: ");
    if (A2C_Compare_Str1(stra_ptr, &strd) == 0) {
        printf("The strings are the same\n");
    } else {
        printf("The strings are different\n");
    }

    /*  Encode stra */
    err = A2C_CreateMemoryStream(&encodeStream_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Allocation error */
    err = A2C_EncodeDer_Str1(stra_ptr, 0, NULL, encodeStream_ptr);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Encoding error */
    /*  Get the data from the stream and print it as hex */
    err = A2C_GetDataFromStream(encodeStream_ptr, &data_buff_ptr, &buff_len);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Getting error */
    printf("The bytes (in hex) were: ");
    for (i=0; i<buff_len; i++) printf("%02x ", data_buff_ptr[i]);
    printf("\n");

    /*  Now do the decode on what we just encoded */
    err = A2C_DecodeDer_Str1(&strDecode_ptr, 0, NULL, data_buff_ptr, buff_len);
    if (err < A2C_ERROR_Success) { exit(1); }  /* Decoding error */
    /*  See if this is the same as the original; it should be */
    printf("Third compare: ");
    if (A2C_Compare_Str1(strDecode_ptr, stra_ptr) == 0) {
        printf("The decode is the same as the original\n");
    } else {
        printf("The decode is different the original\n");
    }

    /*  Free up what we allocated */
    A2C_Free_Str1(stra_ptr);
    A2C_Free_Str1(strDecode_ptr);
    A2C_FreeStream(encodeStream_ptr);
    A2C_FreeStream(printStream_ptr);
    free(data_buff_ptr);
    free(string_buff_ptr);
    exit(0);
}
```

The output is:

The value of stra_ptr is abc
The value of strd is def
First compare: The strings are different
Second compare: The strings are the same
The bytes (in hex) were: 13 03 61 62 63 
Third compare: The decode is the same as the original

## 5.5 Programming with Huge and Native Integers

See the test/program-everything.c program, beginning at the line:
    /* IN2 ::= INTEGER */

## 5.6 Programming with SEQUENCE

CHOICE, SEQUENCE, and SET create structures whose members are the items in the structure. They can be accessed like other items in your programs. See the test/program-everything.c program, beginning at the lines:
    /* CH1 ::= CHOICE { ch1Int [5] INTEGER, ch1IA5 [6] IA5String } */
    /* SE1 ::= SET { se1Int INTEGER, se1PrintStr PrintableString } */
    /* SE2 ::= SET { lots of stuff } */
    /* SQ1 ::= SEQUENCE { seq1 INTEGER, seq2 TeletexString } */
    /* SQ2 ::= SEQUENCE { lot of stuff } */
    /* SQ3 ::= SEQUENCE { sq3Int1 INTEGER, sq3Int2 INTEGER OPTIONAL,
        sq3Str IA5String } */

## 5.7 Programming with SEQUENCE OF

The SEQUENCE OF and SET OF constructs use arrays to hold their values. Use the A2C_AllocArray, A2C_FreeArray, and A2C_ReallocArray routines to handle these arrays. See the test/program-everything.c program, beginning at the line:
    /* SR1 ::= SEQUENCE OF INTEGER */


# 6. Known Limitations of *a2c*

This list contains the known limitations of *a2c* that someone familiar with ASN.1 or C might be surprised by.

Although *a2c* validates the ASN.1 in a module before compiling it, the error messages that result from validation failures are not all that helpful. If the ASN.1 is not valid, the response from *a2c* might not point to the "real" point of failure. You should probably not use *a2c* as an ASN.1 validator.

Real numbers are defined in ASN.1 to allow "5." to be a valid real number. Until further notice, a trailing digit (i.e. "5.0") will be needed for *a2c* to recognize the number as a real. Note, however, the current version of *a2c* does not handle REAL types.

All comments and string values must currently be in ASCII. Fields can take non-ASCII values, of course, but the entire ASN.1 module must currently be in ASCII.

*a2c* does not yet have any helper routines to validate the contents of the various string types. All values are currently allowed, even if they may no sense by the ASN.1 definitions. For example, letters are currently allowed in NumericString types. Similarly, the contents of OBJECT IDENTIFIERs are not checked for validity.


# 7. Installing .NET and Mono

## 7.1 Installing the .NET Framework SDK

There are different packages that install the .NET Framework SDK. The instructions for installing each is different, and will probably change over time. In order to be able to build *a2c*, your installation must include the command-line utility "nmake".

## 7.2 Installing Mono

*a2c* running on non-Windows operating systems such as Linux, BSD, and other Unix systems requires Mono version 1.2 or later (see <http://www.mono-project.com/>). Mono can usually be installed from ports or packages for the particular operating system, and source for Mono is available from the Mono project.

Note that, at the time of this writing, most Mono distributions are version 1.2 or 2.0. Also note that the FreeBSD version of Mono 2.0 does not work consistently with *a2c*, although Mono 2.0 on Linux works fine.


# 8. Acknowledgments

Many thanks go to Blake Ramsdell for early testing and development and to Ken Raeburn for extensive comments and suggestions.


# A. Copyright for *a2c*

Copyright 2008 Jim Schaad and Paul Hoffman. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

This software is provided by Jim Schaad and Paul Hoffman "as is" and any express or implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. In no event shall Jim Schaad and Paul Hoffman or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.
