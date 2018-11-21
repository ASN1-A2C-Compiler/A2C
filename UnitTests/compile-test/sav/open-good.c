#include "open-good.h"


A2C_COMPONENT_RELATIONS_TABLE ContentInfo__content_Relationship_table[] = {
	{"&id", 0, OFFSET_OF(ContentInfo, contentType)},
};
A2C_COMPONENT_RELATIONS ContentInfo__content_Relationship = {
	&ContentSet, "&Type", 1, ContentInfo__content_Relationship_table
};

A2C_ERROR ContentInfo__content_decode_der(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_der(&ContentInfo__content_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR ContentInfo__content_decode_ber(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_ber(&ContentInfo__content_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_COMPONENT_RELATIONS_TABLE ContentInfo2__content_array_Relationship_table[] = {
	{"&id", 1, OFFSET_OF(ContentInfo2, contentType)},
};
A2C_COMPONENT_RELATIONS ContentInfo2__content_array_Relationship = {
	&ContentSet, "&Type", 1, ContentInfo2__content_array_Relationship_table
};

A2C_ERROR ContentInfo2__content_array_decode_der(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_der(&ContentInfo2__content_array_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR ContentInfo2__content_array_decode_ber(A2C_OPEN_TYPE * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_RELATION_decode_ber(&ContentInfo2__content_array_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_COMPONENT_RELATIONS_TABLE ContentInfo3__content_Relationship_table[] = {
	{"&id", 0, OFFSET_OF(ContentInfo3, contentType)},
};
A2C_COMPONENT_RELATIONS ContentInfo3__content_Relationship = {
	&ContentSet, "&Type", 1, ContentInfo3__content_Relationship_table
};

A2C_ERROR ContentInfo3__content_decode_der(A2C_OPEN_OCTET_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_OPEN_OCTET_STRING_RELATION_decode_der(&ContentInfo3__content_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

A2C_ERROR ContentInfo3__content_decode_ber(A2C_OPEN_OCTET_STRING * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,
	A2C_TAG const * ptag, A2C_STREAM_MEMORY * pstm)
{
    return A2C_OPEN_OCTET_STRING_RELATION_decode_ber(&ContentInfo3__content_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);
}

const A2C_ITEM CONTENT_TYPE_description_table[] = {
    {"&id", OFFSET_OF(CONTENT_TYPE, id), {A2C_TAG_CLASS_UNIVERSAL, 6}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT | A2C_FLAGS_UNIQUE | A2C_FLAGS_DATA_PTR, 0},
    {"&Type", OFFSET_OF(CONTENT_TYPE, Type), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OPEN_TYPE_descriptor, 0, A2C_FLAGS_DATA_PTR, 0},

};

const A2C_ITEM_HEADER CONTENT_TYPE_description = {
  sizeof(CONTENT_TYPE), 0, countof(CONTENT_TYPE_description_table), CONTENT_TYPE_description_table, 0, 0
};

const A2C_DESCRIPTOR CONTENT_TYPE_descriptor = {
    A2C_zero, A2C_nop, 
    (A2C_decoder_f) A2C_CLASS_decode_der, (A2C_encoder_f) A2C_CLASS_encode_der,
    (A2C_decoder_f) A2C_CLASS_decode_ber, (A2C_encoder_f) A2C_CLASS_encode_ber,
    (A2C_compare_f) A2C_CLASS_compare,
    (A2C_copy_f) A2C_CLASS_copy,
    (A2C_print_f) A2C_CLASS_print,
    (A2C_validate_f) A2C_CLASS_validate,
    &CONTENT_TYPE_description
};
const A2C_OBJECT_IDENTIFIER id_ct={"1.2.840.113549.1.7"};

const A2C_OBJECT_IDENTIFIER id_data={"1.2.840.113549.1.7.1"};

const CONTENT_TYPE ct_T1 = { &id_data, &A2C_INTEGER_NATIVE_descriptor };
const A2C_OBJECT_IDENTIFIER id_ct2={"1.2.840.113549.1.7.2"};

const CONTENT_TYPE ct_T2 = { &id_ct2, &A2C_OCTET_STRING_descriptor };

const A2C_ITEM ct_T3__Type_description_table[] = {
    {"teletexString", OFFSET_OF(ct_T3__Type, u.teletexString), {A2C_TAG_CLASS_UNIVERSAL, 20}, &A2C_TELETEX_STRING_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"printableString", OFFSET_OF(ct_T3__Type, u.printableString), {A2C_TAG_CLASS_UNIVERSAL, 19}, &A2C_PRINTABLE_STRING_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"universalString", OFFSET_OF(ct_T3__Type, u.universalString), {A2C_TAG_CLASS_UNIVERSAL, 28}, &A2C_UNIVERSAL_STRING_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"utf8String", OFFSET_OF(ct_T3__Type, u.utf8String), {A2C_TAG_CLASS_UNIVERSAL, 12}, &A2C_UTF8_STRING_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
    {"bmpString", OFFSET_OF(ct_T3__Type, u.bmpString), {A2C_TAG_CLASS_UNIVERSAL, 30}, &A2C_BMP_STRING_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER ct_T3__Type_description = {
  sizeof(ct_T3__Type), 0, countof(ct_T3__Type_description_table), ct_T3__Type_description_table, 0, 0
};

const A2C_DESCRIPTOR ct_T3__Type_descriptor = {
    A2C_CHOICE_init, A2C_CHOICE_release, 
    (A2C_decoder_f) A2C_CHOICE_decode_der, (A2C_encoder_f) A2C_CHOICE_encode_der,
    (A2C_decoder_f) A2C_CHOICE_decode_ber, (A2C_encoder_f) A2C_CHOICE_encode_ber,
    (A2C_compare_f) A2C_CHOICE_compare,
    (A2C_copy_f) A2C_CHOICE_copy,
    (A2C_print_f) A2C_CHOICE_print,
    (A2C_validate_f) A2C_CHOICE_validate,
    &ct_T3__Type_description
};
const CONTENT_TYPE ct_T3 = { &ct_T3__id, &ct_T3__Type_descriptor };

const A2C_ITEM_HEADER ct_T4__Type_description = {
  sizeof(ct_T4__Type), 0, 0, 0, 0, 0
};

const A2C_DESCRIPTOR ct_T4__Type_descriptor = {
    A2C_string_init, A2C_string_release, 
    (A2C_decoder_f) A2C_PRINTABLE_STRING_decode_der, (A2C_encoder_f) A2C_PRINTABLE_STRING_encode_der,
    (A2C_decoder_f) A2C_PRINTABLE_STRING_decode_ber, (A2C_encoder_f) A2C_PRINTABLE_STRING_encode_ber,
    (A2C_compare_f) A2C_PRINTABLE_STRING_compare,
    (A2C_copy_f) A2C_PRINTABLE_STRING_copy,
    (A2C_print_f) A2C_PRINTABLE_STRING_print,
    (A2C_validate_f) A2C_PRINTABLE_STRING_validate,
    &ct_T4__Type_description
};
const CONTENT_TYPE ct_T4 = { &ct_T4__id, &ct_T4__Type_descriptor };

const PCVOID ContentSet_table[] = {
    &ct_T1, &ct_T2, &ct_T3, &ct_T4, 
};
const A2C_OBJECT_SET ContentSet = {
    4, &CONTENT_TYPE_description,
    ContentSet_table
};

const A2C_DESCRIPTOR ContentInfo__content_descriptor = {
    A2C_zero, A2C_OPEN_TYPE_release, 
    (A2C_decoder_f) ContentInfo__content_decode_der, (A2C_encoder_f) A2C_OPEN_TYPE_encode_der,
    (A2C_decoder_f) ContentInfo__content_decode_ber, (A2C_encoder_f) A2C_OPEN_TYPE_encode_ber,
    (A2C_compare_f) A2C_OPEN_TYPE_compare,
    (A2C_copy_f) A2C_OPEN_TYPE_copy,
    (A2C_print_f) A2C_OPEN_TYPE_print,
    (A2C_validate_f) A2C_OPEN_TYPE_validate,
    &A2C_OPEN_TYPE_description
};

const A2C_ITEM ContentInfo_description_table[] = {
    {"contentType", OFFSET_OF(ContentInfo, contentType), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, 0, 0},
    {"content", OFFSET_OF(ContentInfo, content), {A2C_TAG_CLASS_UNIVERSAL, 0}, &ContentInfo__content_descriptor, 0, 0, 0},
};

const A2C_ITEM_HEADER ContentInfo_description = {
  sizeof(ContentInfo), 0, countof(ContentInfo_description_table), ContentInfo_description_table, 0, 0
};

const A2C_DESCRIPTOR ContentInfo_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &ContentInfo_description
};

const A2C_DESCRIPTOR ContentInfo2__content_array_descriptor = {
    A2C_zero, A2C_OPEN_TYPE_release, 
    (A2C_decoder_f) ContentInfo2__content_array_decode_der, (A2C_encoder_f) A2C_OPEN_TYPE_encode_der,
    (A2C_decoder_f) ContentInfo2__content_array_decode_ber, (A2C_encoder_f) A2C_OPEN_TYPE_encode_ber,
    (A2C_compare_f) A2C_OPEN_TYPE_compare,
    (A2C_copy_f) A2C_OPEN_TYPE_copy,
    (A2C_print_f) A2C_OPEN_TYPE_print,
    (A2C_validate_f) A2C_OPEN_TYPE_validate,
    &A2C_OPEN_TYPE_description
};

const A2C_ITEM ContentInfo2__content_description_table[] = {
    {"array", REL_OFFSET_OF(ContentInfo2, content.array, content), {A2C_TAG_CLASS_UNIVERSAL, 0}, &ContentInfo2__content_array_descriptor, 0, 0, 0},
};

const A2C_ITEM_HEADER ContentInfo2__content_description = {
  0, 0, countof(ContentInfo2__content_description_table), ContentInfo2__content_description_table, 0, 0
};

const A2C_DESCRIPTOR ContentInfo2__content_descriptor = {
    A2C_SET_OF_init, A2C_SET_OF_release, 
    (A2C_decoder_f) A2C_SET_OF_decode_der, (A2C_encoder_f) A2C_SET_OF_encode_der,
    (A2C_decoder_f) A2C_SET_OF_decode_ber, (A2C_encoder_f) A2C_SET_OF_encode_ber,
    (A2C_compare_f) A2C_SET_OF_compare,
    (A2C_copy_f) A2C_SET_OF_copy,
    (A2C_print_f) A2C_SET_OF_print,
    (A2C_validate_f) A2C_SET_OF_validate,
    &ContentInfo2__content_description
};

const A2C_ITEM ContentInfo2_description_table[] = {
    {"contentType", OFFSET_OF(ContentInfo2, contentType), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, 0, 0},
    {"content", OFFSET_OF(ContentInfo2, content), {A2C_TAG_CLASS_UNIVERSAL, 17}, &ContentInfo2__content_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER ContentInfo2_description = {
  sizeof(ContentInfo2), 0, countof(ContentInfo2_description_table), ContentInfo2_description_table, 0, 0
};

const A2C_DESCRIPTOR ContentInfo2_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &ContentInfo2_description
};

const A2C_DESCRIPTOR ContentInfo3__content_descriptor = {
    A2C_zero, A2C_OPEN_OCTET_STRING_release, 
    (A2C_decoder_f) ContentInfo3__content_decode_der, (A2C_encoder_f) A2C_OPEN_OCTET_STRING_encode_der,
    (A2C_decoder_f) ContentInfo3__content_decode_ber, (A2C_encoder_f) A2C_OPEN_OCTET_STRING_encode_ber,
    (A2C_compare_f) A2C_OPEN_OCTET_STRING_compare,
    (A2C_copy_f) A2C_OPEN_OCTET_STRING_copy,
    (A2C_print_f) A2C_OPEN_OCTET_STRING_print,
    (A2C_validate_f) A2C_OPEN_OCTET_STRING_validate,
    &A2C_OPEN_OCTET_STRING_description
};

const A2C_ITEM ContentInfo3_description_table[] = {
    {"contentType", OFFSET_OF(ContentInfo3, contentType), {A2C_TAG_CLASS_UNIVERSAL, 0}, &A2C_OBJECT_IDENTIFIER_descriptor, 0, 0, 0},
    {"content", OFFSET_OF(ContentInfo3, content), {A2C_TAG_CLASS_UNIVERSAL, 4}, &ContentInfo3__content_descriptor, 0, A2C_FLAGS_TAG_IMPLICIT, 0},
};

const A2C_ITEM_HEADER ContentInfo3_description = {
  sizeof(ContentInfo3), 0, countof(ContentInfo3_description_table), ContentInfo3_description_table, 0, 0
};

const A2C_DESCRIPTOR ContentInfo3_descriptor = {
    A2C_SEQUENCE_init, A2C_SEQUENCE_release, 
    (A2C_decoder_f) A2C_SEQUENCE_decode_der, (A2C_encoder_f) A2C_SEQUENCE_encode_der,
    (A2C_decoder_f) A2C_SEQUENCE_decode_ber, (A2C_encoder_f) A2C_SEQUENCE_encode_ber,
    (A2C_compare_f) A2C_SEQUENCE_compare,
    (A2C_copy_f) A2C_SEQUENCE_copy,
    (A2C_print_f) A2C_SEQUENCE_print,
    (A2C_validate_f) A2C_SEQUENCE_validate,
    &ContentInfo3_description
};
const A2C_OBJECT_IDENTIFIER ct_T3__id={"1.2.840.113549.1.7.3"};

const A2C_OBJECT_IDENTIFIER ct_T4__id={"1.2.840.113549.1.7.4"};

