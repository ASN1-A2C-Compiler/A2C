#include "bitstring-good.h"

const A2C_INTEGER_NATIVE a={1};


const A2C_ITEM_HEADER T0_description = {
  sizeof(T0), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR T0_descriptor = {
    A2C_zero, A2C_BIT_STRING_release, 
    (A2C_decoder_f) A2C_BIT_STRING_decode_der, (A2C_encoder_f) A2C_BIT_STRING_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_decode_ber, (A2C_encoder_f) A2C_BIT_STRING_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_compare,
    (A2C_copy_f) A2C_BIT_STRING_copy,
    (A2C_print_f) A2C_BIT_STRING_print,
    (A2C_validate_f) A2C_BIT_STRING_validate,
    &T0_description
};
const A2C_NAME_ITEM T1_name_table[] = {
    { T1__a, 0, "a"},
};

const A2C_ITEM_HEADER T1_description = {
  sizeof(T1), 0, 0, 0, countof(T1_name_table), T1_name_table
};

const A2C_DESCRIPTOR T1_descriptor = {
    A2C_zero, A2C_BIT_STRING_NAMED_release, 
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_der, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_ber, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_NAMED_compare,
    (A2C_copy_f) A2C_BIT_STRING_NAMED_copy,
    (A2C_print_f) A2C_BIT_STRING_NAMED_print,
    (A2C_validate_f) A2C_BIT_STRING_NAMED_validate,
    &T1_description
};
const A2C_NAME_ITEM T2_name_table[] = {
    { T2__a, 0, "a"},
    { T2__b, 0, "b"},
};

const A2C_ITEM_HEADER T2_description = {
  sizeof(T2), 0, 0, 0, countof(T2_name_table), T2_name_table
};

const A2C_DESCRIPTOR T2_descriptor = {
    A2C_zero, A2C_BIT_STRING_NAMED_release, 
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_der, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_ber, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_NAMED_compare,
    (A2C_copy_f) A2C_BIT_STRING_NAMED_copy,
    (A2C_print_f) A2C_BIT_STRING_NAMED_print,
    (A2C_validate_f) A2C_BIT_STRING_NAMED_validate,
    &T2_description
};
static const BYTE c___array[] = {0xa0};
const T2 c={ 3, 1, (PBYTE) c___array};

static const BYTE d___array[] = {0xa0};
const T2 d={ 3, 1, (PBYTE) d___array};

static const BYTE e___array[] = {0};
const T2 e={ 0, 0, (PBYTE) e___array};

static const BYTE f___array[] = {0x10};
const T2 f={ 4, 1, (PBYTE) f___array};

static const BYTE g___array[] = {0x50};
const T2 g={ 4, 1, (PBYTE) g___array};

static const BYTE h___array[] = {0xa0, 0xc0};
const T0 h={ 12, 2, (PBYTE) h___array};

static const BYTE i___array[] = {0xaa, 0x0};
const T0 i={ 10, 2, (PBYTE) i___array};

const A2C_NAME_ITEM T3_name_table[] = {
    { T3__a, 0, "a"},
    { T3__b, 0, "b"},
    { T3__c, 0, "c"},
};

const A2C_ITEM_HEADER T3_description = {
  sizeof(T3), 0, 0, 0, countof(T3_name_table), T3_name_table
};

const A2C_DESCRIPTOR T3_descriptor = {
    A2C_zero, A2C_BIT_STRING_NAMED_release, 
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_der, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_ber, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_NAMED_compare,
    (A2C_copy_f) A2C_BIT_STRING_NAMED_copy,
    (A2C_print_f) A2C_BIT_STRING_NAMED_print,
    (A2C_validate_f) A2C_BIT_STRING_NAMED_validate,
    &T3_description
};
const A2C_INTEGER_NATIVE a1={1};

const A2C_NAME_ITEM T4_name_table[] = {
    { T4__a1, 0, "a1"},
};

const A2C_ITEM_HEADER T4_description = {
  sizeof(T4), 0, 0, 0, countof(T4_name_table), T4_name_table
};

const A2C_DESCRIPTOR T4_descriptor = {
    A2C_zero, A2C_BIT_STRING_NAMED_release, 
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_der, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_der,
    (A2C_decoder_f) A2C_BIT_STRING_NAMED_decode_ber, (A2C_encoder_f) A2C_BIT_STRING_NAMED_encode_ber,
    (A2C_compare_f) A2C_BIT_STRING_NAMED_compare,
    (A2C_copy_f) A2C_BIT_STRING_NAMED_copy,
    (A2C_print_f) A2C_BIT_STRING_NAMED_print,
    (A2C_validate_f) A2C_BIT_STRING_NAMED_validate,
    &T4_description
};
