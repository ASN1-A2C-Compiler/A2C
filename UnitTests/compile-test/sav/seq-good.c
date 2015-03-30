#include "seq-good.h"


const A2C_ITEM S1_description_table[] = {
    {"i1", OFFSET_OF(S1, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"i2", OFFSET_OF(S1, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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
const S1 a={{5}, {7}};

const S1 b={{1}, {10}};


const A2C_ITEM S2__s1_description_table[] = {
    {"i1", REL_OFFSET_OF(S2, s1.i1, s1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"i2", REL_OFFSET_OF(S2, s1.i2, s1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S2__s1_description = {
  0, 0, countof(S2__s1_description_table), S2__s1_description_table, 0, 0
};

const A2C_DESCRIPTOR S2__s1_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S2__s1_description
};

const A2C_ITEM S2_description_table[] = {
    {"i1", OFFSET_OF(S2, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s1", OFFSET_OF(S2, s1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S2__s1_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S2_description = {
  sizeof(S2), 0, countof(S2_description_table), S2_description_table, 0, 0
};

const A2C_DESCRIPTOR S2_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S2_description
};
const S2 c={{1}, {{11}, {12}}};


const A2C_ITEM S3_description_table[] = {
    {"i1", OFFSET_OF(S3, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s1", OFFSET_OF(S3, s1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S1_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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
const S3 d={{1}, {{5}, {7}}};

const S3 e={{1}, {{11}, {12}}};


const A2C_ITEM S4_description_table[] = {
    {"i1", OFFSET_OF(S4, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"i2", OFFSET_OF(S4, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_OPTIONAL, 0},
    {"b1", OFFSET_OF(S4, b1), {A2C_TAG_CLASS_UNIVERSAL, 1}, &A2C_BOOLEAN_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S4_description = {
  sizeof(S4), 0, countof(S4_description_table), S4_description_table, 0, 0
};

const A2C_DESCRIPTOR S4_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S4_description
};
const S4 f={1, {1}, {2}, {1}};

const S4 g={0, {1}, {0}, {0}};


const A2C_ITEM S5_description_table[] = {
    {"i1", OFFSET_OF(S5, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"i2", OFFSET_OF(S5, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, (PBYTE) 3, A2C_FLAGS_DEFAULT | A2C_FLAGS_TAG_IMPLICIT, 0},
    {"b1", OFFSET_OF(S5, b1), {A2C_TAG_CLASS_UNIVERSAL, 1}, &A2C_BOOLEAN_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S5_description = {
  sizeof(S5), 0, countof(S5_description_table), S5_description_table, 0, 0
};

const A2C_DESCRIPTOR S5_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S5_description
};
const S5 i={{1}, {2}, {1}};

const S5 j={{1}, {3}, {0}};

const S5 k={{1}, {3}, {1}};


const A2C_ITEM S6_description_table[] = {
    {"i1", OFFSET_OF(S6, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_OPTIONAL, 0},
    {"s5", OFFSET_OF(S6, s5), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S5_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_OPTIONAL, 0},
    {"b1", OFFSET_OF(S6, b1), {A2C_TAG_CLASS_UNIVERSAL, 1}, &A2C_BOOLEAN_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_OPTIONAL, 0},
};

const A2C_ITEM_HEADER S6_description = {
  sizeof(S6), 0, countof(S6_description_table), S6_description_table, 0, 0
};

const A2C_DESCRIPTOR S6_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S6_description
};

const A2C_ITEM S7_description_table[] = {
    {"i1", OFFSET_OF(S7, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, (PBYTE) 5, A2C_FLAGS_DEFAULT | A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s1", OFFSET_OF(S7, s1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S1_descriptor, (PBYTE) &S7__s1___default, A2C_FLAGS_DEFAULT_PTR | A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S7_description = {
  sizeof(S7), 0, countof(S7_description_table), S7_description_table, 0, 0
};

const A2C_DESCRIPTOR S7_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S7_description
};

const A2C_ITEM S8_description_table[] = {
    {"i1", OFFSET_OF(S8, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, (PBYTE) 5, A2C_FLAGS_DEFAULT | A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s1", OFFSET_OF(S8, s1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S1_descriptor, (PBYTE) &a, A2C_FLAGS_DEFAULT_PTR | A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S8_description = {
  sizeof(S8), 0, countof(S8_description_table), S8_description_table, 0, 0
};

const A2C_DESCRIPTOR S8_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &S8_description
};
const S1 S7__s1___default={{1}, {2}};

