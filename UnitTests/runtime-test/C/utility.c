#include <stdio.h>
#include <crtdbg.h>
#include <a2c.h>
#include "utility.h"

#define BUFFER_LENGTH   2048

int MemoryCheck()
{
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    if (_CrtDumpMemoryLeaks()) {
        return 1;
    }
    return 0;
}

A2C_ERROR DecodeFromFile(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, char const * szFileName, int fDER)
{
    unsigned char       buffer[BUFFER_LENGTH];
    int                 cb = 0;
    A2C_CONTEXT*        pcxt = NULL;
    A2C_ERROR           err = A2C_ERROR_Success;
    FILE *              fp;

    fp = fopen(szFileName, "rb");
    if (fp == NULL) return A2C_ERROR_ICE;

    while (1) {
        cb = fread(buffer, 1, sizeof(buffer), fp);

        if (cb < 0) {
            err = A2C_ERROR_malformedEncoding;
            break;
        }

        if (cb == 0) {
            break;
        }

        if (fDER) {
            err = A2C_DecodeDer( ppv, pdesc, flags, &pcxt, buffer, cb );
        }
        else {
            err = A2C_DecodeBer( ppv, pdesc, flags, &pcxt, buffer, cb );
        }
        if ((err != A2C_ERROR_Success) && (err != A2C_ERROR_needMoreData)) {
            break;
        }

        flags |= A2C_FLAGS_MORE_DATA;
    }

    if (pcxt != NULL) A2C_FreeContext(pcxt);

    fclose(fp);
    return err;
}

A2C_ERROR DecodeFromFileBy1(PVOID * ppv, PC_A2C_DESCRIPTOR pdesc, int flags, char const * szFileName)
{
    unsigned char       buffer[1];
    int                 cb = 0;
    A2C_CONTEXT*        pcxt = NULL;
    A2C_ERROR           err = A2C_ERROR_Success;
    FILE *              fp;

    fp = fopen(szFileName, "rb");
    if (fp == NULL) return A2C_ERROR_ICE;

    while (1) {
        cb = fread(buffer, 1, sizeof(buffer), fp);

        if (cb < 0) {
            err = A2C_ERROR_malformedEncoding;
            break;
        }

        if (cb == 0) {
            break;
        }

        err = A2C_DecodeBer( ppv, pdesc, flags, &pcxt, buffer, cb );
        if ((err != A2C_ERROR_Success) && (err != A2C_ERROR_needMoreData)) {
            break;
        }

        flags |= A2C_FLAGS_MORE_DATA;
    }

    if (pcxt != NULL) A2C_FreeContext(pcxt);

    fclose(fp);
    return err;
}

A2C_ERROR EncodeToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, int fDER, char const * szFileName)
{
    int                 cb;
    A2C_ERROR           err;
    FILE *              fp = NULL;
    PBYTE               pb = NULL;
    A2C_STREAM *        pstm = NULL;

    /*
     *  Create output buffer
     */

    err = A2C_CreateMemoryStream(&pstm);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Try and encode the object
     */

    if (fDER) {
        err = A2C_EncodeDer(pv, pdesc, flags, NULL, pstm);
    }
    else {
        err = A2C_EncodeBer(pv, pdesc, flags, NULL, pstm);
    }
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    /*
     *  Open the file
     */

    fp = fopen(szFileName, "wb");
    if (fp == NULL) {
        err = A2C_ERROR_ICE;
        goto ErrorExit;
    }

    /*
     *  Get the buffer and write to file
     */

    err = A2C_GetDataFromStream(pstm, &pb, &cb);
    if (err != A2C_ERROR_Success) {
        goto ErrorExit;
    }

    if (fwrite(pb, cb, 1, fp) != cb) {
        err = A2C_ERROR_ICE;
    }

    /*
     *  Cleanup and go home
     */

ErrorExit:
    if (pstm != NULL) pstm->pfnFree(pstm);
    if (pb != NULL) free(pb);
    if (fp != NULL) fclose(fp);
    return err;
}


