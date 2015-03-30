#include "seq2-good.h"


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

const A2C_ITEM S4__c1_description_table[] = {
    {"i3", REL_OFFSET_OF(S4, c1.u.i3, c1.u), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s3", REL_OFFSET_OF(S4, c1.u.s3, c1.u), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S3_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER S4__c1_description = {
  0, 0, countof(S4__c1_description_table), S4__c1_description_table, 0, 0
};

const A2C_DESCRIPTOR S4__c1_descriptor = {
    A2C_CHOICE_init, A2C_CHOICE_release, 
    (A2C_decoder_f) A2C_CHOICE_decode_der, (A2C_encoder_f) A2C_CHOICE_encode_der,
    (A2C_decoder_f) A2C_CHOICE_decode_ber, (A2C_encoder_f) A2C_CHOICE_encode_ber,
    (A2C_compare_f) A2C_CHOICE_compare,
    (A2C_copy_f) A2C_CHOICE_copy,
    (A2C_print_f) A2C_CHOICE_print,
    (A2C_validate_f) A2C_CHOICE_validate,
    &S4__c1_description
};

const A2C_ITEM S4_description_table[] = {
    {"i2", OFFSET_OF(S4, i2), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"c1", OFFSET_OF(S4, c1), {A2C_TAG_CLASS_UNIVERSAL, 0}, &S4__c1_descriptor, 0, 0, 0},
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

const A2C_ITEM S3_description_table[] = {
    {"i1", OFFSET_OF(S3, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"s4", OFFSET_OF(S3, s4), {A2C_TAG_CLASS_UNIVERSAL, 16}, &S4_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
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
