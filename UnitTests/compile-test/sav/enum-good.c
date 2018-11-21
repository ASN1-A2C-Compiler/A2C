#include "enum-good.h"

const A2C_INTEGER_NATIVE a={5};

const A2C_NAME_ITEM T1_name_table[] = {
    { T1__a, 0, "a"},
};

const A2C_ITEM_HEADER T1_description = {
  sizeof(T1), 0, 0, 0, countof(T1_name_table), T1_name_table
};

const A2C_DESCRIPTOR T1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
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
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T2_description
};
const T2 c={5};

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
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T3_description
};
const A2C_NAME_ITEM T4_name_table[] = {
    { T4__a, 0, "a"},
    { T4__b, 0, "b"},
    { T4__c, 0, "c"},
    { T4__d, 0, "d"},
    { T4__e, 0, "e"},
    { T4__f, 0, "f"},
};

const A2C_ITEM_HEADER T4_description = {
  sizeof(T4), 0, 0, 0, countof(T4_name_table), T4_name_table
};

const A2C_DESCRIPTOR T4_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T4_description
};
const A2C_NAME_ITEM T8_name_table[] = {
    { T8__a, 0, "a"},
    { T8__b, 0, "b"},
};

const A2C_ITEM_HEADER T8_description = {
  sizeof(T8), 0, 0, 0, countof(T8_name_table), T8_name_table
};

const A2C_DESCRIPTOR T8_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T8_description
};
const A2C_NAME_ITEM T5_name_table[] = {
    { T5__a, 0, "a"},
    { T5__b, 0, "b"},
};

const A2C_ITEM_HEADER T5_description = {
  sizeof(T5), 0, 0, 0, countof(T5_name_table), T5_name_table
};

const A2C_DESCRIPTOR T5_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T5_description
};
const A2C_NAME_ITEM T6_name_table[] = {
    { T6__a, 0, "a"},
    { T6__b, 0, "b"},
    { T6__c, 0, "c"},
    { T6__d, 0, "d"},
};

const A2C_ITEM_HEADER T6_description = {
  sizeof(T6), 0, 0, 0, countof(T6_name_table), T6_name_table
};

const A2C_DESCRIPTOR T6_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T6_description
};
const A2C_NAME_ITEM T7_name_table[] = {
    { T7__a, 0, "a"},
    { T7__b, 0, "b"},
    { T7__c, 0, "c"},
    { T7__d, 0, "d"},
};

const A2C_ITEM_HEADER T7_description = {
  sizeof(T7), 0, 0, 0, countof(T7_name_table), T7_name_table
};

const A2C_DESCRIPTOR T7_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &T7_description
};
const A2C_NAME_ITEM C_name_table[] = {
    { C__b, 0, "b"},
    { C__a, 0, "a"},
    { C__c, 0, "c"},
};

const A2C_ITEM_HEADER C_description = {
  sizeof(C), 0, 0, 0, countof(C_name_table), C_name_table
};

const A2C_DESCRIPTOR C_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &C_description
};
const A2C_NAME_ITEM D_name_table[] = {
    { D__a, 0, "a"},
    { D__b, 0, "b"},
    { D__c, 0, "c"},
};

const A2C_ITEM_HEADER D_description = {
  sizeof(D), 0, 0, 0, countof(D_name_table), D_name_table
};

const A2C_DESCRIPTOR D_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &D_description
};
const A2C_NAME_ITEM A1_name_table[] = {
    { A1__a, 0, "a"},
    { A1__b, 0, "b"},
    { A1__c, 0, "c"},
};

const A2C_ITEM_HEADER A1_description = {
  sizeof(A1), 0, 0, 0, countof(A1_name_table), A1_name_table
};

const A2C_DESCRIPTOR A1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &A1_description
};
const A2C_NAME_ITEM B1_name_table[] = {
    { B1__c, 0, "c"},
    { B1__a, 0, "a"},
    { B1__b, 0, "b"},
    { B1__d, 0, "d"},
};

const A2C_ITEM_HEADER B1_description = {
  sizeof(B1), 0, 0, 0, countof(B1_name_table), B1_name_table
};

const A2C_DESCRIPTOR B1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &B1_description
};
const A2C_NAME_ITEM C1_name_table[] = {
    { C1__a, 0, "a"},
    { C1__b, 0, "b"},
    { C1__c, 0, "c"},
    { C1__d, 0, "d"},
};

const A2C_ITEM_HEADER C1_description = {
  sizeof(C1), 0, 0, 0, countof(C1_name_table), C1_name_table
};

const A2C_DESCRIPTOR C1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &C1_description
};
const A2C_NAME_ITEM D1_name_table[] = {
    { D1__z, 0, "z"},
    { D1__a, 0, "a"},
    { D1__d, 0, "d"},
};

const A2C_ITEM_HEADER D1_description = {
  sizeof(D1), 0, 0, 0, countof(D1_name_table), D1_name_table
};

const A2C_DESCRIPTOR D1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_der, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_der,
    (A2C_decoder_f) A2C_ENUMERATED_NATIVE_decode_ber, (A2C_encoder_f) A2C_ENUMERATED_NATIVE_encode_ber,
    (A2C_compare_f) A2C_ENUMERATED_NATIVE_compare,
    (A2C_copy_f) A2C_ENUMERATED_NATIVE_copy,
    (A2C_print_f) A2C_ENUMERATED_NATIVE_print,
    (A2C_validate_f) A2C_ENUMERATED_NATIVE_validate,
    &D1_description
};
