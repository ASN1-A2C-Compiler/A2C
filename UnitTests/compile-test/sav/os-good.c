#include "os-good.h"


const A2C_ITEM_HEADER I1_description = {
  sizeof(I1), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I1_descriptor = {
    A2C_zero, A2C_OCTET_STRING_release, 
    (A2C_decoder_f) A2C_OCTET_STRING_decode_der, (A2C_encoder_f) A2C_OCTET_STRING_encode_der,
    (A2C_decoder_f) A2C_OCTET_STRING_decode_ber, (A2C_encoder_f) A2C_OCTET_STRING_encode_ber,
    (A2C_compare_f) A2C_OCTET_STRING_compare,
    (A2C_copy_f) A2C_OCTET_STRING_copy,
    (A2C_print_f) A2C_OCTET_STRING_print,
    (A2C_validate_f) A2C_OCTET_STRING_validate,
    &I1_description
};
static const BYTE i1___array[] = {0xa0, 0x3f, 0x9f, 0x10};
const I1 i1={ 4, (PBYTE) i1___array};

static const BYTE i2___array[] = {0x5a, 0x2f, 0x40};
const I1 i2={ 3, (PBYTE) i2___array};

