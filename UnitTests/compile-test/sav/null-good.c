#include "null-good.h"


const A2C_ITEM_HEADER I1_description = {
  sizeof(I1), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR I1_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_NULL_decode_der, (A2C_encoder_f) A2C_NULL_encode_der,
    (A2C_decoder_f) A2C_NULL_decode_ber, (A2C_encoder_f) A2C_NULL_encode_ber,
    (A2C_compare_f) A2C_NULL_compare,
    (A2C_copy_f) A2C_NULL_copy,
    (A2C_print_f) A2C_NULL_print,
    (A2C_validate_f) A2C_NULL_validate,
    &I1_description
};
const I1 i1={0};

