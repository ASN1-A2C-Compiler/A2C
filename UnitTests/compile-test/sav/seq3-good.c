#include "seq3-good.h"


const A2C_ITEM S2_description_table[] = {
    {"i2", OFFSET_OF(S2, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s1", OFFSET_OF(S2, s1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S1_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_OPTIONAL, 0},
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

const A2C_ITEM S1_description_table[] = {
    {"i1", OFFSET_OF(S1, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s2", OFFSET_OF(S1, s2), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S2_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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

const A2C_ITEM S6_description_table[] = {
    {"type", OFFSET_OF(S6, type), {A2C_TAG_CLASS_UNIVERSAL, 6}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"value", OFFSET_OF(S6, value), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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

const A2C_ITEM S5_description_table[] = {
    {"array", OFFSET_OF(S5, array), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S6_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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

const A2C_ITEM SO4_description_table[] = {
    {"array", OFFSET_OF(SO4, array), {A2C_TAG_CLASS_UNIVERSAL, 17}, &S5_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER SO4_description = {
  sizeof(SO4), 0, countof(SO4_description_table), SO4_description_table, 0, 0
};

const A2C_DESCRIPTOR SO4_descriptor = {
    A2C_SEQUENCE_OF_init, A2C_SEQUENCE_OF_release, 
    (A2C_decoder_f) A2C_SEQUENCE_OF_decode_der, (A2C_encoder_f) A2C_SEQUENCE_OF_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_OF_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_OF_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_OF_compare,
    (A2C_copy_f) A2C_SEQUENCE_OF_copy,
    (A2C_print_f) A2C_SEQUENCE_OF_print,
    (A2C_validate_f) A2C_SEQUENCE_OF_validate,
    &SO4_description
};

const A2C_ITEM S3_description_table[] = {
    {"c1", OFFSET_OF(S3, u.c1), {A2C_TAG_CLASS_UNIVERSAL, 16}, &SO4_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S3_description = {
  sizeof(S3), 0, countof(S3_description_table), S3_description_table, 0, 0
};

const A2C_DESCRIPTOR S3_descriptor = {
    A2C_CHOICE_init, A2C_CHOICE_release, 
    (A2C_decoder_f) A2C_CHOICE_decode_der, (A2C_encoder_f) A2C_CHOICE_encode_der,
    (A2C_decoder_f) A2C_CHOICE_decode_ber, (A2C_encoder_f) A2C_CHOICE_encode_ber,
    (A2C_compare_f) A2C_CHOICE_compare,
    (A2C_copy_f) A2C_CHOICE_copy,
    (A2C_print_f) A2C_CHOICE_print,
    (A2C_validate_f) A2C_CHOICE_validate,
    &S3_description
};