A2C_ERROR EncodeCompareToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, int flags, int fDER, char const * szFileName)
{
    BYTE                buffer[BUFFER_LENGTH];
    int                 cb;
    int                 cbStm;
    A2C_ERROR           err = A2C_ERROR_Success;
    FILE *              fp = NULL;
    A2C_STREAM *        pstm = NULL;
    PBYTE               pbStm = NULL;
    PBYTE               pbStmRoot = NULL;
    
    /*
     *  Create output buffer
     */

    err = A2C_CreateMemoryStream(&pstm);
    if (err != A2C_ERROR_Success) return err;

    /*
     *  Try and encode the object
     */

    if (fDER) {
        err = A2C_EncodeDer(pv, pdesc, flags, NULL, pstm);
    }
    else {
        err = A2C_EncodeBer(pv, pdesc, flags, NULL, pstm);
    }
    if (err != A2C_ERROR_Success) goto ErrorExit;

    /*
     *  Grab the buffer from the stream
     */

    err = A2C_GetDataFromStream(pstm, &pbStmRoot, &cbStm);
    if (err != A2C_ERROR_Success) goto ErrorExit;

    pbStm = pbStmRoot;

    /*
     * Open the file
     */

    fp = fopen(szFileName, "rb");
    if (fp == NULL) {
        err = A2C_ERROR_ICE;
        goto ErrorExit;
    }

    /*
     *  Now start comparing the file
     */

    while (1) {
        cb = fread(buffer, 1, sizeof(buffer), fp);
        if (cb < 0) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        if (cb == 0) {
            if (cbStm != 0) {
                err = A2C_ERROR_malformedEncoding;
                goto ErrorExit;
            }
            err = A2C_ERROR_Success;
            goto ErrorExit;
        }

        if (cb > cbStm) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        if (memcmp(buffer, pbStm, cb) != 0) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        pbStm += cb;
        cbStm -= cb;
    }

ErrorExit:
    if (fp != NULL) fclose(fp);
    if (pbStmRoot != NULL) free(pbStmRoot);
    if (pstm != NULL) pstm->pfnFree(pstm);
    
    return err;
}


A2C_ERROR ValidateCompareToFile(PCVOID pv, PC_A2C_DESCRIPTOR pdesc, char const * szFileName)
{
    unsigned char       buffer[BUFFER_LENGTH];
    int                 cb;
    int                 cbStm;
    A2C_ERROR           err;
    int                 f = FALSE;
    FILE *              fp = NULL;
    PBYTE               pbStm = NULL;
    PBYTE               pbStmRoot = NULL;
    A2C_STREAM *        pstm = NULL;
    A2C_CONTEXT *       pcxt = NULL;

    /*
     *  Create the output buffer
     */

    err = A2C_CreateMemoryStream(&pstm);
    if (err != A2C_ERROR_Success) return err;
    
    /*
     *  Loop until we are finished doing the validate
     */

    do {
        err = A2C_Validate2(pv, pdesc, &pcxt, &f, pstm);
        if (err != A2C_ERROR_Success) goto ErrorExit;

    } while (!f);

    /*
     *  Grab the buffer from the stream
     */

    err = A2C_GetDataFromStream(pstm, &pbStmRoot, &cbStm);
    if (err != A2C_ERROR_Success) goto ErrorExit;
    pbStm = pbStmRoot;

    /*
     * Open the file
     */

    fp = fopen(szFileName, "rb");
    if (fp == NULL) {
        err = A2C_ERROR_ICE;
        goto ErrorExit;
    }

    /*
     *  Now start comparing the file
     */

    while (1) {
        cb = fread(buffer, 1, sizeof(buffer), fp);
        if (cb < 0) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        if (cb == 0) {
            if (cbStm != 0) {
                err = A2C_ERROR_malformedEncoding;
                goto ErrorExit;
            }
            err = A2C_ERROR_Success;
            goto ErrorExit;
        }

        if (cb > cbStm) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        if (memcmp(buffer, pbStm, cb) != 0) {
            err = A2C_ERROR_malformedEncoding;
            goto ErrorExit;
        }

        pbStm += cb;
        cbStm -= cb;
    }

ErrorExit:
    if (fp != NULL) fclose(fp);
    if (pbStmRoot != NULL) free(pbStmRoot);
    if (pstm != NULL) pstm->pfnFree(pstm);
    
    return err;
    
}
