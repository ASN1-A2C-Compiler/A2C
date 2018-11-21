#include "param-good.h"


A2C_COMPONENT_RELATIONS_TABLE Foo__f2_parameters_Relationship_table[] = {
	{"&id", 0, REL_OFFSET_OF(Foo, f2.algorithm, f2)},
};
A2C_COMPONENT_RELATIONS Foo__f2_parameters_Relationship = {
	&KeyAlgorithms, "&Type", 1, Foo__f2_parameters_Relationship_table
};

A2C_ERROR Foo__f2_parameters_decode_der(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_der(&Foo__f2_parameters_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR Foo__f2_parameters_decode_ber(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_ber(&Foo__f2_parameters_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

const A2C_ITEM ALGORITHM_description_table[] = {
    {"&id", OFFSET_OF(ALGORITHM, id), {A2C_TAG_CLASS_UNIVERSAL, 6}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_UNIQUE | A2C_FLAGS_DATA_PTR, 0},
    {"&Type", OFFSET_OF(ALGORITHM, Type), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OPEN_TYPE_descriptor, 0, A2C_FLAGS_DATA_PTR, 0},

};

const A2C_ITEM_HEADER ALGORITHM_description = {
  sizeof(ALGORITHM), 0, countof(ALGORITHM_description_table), ALGORITHM_description_table, 0, 0
};

const A2C_DESCRIPTOR ALGORITHM_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_CLASS_decode_der, (A2C_encoder_f) A2C_CLASS_encode_der,
    (A2C_decoder_f) A2C_CLASS_decode_ber, (A2C_encoder_f) A2C_CLASS_encode_ber,
    (A2C_compare_f) A2C_CLASS_compare,
    (A2C_copy_f) A2C_CLASS_copy,
    (A2C_print_f) A2C_CLASS_print,
    (A2C_validate_f) A2C_CLASS_validate,
    &ALGORITHM_description
};

const A2C_OBJECT_SET KeyAlgorithms = {
    0, &ALGORITHM_description,
};

const A2C_DESCRIPTOR Foo__f2_parameters_descriptor = {
    A2C_zero, A2C_OPEN_TYPE_release, 
    (A2C_decoder_f) Foo__f2_parameters_decode_der, (A2C_encoder_f) A2C_OPEN_TYPE_encode_der,
    (A2C_decoder_f) Foo__f2_parameters_decode_ber, (A2C_encoder_f) A2C_OPEN_TYPE_encode_ber,
    (A2C_compare_f) A2C_OPEN_TYPE_compare,
    (A2C_copy_f) A2C_OPEN_TYPE_copy,
    (A2C_print_f) A2C_OPEN_TYPE_print,
    (A2C_validate_f) A2C_OPEN_TYPE_validate,
    &A2C_OPEN_TYPE_description
};

const A2C_ITEM Foo__f2_description_table[] = {
    {"algorithm", REL_OFFSET_OF(Foo, f2.algorithm, f2), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, 0, 0},
    {"parameters", REL_OFFSET_OF(Foo, f2.parameters, f2), {A2C_TAG_CLASS_UNIVERSAL, 0}, &Foo__f2_parameters_descriptor, 0, A2C_FLAGS_OPTIONAL, 0},
};

const A2C_ITEM_HEADER Foo__f2_description = {
  0, 0, countof(Foo__f2_description_table), Foo__f2_description_table, 0, 0
};

const A2C_DESCRIPTOR Foo__f2_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &Foo__f2_description
};

const A2C_ITEM Foo_description_table[] = {
    {"i1", OFFSET_OF(Foo, i1), {A2C_TAG_CLASS_UNIVERSAL, 2}, &A2C_INTEGER_NATIVE_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"f2", OFFSET_OF(Foo, f2), {A2C_TAG_CLASS_UNIVERSAL, 16}, &Foo__f2_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER Foo_description = {
  sizeof(Foo), 0, countof(Foo_description_table), Foo_description_table, 0, 0
};

const A2C_DESCRIPTOR Foo_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &Foo_description
};
