/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "rrlp-components.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#include "AssistanceData.h"

static asn_TYPE_member_t asn_MBR_AssistanceData_1[] = {
	{ ATF_POINTER, 8, offsetof(struct AssistanceData, referenceAssistData),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReferenceAssistData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"referenceAssistData"
		},
	{ ATF_POINTER, 7, offsetof(struct AssistanceData, msrAssistData),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MsrAssistData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"msrAssistData"
		},
	{ ATF_POINTER, 6, offsetof(struct AssistanceData, systemInfoAssistData),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SystemInfoAssistData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"systemInfoAssistData"
		},
	{ ATF_POINTER, 5, offsetof(struct AssistanceData, gps_AssistData),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GPS_AssistData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"gps-AssistData"
		},
	{ ATF_POINTER, 4, offsetof(struct AssistanceData, moreAssDataToBeSent),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MoreAssDataToBeSent,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"moreAssDataToBeSent"
		},
	{ ATF_POINTER, 3, offsetof(struct AssistanceData, extensionContainer),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 2, offsetof(struct AssistanceData, rel98_AssistanceData_Extension),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Rel98_AssistanceData_Extension,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rel98-AssistanceData-Extension"
		},
	{ ATF_POINTER, 1, offsetof(struct AssistanceData, rel5_AssistanceData_Extension),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Rel5_AssistanceData_Extension,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rel5-AssistanceData-Extension"
		},
};
static int asn_MAP_AssistanceData_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
static ber_tlv_tag_t asn_DEF_AssistanceData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_AssistanceData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* referenceAssistData at 67 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* msrAssistData at 68 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* systemInfoAssistData at 69 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* gps-AssistData at 70 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* moreAssDataToBeSent at 71 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* extensionContainer at 75 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* rel98-AssistanceData-Extension at 78 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* rel5-AssistanceData-Extension at 79 */
};
static asn_SEQUENCE_specifics_t asn_SPC_AssistanceData_specs_1 = {
	sizeof(struct AssistanceData),
	offsetof(struct AssistanceData, _asn_ctx),
	asn_MAP_AssistanceData_tag2el_1,
	8,	/* Count of tags in the map */
	asn_MAP_AssistanceData_oms_1,	/* Optional members */
	6, 2,	/* Root/Additions */
	5,	/* Start extensions */
	9	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_AssistanceData = {
	"AssistanceData",
	"AssistanceData",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_AssistanceData_tags_1,
	sizeof(asn_DEF_AssistanceData_tags_1)
		/sizeof(asn_DEF_AssistanceData_tags_1[0]), /* 1 */
	asn_DEF_AssistanceData_tags_1,	/* Same as above */
	sizeof(asn_DEF_AssistanceData_tags_1)
		/sizeof(asn_DEF_AssistanceData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_AssistanceData_1,
	8,	/* Elements count */
	&asn_SPC_AssistanceData_specs_1	/* Additional specs */
};

