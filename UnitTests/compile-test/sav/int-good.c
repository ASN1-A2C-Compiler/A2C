#include "int-good.h"


const A2C_ITEM_HEADER I1_description = {
  sizeof(I1), 0, 0, 0, 0, 0
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

const A2C_ITEM I2_description_table [] = {
  { "tag", 0, {A2C_TAG_CLASS_CONTEXT, 1},  &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_EXPLICIT, 0 }
};

const A2C_ITEM_HEADER I2_description = {
  sizeof(I2), 0, countof(I2_description_table), I2_description_table, 0, 0
};

const A2C_DESCRIPTOR I2_descriptor = {
    A2C_TAG_init, A2C_TAG_release, 
    (A2C_decoder_f) A2C_TAG_decode_der, (A2C_encoder_f) A2C_TAG_encode_der,
    (A2C_decoder_f) A2C_TAG_decode_ber, (A2C_encoder_f) A2C_TAG_encode_ber,
    (A2C_compare_f) A2C_TAG_compare,
    (A2C_copy_f) A2C_TAG_copy,
    (A2C_print_f) A2C_TAG_print,
    (A2C_validate_f) A2C_TAG_validate,
    &I2_description
};

const A2C_ITEM I3_description_table [] = {
  { "tag", 0, {A2C_TAG_CLASS_CONTEXT, 2},  &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0 }
};

const A2C_ITEM_HEADER I3_description = {
  sizeof(I3), 0, countof(I3_description_table), I3_description_table, 0, 0
};

const A2C_DESCRIPTOR I3_descriptor = {
    A2C_TAG_init, A2C_TAG_release, 
    (A2C_decoder_f) A2C_TAG_decode_der, (A2C_encoder_f) A2C_TAG_encode_der,
    (A2C_decoder_f) A2C_TAG_decode_ber, (A2C_encoder_f) A2C_TAG_encode_ber,
    (A2C_compare_f) A2C_TAG_compare,
    (A2C_copy_f) A2C_TAG_copy,
    (A2C_print_f) A2C_TAG_print,
    (A2C_validate_f) A2C_TAG_validate,
    &I3_description
};

const A2C_ITEM I4_description_table [] = {
  { "tag", 0, {A2C_TAG_CLASS_CONTEXT, 1},  &I1_descriptor, 0, A2C_FLAGS_TAG_EXPLICIT, 0 }
};

const A2C_ITEM_HEADER I4_description = {
  sizeof(I4), 0, countof(I4_description_table), I4_description_table, 0, 0
};

const A2C_DESCRIPTOR I4_descriptor = {
    A2C_TAG_init, A2C_TAG_release, 
    (A2C_decoder_f) A2C_TAG_decode_der, (A2C_encoder_f) A2C_TAG_encode_der,
    (A2C_decoder_f) A2C_TAG_decode_ber, (A2C_encoder_f) A2C_TAG_encode_ber,
    (A2C_compare_f) A2C_TAG_compare,
    (A2C_copy_f) A2C_TAG_copy,
    (A2C_print_f) A2C_TAG_print,
    (A2C_validate_f) A2C_TAG_validate,
    &I4_description
};

const A2C_ITEM_HEADER I5_description = {
  sizeof(I5), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I5_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    (A2C_validate_f) A2C_INTEGER_NATIVE_validate,
    &I5_description
};
const I5 i5_1={1};

const I5 i5_2={10};

const A2C_INTEGER_NATIVE a={1};

const A2C_NAME_ITEM T1_name_table[] = {
    { T1__a, 0, "a"},
};

const A2C_ITEM_HEADER T1_description = {
  sizeof(T1), 0, 0, 0, countof(T1_name_table), T1_name_table
};

const A2C_DESCRIPTOR T1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    (A2C_validate_f) A2C_INTEGER_NATIVE_validate,
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
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    (A2C_validate_f) A2C_INTEGER_NATIVE_validate,
    &T2_description
};
const T2 c={1};

const T2 d={3};

const A2C_NAME_ITEM T3_name_table[] = {
    { T3__a, 0, "a"},
    { T3__b, 0, "b"},
    { T3__c, 0, "c"},
};

const A2C_ITEM_HEADER T3_description = {
  sizeof(T3), 0, 0, 0, countof(T3_name_table), T3_name_table
};

const A2C_DESCRIPTOR T3_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_der, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_der,
    (A2C_decoder_f) A2C_INTEGER_NATIVE_decode_ber, (A2C_encoder_f) A2C_INTEGER_NATIVE_encode_ber,
    (A2C_compare_f) A2C_INTEGER_NATIVE_compare,
    (A2C_copy_f) A2C_INTEGER_NATIVE_copy,
    (A2C_print_f) A2C_INTEGER_NATIVE_print,
    (A2C_validate_f) A2C_INTEGER_NATIVE_validate,
    &T3_description
};
const A2C_INTEGER_NATIVE e={1};

