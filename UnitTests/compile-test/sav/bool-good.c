#include "bool-good.h"


const A2C_ITEM_HEADER I1_description = {
  sizeof(I1), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_BOOLEAN_decode_der, (A2C_encoder_f) A2C_BOOLEAN_encode_der,
    (A2C_decoder_f) A2C_BOOLEAN_decode_ber, (A2C_encoder_f) A2C_BOOLEAN_encode_ber,
    (A2C_compare_f) A2C_BOOLEAN_compare,
    (A2C_copy_f) A2C_BOOLEAN_copy,
    (A2C_print_f) A2C_BOOLEAN_print,
    (A2C_validate_f) A2C_BOOLEAN_validate,
    &I1_description
};

const A2C_ITEM_HEADER I2_description = {
  sizeof(I2), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I2_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_BOOLEAN_decode_der, (A2C_encoder_f) A2C_BOOLEAN_encode_der,
    (A2C_decoder_f) A2C_BOOLEAN_decode_ber, (A2C_encoder_f) A2C_BOOLEAN_encode_ber,
    (A2C_compare_f) A2C_BOOLEAN_compare,
    (A2C_copy_f) A2C_BOOLEAN_copy,
    (A2C_print_f) A2C_BOOLEAN_print,
    (A2C_validate_f) A2C_BOOLEAN_validate,
    &I2_description
};

const A2C_ITEM_HEADER I3_description = {
  sizeof(I3), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I3_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_BOOLEAN_decode_der, (A2C_encoder_f) A2C_BOOLEAN_encode_der,
    (A2C_decoder_f) A2C_BOOLEAN_decode_ber, (A2C_encoder_f) A2C_BOOLEAN_encode_ber,
    (A2C_compare_f) A2C_BOOLEAN_compare,
    (A2C_copy_f) A2C_BOOLEAN_copy,
    (A2C_print_f) A2C_BOOLEAN_print,
    (A2C_validate_f) A2C_BOOLEAN_validate,
    &I3_description
};
const A2C_BOOLEAN t={1};

const A2C_BOOLEAN f={0};

const A2C_BOOLEAN t2={1};

