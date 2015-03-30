#include "set-of-good.h"


const A2C_ITEM S1_description_table[] = {
    {"array", OFFSET_OF(S1, array), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S1_description = {
  sizeof(S1), 0, countof(S1_description_table), S1_description_table, 0, 0
};

const A2C_DESCRIPTOR S1_descriptor = {
    A2C_SET_OF_init, A2C_SET_OF_release, 
    (A2C_decoder_f) A2C_SET_OF_decode_der, (A2C_encoder_f) A2C_SET_OF_encode_der,
    (A2C_decoder_f) A2C_SET_OF_decode_ber, (A2C_encoder_f) A2C_SET_OF_encode_ber,
    (A2C_compare_f) A2C_SET_OF_compare,
    (A2C_copy_f) A2C_SET_OF_copy,
    (A2C_print_f) A2C_SET_OF_print,
    (A2C_validate_f) A2C_SET_OF_validate,
    &S1_description
};

const A2C_ITEM S2_description_table[] = {
    {"i1", OFFSET_OF(S2, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S2_description = {
  sizeof(S2), 0, countof(S2_description_table), S2_description_table, 0, 0
};

const A2C_DESCRIPTOR S2_descriptor = {
    A2C_SET_OF_init, A2C_SET_OF_release, 
    (A2C_decoder_f) A2C_SET_OF_decode_der, (A2C_encoder_f) A2C_SET_OF_encode_der,
    (A2C_decoder_f) A2C_SET_OF_decode_ber, (A2C_encoder_f) A2C_SET_OF_encode_ber,
    (A2C_compare_f) A2C_SET_OF_compare,
    (A2C_copy_f) A2C_SET_OF_copy,
    (A2C_print_f) A2C_SET_OF_print,
    (A2C_validate_f) A2C_SET_OF_validate,
    &S2_description
};

const A2C_ITEM S3_description_table[] = {
    {"i1", OFFSET_OF(S3, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"i2", OFFSET_OF(S3, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S3_description = {
  sizeof(S3), 0, countof(S3_description_table), S3_description_table, 0, 0
};

const A2C_DESCRIPTOR S3_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S3_description
};

const A2C_ITEM S4_description_table[] = {
    {"array", OFFSET_OF(S4, array), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S3_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S4_description = {
  sizeof(S4), 0, countof(S4_description_table), S4_description_table, 0, 0
};

const A2C_DESCRIPTOR S4_descriptor = {
    A2C_SET_OF_init, A2C_SET_OF_release, 
    (A2C_decoder_f) A2C_SET_OF_decode_der, (A2C_encoder_f) A2C_SET_OF_encode_der,
    (A2C_decoder_f) A2C_SET_OF_decode_ber, (A2C_encoder_f) A2C_SET_OF_encode_ber,
    (A2C_compare_f) A2C_SET_OF_compare,
    (A2C_copy_f) A2C_SET_OF_copy,
    (A2C_print_f) A2C_SET_OF_print,
    (A2C_validate_f) A2C_SET_OF_validate,
    &S4_description
};

const A2C_ITEM S5_description_table[] = {
    {"s3", OFFSET_OF(S5, s3), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S3_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S5_description = {
  sizeof(S5), 0, countof(S5_description_table), S5_description_table, 0, 0
};

const A2C_DESCRIPTOR S5_descriptor = {
    A2C_SET_OF_init, A2C_SET_OF_release, 
    (A2C_decoder_f) A2C_SET_OF_decode_der, (A2C_encoder_f) A2C_SET_OF_encode_der,
    (A2C_decoder_f) A2C_SET_OF_decode_ber, (A2C_encoder_f) A2C_SET_OF_encode_ber,
    (A2C_compare_f) A2C_SET_OF_compare,
    (A2C_copy_f) A2C_SET_OF_copy,
    (A2C_print_f) A2C_SET_OF_print,
    (A2C_validate_f) A2C_SET_OF_validate,
    &S5_description
};
const S1 v1={ 0, 0, 0 };

static const A2C_INTEGER_NATIVE v2___array[] = {{1}, {2}};
const S1 v2={ 2, 0, (A2C_INTEGER_NATIVE *) v2___array};

static const A2C_INTEGER_NATIVE v3___array[] = {{1}, {2}};
const S2 v3={ 2, 0, (A2C_INTEGER_NATIVE *) v3___array};

static const S3 v4___array[] = {{{1}, {1}}, {{4}, {5}}, {{10}, {11}}};
const S4 v4={ 3, 0, (S3 *) v4___array};

static const S3 v5___array[] = {{{1}, {1}}, {{4}, {5}}, {{10}, {11}}};
const S5 v5={ 3, 0, (S3 *) v5___array};

