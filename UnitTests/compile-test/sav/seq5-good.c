#include "seq5-good.h"


const A2C_ITEM C1_description_table[] = {
    {"i1", OFFSET_OF(C1, u.i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"b1", OFFSET_OF(C1, u.b1), {A2C_TAG_CLASS_UNIVERSAL, 1}, &A2C_BOOLEAN_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER C1_description = {
  sizeof(C1), 0, countof(C1_description_table), C1_description_table, 0, 0
};

const A2C_DESCRIPTOR C1_descriptor = {
    A2C_CHOICE_init, A2C_CHOICE_release, 
    (A2C_decoder_f) A2C_CHOICE_decode_der, (A2C_encoder_f) A2C_CHOICE_encode_der,
    (A2C_decoder_f) A2C_CHOICE_decode_ber, (A2C_encoder_f) A2C_CHOICE_encode_ber,
    (A2C_compare_f) A2C_CHOICE_compare,
    (A2C_copy_f) A2C_CHOICE_copy,
    (A2C_print_f) A2C_CHOICE_print,
    (A2C_validate_f) A2C_CHOICE_validate,
    &C1_description
};

const A2C_ITEM C2_description_table [] = {
  { "tag", 0, {A2C_TAG_CLASS_CONTEXT, 1},  &C1_descriptor, 0, A2C_FLAGS_TAG_EXPLICIT, 0 }
};

const A2C_ITEM_HEADER C2_description = {
  sizeof(C2), 0, countof(C2_description_table), C2_description_table, 0, 0
};

const A2C_DESCRIPTOR C2_descriptor = {
    A2C_TAG_init, A2C_TAG_release, 
    (A2C_decoder_f) A2C_TAG_decode_der, (A2C_encoder_f) A2C_TAG_encode_der,
    (A2C_decoder_f) A2C_TAG_decode_ber, (A2C_encoder_f) A2C_TAG_encode_ber,
    (A2C_compare_f) A2C_TAG_compare,
    (A2C_copy_f) A2C_TAG_copy,
    (A2C_print_f) A2C_TAG_print,
    (A2C_validate_f) A2C_TAG_validate,
    &C2_description
};

const A2C_ITEM S1_description_table[] = {
    {"c1", OFFSET_OF(S1, c1), {A2C_TAG_CLASS_UNIVERSAL, 0}, &C1_descriptor, 0, 0, 0},
    {"c2", OFFSET_OF(S1, c2), {A2C_TAG_CLASS_CONTEXT, 2}, &C1_descriptor, 0, A2C_FLAGS_TAG_EXPLICIT, 0},
    {"c3", OFFSET_OF(S1, c3), {A2C_TAG_CLASS_CONTEXT, 1}, &C2_descriptor, 0, A2C_FLAGS_TAG_EXPLICIT, 0},
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
