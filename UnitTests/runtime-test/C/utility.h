#ifdef _MSC_VER
#include <crtdbg.h>
#endif // _MSC_VER

A2C_ERROR DecodeFromFile(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, char const * szFileName, int fDER);
A2C_ERROR EncodeToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, int fDER, char const * szFileName);
A2C_ERROR EncodeCompareToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, int fDER, char const * szFileName);

A2C_ERROR DecodeFromFileBy1(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, char const * szFileName);
int MemoryCheck();

A2C_ERROR ValidateCompareToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, char const * szFileName);



#define Check_DecodeBER(a, b, c, d) if (DecodeFromFile(a, b, c, d, FALSE) != A2C_ERROR_Success) return 1;
#define Check_DecodeBER_By1(a, b, c, d) if (DecodeFromFileBy1(a, b, c, d) != A2C_ERROR_Success) return 1;
#define Check_DecodeDER(a, b, c, d) if (DecodeFromFile(a, b, c, d, TRUE) != A2C_ERROR_Success) return 1;

/*
 *   a = Object
 *   b = object descriptor
 *   c = flags
 *   d = file name
 */

#define Check_EncodeBER(a, b, c, d) if (EncodeCompareToFile(a, b, c, FALSE, d) != A2C_ERROR_Success) return 1;
#define Check_EncodeDER(a, b, c, d) if (EncodeCompareToFile(a, b, c, TRUE, d) != A2C_ERROR_Success) return 1;
