#include "seq6-good.h"

const A2C_NAME_ITEM E1_name_table[] = {
    { E1__v1, 0, "v1"},
};

const A2C_ITEM_HEADER E1_description = {
  sizeof(E1), 0, 0, 0, countof(E1_name_table), E1_name_table
};

const A2C_DESCRIPTOR E1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &E1_description
};
const A2C_NAME_ITEM I1_name_table[] = {
    { I1__v1, 0, "v1"},
};

const A2C_ITEM_HEADER I1_description = {
  sizeof(I1), 0, 0, 0, countof(I1_name_table), I1_name_table
};

const A2C_DESCRIPTOR I1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    (A2C_validate_f) A2C_INTEGER_NATIVE_validate,
    &I1_description
};

const A2C_ITEM S1_description_table[] = {
    {"f1", OFFSET_OF(S1, f1), {A2C_TAG_CLASS_UNIVERSAL, 10}, &E1_descriptor, (PBYTE) 1, A2C_FLAGS_DEFAULT | A2C_FLAGS_TAG_IMPLICIT, 0},
    {"f2", OFFSET_OF(S1, f2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &I1_descriptor, (PBYTE) 2, A2C_FLAGS_DEFAULT | A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S1_description = {
  sizeof(S1), 0, countof(S1_description_table), S1_description_table, 0, 0
};

const A2C_DESCRIPTOR S1_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S1_description
};
