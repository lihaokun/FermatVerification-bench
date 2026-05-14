/* ============================================================ */
/* ANSSI x509-parser — flat concatenation for FermatVerification-bench */
/* Original: https://github.com/ANSSI-FR/x509-parser */
/* Order: 6 headers first, then 6 implementations */
/* Internal #include "..." lines are commented out (deps inlined). */
/* ============================================================ */

/* ============== HEADER: x509-cert-parser.h ============== */
/*
 *  Copyright (C) 2019 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_CERT_PARSER_H__
#define __X509_CERT_PARSER_H__

/* removed: #include "x509-config.h" */
/* removed: #include "x509-utils.h" */
/* removed: #include "x509-common.h" */

typedef struct {
	/* tbcCertificate */
	u32 tbs_start;
	u32 tbs_len;

	/* Version */
	u8 version;

	/* Serial */
	u32 serial_start;
	u32 serial_len;

	/* inner sig alg (tbsCertificate.signature) */
	u32 tbs_sig_alg_start;
	u32 tbs_sig_alg_len;
	u32 tbs_sig_alg_oid_start; /* OID for sig alg */
	u32 tbs_sig_alg_oid_len;
	u32 tbs_sig_alg_oid_params_start; /* params for sig alg */
	u32 tbs_sig_alg_oid_params_len;

	/* Issuer */
	u32 issuer_start;
	u32 issuer_len;

	/* Validity */
	u64 not_before;
	u64 not_after;

	/* Subject */
	u32 subject_start;
	u32 subject_len;
	int empty_subject;

	/* 1 if subject and issuer fields are binary equal */
	int subject_issuer_identical;

	/* SubjectPublicKeyInfo */
	u32 spki_start;
	u32 spki_len;
	u32 spki_alg_oid_start;
	u32 spki_alg_oid_len;
	u32 spki_alg_oid_params_start;
	u32 spki_alg_oid_params_len;
	u32 spki_pub_key_start;
	u32 spki_pub_key_len;
	spki_alg_id spki_alg;
	spki_params spki_alg_params;

	/* Extensions */

	    /* SKI related info, if present */
	    int has_ski;
	    u32 ski_start;
	    u32 ski_len;

	    /* AKI related info, if present */
	    int has_aki;
	    int aki_has_keyIdentifier;
	    u32 aki_keyIdentifier_start;
	    u32 aki_keyIdentifier_len;
	    int aki_has_generalNames_and_serial;
	    u32 aki_generalNames_start;
	    u32 aki_generalNames_len;
	    u32 aki_serial_start;
	    u32 aki_serial_len;

	    /* SAN */
	    int has_san;
	    int san_critical;

	    /* Basic constraints */
	    int bc_critical;
	    int ca_true;
	    int pathLenConstraint_set;

	    /* keyUsage */
	    int has_keyUsage;
	    int keyCertSign_set;
	    int cRLSign_set;

	    /* extendedKeyUsage (EKU) */
	    int has_eku;

	    /* CRLDP */
	    int has_crldp;
	    int one_crldp_has_all_reasons;

	    /* Name Constraints */
	    int has_name_constraints;


	/* signature alg */
	u32 sig_alg_start; /* outer sig alg */
	u32 sig_alg_len;
	sig_alg_id sig_alg; /* ID of signature alg */
	hash_alg_id hash_alg;
	sig_params sig_alg_params; /* depends on sig_alg */

	/* raw signature value */
	u32 sig_start;
	u32 sig_len;
} cert_parsing_ctx;

/*
 * Return 0 if parsing went OK, a non zero value otherwise.
 * 'len' must exactly match the size of the certificate
 * in the buffer 'buf' (i.e. nothing is expected behind).
 */
int parse_x509_cert(cert_parsing_ctx *ctx, const u8 *buf, u32 len);

#endif /* __X509_CERT_PARSER_H__ */


/* ============== HEADER: x509-common.h ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_COMMON_H__
#define __X509_COMMON_H__

/* removed: #include "x509-utils.h" */

typedef enum {
	CLASS_UNIVERSAL        = 0x00,
	CLASS_APPLICATION      = 0x01,
	CLASS_CONTEXT_SPECIFIC = 0x02,
	CLASS_PRIVATE          = 0x03
} tag_class;

typedef enum {
	ASN1_TYPE_BOOLEAN         = 0x01,
	ASN1_TYPE_INTEGER         = 0x02,
	ASN1_TYPE_BIT_STRING      = 0x03,
	ASN1_TYPE_OCTET_STRING    = 0x04,
	ASN1_TYPE_NULL            = 0x05,
	ASN1_TYPE_OID             = 0x06,
	ASN1_TYPE_ENUMERATED      = 0x0a,
	ASN1_TYPE_SEQUENCE        = 0x10,
	ASN1_TYPE_SET             = 0x11,
	ASN1_TYPE_PrintableString = 0x13,
	ASN1_TYPE_T61String       = 0x14,
	ASN1_TYPE_IA5String       = 0x16,
	ASN1_TYPE_UTCTime         = 0x17,
	ASN1_TYPE_GeneralizedTime = 0x18,
} asn1_type;


typedef enum {
	HASH_ALG_UNKNOWN      =  0,
	HASH_ALG_MD2          =  1,
	HASH_ALG_MD4	      =  2,
	HASH_ALG_MD5	      =  3,
	HASH_ALG_MDC2	      =  4,
	HASH_ALG_SHA1	      =  5,
	HASH_ALG_WHIRLPOOL    =  6,
	HASH_ALG_RIPEMD160    =  7,
	HASH_ALG_RIPEMD128    =  8,
	HASH_ALG_RIPEMD256    =  9,
	HASH_ALG_SHA224	      = 10,
	HASH_ALG_SHA256	      = 11,
	HASH_ALG_SHA384	      = 12,
	HASH_ALG_SHA512	      = 13,
	HASH_ALG_SHA512_224   = 14,
	HASH_ALG_SHA512_256   = 15,
	HASH_ALG_SHA3_224     = 16,
	HASH_ALG_SHA3_256     = 17,
	HASH_ALG_SHA3_384     = 18,
	HASH_ALG_SHA3_512     = 19,
	HASH_ALG_SHAKE128     = 20,
	HASH_ALG_SHAKE256     = 21,
	HASH_ALG_SM3          = 22,
	HASH_ALG_GOSTR3411_94 = 23,
	HASH_ALG_STREEBOG256  = 24,
	HASH_ALG_STREEBOG512  = 25,
	HASH_ALG_HBELT        = 26,
	HASH_ALG_BASH256      = 27,
	HASH_ALG_BASH384      = 28,
	HASH_ALG_BASH512      = 29
} hash_alg_id;


/*
 * For a given signature algorithm, optional parameters specific to the
 * algorithm may be given inside the AlgorithmIdentfier structure. Below,
 * we map each signature algorithm OID to a simple identifier and a
 * specific structure we use to export the info we parsed.
 */
typedef enum {
	SIG_ALG_UNKNOWN            =  0,
	SIG_ALG_DSA                =  1,
	SIG_ALG_RSA_SSA_PSS        =  2,
	SIG_ALG_RSA_PKCS1_V1_5     =  3,
	SIG_ALG_ED25519            =  4,
	SIG_ALG_ED448              =  5,
	SIG_ALG_SM2                =  6,
	SIG_ALG_GOSTR3410_2012_256 =  7,
	SIG_ALG_GOSTR3410_2012_512 =  8,
	SIG_ALG_GOSTR3410_2001     =  9,
	SIG_ALG_GOSTR3410_94       = 10,
	SIG_ALG_BIGN               = 11,
	SIG_ALG_ECDSA              = 12,
	SIG_ALG_RSA_9796_2_PAD     = 13,
	SIG_ALG_MONKEYSPHERE       = 14,
	SIG_ALG_BELGIAN_RSA        = 15,
} sig_alg_id;


typedef enum {
	MGF_ALG_UNKNOWN   = 0,
	MGF_ALG_MGF1      = 1  /* default for RSA SSA PSS */
} mgf_alg_id;


typedef enum {
	SPKI_ALG_UNKNOWN            =  0,
	SPKI_ALG_ECPUBKEY           =  1,
	SPKI_ALG_ED25519            =  2,
	SPKI_ALG_ED448              =  3,
	SPKI_ALG_X25519             =  4,
	SPKI_ALG_X448               =  5,
	SPKI_ALG_RSA                =  6,
	SPKI_ALG_DSA                =  7,
	SPKI_ALG_GOSTR3410_2012_256 =  8, /* 1.2.643.7.1.1.1.1 */
	SPKI_ALG_GOSTR3410_2012_512 =  9, /* 1.2.643.7.1.1.1.2 */
	SPKI_ALG_GOSTR3410_2001     = 10,
	SPKI_ALG_GOSTR3410_94       = 11,
	SPKI_ALG_BIGN_PUBKEY        = 12,
} spki_alg_id;


typedef enum {
	CURVE_UNKNOWN					=  0,
	CURVE_BIGN256v1					=  1,
	CURVE_BIGN384v1					=  2,
	CURVE_BIGN512v1					=  3,
	CURVE_C2PNB163V1				=  4,
	CURVE_SECT571K1					=  5,
	CURVE_SECT163K1					=  6,
	CURVE_SECP192K1					=  7,
	CURVE_SECP224K1					=  8,
	CURVE_SECP256K1					=  9,
	CURVE_SECP192R1					= 10,
	CURVE_SECP224R1					= 11,
	CURVE_SECP256R1					= 12,
	CURVE_SECP384R1					= 13,
	CURVE_SECP521R1					= 14,
	CURVE_BRAINPOOLP192R1				= 15,
	CURVE_BRAINPOOLP224R1				= 16,
	CURVE_BRAINPOOLP256R1				= 17,
	CURVE_BRAINPOOLP384R1				= 18,
	CURVE_BRAINPOOLP512R1				= 19,
	CURVE_BRAINPOOLP192T1				= 20,
	CURVE_BRAINPOOLP224T1				= 21,
	CURVE_BRAINPOOLP256T1				= 22,
	CURVE_BRAINPOOLP320R1				= 23,
	CURVE_BRAINPOOLP320T1				= 24,
	CURVE_BRAINPOOLP384T1				= 25,
	CURVE_BRAINPOOLP512T1				= 26,
	CURVE_SM2P256TEST				= 27,
	CURVE_SM2P256V1					= 28,
	CURVE_FRP256V1					= 29,
	CURVE_WEI25519					= 30,
	CURVE_WEI448					= 31,
	CURVE_GOST256					= 32,
	CURVE_GOST512					= 33,
	CURVE_GOST_R3410_2012_256_PARAMSETA		= 34,
	CURVE_GOST_R3410_2001_TESTPARAMSET		= 35,
	CURVE_GOST_R3410_2001_CRYPTOPRO_A_PARAMSET	= 36,
	CURVE_GOST_R3410_2001_CRYPTOPRO_B_PARAMSET	= 37,
	CURVE_GOST_R3410_2001_CRYPTOPRO_C_PARAMSET	= 38,
	CURVE_GOST_R3410_2001_CRYPTOPRO_XCHA_PARAMSET	= 39,
	CURVE_GOST_R3410_2001_CRYPTOPRO_XCHB_PARAMSET	= 40,
	CURVE_GOST_R3410_2012_256_PARAMSETB		= 41,
	CURVE_GOST_R3410_2012_256_PARAMSETC		= 42,
	CURVE_GOST_R3410_2012_256_PARAMSETD		= 43,
	CURVE_GOST_R3410_2012_512_PARAMSETTEST		= 44,
	CURVE_GOST_R3410_2012_512_PARAMSETA		= 45,
	CURVE_GOST_R3410_2012_512_PARAMSETB		= 46,
	CURVE_GOST_R3410_2012_512_PARAMSETC		= 47,
} curve_id;


typedef enum {
	GOST94_PARAMS_UNKNOWN         = 0,
	GOST94_PARAMS_TEST            = 1,
	GOST94_PARAMS_CRYPTOPRO_A     = 2,
	GOST94_PARAMS_CRYPTOPRO_B     = 3,
	GOST94_PARAMS_CRYPTOPRO_C     = 4,
	GOST94_PARAMS_CRYPTOPRO_D     = 5,
	GOST94_PARAMS_CRYPTOPRO_XCHA  = 6,
	GOST94_PARAMS_CRYPTOPRO_XCHB  = 7,
	GOST94_PARAMS_CRYPTOPRO_XCHC  = 8
} _gost94_pub_params_id;


typedef struct {
	const u8 *alg_name;
	const u8 *alg_printable_oid;
	const u8 *alg_der_oid;
	const u32 alg_der_oid_len;
	hash_alg_id hash_id;
} _hash_alg;

typedef struct {
	const u8 *alg_name;
	const u8 *alg_printable_oid;
	const u8 *alg_der_oid;
	const u32 alg_der_oid_len;
	const mgf_alg_id mgf_id;
} _mgf;

typedef struct {
	const _hash_alg *hash;
	const _mgf *mgf;
	const _hash_alg *mgf_hash;
	u32 salt_len;
	u32 trailer_field;
} _rsassa_pss;

typedef struct {
	const u8 *crv_name;
	const u8 *crv_printable_oid;
	const u8 *crv_der_oid;
	const u32 crv_der_oid_len;
	const u32 crv_order_bit_len;
	curve_id crv_id;
} _curve;


/*
 *                           SPKI
 *
 * Now, we need some specific structure to export information
 * extracted from SPKI for the various kind of subject public
 * key we support. We define a structure per type, and put
 * everything in a spki_params union.
 */

/*
 * RFC 5480, RFC 8813. implicitCurve and specifiedCurve MUST NOT be used; we
 * only support namedCurve, i.e. a curve_id is always set in the structure.
 */
typedef struct { /* SPKI_ALG_ECPUBKEY */
	curve_id curve;
	u32 curve_order_bit_len;

	int compression; /* 0x04: none, 0x02/0x03: compression w/ even/odd y */
	u32 ecc_raw_x_off;
	u32 ecc_raw_x_len;
	u32 ecc_raw_y_off; /* meaningful only if compression is 0x04 */
	u32 ecc_raw_y_len; /* meaningful only if compression is 0x04, 0 otherwise */
} spki_ecpubkey_params;

typedef struct { /* SPKI_ALG_ED25519 */
	curve_id curve;
	u32 curve_order_bit_len;

	u32 ed25519_raw_pub_off;
	u32 ed25519_raw_pub_len;
} spki_ed25519_params;

typedef struct { /* SPKI_ALG_ED448 */
	curve_id curve;
	u32 curve_order_bit_len;

	u32 ed448_raw_pub_off;
	u32 ed448_raw_pub_len;
} spki_ed448_params;

typedef struct { /* SPKI_ALG_X25519 */
	curve_id curve;
	u32 curve_order_bit_len;

	u32 x25519_raw_pub_off;
	u32 x25519_raw_pub_len;
} spki_x25519_params;

typedef struct { /* SPKI_ALG_X448 */
	curve_id curve;
	u32 curve_order_bit_len;

	u32 x448_raw_pub_off;
	u32 x448_raw_pub_len;
} spki_x448_params;

typedef struct { /* SPKI_ALG_RSA */
	/*
	 * Set when advertised in params (e.g. EA-RSA optionally has that).
	 * Otherwise, it is left to 0 when not advertised (cannot be 0 when
	 * advertised)
	 */
	u32 rsa_advertised_bit_len;

	u32 rsa_raw_modulus_off; /* modulus */
	u32 rsa_raw_modulus_len;
	u32 rsa_raw_pub_exp_off; /* public exponent */
	u32 rsa_raw_pub_exp_len;
} spki_rsa_params;

typedef struct { /* For SPKI_ALG_DSA */
	u32 dsa_raw_pub_off; /* public key */
	u32 dsa_raw_pub_len;
	u32 dsa_raw_p_off; /* group modulus */
	u32 dsa_raw_p_len;
	u32 dsa_raw_q_off; /* subgroup order */
	u32 dsa_raw_q_len;
	u32 dsa_raw_g_off; /* subgroup generator */
	u32 dsa_raw_g_len;
} spki_dsa_params;

/* RFC 4357, RFC 4491 */
typedef struct { /* SPKI_ALG_GOSTR3410_94 */
	u32 gost94_raw_pub_off;
	u32 gost94_raw_pub_len;
	_gost94_pub_params_id gost94_params_id;
} spki_gost94_params;

/* RFC 4491 */
typedef struct { /* SPKI_ALG_GOSTR3410_2001 */
	curve_id curve; /* publicKeyParamSet  in GOST parlance */
	u32 curve_order_bit_len;

	u32 gost2001_raw_x_pub_off; /* X */
	u32 gost2001_raw_x_pub_len; /* MUST be 32 */
	u32 gost2001_raw_y_pub_off; /* Y */
	u32 gost2001_raw_y_pub_len; /* MUST be 32 */
} spki_gost2001_params;

/* draft-deremin-rfc4491-bis-06 */
typedef struct { /* SPKI_ALG_GOSTR3410_2012_256 */
	curve_id curve; /* publicKeyParamSet in GOST parlance */
	u32 curve_order_bit_len;

	u32 gost2012_256_raw_x_pub_off; /* X */
	u32 gost2012_256_raw_x_pub_len; /* MUST be 32 */
	u32 gost2012_256_raw_y_pub_off; /* Y */
	u32 gost2012_256_raw_y_pub_len; /* MUST be 32 */
} spki_gost2012_256_params;

/* draft-deremin-rfc4491-bis-06 */
typedef struct { /* SPKI_ALG_GOSTR3410_2012_512 */
	curve_id curve; /* publicKeyParamSet  in GOST parlance */
	u32 curve_order_bit_len;

	u32 gost2012_512_raw_x_pub_off; /* X */
	u32 gost2012_512_raw_x_pub_len; /* MUST be 64 */
	u32 gost2012_512_raw_y_pub_off; /* Y */
	u32 gost2012_512_raw_y_pub_len; /* MUST be 64 */
} spki_gost2012_512_params;

/*
 * 7 certs with that SPKI. The only singularity is the
 * optional parameter providing the modulus length
 */
typedef spki_rsa_params spki_ea_rsa_params; /* SPKI_ALG_EA_RSA */

typedef struct { /* SPKI_ALG_BIGN_PUBKEY */
	curve_id curve;
	u32 curve_order_bit_len;

	/*
	 * BIGN pubkey is a bitstring encoding the public point on
	 * 512/8, 768/8, 1024/8 bytes based on the curve. The x
	 * and y coordinates of the public point are concatenated.
	 * For ease of use, we provide offset and length of
	 * each coordinates.
	 */
	u32 bign_raw_x_pub_off;
	u32 bign_raw_x_pub_len;
	u32 bign_raw_y_pub_off;
	u32 bign_raw_y_pub_len;
} spki_bign_params;

typedef struct { /* SPKI_ALG_AVEST */
	u32 avest_raw_pub_off;
	u32 avest_raw_pub_len;
} spki_avest_params;

/* we have only 2 different certs with that spki. */
typedef spki_rsa_params spki_weird_rsa_params; /* SPKI_ALG_WEIRD_RSA */

typedef union {
	spki_ecpubkey_params	 ecpubkey;     /* SPKI_ALG_ECPUBKEY	      */
	spki_ed25519_params	 ed25519;      /* SPKI_ALG_ED25519	      */
	spki_ed448_params	 ed448;	       /* SPKI_ALG_ED448	      */
	spki_x25519_params	 x25519;       /* SPKI_ALG_X25519	      */
	spki_x448_params	 x448;	       /* SPKI_ALG_X448		      */
	spki_rsa_params		 rsa;	       /* SPKI_ALG_RSA		      */
	spki_ea_rsa_params	 ea_rsa;       /* SPKI_ALG_EA_RSA	      */
	spki_dsa_params		 dsa;	       /* SPKI_ALG_DSA		      */
	spki_gost94_params	 gost94;       /* SPKI_ALG_GOSTR3410_94	      */
	spki_gost2001_params	 gost2001;     /* SPKI_ALG_GOSTR3410_2001     */
	spki_gost2012_256_params gost2012_256; /* SPKI_ALG_GOSTR3410_2012_256 */
	spki_gost2012_512_params gost2012_512; /* SPKI_ALG_GOSTR3410_2012_512 */
	spki_bign_params	 bign;	       /* SPKI_ALG_BIGN_PUBKEY	      */
	spki_avest_params	 avest;        /* SPKI_ALG_AVEST	      */
} spki_params;





/*
 *                           SIG
 *
 * Now, we need some specific structure to export information
 * extracted from signature algorithm identifier, associated
 * optional parameters and signature value. We define a structure
 * per sig alg type, and put everything in a sig_params union.
 */

typedef struct { /* SIG_ALG_DSA */
	u32 r_raw_off;
	u32 r_raw_len; /* depends on hash alg output size */
	u32 s_raw_off;
	u32 s_raw_len; /* depends on hash alg output size */
} sig_dsa_params;

typedef struct { /* SIG_ALG_RSA_SSA_PSS */
	u32 sig_raw_off;
	u32 sig_raw_len;

	mgf_alg_id mgf_alg;
	hash_alg_id mgf_hash_alg;
	u8 salt_len;
	u8 trailer_field;
} sig_rsa_ssa_pss_params;

typedef struct { /* SIG_ALG_RSA_PKCS1_V1_5 */
	u32 sig_raw_off;
	u32 sig_raw_len;
} sig_rsa_pkcs1_v1_5_params;

typedef sig_rsa_pkcs1_v1_5_params sig_rsa_9796_2_pad_params; /* SIG_ALG_RSA_9796_2_PAD */
typedef sig_rsa_pkcs1_v1_5_params sig_belgian_rsa_params; /* SIG_ALG_BELGIAN_RSA */

typedef struct { /* SIG_ALG_ED25519 */
	u32 r_raw_off;
	u32 r_raw_len; /* expects 32 */
	u32 s_raw_off;
	u32 s_raw_len; /* expects 32 */
} sig_ed25519_params;

typedef struct { /* SIG_ALG_ED448 */
	u32 r_raw_off;
	u32 r_raw_len; /* expects 57 */
	u32 s_raw_off;
	u32 s_raw_len; /* expects 57 */
} sig_ed448_params;

typedef struct { /* SIG_ALG_SM2 */
	u32 r_raw_off;
	u32 r_raw_len; /* expects 32 */
	u32 s_raw_off;
	u32 s_raw_len; /* expects 32 */
} sig_sm2_params;

typedef struct { /* SIG_ALG_GOSTR3410_2012_256 */
	u32 r_raw_off;
	u32 r_raw_len;  /* expect 32 */
	u32 s_raw_off;
	u32 s_raw_len;  /* expect 32 */
} sig_gost_r3410_2012_256_params;

typedef struct { /* SIG_ALG_GOSTR3410_2012_512 */
	u32 r_raw_off;
	u32 r_raw_len;  /* expect 64 */
	u32 s_raw_off;
	u32 s_raw_len;  /* expect 64 */
} sig_gost_r3410_2012_512_params;

typedef struct { /* SIG_ALG_GOSTR3410_2001 */
	u32 r_raw_off;
	u32 r_raw_len;  /* expect 32 */
	u32 s_raw_off;
	u32 s_raw_len;  /* expect 32 */
} sig_gost_r3410_2001_params;

typedef struct { /* SIG_ALG_GOSTR3410_94 */
	u32 r_raw_off;
	u32 r_raw_len; /* expect 32 */
	u32 s_raw_off;
	u32 s_raw_len; /* expect 32 */
} sig_gost_r3410_94_params;

typedef struct { /* SIG_ALG_BIGN */
	u32 sig_raw_off;
	u32 sig_raw_len; /* depends on curve */
} sig_bign_params;

typedef struct { /* SIG_ALG_ECDSA */
	u32 r_raw_off;
	u32 r_raw_len; /* depends on curve */
	u32 s_raw_off;
	u32 s_raw_len; /* depends on curve */
} sig_ecdsa_params;

typedef struct { /* SIG_ALG_MONKEYSPHERE */
	u32 sig_raw_off;
	u32 sig_raw_len;
} sig_monkeysphere_params;

typedef union {
	sig_dsa_params                 dsa;                 /* SIG_ALG_DSA */
	sig_rsa_ssa_pss_params         rsa_ssa_pss;         /* SIG_ALG_RSA_SSA_PSS */
	sig_rsa_pkcs1_v1_5_params      rsa_pkcs1_v1_5;      /* SIG_ALG_RSA_PKCS1_V1_5 */
	sig_ed25519_params             ed25519;             /* SIG_ALG_ED25519 */
	sig_ed448_params               ed448;               /* SIG_ALG_ED448 */
	sig_sm2_params                 sm2;                 /* SIG_ALG_SM2 */
	sig_gost_r3410_2012_256_params gost_r3410_2012_256; /* SIG_ALG_GOSTR3410_2012_256 */
	sig_gost_r3410_2012_512_params gost_r3410_2012_512; /* SIG_ALG_GOSTR3410_2012_512 */
	sig_gost_r3410_2001_params     gost_r3410_2001;     /* SIG_ALG_GOSTR3410_2001 */
	sig_gost_r3410_94_params       gost_r3410_94;       /* SIG_ALG_GOSTR3410_94 */
	sig_bign_params                bign;                /* SIG_ALG_BIGN */
	sig_ecdsa_params               ecdsa;               /* SIG_ALG_ECDSA */
	sig_rsa_9796_2_pad_params      rsa_9796_2_pad;      /* SIG_ALG_RSA_9796_2_PAD */
	sig_monkeysphere_params        monkeysphere;        /* SIG_ALG_MONKEYSPHERE */
	sig_belgian_rsa_params         belgian_rsa;         /* SIG_ALG_BELGIAN_RSA */
} sig_params;

/* Signature and sig alg parameters parsing functions */
int parse_sig_ed448(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_ed25519(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_ecdsa(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_sm2(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_dsa(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_rsa_pkcs1_v15(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_rsa_ssa_pss(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_rsa_9796_2_pad(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_rsa_belgian(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_gost94(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_gost2001(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_gost2012_512(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_gost2012_256(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_bign(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
int parse_sig_monkey(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);

int parse_algoid_sig_params_ecdsa_with(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_ecdsa_with_specified(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_sm2(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_eddsa(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_params_rsa(const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_rsa(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_rsassa_pss(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_sig_params_none(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 ATTRIBUTE_UNUSED len);
int parse_algoid_sig_params_bign_with_hspec(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
int parse_algoid_params_none(const u8 *cert, u32 off, u32 len);


typedef struct {
	const u8 *alg_name;
	const u8 *alg_printable_oid;
	const u8 *alg_der_oid;
	const u32 alg_der_oid_len;

	sig_alg_id sig_id;
	hash_alg_id hash_id;

	int (*parse_algoid_sig_params)(sig_params *params, hash_alg_id *hash_alg, const u8 *cert, u32 off, u32 len);
	int (*parse_sig)(sig_params *params, const u8 *cert, u32 off, u32 len, u32 *eaten);
} _sig_alg;

extern const _sig_alg *known_sig_algs[];
extern const u16 num_known_sig_algs;

extern const _curve *known_curves[];

const _sig_alg * find_sig_alg_by_oid(const u8 *buf, u32 len);
const _hash_alg * find_hash_by_oid(const u8 *buf, u32 len);
const _curve * find_curve_by_oid(const u8 *buf, u32 len);


int get_length(const u8 *buf, u32 len,
	       u32 *adv_len, u32 *eaten);

int parse_id_len(const u8 *buf, u32 len,
		 tag_class exp_class, u32 exp_type,
		 u32 *parsed, u32 *content_len);

int parse_explicit_id_len(const u8 *buf, u32 len,
			  u32 exp_ext_type,
			  tag_class exp_int_class, u32 exp_int_type,
			  u32 *parsed, u32 *data_len);

int parse_null(const u8 *buf, u32 len,
	       u32 *parsed);

int parse_OID(const u8 *buf, u32 len,
	      u32 *parsed);

int parse_integer(const u8 *buf, u32 len,
		  tag_class exp_class, u32 exp_type,
		  u32 *hdr_len, u32 *data_len);

int parse_non_negative_integer(const u8 *buf, u32 len,
			       tag_class exp_class, u32 exp_type,
			       u32 *hdr_len, u32 *data_len);

int parse_boolean(const u8 *buf, u32 len, u32 *eaten);




int parse_generalizedTime(const u8 *buf, u32 len, u32 *eaten,
			  u16 *year, u8 *month, u8 *day,
			  u8 *hour, u8 *min, u8 *sec);

#define NAME_TYPE_rfc822Name     0x81
#define NAME_TYPE_dNSName        0x82
#define NAME_TYPE_URI            0x86
#define NAME_TYPE_iPAddress      0x87
#define NAME_TYPE_registeredID   0x88
#define NAME_TYPE_otherName      0xa0
#define NAME_TYPE_x400Address    0xa3
#define NAME_TYPE_directoryName  0xa4
#define NAME_TYPE_ediPartyName   0xa5

int parse_GeneralName(const u8 *buf, u32 len, u32 *eaten, int *empty);

int parse_SerialNumber(const u8 *cert, u32 off, u32 len,
		       tag_class exp_class, u32 exp_type,
		       u32 *eaten);

int verify_correct_time_use(u8 time_type, u16 yyyy);

int parse_Time(const u8 *buf, u32 len, u8 *t_type, u32 *eaten,
	       u16 *year, u8 *month, u8 *day,
	       u8 *hour, u8 *min, u8 *sec);

int verify_correct_time_use(u8 time_type, u16 yyyy);

int parse_AKICertSerialNumber(const u8 *cert, u32 off, u32 len,
			      tag_class exp_class, u32 exp_type,
			      u32 *eaten);

int parse_crldp_reasons(const u8 *buf, u32 len, u32 exp_type, u32 *eaten);

int parse_DistributionPoint(const u8 *buf, u32 len,
			    int *crldp_has_all_reasons, u32 *eaten);

int parse_AIA(const u8 *cert, u32 off, u32 len, int critical);

int parse_ia5_string(const u8 *buf, u32 len, u32 lb, u32 ub);

int parse_x509_Name(const u8 *buf, u32 len, u32 *eaten, int *empty);

int parse_DisplayText(const u8 *buf, u32 len, u32 *eaten);

int parse_nine_bit_named_bit_list(const u8 *buf, u32 len, u16 *val);

int parse_GeneralName(const u8 *buf, u32 len, u32 *eaten, int *empty);

int parse_GeneralNames(const u8 *buf, u32 len, tag_class exp_class,
		       u32 exp_type, u32 *eaten);

u64 time_components_to_comparable_u64(u16 na_year, u8 na_month, u8 na_day,
				      u8 na_hour, u8 na_min, u8 na_sec);

#endif /* __X509_COMMON_H__ */


/* ============== HEADER: x509-config.h ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_CONFIG_H__
#define __X509_CONFIG_H__


/*
 * Max allowed buffer size for ASN.1 structures. Also note that
 * the type used for length in the whole code is an u32, so it
 * is pointless to set something higher than 2^32 - 1
 */
#define MAX_UINT32 (0xffffffffUL)
#define ASN1_MAX_BUFFER_SIZE (MAX_UINT32)

/*
 * The following can be defined to enable an error trace to be
 * printed on standard output. The error path is made of the
 * lines in the representing the call graph leading to the
 * error.
 */
// #define ERROR_TRACE_ENABLE

/*
 * FIXME: document error values
 */

typedef enum {
	X509_PARSER_ERROR_VERSION_ABSENT            = -1,
	X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH = -2,
	X509_PARSER_ERROR_VERSION_NOT_3             = -3,
} x509_parser_errors;


/* Knob to skip over currently unknown RDN elements */
#define TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_RDN_OIDS

/*
 * Each certificate extension is made of an OID and an associated data value
 * for which we need a specific parsing function to validate the structure
 * of the data. This means that by default a certificate will be rejected if
 * an extensions is unknown. We define two macros to allow parsing to continue
 * when encoutering unsupported extensions (for which we do not have a specific
 * parsing function for data value)
 *
 * The first (TEMPORARY_LAXIST_HANDLE_COMMON_UNSUPPORTED_EXT_OIDS) handles
 * common extensions found in certificates which we know of but currently
 * have no parsing functions. Those extensions OID are explicitly referenced
 * in known_ext_oids table. When the knob is defined, the extensions data is
 * skipped to continue parsing, i.e. the structure of the data it carries is
 * NOT VERIFIED AT ALL. The check that the extension only appear once in the
 * certificate is performed.
 *
 * The second (TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_EXT_OIDS) is used as a
 * catch-all for extensions that are not known. When the knob is defined:
 *
 *  - unknown extensions data structure is NOT VERIFIED AT ALL
 *  - NO CHECK is performed to verify that the extension appears only once
 *    in the certificate.
 */
#define TEMPORARY_LAXIST_HANDLE_COMMON_UNSUPPORTED_EXT_OIDS
#define TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_EXT_OIDS

/*
 * Double the defined upper upper bound value of on common RDN components
 * (CN, O and OU) length from 64 to 128.
 */
#define TEMPORARY_LAXIST_RDN_UPPER_BOUND

/* Allow CA certificates w/o SKI. */
#define TEMPORARY_LAXIST_CA_WO_SKI

/* Allow emailAddress using UTF-8 encoding instead for IA5String. */
#define TEMPORARY_LAXIST_EMAILADDRESS_WITH_UTF8_ENCODING

/*
 * Same for otherwise unsupported extensions but for which we have an
 * internal reference to the OID
 */
#define TEMPORARY_BAD_EXT_OIDS

/*
 * Same for otherwise unsupported RDN but for which we have an internal
 * reference to the OID
 */
#define TEMPORARY_BAD_OID_RDN

/* Allow certificates w/ full directoryString . */
#define TEMPORARY_LAXIST_DIRECTORY_STRING

/*
 * Allow negative serial value
 */
#define TEMPORARY_LAXIST_SERIAL_NEGATIVE

/*
 * Allow large serial value. Limit is 20 bytes but some implementation
 * use larger serial.
 */
#define TEMPORARY_LAXIST_SERIAL_LENGTH

/*
 * Serial value is not expected to be 0. This knob make such certificate
 * valid.
 */
#define TEMPORARY_LAXIST_SERIAL_NULL

/*
 * Allow certificates w/ full basic constraints boolean explicitly set to false.
 * As this is the DEFAULT value, DER forbids encoding of that value.
 */
#define TEMPORARY_LAXIST_CA_BASIC_CONSTRAINTS_BOOLEAN_EXPLICIT_FALSE

/*
 * Allow certificates w/ extension's critical flag explicitly set to false.
 * As this is the DEFAULT value, DER forbids encoding of that value. The
 * knob also applies to CRL extensions.
 */
#define TEMPORARY_LAXIST_EXTENSION_CRITICAL_FLAG_BOOLEAN_EXPLICIT_FALSE

/*
 * Allow certificates w/ SKI extension critical flag set. Section 4.2.1.1. of
 * RFC 5280 forbids that with a MUST.
 */
#define TEMPORARY_LAXIST_SKI_CRITICAL_FLAG_SET

/*
 * Allow serial DN component encoded as an IA5String whereas RFC 5280
 * requires such element to be encoded using printable string.
 */
#define TEMPORARY_LAXIST_SERIAL_RDN_AS_IA5STRING

/*
 * Do not kick certificates with empty RSA pubkey algoid params or empty sig
 * algoid parmas instead of the expected NULL.
 */
#define TEMPORARY_LAXIST_RSA_PUBKEY_AND_SIG_NO_PARAMS_INSTEAD_OF_NULL

/*
 * nextUpdate field in CRL is optional in ASN.1 definition but RFC5280
 * explicitly states both at end of section 5. introduction and in section
 * 5.1.2 that conforming CRL issuers are required / MUST include
 * nextUpdate field. When following knob is defined, invalid CRL with missing
 * nextUpdate field are allowed.
 */
#define TEMPORARY_LAXIST_ALLOW_MISSING_CRL_NEXT_UPDATE

/*
 * CRL v2 may include extensions. The field is either absent or present,
 * in which case "this field is a sequence of one or more CRL extensions."
 * i.e. it is not valid to have the field with no extension in it. Some
 * CRL nonetheless exhibit the field with an empty content. Defining
 * this knob accepts those CRL.
 */
#define TEMPORARY_LAXIST_ALLOW_CRL_ENTRY_EXT_WITH_EMPTY_SEQ

/*
 * CRL Issuing Distribution Point (IDP) extension is a critical one.
 * Nonetheless, some CRL issuers did not get the memo and do not
 * assert the bit in the CRL they emit. Such invalid CRL can be let
 * pass through by defining the following knob.
 */
#define TEMPORARY_LAXIST_ALLOW_IDP_CRL_EXT_WITHOUT_CRITICAL_BIT_SET

/*
 * RFC 5280 section 5.1.2.6 has "When there are no revoked certificates,
 * the revoked certificates list MUST be absent". Some CRL wrongfully
 * include an empty list in that case. Defining this knob let those
 * CRL pass through.
 */
#define TEMPORARY_LAXIST_ALLOW_REVOKED_CERTS_LIST_EMPTY

/*
 * RFC 5280 has "When CRLs are issued, the CRLs MUST be version 2 CRLs,
 * include the date by which the next CRL will be issued in the
 * nextUpdate field (Section 5.1.2.5), include the CRL number extension
 * (Section 5.2.3), and include the authority key identifier extension
 * (Section 5.2.1)." If the CRL misses the AKI and CRL number extensions
 * then it is invalid ;-)
 */
#define TEMPORARY_LAXIST_ALLOW_MISSING_AKI_OR_CRLNUM

#endif /* __X509_CONFIG_H__ */


/* ============== HEADER: x509-crl-parser.h ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_CRL_PARSER_H__
#define __X509_CRL_PARSER_H__

/* removed: #include "x509-config.h" */
/* removed: #include "x509-utils.h" */
/* removed: #include "x509-common.h" */

typedef struct {
	/* tbcCertificate */
	u32 tbs_start;
	u32 tbs_len;

	/* Version */
	u8 version;

	/* inner sig alg (tbsCrl.signature) */
	u32 tbs_sig_alg_start;
	u32 tbs_sig_alg_len;
	u32 tbs_sig_alg_oid_start; /* OID for sig alg */
	u32 tbs_sig_alg_oid_len;
	u32 tbs_sig_alg_oid_params_start; /* params for sig alg */
	u32 tbs_sig_alg_oid_params_len;

	/* Issuer */
	u32 issuer_start;
	u32 issuer_len;

	/* {this,next}Update */
	u64 this_update;
	u64 next_update;

	/* Extensions */

	    /* AKI related info, if present */
	    int has_aki;
	    int aki_has_keyIdentifier;
	    u32 aki_keyIdentifier_start;
	    u32 aki_keyIdentifier_len;
	    int aki_has_generalNames_and_serial;
	    u32 aki_generalNames_start;
	    u32 aki_generalNames_len;
	    u32 aki_serial_start;
	    u32 aki_serial_len;

	    /* Freshest CRL (Delta CRL Distribution Point) */
	    int has_crldp;
	    int one_crldp_has_all_reasons;

	    /* Freshest CRL (Delta CRL Distribution Point) */
	    int has_crlnumber;
	    u32 crlnumber_start;
	    u32 crlnumber_len;

	    /* Flags which tells if CRL has revoked certs */
	    int has_revoked_certs;

	/* signature alg */
	u32 sig_alg_start; /* outer sig alg */
	u32 sig_alg_len;
	sig_alg_id sig_alg; /* ID of signature alg */
	hash_alg_id hash_alg;
	sig_params sig_alg_params; /* depends on sig_alg */

	/* raw signature value */
	u32 sig_start;
	u32 sig_len;
} crl_parsing_ctx;


/*
 * Return 0 if parsing went OK, a non zero value otherwise.
 * 'len' must exactly match the size of the CRL in the buffer 'buf'
 * (i.e. nothing is expected behind).
 */
int parse_x509_crl(crl_parsing_ctx *ctx, const u8 *buf, u32 len);

#endif /* __X509_CRL_PARSER_H__ */


/* ============== HEADER: x509-parser.h ============== */
/*
 *  Copyright (C) 2019 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_PARSER_H__
#define __X509_PARSER_H__

/* removed: #include "x509-cert-parser.h" */
/* removed: #include "x509-crl-parser.h" */

/*
 * This wrappers around parse_x509_cert() and parse_x509_crl() do not expect the
 * buffer to exactly contain a DER-encoded certificate, but to start with one.
 * It returns the length of the first sequence found in the buffer, no matter
 * if the certificate or CRL (this sequence) is valid or not. It only requires
 * the buffer to start with a sequence.
 *
 * Behavior based on return value:
 *
 *   1 : - buffer does not start with a valid sequence, so 'eaten' has not
 *         been updated with useful info
 *       - parsing has not been performed
 *       - ctx has not been updated
 *
 *   0 : - 'eaten' contains the length of parsed
 *       - Cert/CRL parsing went OK
 *       - ctx is usable and contains parsing info
 *
 * < 0 : - 'eaten' contains the length of parsed
 *       - Cert/CRL parsing went wrong and return value contains parsing
 *         error value
 *       - ctx is usable and contains parsing info
 *
 */
int parse_x509_cert_relaxed(cert_parsing_ctx *ctx, const u8 *buf, u32 len, u32 *eaten);
int parse_x509_crl_relaxed(crl_parsing_ctx *ctx, const u8 *buf, u32 len, u32 *eaten);

#endif /* __X509_PARSER_H__ */


/* ============== HEADER: x509-utils.h ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */
#ifndef __X509_UTILS_H__
#define __X509_UTILS_H__

#include <stdint.h>
#include <unistd.h>
#include <string.h>
/* removed: #include "x509-config.h" */

typedef uint8_t	  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#if defined(__FRAMAC__)
#define ATTRIBUTE_UNUSED
#else
#define ATTRIBUTE_UNUSED __attribute__((unused))
#endif

#ifdef ERROR_TRACE_ENABLE
#define ERROR_TRACE_APPEND(x) do {			    \
	       extern int printf(const char *format, ...);  \
	       printf("%06d ", (x));			    \
	} while (0);
#else
#define ERROR_TRACE_APPEND(x)
#endif

/*
 * Historically, we used -__LINE__ as return value. This worked well when
 * the parser was a single file. Now that we have multiple files in the
 * project, we encode a unique numerical identifier for each file in the
 * return value. For that to work, we need each *implementation* file
 * to define a unique value for X509_FILE_NUM at its beginning.
 */
#define X509_FILE_LINE_NUM_ERR ((X509_FILE_NUM * 100000) + __LINE__)

/*
 * We need to pass some array as macro argument. Protection is needed in that
 * case.
 */
#define P99_PROTECT(...) __VA_ARGS__

int bufs_differ(const u8 *b1, const u8 *b2, u32 n);


#endif /* __X509_UTILS_H__ */


/* ============== IMPL: main.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
/* removed: #include "x509-parser.h" */

#define X509_FILE_NUM 1 /* See x509-utils.h for rationale */

static void usage(char *argv0)
{
	printf("Usage: %s file.der\n", argv0);
}

int main(int argc, char *argv[])
{
	char *path = argv[1];
	cert_parsing_ctx cert_ctx;
	crl_parsing_ctx crl_ctx;
	struct stat st;
	off_t fsize, offset, remain;
	u32 eaten, to_be_parsed;
	u8 *buf, *ptr;;
	int ret;
	int fd;
	int num_x509_files;
	int num_certs_v3_ok;
	int num_crl_ok;

	if (argc != 2) {
		usage(argv[0]);
		ret = -1;
		goto out;
	}

	fd = open(path, O_RDONLY);
	if (fd == -1) {
		printf("Unable to open input file %s\n", path);
		ret = -1;
		goto out;
	}

	/*
	 * Get size of the file. Note: fstat and mmap uses off_t and size_t
	 * types which can be 32-bits (additionally, off_t is a signed int).
	 * As specified in man pages, we use -D_FILE_OFFSET_BITS=64 to have
	 * those on 64 bits.
	 */
	ret = fstat(fd, &st);
	if (ret) {
		printf("fstat() on file %s falied with err %d\n", path, ret);
		ret = -1;
		goto out;
	}
	fsize = st.st_size;

	/* mmap the file */
	ptr = mmap(0, fsize, PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		printf("mmap() failed with errno=%d\n", errno);
		ret = -1;
		goto out;
	}

	/* Initialize stat values */
	num_x509_files = 0;
	num_certs_v3_ok = 0;
	num_crl_ok = 0;

	remain = fsize;
	buf = ptr;
	offset = 0;
	while (remain) {
		/*
		 * File may be larger than 4GB but parser expects u32
		 * and has limits on size for individual elements to be
		 * parsed.
		 */
		to_be_parsed = ASN1_MAX_BUFFER_SIZE;
		if (to_be_parsed > remain) {
			to_be_parsed = remain;
		}
		eaten = 0;

		/* First try and parse buffer as a certificate */

		memset(&cert_ctx, 0, sizeof(cert_ctx));
		ret = parse_x509_cert_relaxed(&cert_ctx, buf, to_be_parsed, &eaten);
#ifdef ERROR_TRACE_ENABLE
		printf("- CERT %06d off %llu eaten %lu file %s\n", -ret, offset, eaten, path);
#endif

		switch (ret) {
		case 0:
			// printf("offset %llu %d\n", offset, eaten);
			num_x509_files += 1;
			num_certs_v3_ok += 1;
			break;

		case 1:
			printf("Invalid sequence element #%d at offset %ld\n",
				num_x509_files, offset);
			ret = -1;
			goto out;
			break;

		case X509_PARSER_ERROR_VERSION_NOT_3:
		case X509_PARSER_ERROR_VERSION_ABSENT:
		case X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH:
			num_x509_files += 1;
			break;

		default:
			num_x509_files += 1;
			break;
		}

		/*
		 * if this is not even a valid sequence or the certificate was
		 * considered valid, we do not need to bother parsing it as
		 * a CRL.
		 */
		if ((ret != 1) && (ret != 0)) {
			memset(&crl_ctx, 0, sizeof(crl_ctx));
			ret = parse_x509_crl_relaxed(&crl_ctx, buf, to_be_parsed, &eaten);
#ifdef ERROR_TRACE_ENABLE
			printf("- CRL %06d off %llu eaten %lu file %s\n", -ret, offset, eaten, path);
#endif

			switch (ret) {
			case 0:
				// printf("offset %llu %d\n", offset, eaten);
				num_crl_ok += 1;
				break;

			default:
				break;
			}

		}

		offset += eaten;
		buf += eaten;
		remain -= eaten;
	}

	ret = munmap(ptr, fsize);
	close(fd);

	printf("Valid X.509v3 Certs: %d/%d (%.2f%%)\n",
		num_certs_v3_ok, num_x509_files, ((float)(100*num_certs_v3_ok) / ((float)num_x509_files)));
	printf("Valid X.509 CRL    : %d/%d (%.2f%%)\n",
		num_crl_ok, num_x509_files, ((float)(100*num_crl_ok) / ((float)num_x509_files)));

out:
	return ret;
}


/* ============== IMPL: x509-cert-parser.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

/* removed: #include "x509-cert-parser.h" */

#define X509_FILE_NUM 2 /* See x509-utils.h for rationale */

/* subject public key and spki params parsing functions */
static int parse_pubkey_ed448(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_x448(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_ed25519(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_x25519(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_ec(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_rsa(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_gostr3410_94(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_gostr3410_2001(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_gostr3410_2012_256(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_gostr3410_2012_512(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_dsa(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_pubkey_bign(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_ecPublicKey(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_ed25519(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_ed448(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_x25519(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_x448(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_rsa(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_gost_r3410_2012_256(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_gost_r3410_2012_512(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_gost_r3410_2001(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_gost_r3410_94(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_dsa(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 len);
static int parse_algoid_pubkey_params_ea_rsa(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 ATTRIBUTE_UNUSED len);
static int parse_algoid_pubkey_params_none(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 ATTRIBUTE_UNUSED len);
static int parse_algoid_pubkey_params_bign(cert_parsing_ctx *ctx, const u8 *cert, u32 off, u32 ATTRIBUTE_UNUSED len);

/********************************************************************
 * Subject public key algs
 ********************************************************************/

typedef struct {
	const u8 *alg_name;
	const u8 *alg_printable_oid;
	const u8 *alg_der_oid;
	const u32 alg_der_oid_len;

	spki_alg_id pubkey_id;

	int (*parse_algoid_pubkey_params)(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx, const u8 *cert, u32 off, u32 len);
	int (*parse_pubkey)(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx, const u8 *cert, u32 off, u32 len);
} _pubkey_alg;

#define DECL_PUBKEY_ALG(TTalg, XXtype, YYparse_pubkey, ZZparse_algoid, UUname, VVoid, WWoidbuf) \
static const u8 _##TTalg##_pubkey_name[] = UUname;            \
static const u8 _##TTalg##_pubkey_printable_oid[] = VVoid;    \
static const u8 _##TTalg##_pubkey_der_oid[] = WWoidbuf;       \
							      \
static const _pubkey_alg _##TTalg##_pubkey_alg = {            \
	.alg_name = _##TTalg##_pubkey_name,                   \
	.alg_printable_oid = _##TTalg##_pubkey_printable_oid, \
	.alg_der_oid = _##TTalg##_pubkey_der_oid,             \
	.alg_der_oid_len = sizeof(_##TTalg##_pubkey_der_oid), \
	.pubkey_id = (XXtype),				      \
	.parse_pubkey = (YYparse_pubkey),		      \
	.parse_algoid_pubkey_params = (ZZparse_algoid),	      \
}

/*
 *------------------------------------------+----------------------------+--------------------------------+-----------------------------------------------+----------------------------------+------------------------------+----------------------------------...
 *              struct name                 | pubkey alg                 | pubkey parsing func            | pubkey alg oid params parsing func            | pretty name for pubkey alg       | printable OID                | OID in DER format
 *------------------------------------------+----------------------------+--------------------------------+-----------------------------------------------+----------------------------------+------------------------------+----------------------------------...
 */
DECL_PUBKEY_ALG(pkcs1_rsaEncryption         , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_rsa                , "PKCS-1 rsaEncryption"           , "1.2.840.113549.1.1.1"       , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01 }));
DECL_PUBKEY_ALG(weird_rsa_pub_1             , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_rsa                , "Undocumented RSA pub key oid"   , "1.2.840.887.13.1.1.1"       , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0x77, 0x0d, 0x01, 0x01, 0x01 }));
DECL_PUBKEY_ALG(weird_rsa_pub_2             , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_rsa                , "another rsa pubkey oid"         , "1.18.840.113549.1.1.1"      , P99_PROTECT({ 0x06, 0x09, 0x3a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01 })); /*CRAP*/
DECL_PUBKEY_ALG(rsa_gip_cps                 , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_rsa                , "GIP-CPS"                        , "1.2.250.1.71.2.6.1"         , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x81, 0x7a, 0x01, 0x47, 0x02, 0x06, 0x01 }));
DECL_PUBKEY_ALG(rsassa_pss_shake256         , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_none               , "RSASSA-PSS-SHAKE256"            , "1.3.6.1.5.5.7.6.31"         , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x1f }));
DECL_PUBKEY_ALG(rsassa_pss_shake128         , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_none               , "RSASSA-PSS-SHAKE128"            , "1.3.6.1.5.5.7.6.30"         , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x1e }));
DECL_PUBKEY_ALG(ea_rsa                      , SPKI_ALG_RSA               , parse_pubkey_rsa               , parse_algoid_pubkey_params_ea_rsa             , "id-ea-rsa"                      , "2.5.8.1.1"                  , P99_PROTECT({ 0x06, 0x04, 0x55, 0x08, 0x01, 0x01 }));
DECL_PUBKEY_ALG(ecpublickey                 , SPKI_ALG_ECPUBKEY          , parse_pubkey_ec                , parse_algoid_pubkey_params_ecPublicKey        , "ecPublicKey"                    , "1.2.840.10045.2.1"          , P99_PROTECT({ 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d,  0x02, 0x01 }));
DECL_PUBKEY_ALG(dsa_pubkey                  , SPKI_ALG_DSA               , parse_pubkey_dsa               , parse_algoid_pubkey_params_dsa                , "DSA subject public key"         , "1.2.840.10040.4.1"          , P99_PROTECT({ 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x38, 0x04, 0x01 }));
DECL_PUBKEY_ALG(x448                        , SPKI_ALG_X448              , parse_pubkey_x448              , parse_algoid_pubkey_params_x448               , "X448"                           , "1.3.101.111"                , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x6f }));
DECL_PUBKEY_ALG(ed448                       , SPKI_ALG_ED448             , parse_pubkey_ed448             , parse_algoid_pubkey_params_ed448              , "Ed448"                          , "1.3.101.113"                , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x71 }));
DECL_PUBKEY_ALG(x25519                      , SPKI_ALG_X25519            , parse_pubkey_x25519            , parse_algoid_pubkey_params_x25519             , "X25519"                         , "1.3.101.110"                , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x6e }));
DECL_PUBKEY_ALG(ed25519                     , SPKI_ALG_ED25519           , parse_pubkey_ed25519           , parse_algoid_pubkey_params_ed25519            , "Ed25519"                        , "1.3.101.112"                , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x70 }));
DECL_PUBKEY_ALG(bign                        , SPKI_ALG_BIGN_PUBKEY       , parse_pubkey_bign              , parse_algoid_pubkey_params_bign               , "bign-pubkey"                    , "1.2.112.0.2.0.34.101.45.2.1", P99_PROTECT({ 0x06, 0x0a, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x02, 0x01 }));
DECL_PUBKEY_ALG(gost_R3410_94               , SPKI_ALG_GOSTR3410_94      , parse_pubkey_gostr3410_94      , parse_algoid_pubkey_params_gost_r3410_94      , "gostR3410-94 public key"        , "1.2.643.2.2.20"             , P99_PROTECT({ 0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x14 }));
DECL_PUBKEY_ALG(gost_R3410_2001             , SPKI_ALG_GOSTR3410_2001    , parse_pubkey_gostr3410_2001    , parse_algoid_pubkey_params_gost_r3410_2001    , "gostR3410-2001 public key"      , "1.2.643.2.2.19"             , P99_PROTECT({ 0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x13 }));
DECL_PUBKEY_ALG(gost_R3410_2012_512         , SPKI_ALG_GOSTR3410_2012_512, parse_pubkey_gostr3410_2012_512, parse_algoid_pubkey_params_gost_r3410_2012_512, "gost3410-2012-512 public key"   , "1.2.643.7.1.1.1.2"          , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x01, 0x02 }));
DECL_PUBKEY_ALG(gost_R3410_2012_256         , SPKI_ALG_GOSTR3410_2012_256, parse_pubkey_gostr3410_2012_256, parse_algoid_pubkey_params_gost_r3410_2012_256, "gost3410-2012-256 public key"   , "1.2.643.7.1.1.1.1"          , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x01, 0x01 }));

static const _pubkey_alg ATTRIBUTE_UNUSED *known_pubkey_algs[] = {
	&_ecpublickey_pubkey_alg,

	&_pkcs1_rsaEncryption_pubkey_alg,
	&_rsa_gip_cps_pubkey_alg,
	&_rsassa_pss_shake256_pubkey_alg,
	&_rsassa_pss_shake128_pubkey_alg,

	&_ed448_pubkey_alg,
	&_ed25519_pubkey_alg,

	&_x448_pubkey_alg,
	&_x25519_pubkey_alg,

	&_gost_R3410_94_pubkey_alg,
	&_gost_R3410_2001_pubkey_alg,
	&_gost_R3410_2012_512_pubkey_alg,
	&_gost_R3410_2012_256_pubkey_alg,

	&_bign_pubkey_alg,

	&_weird_rsa_pub_1_pubkey_alg,
	&_weird_rsa_pub_2_pubkey_alg,
	&_dsa_pubkey_pubkey_alg,
	&_ea_rsa_pubkey_alg,
};

const u16 num_known_pubkey_algs = (sizeof(known_pubkey_algs) / sizeof(known_pubkey_algs[0]));




/********************************************************************
 * Gost 94 pubkey parameters
 ********************************************************************/

typedef struct {
	const u8 *params_name;
	const u8 *params_printable_oid;
	const u8 *params_der_oid;
	const u32 params_der_oid_len;

	_gost94_pub_params_id params_id;
} _gost94_pub_params;

#define DECL_GOST94_PARAMS(EEparams, AAid, BBname, CCoid, DDoidbuf)	    \
static const u8 _##EEparams##_gost_94_params_name[] = BBname;               \
static const u8 _##EEparams##_gost_94_params_printable_oid[] = CCoid;       \
static const u8 _##EEparams##_gost_94_params_der_oid[] = DDoidbuf;          \
									    \
static const _gost94_pub_params _##EEparams##_ParamSet = {                  \
	.params_name = _##EEparams##_gost_94_params_name,                   \
	.params_printable_oid = _##EEparams##_gost_94_params_printable_oid, \
	.params_der_oid = _##EEparams##_gost_94_params_der_oid,             \
	.params_der_oid_len = sizeof(_##EEparams##_gost_94_params_der_oid), \
	.params_id = (AAid),						    \
}

DECL_GOST94_PARAMS(GostR3410_94_Test,           GOST94_PARAMS_TEST	    , "GostR3410_94_TestParamSet",            "1.2.643.2.2.32.0", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x20, 0x00 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_A,    GOST94_PARAMS_CRYPTOPRO_A   , "GostR3410_94_CryptoPro_A_ParamSet",    "1.2.643.2.2.32.2", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x20, 0x02 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_B,    GOST94_PARAMS_CRYPTOPRO_B   , "GostR3410_94_CryptoPro_B_ParamSet",    "1.2.643.2.2.32.3", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x20, 0x03 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_C,    GOST94_PARAMS_CRYPTOPRO_C   , "GostR3410_94_CryptoPro_C_ParamSet",    "1.2.643.2.2.32.4", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x20, 0x04 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_D,    GOST94_PARAMS_CRYPTOPRO_D   , "GostR3410_94_CryptoPro_D_ParamSet",    "1.2.643.2.2.32.5", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x20, 0x05 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_XchA, GOST94_PARAMS_CRYPTOPRO_XCHA, "GostR3410_94_CryptoPro_XchA_ParamSet", "1.2.643.2.2.33.1", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x21, 0x01 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_XchB, GOST94_PARAMS_CRYPTOPRO_XCHB, "GostR3410_94_CryptoPro_XchB_ParamSet", "1.2.643.2.2.33.2", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x21, 0x02 }));
DECL_GOST94_PARAMS(GostR3410_94_CryptoPro_XchC, GOST94_PARAMS_CRYPTOPRO_XCHC, "GostR3410_94_CryptoPro_XchC_ParamSet", "1.2.643.2.2.33.3", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x21, 0x03 }));

static const _gost94_pub_params ATTRIBUTE_UNUSED *known_gost_94_params[] = {
	&_GostR3410_94_Test_ParamSet,
	&_GostR3410_94_CryptoPro_A_ParamSet,
	&_GostR3410_94_CryptoPro_B_ParamSet,
	&_GostR3410_94_CryptoPro_C_ParamSet,
	&_GostR3410_94_CryptoPro_D_ParamSet,
	&_GostR3410_94_CryptoPro_XchA_ParamSet,
	&_GostR3410_94_CryptoPro_XchB_ParamSet,
	&_GostR3410_94_CryptoPro_XchC_ParamSet,
};

const u16 num_known_gost94_params = (sizeof(known_gost_94_params) / sizeof(known_gost_94_params[0]));



/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \forall integer k; 0 <= k < num_known_gost94_params ==> \valid_read(known_gost_94_params[k]);
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < num_known_gost94_params && \result == known_gost_94_params[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
const _gost94_pub_params * find_gost94_params_by_oid(const u8 *buf, u32 len)
{
	const _gost94_pub_params *found = NULL;
	const _gost94_pub_params *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@
	  @ loop invariant 0 <= k <= num_known_gost94_params;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (num_known_gost94_params - k);
	  @*/
	for (k = 0; k < num_known_gost94_params; k++) {
		int ret;

		cur = known_gost_94_params[k];

		/*@ assert cur == known_gost_94_params[k];*/
		if (cur->params_der_oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->params_der_oid, buf, cur->params_der_oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}


/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \forall integer k; 0 <= k < num_known_pubkey_algs ==> \valid_read(&(known_pubkey_algs[k]->alg_der_oid_len));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < num_known_pubkey_algs && \result == known_pubkey_algs[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
const _pubkey_alg * find_pubkey_alg_by_oid(const u8 *buf, u32 len)
{
	const _pubkey_alg *found = NULL;
	const _pubkey_alg *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll num_known_pubkey_algs;
	  @ loop invariant 0 <= k <= num_known_pubkey_algs;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (num_known_pubkey_algs - k);
	  @*/
	for (k = 0; k < num_known_pubkey_algs; k++) {
		int ret;

		cur = known_pubkey_algs[k];

		/*@ assert cur == known_pubkey_algs[k]; */
		if (cur->alg_der_oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->alg_der_oid, buf, cur->alg_der_oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*
 * The implementation is based on 4.1 and 4.1.2.1 of RFC5280 + Section
 * 8.3 of X.690. The version field is mandatory and it is encoded as
 * an integer. As we only limit ourselves to version 3 certificates
 * (i.e. a value of 0x02 for the integer encoding the version) and the
 * version field is marked EXPLICIT in the definition for the certificate,
 * this makes things pretty simple. Note that CRL also have a Version
 * field but the field is not explicit and parsing not is done with this
 * function.
 *
 * version         [0]  EXPLICIT Version DEFAULT v1,
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (version != NULL) ==> \valid(version);
  @ requires \separated(eaten, cert+(..), version);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (version == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(version);
  @
  @ assigns *eaten, *version;
  @*/
static int parse_x509_cert_Version(const u8 *cert, u32 off, u32 len, u8 *version, u32 *eaten)
{
	const u8 *buf = cert + off;
	u32 data_len = 0;
	u32 hdr_len = 0;
	int ret;

	if ((cert == NULL) || (version == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */
	ret = parse_explicit_id_len(buf, len, 0,
				    CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				    &hdr_len, &data_len);
	if (ret) {
		ret = X509_PARSER_ERROR_VERSION_ABSENT;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;

	/*
	 * As the value we expect for the integer is 0x02 (version 3),
	 * data_len must be 1.
	 */
	if (data_len != 1) {
		ret = X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*version = buf[0];
	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}


/* Specification version for main serial number field of certificate */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(eaten, cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(&ctx->serial_start) && \initialized(&ctx->serial_len);
  @
  @ assigns *eaten, ctx->serial_start, ctx->serial_len;
  @*/
static int parse_CertSerialNumber(cert_parsing_ctx *ctx,
				  const u8 *cert, u32 off, u32 len,
				  tag_class exp_class, u32 exp_type,
				  u32 *eaten)
{
	int ret;

	if ((cert == NULL) || (len == 0) || (eaten == NULL) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_SerialNumber(cert, off, len, exp_class, exp_type, eaten);
	if (ret) {
	       ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	       goto out;
	}

	ctx->serial_start = off + 2; /* 2 bytes long hdr for a valid SN */
	ctx->serial_len = *eaten - 2;

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((ctx != \null)) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve));
  @ ensures (\result == 0) ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571;
  @
  @ assigns ctx->spki_alg_params.ecpubkey.curve_order_bit_len,
	    ctx->spki_alg_params.ecpubkey.curve;
  @*/
static int parse_algoid_pubkey_params_ecPublicKey(cert_parsing_ctx *ctx,
						  const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	const _curve *curve = NULL;
	u32 oid_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 2.3.5 of RFC 3279 specifically describe the expected
	 * content of the parameters for ECDSA or ECDH public key embedded
	 * in the subjectPublicKeyInfo of a certificate. Those parameters
	 * follow the OID describing the algotithm in the AlgorithmIdentifier
	 * sequence:
	 *
	 *  AlgorithmIdentifier  ::=  SEQUENCE  {
	 *         algorithm     OBJECT IDENTIFIER,
	 *         parameters    ANY DEFINED BY algorithm OPTIONAL
	 *  }
	 *
	 * Usually, when an AlgorithmIdentifier is used to describe the
	 * signature algorithm in the certificate or the signature
	 * itself, the OID comes with a NULL parameter. But for the
	 * specific OID 1.2.840.10045.2.1 described in Section 2.3.5 of
	 * RFC 3279 to support ECDSA and ECDH public keys in
	 * subjectPublicKeyInfo field, the associated parameters are
	 * expected to be of the following form:
	 *
	 *  EcpkParameters ::= CHOICE {
	 *     ecParameters  ECParameters,
	 *     namedCurve    OBJECT IDENTIFIER,
	 *     implicitlyCA  NULL }
	 *
	 * In practice, to simplify things a lot w/o real lack of
	 * support, we only accept namedCurves (ECParameters is
	 * quite complex and never used in practice), i.e. we
	 * expect to find an OID for a curve we support.
	 */

	if (buf[0] == 0x30) {
		/* Sequence means we are dealing with ecParameters */
		ret = -1;
		goto out;
	}

	/* We should be dealing with a named curve (OID) */
	/* The first thing we should find in the sequence is an OID */
	ret = parse_OID(buf, len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* We do not expected anything after the parameters */
	if (oid_len != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now see if that OID is one associated w/ a curve we support */
	curve = find_curve_by_oid(buf, oid_len);
	if (curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert curve->crv_order_bit_len <= 571; */

	ctx->spki_alg_params.ecpubkey.curve_order_bit_len = curve->crv_order_bit_len;
	ctx->spki_alg_params.ecpubkey.curve = curve->crv_id;

	ret = 0;

out:
	return ret;
}

/*
 * RFC 8410 defines Agorithm Identifiers for Ed25519 and Ed448
 *
 * subject public key encoding:
 *
 * SubjectPublicKeyInfo  ::=  SEQUENCE  {
 *      algorithm         AlgorithmIdentifier,
 *      subjectPublicKey  BIT STRING
 * }
 *
 *
 *  The fields in SubjectPublicKeyInfo have the following meanings:
 *
 *  o  algorithm is the algorithm identifier and parameters for the
 *     public key (see above).
 *
 *  o  subjectPublicKey contains the byte stream of the public key.  The
 *     algorithms defined in this document always encode the public key
 *     as an exact multiple of 8 bits.
 *
 * OID are 1.3.101.112 for Ed25519 and 1.3.101.113 for Ed448.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (raw_pub_off != NULL) ==> \valid(raw_pub_off);
  @ requires (raw_pub_len != NULL) ==> \valid(raw_pub_len);
  @ requires \separated(cert+(..),raw_pub_off, raw_pub_len);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns *raw_pub_off, *raw_pub_len;
  @*/
static int parse_pubkey_eddsa(const u8 *cert, u32 off, u32 len,
			      u32 exp_pub_len, u32 *raw_pub_off, u32 *raw_pub_len)
{
	u32 remain, hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) ||
	    (raw_pub_off == NULL) || (raw_pub_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* subjectPublicKey field of SubjectPublicKeyInfo is a BIT STRING */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	remain = data_len - 1;

	if (remain != exp_pub_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;
	*raw_pub_off = off + hdr_len + 1;
	*raw_pub_len = exp_pub_len;
out:
	return ret;
}

#define ED25519_PUB_LEN 32
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.ed25519.ed25519_raw_pub_off,
	    ctx->spki_alg_params.ed25519.ed25519_raw_pub_len;
  @*/
static int parse_pubkey_ed25519(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len)
{
	return parse_pubkey_eddsa(cert, off, len, ED25519_PUB_LEN,
				  &ctx->spki_alg_params.ed25519.ed25519_raw_pub_off,
				  &ctx->spki_alg_params.ed25519.ed25519_raw_pub_len);
}

#define X25519_PUB_LEN ED25519_PUB_LEN
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.x25519.x25519_raw_pub_off,
	    ctx->spki_alg_params.x25519.x25519_raw_pub_len;
  @*/
static int parse_pubkey_x25519(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len)
{
	return parse_pubkey_eddsa(cert, off, len, X25519_PUB_LEN,
				  &ctx->spki_alg_params.x25519.x25519_raw_pub_off,
				  &ctx->spki_alg_params.x25519.x25519_raw_pub_len);
}

#define ED448_PUB_LEN  57
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.ed448.ed448_raw_pub_off,
	    ctx->spki_alg_params.ed448.ed448_raw_pub_len;
  @*/
static int parse_pubkey_ed448(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len)
{
	return parse_pubkey_eddsa(cert, off, len, ED448_PUB_LEN,
				  &ctx->spki_alg_params.ed448.ed448_raw_pub_off,
				  &ctx->spki_alg_params.ed448.ed448_raw_pub_len);
}

#define X448_PUB_LEN ED448_PUB_LEN
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.x448.x448_raw_pub_off,
	    ctx->spki_alg_params.x448.x448_raw_pub_len;
  @*/
static int parse_pubkey_x448(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len)
{
	return parse_pubkey_eddsa(cert, off, len, X448_PUB_LEN,
				  &ctx->spki_alg_params.x448.x448_raw_pub_off,
				  &ctx->spki_alg_params.x448.x448_raw_pub_len);
}

/*
 *  From RFC 3279:
 *
 *  The elliptic curve public key (an ECPoint which is an
 *  OCTET STRING) is mapped to a subjectPublicKey (a BIT
 *  STRING) as follows:  the most significant bit of the
 *  OCTET STRING becomes the most significant bit of the
 *  BIT STRING, and the least significant bit of the OCTET
 *  STRING becomes the least significant bit of the BIT
 *  STRING.
 *
 *  ECPoint ::= OCTET STRING
 *
 *  The value of FieldElement SHALL be the octet string
 *  representation of a field element following the
 *  conversion routine in [X9.62], Section 4.3.3.
 *  The value of ECPoint SHALL be the octet string
 *  representation of an elliptic curve point following
 *  the conversion routine in [X9.62], Section 4.3.6.
 *  Note that this octet string may represent an elliptic
 *  curve point in compressed or uncompressed form.
 *
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),ctx);
  @ requires \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len));
  @ requires \initialized(&(ctx->spki_alg_params.ecpubkey.curve));
  @ requires ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571;
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.ecpubkey.compression,
	    ctx->spki_alg_params.ecpubkey.ecc_raw_x_off,
	    ctx->spki_alg_params.ecpubkey.ecc_raw_x_len,
	    ctx->spki_alg_params.ecpubkey.ecc_raw_y_off,
	    ctx->spki_alg_params.ecpubkey.ecc_raw_y_len;
  @*/
static int parse_pubkey_ec(cert_parsing_ctx *ctx,
			   const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 remain;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 order_ceil_len;
	int ret;
	u8 pc;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* subjectPublicKey field of SubjectPublicKeyInfo is a BIT STRING */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * From that point on, the parsing of the public key is done as
	 * described in section 4.3.7 of X9.62 version 1998.
	 */

	/*
	 * The first thing we should find is the PC byte, which means
	 * at least one byte should remain at that point.
	 */
	if (remain == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	pc = buf[0];

	remain -= 1;
	buf += 1;
	off += 1;

	/*
	 * We expect a specific length for the remaining data based on
	 * pc byte value.
	 */
	order_ceil_len = (ctx->spki_alg_params.ecpubkey.curve_order_bit_len + 7) / 8;

	switch (pc) {
	case 0x04: /* uncompressed */
		if (remain != (order_ceil_len * 2)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		ctx->spki_alg_params.ecpubkey.compression = pc;
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_off = off;
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_len = order_ceil_len;
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_off = off + order_ceil_len;
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_len = order_ceil_len;
		break;
	case 0x02: /* compressed point with even y */
	case 0x03: /* compressed point with odd y */
		if (remain != order_ceil_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		ctx->spki_alg_params.ecpubkey.compression = pc;
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_off = off;
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_len = order_ceil_len;
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_off = 0;
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_len = 0;
		break;
	default: /* hybrid or other forms: no support */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

	ret = 0;

out:
	return ret;
}


/*
 * draft-deremin-rfc4491-bis-06 has the following on GOST public keys:
 *
 * The GOST R 34.10-2012 public key MUST be ASN.1 DER encoded as an
 * OCTET STRING.  This encoding SHALL be used as the content (i.e., the
 * value) of the subjectPublicKey field (a BIT STRING) of
 * SubjectPublicKeyInfo structure.
 *
 * GostR3410-2012-256-PublicKey ::= OCTET STRING (64),
 * GostR3410-2012-512-PublicKey ::= OCTET STRING (128).
 *
 * "GostR3410-2012-256-PublicKey" MUST contain 64 octets, where the
 * first 32 octets contain the little-endian representation of "x" and
 * the second 32 octets contains the little-endian representation of "y"
 * coordinates of the public key.
 *
 * "GostR3410-2012-512-PublicKey" MUST contain 128 octets, where the
 * first 64 octets contain the little-endian representation of "x" and
 * the second 64 octets contains the little-endian representation of "y"
 * coordinates of the public key.
 *
 * Note: The format is the same for GOST R 34.10-2001 which has public keys
 * on 256 bit curve, i.e. with a exp_pub_len = 32 * 2 = 64.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (raw_x_off != NULL) ==> \valid(raw_x_off);
  @ requires (raw_x_len != NULL) ==> \valid(raw_x_len);
  @ requires (raw_y_off != NULL) ==> \valid(raw_y_off);
  @ requires (raw_y_len != NULL) ==> \valid(raw_y_len);
  @ requires \separated(cert+(..), raw_x_off, raw_x_len, raw_y_off, raw_y_len);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns *raw_x_off, *raw_x_len, *raw_y_off, *raw_y_len;
  @*/
static int _parse_pubkey_gost_on_curves(const u8 *cert, u32 off, u32 len,
					u32 exp_pub_len,
					u32 *raw_x_off, u32 *raw_x_len,
					u32 *raw_y_off, u32 *raw_y_len)
{
	const u8 *buf = cert + off;
	u32 remain;
	u32 hdr_len = 0;
	u32 data_len = 0;
	int ret;

	if ((cert == NULL) || (len == 0) ||
	    (raw_x_off == NULL) || (raw_x_len == NULL) ||
	    (raw_y_off == NULL) || (raw_y_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	off += hdr_len;
	buf += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * We can now consider the content of the bitstring as an ASN.1 octet
	 * string.
	 */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (data_len != exp_pub_len) {
		ret = -1;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;

	}

	buf += hdr_len;
	off += hdr_len;
	remain -= hdr_len;

	*raw_x_off = off;
	*raw_x_len = exp_pub_len / 2;
	*raw_y_off = off + *raw_x_len;
	*raw_y_len = exp_pub_len / 2;

	if (remain != data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * RFC 4491 section 2.3.5 has:
 *
 *    The GOST R 34.10-94 public key MUST be ASN.1 DER encoded as an OCTET
 *    STRING; this encoding shall be used as the contents (i.e., the value)
 *    of the subjectPublicKey component (a BIT STRING) of the
 *    SubjectPublicKeyInfo data element.
 *
 *    GostR3410-94-PublicKey ::= OCTET STRING -- public key, Y
 *
 *    GostR3410-94-PublicKey MUST contain 128 octets of the little-endian
 *    representation of the public key Y = a^x (mod p), where a and p are
 *    public key parameters, and x is a private key.
 *
 */
#define GOST94_PUB_LEN 128 /* bytes */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.gost94.gost94_raw_pub_off,
	    ctx->spki_alg_params.gost94.gost94_raw_pub_len;
  @*/
static int parse_pubkey_gostr3410_94(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 remain;
	u32 hdr_len = 0;
	u32 data_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	off += hdr_len;
	buf += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * We can now consider the content of the bitstring as an ASN.1 octet
	 * string.
	 */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (data_len != GOST94_PUB_LEN) {
		ret = -1;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;

	}

	buf += hdr_len;
	off += hdr_len;
	remain -= hdr_len;

	if (remain != data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.gost94.gost94_raw_pub_off = off;
	ctx->spki_alg_params.gost94.gost94_raw_pub_len = GOST94_PUB_LEN;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_off,
	    ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_len,
	    ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_off,
	    ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_len;
  @*/
static inline int parse_pubkey_gostr3410_2001(cert_parsing_ctx *ctx,
				       const u8 *cert, u32 off, u32 len)
{
	return _parse_pubkey_gost_on_curves(cert, off, len, 64,
				 &ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_off,
				 &ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_len,
				 &ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_off,
				 &ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_len);
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_off,
	    ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_len,
	    ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_off,
	    ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_len;
  @*/
static inline int parse_pubkey_gostr3410_2012_256(cert_parsing_ctx *ctx,
					   const u8 *cert, u32 off, u32 len)
{
	return _parse_pubkey_gost_on_curves(cert, off, len, 64,
				 &ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_off,
				 &ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_len,
				 &ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_off,
				 &ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_len);
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_off,
	    ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_len,
	    ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_off,
	    ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_len;
  @*/
static inline int parse_pubkey_gostr3410_2012_512(cert_parsing_ctx *ctx,
				    const u8 *cert, u32 off, u32 len)
{
	return _parse_pubkey_gost_on_curves(cert, off, len, 128,
				 &ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_off,
				 &ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_len,
				 &ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_off,
				 &ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_len);
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @ requires \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len));
  @ requires \initialized(&(ctx->spki_alg_params.bign.curve));
  @ requires ctx->spki_alg_params.bign.curve_order_bit_len <= 571;
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.bign.bign_raw_x_pub_off,
	    ctx->spki_alg_params.bign.bign_raw_x_pub_len,
	    ctx->spki_alg_params.bign.bign_raw_y_pub_off,
	    ctx->spki_alg_params.bign.bign_raw_y_pub_len;
  @*/
static int parse_pubkey_bign(cert_parsing_ctx *ctx,
			     const u8 *cert, u32 off, u32 len)
{
	u32 order_ceil_len, remain, hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* subjectPublicKey field of SubjectPublicKeyInfo is a BIT STRING */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * We expect a specific length for the remaining data based on
	 * pc byte value.
	 */
	order_ceil_len = (ctx->spki_alg_params.bign.curve_order_bit_len + 7) / 8;
	if (remain != (2 * order_ceil_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.bign.bign_raw_x_pub_off = off;
	ctx->spki_alg_params.bign.bign_raw_x_pub_len = order_ceil_len;
	ctx->spki_alg_params.bign.bign_raw_y_pub_off = off + order_ceil_len;
	ctx->spki_alg_params.bign.bign_raw_y_pub_len = order_ceil_len;

	ret = 0;

out:
	return ret;
}

/*
 * When Alg OID is 1.2.112.0.2.0.34.101.45.2.1 (bign-pubkey), the parameters
 * contains an OID for the curve for the public key.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_params.bign.curve));
  @ ensures (\result == 0) ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571;
  @
  @ assigns ctx->spki_alg_params.bign.curve_order_bit_len,
	    ctx->spki_alg_params.bign.curve;
  @*/
static int parse_algoid_pubkey_params_bign(cert_parsing_ctx *ctx,
					   const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	const _curve *curve;
	u32 oid_len = 0;
	u32 remain;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	remain = len;
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	curve = find_curve_by_oid(buf, oid_len);
	if (curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert curve->crv_order_bit_len <= 571; */

	buf += oid_len;
	remain -= oid_len;

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.bign.curve_order_bit_len = curve->crv_order_bit_len;
	ctx->spki_alg_params.bign.curve = curve->crv_id;

	ret = 0;

out:
	return ret;
}




/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len != 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.ed448.curve,
	    ctx->spki_alg_params.ed448.curve_order_bit_len;
  @*/
static int parse_algoid_pubkey_params_ed448(cert_parsing_ctx *ctx,
					    const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len != 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.ed448.curve = CURVE_WEI448;
	ctx->spki_alg_params.ed448.curve_order_bit_len = 448;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len != 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.x448.curve,
	    ctx->spki_alg_params.x448.curve_order_bit_len;
  @*/
static int parse_algoid_pubkey_params_x448(cert_parsing_ctx *ctx,
					      const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len != 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.x448.curve = CURVE_WEI448;
	ctx->spki_alg_params.x448.curve_order_bit_len = 448;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len != 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.ed25519.curve,
	    ctx->spki_alg_params.ed25519.curve_order_bit_len;
  @*/
static int parse_algoid_pubkey_params_ed25519(cert_parsing_ctx *ctx,
					      const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len != 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.ed25519.curve = CURVE_WEI25519;
	ctx->spki_alg_params.ed25519.curve_order_bit_len = 256;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len != 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.x25519.curve,
	    ctx->spki_alg_params.x25519.curve_order_bit_len;
  @*/
static int parse_algoid_pubkey_params_x25519(cert_parsing_ctx *ctx,
					      const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len != 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.x25519.curve = CURVE_WEI25519;
	ctx->spki_alg_params.x25519.curve_order_bit_len = 256;

	ret = 0;

out:
	return ret;
}

/*
 * From RFC 5280:
 *
 * subject public key encoding:
 *
 * SubjectPublicKeyInfo  ::=  SEQUENCE  {
 *      algorithm         AlgorithmIdentifier,
 *      subjectPublicKey  BIT STRING
 * }
 *
 *    AlgorithmIdentifier  ::=  SEQUENCE  {
 *       algorithm               OBJECT IDENTIFIER,
 *       parameters              ANY DEFINED BY algorithm OPTIONAL  }
 *
 * From draft-deremin-rfc4491-bis-06:
 *
 * GOST R 34.10-2012 public keys with 256 bits private key length are
 * identified by the following OID: 1.2.643.7.1.1.1.1
 * GOST R 34.10-2012 public keys with 512 bits private key length are
 * identified by the following OID: 1.2.643.7.1.1.1.2
 *
 * When either of these identifiers appears as algorithm field in
 * SubjectPublicKeyInfo.algorithm.algorithm field, parameters field MUST
 * have the following structure:
 *
 * GostR3410-2012-PublicKeyParameters ::= SEQUENCE {
 *       publicKeyParamSet OBJECT IDENTIFIER,
 *       digestParamSet OBJECT IDENTIFIER OPTIONAL
 * }
 *
 * The function below parses the GostR3410-2012-PublicKeyParameters sequence.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (curve != NULL) ==> \valid(curve);
  @ requires (hash != NULL) ==> \valid(hash);
  @ requires \separated(curve, hash, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (hash == \null) ==> \result < 0;
  @ ensures (curve == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (*curve)->crv_order_bit_len <= 571;
  @
  @ assigns *curve, *hash;
  @*/
static int parse_algoid_params_gost2012PublicKey(const u8 *cert, u32 off, u32 len,
						 const _curve **curve, const _hash_alg **hash)
{
	u32 remain, hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	u32 oid_len = 0;
	int ret;

	if ((cert == NULL) || (len == 0) || (curve == NULL) || (hash == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * draft-deremin-rfc4491-bis-06 expects the OID to be in a limited set
	 * defined as:
	 * "public key parameters identifier for GOST R 34.10-2012 (see Sections
	 * 5.1 and 5.2 of [RFC7836] or Appendix B) or GOST R 34.10-2001 (see
	 * Section 8.4 of [RFC4357]) parameters.
	 *
	 * XXX verify that later. At the moment, we accept any valid OID
	 */

	buf += hdr_len;
	remain = data_len;

	/*
	 * The first thing we should find in the sequence is an OID for
	 * publicKeyParamSet
	 */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*curve = find_curve_by_oid(buf, oid_len);
	if (*curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert (*curve)->crv_order_bit_len <= 571; */

	buf += oid_len;
	remain -= oid_len;

	/*
	 * draft-deremin-rfc4491-bis-06 has:
	 *
	 * The field digestParamSet:
	 *
	 * o  SHOULD be omitted if GOST R 34.10-2012 signature algorithm is used
	 *    with 512-bit key length;
	 *
	 * o  MUST be present and must be equal to "id-tc26-digest-
	 *    gost3411-12-256" if one of the following values is used as
	 *    "publicKeyParamSet":
	 *
	 *  "id-GostR3410-2001-CryptoPro-A-ParamSet",
	 *  "id-GostR3410-2001-CryptoPro-B-ParamSet",
	 *  "id-GostR3410-2001-CryptoPro-C-ParamSet",
	 *  "id-GostR3410-2001-CryptoPro-XchA-ParamSet",
	 *  "id-GostR3410-2001-CryptoPro-XchB-ParamSet";
	 *
	 * o  SHOULD be omitted if publicKeyParamSet is equal to:
	 *
	 *  "id-tc26-gost-3410-2012-256-paramSetA";
	 *
	 * o  MUST be omitted if one of the following values is used as
	 *    publicKeyParamSet:
	 *
	 *  "id-tc26-gost-3410-2012-256-paramSetB",
	 *  "id-tc26-gost-3410-2012-256-paramSetC",
	 *  "id-tc26-gost-3410-2012-256-paramSetD".
	 *
	 * XXX At the moment, we just verify we either have nothing following
	 * or a valid OID.
	 */
	if (remain)  {
		/* If something follows, it must be and OID defining digestParamSet. */
		ret = parse_OID(buf, remain, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		*hash = find_hash_by_oid(buf, oid_len);
		if (*hash == NULL) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += oid_len;
		remain -= oid_len;

		if (remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	/*
	 * XXX At that point, we should probably have verified the association of
	 * OID, curves is valid for GOST. This is not done yet
	 */
	ret = 0;

out:
	return ret;
}


/* EA-RSA SPKI algoid optionally contains an integer giving the key size:
 *
 *  See https://www.alvestrand.no/objectid/2.5.8.1.1.html
 *
 * From one cert:
 *
 * 219 156:     SEQUENCE {
 * 222  10:       SEQUENCE {
 * 224   4:         OBJECT IDENTIFIER rsa (2 5 8 1 1)
 * 230   2:         INTEGER 1024
 *        :         }
 * 234 141:       BIT STRING, encapsulates {
 * 238 137:         SEQUENCE {
 * 241 129:           INTEGER
 *        :             00 84 89 E4 7C 35 C5 E4 51 6D 5F D0 6E 9A 0B AB
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.rsa.rsa_advertised_bit_len;
  @*/
static int parse_algoid_pubkey_params_ea_rsa(cert_parsing_ctx *ctx,
					     const u8 *cert, u32 off, u32 len)
{
	u32 hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	u32 bit_len = 0, parsed = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (len > 0) ==> \valid_read(buf + (0 .. len - 1)); */

	/*
	 * We can either find nothing, a NULL or an integer with the bit size
	 * of the key.
	 */
	switch (len) {
	case 0: /* nothing */
		ret = 0;
		break;

	case 2: /* NULL */
		ret = parse_null(buf, len, &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		break;

	case 4: /* Integer */
		/*
		 * Having a RSA key with a bit length value below 127 and above
		 * 32768 does not make sense. For that reason, we expected the
		 * integer to be encoded on 4 bytes: 1 for class/id, 1 for
		 * length, 2 for integer value
		 */

		/* Verify the integer is DER-encoded as it should */
		ret = parse_non_negative_integer(buf, len, CLASS_UNIVERSAL,
					     ASN1_TYPE_INTEGER,
					     &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		bit_len = (((u32)buf[2]) * (1 << 8)) + buf[3];

		break;

	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		break;
	}

	ctx->spki_alg_params.rsa.rsa_advertised_bit_len = bit_len;

out:
	return ret;
}


/*
 * RFC 4491, RFC 4357
 *
 * Section 2.3.1 of RFC 4491 has:
 *
 *  GostR3410-94-PublicKeyParameters ::=
 *         SEQUENCE {
 *             publicKeyParamSet
 *                 OBJECT IDENTIFIER,
 *             digestParamSet
 *                 OBJECT IDENTIFIER,
 *             encryptionParamSet
 *                 OBJECT IDENTIFIER DEFAULT
 *                     id-Gost28147-89-CryptoPro-A-ParamSet
 *         }
 *
 * In the certificate we have at hand, we only have the first 2 OIDs and the
 * optional is not there e.g.
 *
 * 3012 06072a850302022002 06072a850302021e01
 *
 * i.e. a sequence of 2 OIDs:
 *
 * 06072a850302022002  1.2.643.2.2.32.2   id-GostR3410-94-CryptoPro-A-ParamSet
 * 06072a850302021e01  1.2.643.2.2.30.1   GOST R 3411-94 CryptoProParamSet
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.gost94.gost94_params_id;
  @*/
static int parse_algoid_pubkey_params_gost_r3410_94(cert_parsing_ctx *ctx,
						   const u8 *cert, u32 off, u32 len)
{
	u32 remain, hdr_len = 0, data_len = 0, oid_len = 0;
	const _gost94_pub_params *params;
	const u8 *buf = cert + off;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* We expect a sequence ...  */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/* ... starting with a first OID for GOST R 34.10-94 paramset */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	params = find_gost94_params_by_oid(buf, oid_len);
	if (params == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	ctx->spki_alg_params.gost94.gost94_params_id = params->params_id;

	buf += oid_len;
	remain -= oid_len;

	/* ... followed by a second OID for GOST R 34.11-94 paramset */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * XXX at some point we could create a list of known paramset
	 * and associated OID as defined in section 8.3. of RFC 4357
	 */

	buf += oid_len;
	remain -= oid_len;

	/* Let's verify we have nothing left behind */
	if (remain) {
		/* We may have an optional encryptionParamset */
		ret = parse_OID(buf, remain, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= oid_len;

		if (remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	ret = 0;

out:
	return ret;

}

/*
 * Used for parsing AlgorithmIdentifiter parameters for GOST R3410-2001 public
 * key. As described in RFC 4491:
 *
 *  When the id-GostR3410-2001 algorithm identifier appears as the algorithm
 *  field in an AlgorithmIdentifier, the encoding MAY omit the parameters field
 *  or set it to NULL.  Otherwise, this field MUST have the following structure:
 *
 *     GostR3410-2001-PublicKeyParameters ::=
 *         SEQUENCE {
 *             publicKeyParamSet
 *                 OBJECT IDENTIFIER,
 *             digestParamSet
 *                 OBJECT IDENTIFIER,
 *             encryptionParamSet
 *                 OBJECT IDENTIFIER DEFAULT
 *                     id-Gost28147-89-CryptoPro-A-ParamSet
 *         }
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> ctx->spki_alg_params.gost2001.curve_order_bit_len <= 571;
  @
  @ assigns ctx->spki_alg_params.gost2001.curve_order_bit_len,
	    ctx->spki_alg_params.gost2001.curve;
  @*/
static int parse_algoid_pubkey_params_gost_r3410_2001(cert_parsing_ctx *ctx,
						      const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 remain, hdr_len = 0, data_len = 0;
	const _curve *curve;
	const _hash_alg *h;
	u32 parsed = 0;
	u32 oid_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.gost2001.curve_order_bit_len = 0;
	ctx->spki_alg_params.gost2001.curve = CURVE_UNKNOWN;
	/*@ assert ctx->spki_alg_params.gost2001.curve_order_bit_len <= 571; */
	// XXX investigate hash hash->hash_id purpose for pubkey

	if (len == 0) { /* the encoding MAY omit the parameters field ... */
		ret = 0;
		goto out;
	}

	if (len == 2 && !parse_null(buf, len, &parsed)) { /* or set it to NULL */
		ret = 0;
		goto out;
	}

	/*
	 * From now on, we expect a gostR3410-2001-PublicKeyParameters as
	 * defined above. It must start with a valid sequence.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/*
	 * The first thing we should find in the sequence is an OID for
	 * publicKeyParamSet
	 */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	curve = find_curve_by_oid(buf, oid_len);
	if (curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert curve->crv_order_bit_len <= 571; */

	buf += oid_len;
	remain -= oid_len;

	/* Then, we may  find an OID for digestParamSet. It is not optional */
	/* If something follows, it must be and OID defining digestParamSet. */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	h = find_hash_by_oid(buf, oid_len);
	if (h == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += oid_len;
	remain -= oid_len;

	if (remain)  {
		/* Something follows. This must be the OID for encryptionParamSet. */
		ret = parse_OID(buf, remain, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += oid_len;
		remain -= oid_len;

		/* Nothings should remain behind */
		if (remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	/*@ assert curve->crv_order_bit_len <= 571; */
	ctx->spki_alg_params.gost2001.curve_order_bit_len = curve->crv_order_bit_len;
	/*@ assert ctx->spki_alg_params.gost2001.curve_order_bit_len <= 571; */
	ctx->spki_alg_params.gost2001.curve = curve->crv_id;
	// XXX investigate hash hash->hash_id purpose for pubkey

	ret = 0;

out:
	return ret;}


/*
 * Used for parsing AlgorithmIdentifiter parameters for GOST R3410-2001 with 256
 * bits public key
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.gost2012_256.curve_order_bit_len,
	    ctx->spki_alg_params.gost2012_256.curve;
  @*/
static int parse_algoid_pubkey_params_gost_r3410_2012_256(cert_parsing_ctx *ctx,
							  const u8 *cert, u32 off, u32 len)
{
	const _hash_alg *hash = NULL;
	const _curve *curve = NULL;
	int ret;

	if ((ctx == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_gost2012PublicKey(cert, off, len, &curve, &hash);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.gost2012_256.curve_order_bit_len = curve->crv_order_bit_len;
	ctx->spki_alg_params.gost2012_256.curve = curve->crv_id;
	// investiage hash hash->hash_id purpose for pubkey

out:

	return ret;
}

/*
 * Used for parsing AlgorithmIdentifiter parameters for GOST R3410-2012 with 512
 * bit public key.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.gost2012_512.curve_order_bit_len,
	    ctx->spki_alg_params.gost2012_256.curve;
  @*/
static int parse_algoid_pubkey_params_gost_r3410_2012_512(cert_parsing_ctx *ctx,
							  const u8 *cert, u32 off, u32 len)
{
	const _hash_alg *hash = NULL;
	const _curve *curve = NULL;
	int ret;

	if (ctx == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_gost2012PublicKey(cert, off, len, &curve, &hash);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (curve == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.gost2012_512.curve_order_bit_len = curve->crv_order_bit_len;
	ctx->spki_alg_params.gost2012_256.curve = curve->crv_id;
	// XXX investigate hash hash->hash_id purpose for pubkey

out:
	return ret;
}


/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
static int parse_algoid_pubkey_params_none(cert_parsing_ctx *ctx,
					   const u8 *cert, u32 off, u32 len)
{
	int ret;

	if (ctx == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_none(cert, off, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
static int parse_algoid_pubkey_params_rsa(cert_parsing_ctx *ctx,
					const u8 *cert, u32 off, u32 len)
{
	int ret;

	if ((ctx == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_rsa(cert, off, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

/*
 * From RFC 3279:
 *
 * The RSA public key MUST be encoded using the ASN.1
 * type RSAPublicKey:
 *
 * RSAPublicKey ::= SEQUENCE {
 *  modulus            INTEGER,    -- n
 *  publicExponent     INTEGER  }  -- e
 *
 * where modulus is the modulus n, and publicExponent
 * is the public exponent e. The DER encoded
 * RSAPublicKey is the value of the BIT STRING
 * subjectPublicKey.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (n_start_off != NULL) ==> \valid(n_start_off);
  @ requires (n_len != NULL) ==> \valid(n_len);
  @ requires (e_start_off != NULL) ==> \valid(e_start_off);
  @ requires (e_len != NULL) ==> \valid(e_len);
  @ requires \separated(buf+(..), n_start_off, n_len, e_start_off, e_len);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (u64)*n_start_off + (u64)*n_len <= (u64)len;
  @ ensures (\result == 0) ==> (u64)*e_start_off + (u64)*e_len <= (u64)len;
  @
  @ assigns *n_start_off, *n_len, *e_start_off, *e_len;
  @*/
static int spki_rsa_export_n_e(const u8 *buf, u32 len,
			       u32 *n_start_off, u32 *n_len,
			       u32 *e_start_off, u32 *e_len)
{
	u32 remain;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 parsed = 0;
	u32 off;
	int ret;

	if ((buf == NULL) || (len == 0) ||
	    (n_start_off == NULL) || (n_len == NULL) ||
	    (e_start_off == NULL) || (e_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* subjectPublicKey field of SubjectPublicKeyInfo is a BIT STRING */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off = hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	remain = data_len - 1;
	off += 1;

	/*
	 * Now, in the case of a RSA public key, we expect the content of
	 * the BIT STRING to contain a SEQUENCE of two INTEGERS
	 */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;
	off += hdr_len;

	/*
	 * Now, we should find the first integer, n (modulus). We first parse
	 * it to validate it. Then, we skip over the header to move to get up
	 * to the position of the modulus
	 */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				     &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	parsed = hdr_len + data_len;

	/*@ assert ((u64)hdr_len + (u64)data_len) <= (u64)remain; */
	/*@ assert remain <= len; */
	/*@ assert (u64)off + (u64)hdr_len + (u64)data_len <= (u64)len; */
	*n_start_off = off + hdr_len;
	*n_len = data_len;
	/*@ assert (u64)*n_start_off + (u64)*n_len <= (u64)len; */

	/* if MSB of modulus is 0, remove it */
	if ((data_len != 0) && (buf[hdr_len] == 0)) {
		*n_start_off += 1;
		*n_len -= 1;
	}

	buf += parsed;
	off += parsed;
	remain -= parsed;

	/* An then, the second one, e (publicExponent) */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				     &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	parsed = hdr_len + data_len;

	/*@ assert ((u64)hdr_len + (u64)data_len) <= (u64)remain; */
	/*@ assert remain <= len; */
	/*@ assert (u64)off + (u64)hdr_len + (u64)data_len <= (u64)len; */
	*e_start_off = off + hdr_len;
	*e_len = data_len;
	/*@ assert (u64)*e_start_off + (u64)*e_len <= (u64)len; */

	buf += parsed;
	off += parsed;
	remain -= parsed;

	if (remain != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns ctx->spki_alg_params.rsa.rsa_raw_modulus_off,
	    ctx->spki_alg_params.rsa.rsa_raw_modulus_len,
	    ctx->spki_alg_params.rsa.rsa_raw_pub_exp_off,
	    ctx->spki_alg_params.rsa.rsa_raw_pub_exp_len;
  @*/
static int parse_pubkey_rsa(cert_parsing_ctx *ctx,
			    const u8 *cert, u32 off, u32 len)
{
	u32 n_start_off = 0, n_len = 0, e_start_off= 0, e_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = spki_rsa_export_n_e(buf, len, &n_start_off, &n_len, &e_start_off, &e_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.rsa.rsa_raw_modulus_off = off + n_start_off;
	ctx->spki_alg_params.rsa.rsa_raw_modulus_len = n_len;
	ctx->spki_alg_params.rsa.rsa_raw_pub_exp_off = off + e_start_off;
	ctx->spki_alg_params.rsa.rsa_raw_pub_exp_len = e_len;

out:
	return ret;
}

/*
 * Section 7.3.3 of RFC 2459 has:
 *
 *   If the DSA algorithm parameters are present in the
 *   subjectPublicKeyInfo AlgorithmIdentifier, the parameters are included
 *   using the following ASN.1 structure:
 *
 *        Dss-Parms  ::=  SEQUENCE  {
 *            p             INTEGER,
 *            q             INTEGER,
 *            g             INTEGER  }
 *
 *
 *   If the DSA algorithm parameters are absent from the
 *   subjectPublicKeyInfo AlgorithmIdentifier and the CA signed the
 *   subject certificate using DSA, then the certificate issuer's DSA
 *   parameters apply to the subject's DSA key.  If the DSA algorithm
 *   parameters are absent from the subjectPublicKeyInfo
 *   AlgorithmIdentifier and the CA signed the subject certificate using a
 *   signature algorithm other than DSA, then the subject's DSA parameters
 *   are distributed by other means.  If the subjectPublicKeyInfo
 *   AlgorithmIdentifier field omits the parameters component and the CA
 *   signed the subject with a signature algorithm other than DSA, then
 *   clients shall reject the certificate.
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (p_start_off != NULL) ==> \valid(p_start_off);
  @ requires (p_len != NULL) ==> \valid(p_len);
  @ requires (q_start_off != NULL) ==> \valid(q_start_off);
  @ requires (q_len != NULL) ==> \valid(q_len);
  @ requires (g_start_off != NULL) ==> \valid(g_start_off);
  @ requires (g_len != NULL) ==> \valid(g_len);
  @ requires \separated(buf+(..), p_start_off, p_len, q_start_off, q_len, g_start_off, g_len);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (p_start_off == \null) ==> \result < 0;
  @ ensures (p_len == \null) ==> \result < 0;
  @ ensures (q_start_off == \null) ==> \result < 0;
  @ ensures (q_len == \null) ==> \result < 0;
  @ ensures (g_start_off == \null) ==> \result < 0;
  @ ensures (g_len == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> *p_start_off <= len;
  @ ensures (\result == 0) ==> *q_start_off <= len;
  @ ensures (\result == 0) ==> *g_start_off <= len;
  @ ensures (\result == 0) ==> (u64)*p_start_off + (u64)*p_len <= (u64)len;
  @ ensures (\result == 0) ==> (u64)*q_start_off + (u64)*q_len <= (u64)len;
  @ ensures (\result == 0) ==> (u64)*g_start_off + (u64)*g_len <= (u64)len;
  @
  @ assigns *p_start_off, *p_len, *q_start_off, *q_len, *g_start_off, *g_len;
  @*/
int parse_algoid_dsa_export_params(const u8 *buf, u32 len,
				   u32 *p_start_off, u32 *p_len,
				   u32 *q_start_off, u32 *q_len,
				   u32 *g_start_off, u32 *g_len)
{
	u32 remain = 0;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 eaten = 0;
	u32 parsed = 0;
	u32 off = 0;
	int ret;

	if ((buf == NULL) ||
	    (p_start_off == NULL) || (p_len == NULL) ||
	    (q_start_off == NULL) || (q_len == NULL) ||
	    (g_start_off == NULL) || (g_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*p_start_off = 0;
	*p_len = 0;
	*q_start_off = 0;
	*q_len = 0;
	*g_start_off = 0;
	*g_len = 0;

	/*
	 * It is acceptable for DSA params to be absent (i.e. null or nothing
	 * at all aka zero length), as explained in RFC description
	 */
	if (len == 0) {
		ret = 0;
		goto out;
	}

	ret = parse_null(buf, len, &parsed);
	if (!ret) {
		ret = 0;
		goto out;
	}

	/*
	 * If not absent, params are expected to contain a sequence of
	 * 3 integers p, q and s.
	 *
	 *         Dss-Parms  ::=  SEQUENCE  {
	 *            p             INTEGER,      -- DSA prime
	 *            q             INTEGER,      -- DSA group order
	 *            g             INTEGER  }    -- DSA group generator
	 */
	remain = len;
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	/* Now, we should find the first integer, p, the DSA prime */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	eaten = hdr_len + data_len;

	*p_start_off = off + hdr_len;
	*p_len = data_len;

	remain -= eaten;
	buf += eaten;
	off += eaten;

	/* An then, the second one, q, the DSA group order */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	eaten = hdr_len + data_len;

	*q_start_off = off + hdr_len;
	*q_len = data_len;

	remain -= eaten;
	buf += eaten;
	off += eaten;

	/* An in the end, the third one, g, the group genrator */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	eaten = hdr_len + data_len;

	*g_start_off = off + hdr_len;
	*g_len = data_len;
	remain -= eaten;

	/* Let's check that nothing remains behind */
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns ctx->spki_alg_params.dsa.dsa_raw_p_off,
	    ctx->spki_alg_params.dsa.dsa_raw_p_len,
	    ctx->spki_alg_params.dsa.dsa_raw_q_off,
	    ctx->spki_alg_params.dsa.dsa_raw_q_len,
	    ctx->spki_alg_params.dsa.dsa_raw_g_off,
	    ctx->spki_alg_params.dsa.dsa_raw_g_len;
  @*/
static int parse_algoid_pubkey_params_dsa(cert_parsing_ctx *ctx,
					  const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 p_start_off, p_len;
	u32 q_start_off, q_len;
	u32 g_start_off, g_len;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_algoid_dsa_export_params(buf, len,
					     &p_start_off, &p_len,
					     &q_start_off, &q_len,
					     &g_start_off, &g_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.dsa.dsa_raw_p_off = off + p_start_off;
	ctx->spki_alg_params.dsa.dsa_raw_p_len = p_len;
	ctx->spki_alg_params.dsa.dsa_raw_q_off = off + q_start_off;
	ctx->spki_alg_params.dsa.dsa_raw_q_len = q_len;
	ctx->spki_alg_params.dsa.dsa_raw_g_off = off + g_start_off;
	ctx->spki_alg_params.dsa.dsa_raw_g_len = g_len;

out:
	return ret;
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (pub_start_off != NULL) ==> \valid(pub_start_off);
  @ requires (pub_len != NULL) ==> \valid(pub_len);
  @ requires \separated(buf+(..), pub_start_off, pub_len);
  @
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> *pub_start_off <= len;
  @ ensures (\result == 0) ==> (u64)*pub_start_off + (u64)*pub_len <= (u64)len;
  @
  @ assigns *pub_start_off, *pub_len;
  @*/
int parse_pubkey_dsa_export_pub(const u8 *buf, u32 len,
				u32 *pub_start_off, u32 *pub_len)
{
	u32 remain;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 off;
	int ret;

	if ((buf == NULL) || (len == 0) ||
	    (pub_start_off == NULL) || (pub_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* subjectPublicKey field of SubjectPublicKeyInfo is a BIT STRING */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off = hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	remain = data_len - 1;
	off += 1;

	/*
	 * Now, in the case of a DSA public key, we expect the content of
	 * the BIT STRING to hold an INTEGER
	 */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
	*pub_start_off = off;
	*pub_len = data_len;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns *ctx;
  @*/
static int parse_pubkey_dsa(cert_parsing_ctx *ctx,
			    const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 pub_start_off = 0, pub_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (len >= 0) ==> \valid_read(buf + (0 .. len - 1)); */

	ret = parse_pubkey_dsa_export_pub(buf, len, &pub_start_off, &pub_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_params.dsa.dsa_raw_pub_off = off + pub_start_off;
	ctx->spki_alg_params.dsa.dsa_raw_pub_len = pub_len;

out:
	return ret;
}


/*
 * The algorithmIdentifier structure is used at different location
 * in a certificate for different kind of algorithms:
 *
 *  - in signature and signatureAlgorithm fields, it describes a
 *    signature algorithm.
 *  - in subjectPublicKeyInfo, it describes a public key
 *    algorithm.
 *
 * It has the following structure:
 *
 * AlgorithmIdentifier. Used for signature field.
 *
 *    AlgorithmIdentifier  ::=  SEQUENCE  {
 *       algorithm               OBJECT IDENTIFIER,
 *       parameters              ANY DEFINED BY algorithm OPTIONAL  }
 *
 * In the parser, we define currently define two functions:
 *
 *  - one for parsing tbsCertificate.signature algoid field
 *  - one for parsing spki algoid field
 *
 * Parsing of certificate signatureAlgorithm field is not performed.
 * as it must exactly match the content of tbsCertificate.signature
 * field. A simple comparison is performed for that purpose.
 */

/*
 * This function parses tbsCertificate.signature field and populate ctx with useful
 * information on success.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (alg != NULL) ==> \valid(alg);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(cert+(..),alg,ctx,eaten);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \valid_read(*alg);
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_oid_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_oid_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->sig_alg));
  @ ensures (\result == 0) ==> \initialized(&(ctx->hash_alg));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_len));
  @ ensures (\result == 0) ==> ctx->tbs_sig_alg_start == off;
  @ ensures (\result == 0) ==> \exists integer x; 0 <= x < num_known_sig_algs && *alg == known_sig_algs[x];
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @
  @ assigns *alg, *eaten, *ctx;
  @*/
static int parse_x509_tbsCert_sig_AlgorithmIdentifier(cert_parsing_ctx *ctx,
						      const u8 *cert, u32 off, u32 len,
						      const _sig_alg **alg,
						      u32 *eaten)
{
	const _sig_alg *talg = NULL;
	const u8 *buf = cert + off;
	u32 saved_off = off;
	u32 parsed = 0;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 param_len;
	u32 oid_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	parsed = hdr_len + data_len;
	/*@ assert (1 < parsed <= len); */

	buf += hdr_len;
	off += hdr_len;

	/* The first thing we should find in the sequence is an OID */
	ret = parse_OID(buf, data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now see if that OID is one associated w/ an alg we support */
	talg = find_sig_alg_by_oid(buf, oid_len);
	/*@ assert (talg != NULL) ==> \exists integer i ; 0 <= i < num_known_sig_algs && talg == known_sig_algs[i]; */
	if (talg == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert \exists integer i ; 0 <= i < num_known_sig_algs && talg == known_sig_algs[i]; */
	/*@ assert \valid_read(talg); */

	/*
	 * Record position of spki alg oid. Also keep track of
	 * the signature mechanism alg identifier and possibly
	 * the associated hash identifier if it was explicit in
	 * the mechanism. Otherwise, we will temporarily inherit
	 * from HASH_ALG_UNKNOWN and possibly get the hash during
	 * the parsing of alg oid sig parameters just below.
	 */
	ctx->tbs_sig_alg_oid_start = off;
	ctx->tbs_sig_alg_oid_len = oid_len;
	ctx->sig_alg = talg->sig_id;
	ctx->hash_alg = talg->hash_id;

	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;

	/*@ assert talg->parse_algoid_sig_params \in {
		  parse_algoid_sig_params_ecdsa_with,
		  parse_algoid_sig_params_ecdsa_with_specified,
		  parse_algoid_sig_params_sm2,
		  parse_algoid_sig_params_eddsa,
		  parse_algoid_sig_params_rsa,
		  parse_algoid_sig_params_rsassa_pss,
		  parse_algoid_sig_params_none,
		  parse_algoid_sig_params_bign_with_hspec }; @*/
	/*@ calls parse_algoid_sig_params_ecdsa_with,
		  parse_algoid_sig_params_ecdsa_with_specified,
		  parse_algoid_sig_params_sm2,
		  parse_algoid_sig_params_eddsa,
		  parse_algoid_sig_params_rsa,
		  parse_algoid_sig_params_rsassa_pss,
		  parse_algoid_sig_params_none,
		  parse_algoid_sig_params_bign_with_hspec; @*/
	ret = talg->parse_algoid_sig_params(&ctx->sig_alg_params, &ctx->hash_alg, cert, off, param_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * parsing went ok; we have verified that nothing remains behind, i.e.
	 * that 'param_len' was indeed the length of the parameters.
	 */
	ctx->tbs_sig_alg_oid_params_start = off;
	ctx->tbs_sig_alg_oid_params_len = param_len;

	/*@ assert \exists integer i ; 0 <= i < num_known_sig_algs && talg == known_sig_algs[i]; */
	*alg = talg;
	*eaten = parsed;
	ctx->tbs_sig_alg_start = saved_off;
	ctx->tbs_sig_alg_len = parsed;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (alg != NULL) ==> \valid(alg);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(cert+(..),alg,eaten,ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> \exists integer i ; 0 <= i < num_known_pubkey_algs && *alg == known_pubkey_algs[i];
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \valid_read(*alg);
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg));
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_oid_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->spki_alg_oid_len));
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len));
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve));
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_bign ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571;
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len));
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_ec ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571;
  @ ensures (\result == 0) && (*alg)->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve));
  @
  @ assigns *alg, *eaten, *ctx;
  @*/
static int parse_x509_pubkey_AlgorithmIdentifier(cert_parsing_ctx *ctx,
						 const u8 *cert, u32 off, u32 len,
						 const _pubkey_alg **alg,
						 u32 *eaten)
{
	const _pubkey_alg *talg = NULL;
	const u8 *buf = cert + off;
	u32 saved_off = off;
	u32 parsed = 0;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 param_len;
	u32 oid_len = 0;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert 1 < (hdr_len + data_len) <= len; */
	buf += hdr_len;
	off += hdr_len;

	/* The first thing we should find in the sequence is an OID */
	ret = parse_OID(buf, data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* record position of spki alg oid */
	ctx->spki_alg_oid_start = off;
	ctx->spki_alg_oid_len = oid_len;

	/* Let's see if that OID is one associated w/ a pubkey alg we support */
	talg = find_pubkey_alg_by_oid(buf, oid_len);
	if (talg == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert \exists integer i ; 0 <= i < num_known_pubkey_algs && talg == known_pubkey_algs[i]; */
	/*@ assert \valid_read(talg); */

	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;

	/*@ assert talg->parse_algoid_pubkey_params \in {
		   parse_algoid_pubkey_params_ecPublicKey,
		   parse_algoid_pubkey_params_ed25519,
		   parse_algoid_pubkey_params_ed448,
		   parse_algoid_pubkey_params_x25519,
		   parse_algoid_pubkey_params_x448,
		   parse_algoid_pubkey_params_rsa,
		   parse_algoid_pubkey_params_gost_r3410_2012_256,
		   parse_algoid_pubkey_params_gost_r3410_2012_512,
		   parse_algoid_pubkey_params_gost_r3410_2001,
		   parse_algoid_pubkey_params_gost_r3410_94,
		   parse_algoid_pubkey_params_dsa,
		   parse_algoid_pubkey_params_ea_rsa,
		   parse_algoid_pubkey_params_none,
		   parse_algoid_pubkey_params_bign}; @*/
	/*@ calls parse_algoid_pubkey_params_ecPublicKey,
		   parse_algoid_pubkey_params_ed25519,
		   parse_algoid_pubkey_params_ed448,
		   parse_algoid_pubkey_params_x25519,
		   parse_algoid_pubkey_params_x448,
		   parse_algoid_pubkey_params_rsa,
		   parse_algoid_pubkey_params_gost_r3410_2012_256,
		   parse_algoid_pubkey_params_gost_r3410_2012_512,
		   parse_algoid_pubkey_params_gost_r3410_2001,
		   parse_algoid_pubkey_params_gost_r3410_94,
		   parse_algoid_pubkey_params_dsa,
		   parse_algoid_pubkey_params_ea_rsa,
		   parse_algoid_pubkey_params_none,
		   parse_algoid_pubkey_params_bign; @*/
	ret = talg->parse_algoid_pubkey_params(ctx, cert, off, param_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_alg_oid_params_start = off;
	ctx->spki_alg_oid_params_len = param_len;

	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len)); */
	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_bign ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571; */
	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve)); */

	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_ecPublicKey ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len)); */
	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_ecPublicKey ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571; */
	/*@ assert talg->parse_algoid_pubkey_params == parse_algoid_pubkey_params_ecPublicKey ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve)); */

	/*@ assert talg->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len)); */
	/*@ assert talg->parse_pubkey == parse_pubkey_bign ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571; */
	/*@ assert talg->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve)); */

	/*@ assert talg->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len)); */
	/*@ assert talg->parse_pubkey == parse_pubkey_ec ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571; */
	/*@ assert talg->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve)); */

	parsed = hdr_len + data_len;
	/*@ assert 1 < parsed <= len; */
	*alg = talg;
	/*@ assert (*alg)->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len)); */
	/*@ assert (*alg)->parse_pubkey == parse_pubkey_bign ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571; */
	/*@ assert (*alg)->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve)); */

	/*@ assert (*alg)->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len)); */
	/*@ assert (*alg)->parse_pubkey == parse_pubkey_ec ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571; */
	/*@ assert (*alg)->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve)); */
	*eaten = parsed;
	/*@ assert 1 < *eaten <= len; */
	ctx->spki_alg = talg->pubkey_id;
	ctx->spki_alg_oid_start = saved_off;
	ctx->spki_alg_oid_len = parsed;

	ret = 0;

out:
	return ret;
}


/*
 * Verify Validity by checking it is indeed a sequence of two
 * valid UTCTime elements. Note that the function only perform
 * syntaxic checks on each element individually and does not
 * compare the two values together (e.g. to verify notBefore
 * is indeed before notAfter, etc).
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns *eaten, ctx->not_before, ctx->not_after;
  @*/
static int parse_x509_Validity(cert_parsing_ctx *ctx,
			       const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	const u8 *buf = cert + off;
	int ret;
	u32 hdr_len = 0;
	u32 remain = 0;
	u32 data_len = 0;
	u32 nb_len = 0, na_len = 0;
	u16 na_year = 0, nb_year = 0;
	u8 na_month = 0, na_day = 0, na_hour = 0, na_min = 0, na_sec = 0;
	u8 nb_month = 0, nb_day = 0, nb_hour = 0, nb_min = 0, nb_sec = 0;
	u8 t_type = 0;
	u64 not_after, not_before;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/* Parse notBefore */
	ret = parse_Time(buf, remain, &t_type, &nb_len, &nb_year, &nb_month,
			 &nb_day, &nb_hour, &nb_min, &nb_sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check valid time type was used for year value */
	ret = verify_correct_time_use(t_type, nb_year);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= nb_len;
	buf += nb_len;

	/* Parse notAfter */
	ret = parse_Time(buf, remain, &t_type, &na_len, &na_year, &na_month,
			 &na_day, &na_hour, &na_min, &na_sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check valid time type was used for year value */
	ret = verify_correct_time_use(t_type, na_year);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= na_len;
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * To export time to context we do not bother converting to unix
	 * but encode all the components on a u64 in the following way.
	 * this makes resulting not_after and not_before values comparable.
	 */

	/*@ assert na_year <= 9999; */
	/*@ assert na_month <= 12; */
	/*@ assert na_day <= 31; */
	/*@ assert na_hour <= 23; */
	/*@ assert na_min <= 59; */
	/*@ assert na_sec <= 59; */
	not_after = time_components_to_comparable_u64(na_year, na_month, na_day,
						      na_hour, na_min, na_sec);


	/*@ assert nb_year <= 9999; */
	/*@ assert nb_month <= 12; */
	/*@ assert nb_day <= 31; */
	/*@ assert nb_hour <= 23; */
	/*@ assert nb_min <= 59; */
	/*@ assert nb_sec <= 59; */
	not_before = time_components_to_comparable_u64(nb_year, nb_month, nb_day,
						       nb_hour, nb_min, nb_sec);

	if (not_before >= not_after) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->not_before = not_before;
	ctx->not_after = not_after;
	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/* SubjectPublicKeyInfo,
 *
 *    SubjectPublicKeyInfo  ::=  SEQUENCE  {
 *         algorithm            AlgorithmIdentifier,
 *         subjectPublicKey     BIT STRING  }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_subjectPublicKeyInfo(cert_parsing_ctx *ctx,
					   const u8 *cert, u32 off, u32 len,
					   u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, parsed = 0, remain = 0;
	u32 saved_off = off;
	const u8 *buf = cert + off;
	const _pubkey_alg *alg = NULL;
	int ret;

	if ((ctx == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;
	off += hdr_len;

	ret = parse_x509_pubkey_AlgorithmIdentifier(ctx, cert, off, remain,
						    &alg, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert alg->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve_order_bit_len)); */
	/*@ assert alg->parse_pubkey == parse_pubkey_ec ==> ctx->spki_alg_params.bign.curve_order_bit_len <= 571; */
	/*@ assert alg->parse_pubkey == parse_pubkey_bign ==> \initialized(&(ctx->spki_alg_params.bign.curve)); */
	/*@ assert alg->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve_order_bit_len)); */
	/*@ assert alg->parse_pubkey == parse_pubkey_ec ==> ctx->spki_alg_params.ecpubkey.curve_order_bit_len <= 571; */
	/*@ assert alg->parse_pubkey == parse_pubkey_ec ==> \initialized(&(ctx->spki_alg_params.ecpubkey.curve)); */

	buf += parsed;
	remain -= parsed;
	off += parsed;

	/*
	 * Let's now check if subjectPublicKey is ok based on the
	 * algorithm and parameters we found.
	 */
	if (!alg->parse_pubkey) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert alg->parse_pubkey \in {
		  parse_pubkey_ed448,
		  parse_pubkey_x448,
		  parse_pubkey_ed25519,
		  parse_pubkey_x25519,
		  parse_pubkey_ec,
		  parse_pubkey_rsa,
		  parse_pubkey_gostr3410_94,
		  parse_pubkey_gostr3410_2001,
		  parse_pubkey_gostr3410_2012_256,
		  parse_pubkey_gostr3410_2012_512,
		  parse_pubkey_dsa,
		  parse_pubkey_bign } ; @*/
	/*@ calls parse_pubkey_ed448,
		  parse_pubkey_x448,
		  parse_pubkey_ed25519,
		  parse_pubkey_x25519,
		  parse_pubkey_ec,
		  parse_pubkey_rsa,
		  parse_pubkey_gostr3410_94,
		  parse_pubkey_gostr3410_2001,
		  parse_pubkey_gostr3410_2012_256,
		  parse_pubkey_gostr3410_2012_512,
		  parse_pubkey_dsa,
		  parse_pubkey_bign ; @*/
	ret = alg->parse_pubkey(ctx, cert, off, remain);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->spki_pub_key_start = off;
	ctx->spki_pub_key_len = remain;
	ctx->spki_start = saved_off;
	ctx->spki_len = hdr_len + data_len;
	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}


/*
 * Extensions -- optional
 */


#if 0
/* WIP! */

/*
 * RFC 5280 has "When the subjectAltName extension contains a domain name
 * system label, the domain name MUST be stored in the dNSName (an
 * IA5String). The name MUST be in the "preferred name syntax", as
 * specified by Section 3.5 of [RFC1034] and as modified by Section 2.1
 * of [RFC1123]. In addition, while the string " " is a legal domain name,
 * subjectAltName extensions with a dNSName of " " MUST NOT be used."
 *                                                              |
 * This function implements that checks, namely:                |
 *                                                              |
 * From 3.5 of RFC 1034:                                        |
 *                                                              |
 *    <domain> ::= <subdomain> | " "          " " not allowed --+
 *
 *    <subdomain> ::= <label> | <subdomain> "." <label>
 *
 *    <label> ::= <letter> [ [ <ldh-str> ] <let-dig> ]
 *
 *    <ldh-str> ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
 *
 *    <let-dig-hyp> ::= <let-dig> | "-"
 *
 *    <let-dig> ::= <letter> | <digit>
 *
 *    <letter> ::= any one of the 52 alphabetic characters A through Z in
 *    upper case and a through z in lower case
 *
 *    <digit> ::= any one of the ten digits 0 through 9
 *
 *    Note that while upper and lower case letters are allowed in domain
 *    names, no significance is attached to the case.  That is, two names with
 *    the same spelling but different case are to be treated as if identical.
 *
 *    The labels must follow the rules for ARPANET host names.  They must
 *    start with a letter, end with a letter or digit, and have as interior
 *    characters only letters, digits, and hyphen.  There are also some
 *    restrictions on the length.  Labels must be 63 characters or less.
 *
 * From 2.1 of RFC 1123:
 *
 *    The syntax of a legal Internet host name was specified in RFC-952
 *    [DNS:4].  One aspect of host name syntax is hereby changed: the
 *    restriction on the first character is relaxed to allow either a
 *    letter or a digit.  Host software MUST support this more liberal
 *    syntax.
 *
 *    Host software MUST handle host names of up to 63 characters and
 *    SHOULD handle host names of up to 255 characters.
 *
 *    Whenever a user inputs the identity of an Internet host, it SHOULD
 *    be possible to enter either (1) a host domain name or (2) an IP
 *    address in dotted-decimal ("#.#.#.#") form.  The host SHOULD check
 *    the string syntactically for a dotted-decimal number before
 *    looking it up in the Domain Name System.
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_prefered_name_syntax(const u8 *buf, u32 len)
{

	/* FIXME! */

	ret = 0;

out:
	return ret;
}
#endif




/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (critical != 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_AIA(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
			 const u8 *cert, u32 off, u32 len, int critical)
{
	int ret;

	if (ctx == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_AIA(cert, off, len, critical);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}


/* 4.2.1.1. Authority Key Identifier
 *
 * The identification MAY be based on either the key identifier (the subject
 * key identifier in the issuer's certificate) or the issuer name and serial
 * number.
 *
 *   id-ce-authorityKeyIdentifier OBJECT IDENTIFIER ::=  { id-ce 35 }
 *
 *   AuthorityKeyIdentifier ::= SEQUENCE {
 *      keyIdentifier             [0] KeyIdentifier           OPTIONAL,
 *      authorityCertIssuer       [1] GeneralNames            OPTIONAL,
 *      authorityCertSerialNumber [2] CertificateSerialNumber OPTIONAL  }
 *      -- authorityCertIssuer and authorityCertSerialNumber MUST both
 *      -- be present or both be absent
 *
 *   KeyIdentifier ::= OCTET STRING
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->aki_has_keyIdentifier,
	    ctx->aki_keyIdentifier_start,
	    ctx->aki_keyIdentifier_len,
	    ctx->aki_has_generalNames_and_serial,
	    ctx->aki_generalNames_start,
	    ctx->aki_generalNames_len,
	    ctx->aki_serial_start,
	    ctx->aki_serial_len,
	    ctx->has_aki;
  @*/
static int parse_ext_AKI(cert_parsing_ctx *ctx,
			 const u8 *cert, u32 off, u32 len, int critical)
{
	u32 hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	u32 key_id_hdr_len = 0, key_id_data_len = 0, key_id_data_off = 0;
	u32 gen_names_off = 0, gen_names_len = 0;
	u32 cert_serial_off = 0, cert_serial_len = 0;
	u32 remain;
	u32 parsed = 0;
	int ret, has_keyIdentifier = 0, has_gen_names_and_serial = 0;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * As specified in section 4.2.1.1. of RFC 5280, it is recommended
	 * for conforming CA not to set the critical bit for AKI extension
	 */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are indeed dealing w/ a sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We should now find a KeyIdentifier or/and a couple of
	 * (GeneralNames, and CertificateSerialNumber).
	 */

	/*
	 * First, the KeyIdentifier if present (KeyIdentifier ::= OCTET STRING)
	 */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &key_id_hdr_len, &key_id_data_len);
	if (!ret) {
		/* An empty KeyIdentifier does not make any sense. Drop it! */
		if (!key_id_data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		key_id_data_off = off + key_id_hdr_len;
		buf += key_id_hdr_len + key_id_data_len;
		off += key_id_hdr_len + key_id_data_len;
		remain -= key_id_hdr_len + key_id_data_len;
		has_keyIdentifier = 1;
	}


	/*
	 * See if a (GeneralNames, CertificateSerialNumber) couple follows.
	 * GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName. We do
	 * not accept one w/o the other.
	 */
	ret = parse_GeneralNames(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
				 &parsed);
	if (!ret) {
		gen_names_off = off;
		gen_names_len = parsed;

		buf += parsed;
		off += parsed;
		remain -= parsed;

		/* CertificateSerialNumber ::= INTEGER */
		ret = parse_AKICertSerialNumber(cert, off, remain,
						CLASS_CONTEXT_SPECIFIC, 2,
						&cert_serial_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert cert_serial_len > 2; */

		has_gen_names_and_serial = 1;
		cert_serial_off = off;

		buf += cert_serial_len;
		off += cert_serial_len;
		remain -= cert_serial_len;
	}

	/* Nothing should remain behind */
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Only populate context when we know everything is ok. */
	ctx->aki_has_keyIdentifier = has_keyIdentifier;
	/*@ assert \initialized(&ctx->aki_has_keyIdentifier); */
	if (ctx->aki_has_keyIdentifier) {
		ctx->aki_keyIdentifier_start = key_id_data_off;
		/*@ assert \initialized(&ctx->aki_keyIdentifier_start); */
		ctx->aki_keyIdentifier_len = key_id_data_len;
		/*@ assert \initialized(&ctx->aki_keyIdentifier_len); */
	}
	ctx->aki_has_generalNames_and_serial = has_gen_names_and_serial;
	/*@ assert \initialized(&ctx->aki_has_generalNames_and_serial); */
	if (ctx->aki_has_generalNames_and_serial) {
		ctx->aki_generalNames_start = gen_names_off;
		/*@ assert \initialized(&ctx->aki_generalNames_start); */
		ctx->aki_generalNames_len = gen_names_len;
		/*@ assert \initialized(&ctx->aki_generalNames_len); */
		ctx->aki_serial_start = cert_serial_off + 2;  /* 2 bytes long hdr for a valid SN */
		/*@ assert \initialized(&ctx->aki_serial_start); */
		ctx->aki_serial_len = cert_serial_len - 2;
		/*@ assert \initialized(&ctx->aki_serial_len); */
	}
	ctx->has_aki = 1;
	/*@ assert \initialized(&ctx->has_aki); */
	ret = 0;

out:
	return ret;
}

/*
 * 4.2.1.2. Subject Key Identifier
 *
 * SubjectKeyIdentifier ::= KeyIdentifier
 * KeyIdentifier ::= OCTET STRING
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx,cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_ski,
	    ctx->ski_start,
	    ctx->ski_len;
  @*/
static int parse_ext_SKI(cert_parsing_ctx *ctx,
			 const u8 *cert, u32 off, u32 len, int critical)
{
	u32 key_id_hdr_len = 0, key_id_data_len = 0;
	const u8 *buf = cert + off;
	u32 remain;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	remain = len;

	/*
	 * As specified in section 4.2.1.1. of RFC 5280, conforming CA
	 * must mark this extension as non-critical.
	 */
#ifdef TEMPORARY_LAXIST_SKI_CRITICAL_FLAG_SET
	(void)critical;
#else
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#endif

	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &key_id_hdr_len, &key_id_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len != (key_id_hdr_len + key_id_data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* An empty KeyIdentifier does not make any sense. Drop it! */
	if (!key_id_data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= key_id_hdr_len + key_id_data_len;
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->has_ski = 1;
	ctx->ski_start = off + key_id_hdr_len;
	ctx->ski_len = key_id_data_len;
	ret = 0;

out:
	return ret;
}


/* 4.2.1.3. Key Usage
 *
 * id-ce-keyUsage OBJECT IDENTIFIER ::=  { id-ce 15 }
 *
 * KeyUsage ::= BIT STRING {
 *      digitalSignature        (0),
 *      nonRepudiation          (1), -- recent editions of X.509 have
 *                           -- renamed this bit to contentCommitment
 *      keyEncipherment         (2),
 *      dataEncipherment        (3),
 *      keyAgreement            (4),
 *      keyCertSign             (5),
 *      cRLSign                 (6),
 *      encipherOnly            (7),
 *      decipherOnly            (8) }
 *
 */

/*
 * Masks for keyusage bits. Those masks are only usable on values
 * returned by parse_nine_bit_named_bit_list(), i.e. reversed
 * bits. Those not already used in the code are commented to avoid
 * unused macros and make clang compiler happy.
 */
//#define KU_digitalSignature  0x0001
//#define KU_nonRepudiation    0x0002
//#define KU_keyEncipherment   0x0004
//#define KU_dataEncipherment  0x0008
#define KU_keyAgreement      0x0010
#define KU_keyCertSign       0x0020
#define KU_cRLSign           0x0040
#define KU_encipherOnly      0x0080
#define KU_decipherOnly      0x0100

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_keyUsage,
	    ctx->keyCertSign_set,
	    ctx->cRLSign_set;
  @*/
static int parse_ext_keyUsage(cert_parsing_ctx *ctx,
			      const u8 *cert, u32 off, u32 len,
			      int ATTRIBUTE_UNUSED critical)
{
	u32 hdr_len = 0, data_len = 0;
	u16 val = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

       /*
	 * As specified in section 4.2.1.3 of RFC 5280, when the extension
	 * is present, "conforming CAs SHOULD mark this extension as
	 * critical." For that reason, and because various CA emit certificates
	 * with critical bit not set, we do not enforce critical bit value.
	 */

	/* Check we are indeed dealing w/ a bit string */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
				   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	len -= hdr_len;

	/*
	 * As expected in section 4.2.1.3 of RFC 5280, the function will
	 * enforce that at least one bit is set : "When the keyUsage extension
	 * appears in a certificate, at least one of the bits MUST be set to 1"
	 */
	ret = parse_nine_bit_named_bit_list(buf, data_len, &val);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Section 4.2.1.3 of RFC 5280 has: "The meaning of the decipherOnly
	 * bit is undefined in the absence of the keyAgreement bit". We
	 * consider it invalid to have the former but not the latter.
	 */
	if ((val & KU_decipherOnly) && !(val & KU_keyAgreement)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Section 4.2.1.3 of RFC 5280 has: "The meaning of the decipherOnly
	 * bit is undefined in the absence of the keyAgreement bit". We
	 * consider it invalid to have the former but not the latter.
	 */
	if ((val & KU_encipherOnly) && !(val & KU_keyAgreement)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->has_keyUsage = 1;
	ctx->keyCertSign_set = !!(val & KU_keyCertSign);
	ctx->cRLSign_set = !!(val & KU_cRLSign);

	ret = 0;

out:
	return ret;
}

/* CPSuri ::= IA5String */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten,buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_CPSuri(const u8 *buf, u32 len, u32 *eaten)
{
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_ia5_string(buf, len, 1, 65534);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = len;

	ret = 0;

out:
	return ret;
}


/*
 *     NoticeReference ::= SEQUENCE {
 *          organization     DisplayText,
 *          noticeNumbers    SEQUENCE OF INTEGER }
 *
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_NoticeReference(const u8 *buf, u32 len, u32 *eaten)
{
	u32 remain, parsed = 0, saved_len = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/* NoticeReference is a sequence */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	saved_len = hdr_len + data_len;
	remain = data_len;
	buf += hdr_len;

	/*
	 * First element of the sequence is the organization (of type
	 * DisplayText)
	 */
	ret = parse_DisplayText(buf, remain, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= parsed;
	buf += parsed;

	/*
	 * Second element is the noticeNumbers, i.e. a sequence of
	 * integers.
	 */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= hdr_len;
	buf += hdr_len;

	/* Advertised data in the sequence must exactly match what remains */
	if (remain != data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* The sequence contains integers */
	/*@
	  @ loop assigns ret, buf, remain, parsed, hdr_len, data_len;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		/* Verify the integer is encoded as it should */
		ret = parse_integer(buf, remain,
				    CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				    &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		parsed = hdr_len + data_len;
		remain -= parsed;
		buf += parsed;
	}

	*eaten = saved_len;

	ret = 0;

out:
	return ret;
}

/*
 *     UserNotice ::= SEQUENCE {
 *          noticeRef        NoticeReference OPTIONAL,
 *          explicitText     DisplayText OPTIONAL }
 *
 *     NoticeReference ::= SEQUENCE {
 *          organization     DisplayText,
 *          noticeNumbers    SEQUENCE OF INTEGER }
 *
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_UserNotice(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, remain = 0, parsed = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/* USerNotice is a sequence */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= hdr_len;
	buf += hdr_len;

	/* Having an empty sequence is considered invalid */
	if (!data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* First element (if present) is a noticeRef of type NoticeReference */
	ret = parse_NoticeReference(buf, remain, &parsed);
	if (!ret) {
		remain -= parsed;
		buf += parsed;
	}

	/* Second element (if present) is an explicitText of type DisplayText */
	ret = parse_DisplayText(buf, remain, &parsed);
	if (!ret) {
		remain -= parsed;
		buf += parsed;
	}

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 *   PolicyQualifierInfo ::= SEQUENCE {
 *        policyQualifierId  PolicyQualifierId,
 *        qualifier          ANY DEFINED BY policyQualifierId }
 *
 *   -- policyQualifierIds for Internet policy qualifiers
 *
 *   id-qt          OBJECT IDENTIFIER ::=  { id-pkix 2 }
 *   id-qt-cps      OBJECT IDENTIFIER ::=  { id-qt 1 }
 *   id-qt-unotice  OBJECT IDENTIFIER ::=  { id-qt 2 }
 *
 *   PolicyQualifierId ::= OBJECT IDENTIFIER ( id-qt-cps | id-qt-unotice )
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_policyQualifierInfo(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, oid_len = 0, remain = 0;
	u8 id_qt_cps_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05,
			       0x07, 0x02, 0x01 };
	u8 id_qt_unotice_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05,
				   0x07, 0x02, 0x02 };
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* It's a sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;

	/*
	 * First element of the sequence (policyQualifierId) is an OID
	 * which can either take a value of id-qt-cps or id-qt-unotice.
	 */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if ((oid_len == sizeof(id_qt_cps_oid)) &&
	    !bufs_differ(buf, id_qt_cps_oid, oid_len)) { /* id-qt-cps */
		u32 cpsuri_len = 0;

		buf += oid_len;
		remain -= oid_len;

		ret = parse_CPSuri(buf, remain, &cpsuri_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= cpsuri_len;
		buf += cpsuri_len;

	} else if ((oid_len == sizeof(id_qt_unotice_oid)) &&
	    !bufs_differ(buf, id_qt_unotice_oid, oid_len)) { /* id-qt-unotice */
		u32 cpsunotice_len = 0;

		buf += oid_len;
		remain -= oid_len;

		ret = parse_UserNotice(buf, remain, &cpsunotice_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= cpsunotice_len;
		buf += cpsunotice_len;

	} else {                                        /* unsupported! */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 *   PolicyInformation ::= SEQUENCE {
 *        policyIdentifier   CertPolicyId,
 *        policyQualifiers   SEQUENCE SIZE (1..MAX) OF
 *                                PolicyQualifierInfo OPTIONAL }
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten,buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_PolicyInformation(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, oid_len = 0, saved_pi_len, remain;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	saved_pi_len = hdr_len + data_len;

	remain = data_len;
	buf += hdr_len;

	/* policyIdentifier is a CertPolicyId, i.e. an OID */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= oid_len;
	buf += oid_len;

	/* policyQualifiers is optional */
	if (remain) {
		/* policyQualifiers is a sequence */
		ret = parse_id_len(buf, remain,
				   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= hdr_len;
		buf += hdr_len;

		/* Nothing should remain after policyQualifiers */
		if (remain != data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Let's parse each PolicyQualifierInfo in the
		 * policyQualifiers sequence
		 */
		/*@
		  @ loop assigns ret, buf, remain;
		  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
		  @ loop variant remain;
		  @ */
		while (remain) {
			u32 pqi_len = 0;

			ret = parse_policyQualifierInfo(buf, remain, &pqi_len);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			remain -= pqi_len;
			buf += pqi_len;
		}
	}

	*eaten = saved_pi_len;

	/*
	 * FIXME! At that point, we should verify we know the OID
	 * (policyIdentifier) and the associated optional
	 * content is indeed valid.
	 */

	ret = 0;

out:
	return ret;
}


/* 4.2.1.4. Certificate Policies
 *
 * id-ce-certificatePolicies OBJECT IDENTIFIER ::=  { id-ce 32 }
 *
 *   anyPolicy OBJECT IDENTIFIER ::= { id-ce-certificatePolicies 0 }
 *
 *   certificatePolicies ::= SEQUENCE SIZE (1..MAX) OF PolicyInformation
 *
 *   PolicyInformation ::= SEQUENCE {
 *        policyIdentifier   CertPolicyId,
 *        policyQualifiers   SEQUENCE SIZE (1..MAX) OF
 *                                PolicyQualifierInfo OPTIONAL }
 *
 *   CertPolicyId ::= OBJECT IDENTIFIER
 *
 *   PolicyQualifierInfo ::= SEQUENCE {
 *        policyQualifierId  PolicyQualifierId,
 *        qualifier          ANY DEFINED BY policyQualifierId }
 *
 *   -- policyQualifierIds for Internet policy qualifiers
 *
 *   id-qt          OBJECT IDENTIFIER ::=  { id-pkix 2 }
 *   id-qt-cps      OBJECT IDENTIFIER ::=  { id-qt 1 }
 *   id-qt-unotice  OBJECT IDENTIFIER ::=  { id-qt 2 }
 *
 *   PolicyQualifierId ::= OBJECT IDENTIFIER ( id-qt-cps | id-qt-unotice )
 *
 *   Qualifier ::= CHOICE {
 *        cPSuri           CPSuri,
 *        userNotice       UserNotice }
 *
 *   CPSuri ::= IA5String
 *
 *   UserNotice ::= SEQUENCE {
 *        noticeRef        NoticeReference OPTIONAL,
 *        explicitText     DisplayText OPTIONAL }
 *
 *   NoticeReference ::= SEQUENCE {
 *        organization     DisplayText,
 *        noticeNumbers    SEQUENCE OF INTEGER }
 *
 *   DisplayText ::= CHOICE {
 *        ia5String        IA5String      (SIZE (1..200)),
 *        visibleString    VisibleString  (SIZE (1..200)),
 *        bmpString        BMPString      (SIZE (1..200)),
 *        utf8String       UTF8String     (SIZE (1..200)) }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_certPolicies(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
				  const u8 *cert, u32 off, u32 len,
				  int ATTRIBUTE_UNUSED critical)
{
	u32 remain = 0, data_len = 0, hdr_len = 0, eaten = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * FIXME!
	 *
	 * This one will be a pain to deal with if we decide
	 * to support the full version, i.e. non empty sequence
	 * for policyQualifiersOID. RFC 5280 has:
	 *
	 *  To promote interoperability, this profile RECOMMENDS that policy
	 *  information terms consist of only an OID.	 Where an OID alone is
	 *  insufficient, this profile strongly recommends that the use of
	 *  qualifiers be limited to those identified in this section.  When
	 *  qualifiers are used with the special policy anyPolicy, they MUST be
	 *  limited to the qualifiers identified in this section.  Only those
	 *  qualifiers returned as a result of path validation are considered.
	 *
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now check each individual PolicyInformation sequence */
	/*@
	  @ loop assigns ret, buf, remain, eaten, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_PolicyInformation(buf, remain, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= eaten;
		off += eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}

/*
 * 4.2.1.5. Policy Mappings
 *
 * id-ce-policyMappings OBJECT IDENTIFIER ::=  { id-ce 33 }
 *
 * PolicyMappings ::= SEQUENCE SIZE (1..MAX) OF SEQUENCE {
 *      issuerDomainPolicy      CertPolicyId,
 *      subjectDomainPolicy     CertPolicyId }
 *
 * CertPolicyId ::= OBJECT IDENTIFIER
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx,cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_policyMapping(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
				   const u8 *cert, u32 off, u32 len,
				   int critical)
{
	u32 remain = 0, data_len = 0, hdr_len = 0, eaten = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * As specified in section 4.2.1.5. of RFC 5280, "conforming CAs
	 * SHOULD mark this extension as critical".
	 */
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now check each sequence of {issuer,subject}DomainPolicy pair */
	/*@
	  @ loop assigns ret, buf, remain, hdr_len, data_len, eaten, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_id_len(buf, remain,
				   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;

		/* issuerDomainPolicy (an OID)*/
		ret = parse_OID(buf, data_len, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += eaten;
		off += eaten;
		remain -= eaten;
		data_len -= eaten;

		/* subjectDomainPolicy (an OID) */
		ret = parse_OID(buf, data_len, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		data_len -= eaten;
		if (data_len) {
			/*
			 * Nothing should follow the two OIDs
			 * in the sequence.
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += eaten;
		off += eaten;
		remain -=  eaten;
	}

	ret = 0;

out:
	return ret;
}


/*  4.2.1.6. Subject Alternative Name
 *
 *   id-ce-subjectAltName OBJECT IDENTIFIER ::=  { id-ce 17 }
 *
 *   SubjectAltName ::= GeneralNames
 *
 *   GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName
 *
 *   GeneralName ::= CHOICE {
 *        otherName                       [0]     OtherName,
 *        rfc822Name                      [1]     IA5String,
 *        dNSName                         [2]     IA5String,
 *        x400Address                     [3]     ORAddress,
 *        directoryName                   [4]     Name,
 *        ediPartyName                    [5]     EDIPartyName,
 *        uniformResourceIdentifier       [6]     IA5String,
 *        iPAddress                       [7]     OCTET STRING,
 *        registeredID                    [8]     OBJECT IDENTIFIER }
 *
 *   OtherName ::= SEQUENCE {
 *        type-id    OBJECT IDENTIFIER,
 *        value      [0] EXPLICIT ANY DEFINED BY type-id }
 *
 *   EDIPartyName ::= SEQUENCE {
 *        nameAssigner            [0]     DirectoryString OPTIONAL,
 *        partyName               [1]     DirectoryString }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_san,
	    ctx->san_critical;
  @*/
static int parse_ext_SAN(cert_parsing_ctx *ctx,
			 const u8 *cert, u32 off, u32 len,
			 int critical)
{
	u32 data_len = 0, hdr_len = 0, remain = 0, eaten = 0;
	const u8 *buf = cert + off;
	int ret, empty_gen_name;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As specified in section 4.2.1.6. of RFC 5280, "if the subjectAltName
	 * extension is present, the sequence MUST contain at least one entry.
	 */
	if (!data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop assigns ret, buf, remain, eaten, empty_gen_name;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		empty_gen_name = 0;
		ret = parse_GeneralName(buf, remain, &eaten, &empty_gen_name);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Section 4.2.16 of RFC 5280 has "Unlike the subject field,
		 * conforming CAs MUST NOT issue certificates with
		 * subjectAltNames containing empty GeneralName fields.
		 */
		if (empty_gen_name) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * RFC5280 has: "When the subjectAltName extension contains an
		 * iPAddress, the address MUST be stored in the octet string in
		 * "network byte order", as specified in [RFC791].  The least
		 * significant bit (LSB) of each octet is the LSB of the
		 * corresponding byte in the network address.  For IP version
		 * 4, as specified in [RFC791], the octet string MUST contain
		 * exactly four octets. For IP version 6, as specified in
		 * [RFC2460], the octet string MUST contain exactly sixteen
		 * octets.
		 */
		if (buf[0] == NAME_TYPE_iPAddress) {
			switch (eaten) {
			case 6: /* id/len/IPv4(4 bytes) */
				break;
			case 18: /* id/len/IPv6(16 bytes) */
				break;
			default: /* invalid */
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
				break;
			}
		}

		remain -= eaten;
		buf += eaten;
	}

	/*
	 * Now that we know the extension is valid, let's record some
	 * useful info.
	 */
	ctx->has_san = 1;
	ctx->san_critical = critical;

	ret = 0;

out:
	return ret;
}

/* 4.2.1.7. Issuer Alternative Name */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_IAN(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
			 const u8 *cert, u32 off, u32 len,
			 int ATTRIBUTE_UNUSED critical)
{
	u32 data_len = 0, hdr_len = 0, remain = 0, eaten = 0;
	const u8 *buf = cert + off;
	int ret, unused = 0;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 4.2.1.7 of RFC 5280 has "Where present, conforming CAs
	 * SHOULD mark this extension as non-critical."
	 *
	 * FIXME! add a check?
	 */

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As specified in section 4.2.1.6. of RFC 5280, "if the subjectAltName
	 * extension is present, the sequence MUST contain at least one entry.
	 * Unlike the subject field, conforming CAs MUST NOT issue certificates
	 * with subjectAltNames containing empty GeneralName fields.
	 *
	 * The first check is done here.
	 *
	 * FIXME! second check remains to be done. Possibly in adding an additional
	 * out parameter to parse_GeneralName(), to tell if an empty one is
	 * empty. This is because
	 */
	if (!data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop assigns ret, buf, remain, eaten, unused, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_GeneralName(buf, remain, &eaten, &unused);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= eaten;
		off += eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}

/*
 * 4.2.1.8. Subject Directory Attributes
 *
 * id-ce-subjectDirectoryAttributes OBJECT IDENTIFIER ::=  { id-ce 9 }
 *
 * SubjectDirectoryAttributes ::= SEQUENCE SIZE (1..MAX) OF Attribute
 *
 * Attribute ::= SEQUENCE {
 *   type    AttributeType,
 *   values  SET OF AttributeValue
 *   -- at least one value is required --
 * }
 *
 * AttributeType           ::= OBJECT IDENTIFIER
 *
 * AttributeValue          ::= ANY -- DEFINED BY AttributeType
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_subjectDirAttr(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
				    const u8 *cert, u32 off, u32 len,
				    int critical)
{
	u32 hdr_len = 0, data_len = 0, oid_len = 0, remain = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As specified in section 4.2.1.8. of RFC 5280, conforming CAs
	 * MUST mark this extension as non-critical.
	 */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop assigns ret, buf, remain, hdr_len, data_len, oid_len, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		/* Parse current attribute. Each one is a sequence ... */
		ret = parse_id_len(buf, remain,
				   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;

		/* ... containing an OID (AttributeType) */
		ret = parse_OID(buf, data_len, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* FIXME! check the value depanding on the OID */

		remain -= data_len;
		off += data_len;
		buf += data_len;
	}

	ret = 0;

out:
	return ret;
}

/* 4.2.1.9. Basic Constraints
 *
 * id-ce-basicConstraints OBJECT IDENTIFIER ::=  { id-ce 19 }
 *
 * BasicConstraints ::= SEQUENCE {
 *      cA                      BOOLEAN DEFAULT FALSE,
 *      pathLenConstraint       INTEGER (0..MAX) OPTIONAL }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->bc_critical,
	    ctx->ca_true,
	    ctx->pathLenConstraint_set;
  @*/
static int parse_ext_basicConstraints(cert_parsing_ctx *ctx,
				      const u8 *cert, u32 off, u32 len,
				      int critical)
{
	u32 hdr_len = 0, data_len = 0;
	const u8 ca_true_wo_plc[] = { 0x01, 0x01, 0xff };
	const u8 ca_true_w_plc[] = { 0x01, 0x01, 0xff, 0x02, 0x01 };
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Record if basicConstraints extension was mared critical */
	ctx->bc_critical = critical;

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Only the following cases are valid/reasonable:
	 *
	 *  1) an empty sequence (cA default to false, resulting in
	 *    no pathLenConstraint): { }
	 *  2) cA is explicitly set to TRUE and no pathLenConstraint
	 *    is enforced. { 0x01, 0x01, 0xff }
	 *  3) cA is explicitly set to TRUE and a pathLenConstraint
	 *    is enforced, in which case it is reasonable to limit
	 *    allowed pathLenConstraint values to [0, 255]:
	 *    { 0x01, 0x01, 0xff, 0x02, 0x01, 0xXX }
	 *
	 * Note:
	 *
	 *  - encoding an explicit FALSE value for cA is invalid
	 *    because this is the default value.
	 *  - providing a pathLenConstraint w/o setting cA boolean
	 *    does not make sense
	 */
	switch (data_len) {
	case 0: /* empty sequence */
		ret = 0;
		break;
	case 3: /* no pathLenConstraint */
		/*
		 * We should indeed find a CA TRUE here. If this is
		 * the case everything is fine.
		 */
		ret = bufs_differ(buf, ca_true_wo_plc, 3);
		if (!ret) {
			ctx->ca_true = 1;
			break;
		}

		/*
		 * Here, we should directly leave w/o spending more
		 * time except if we were instructed to accept
		 * wrongdoing CAs asserting FALSE boolean for CA.
		 */
#ifdef TEMPORARY_LAXIST_CA_BASIC_CONSTRAINTS_BOOLEAN_EXPLICIT_FALSE
		{
			const u8 ca_false_explicit_wo_plc[] = { 0x01, 0x01, 0x00 };

			ret = bufs_differ(buf, ca_false_explicit_wo_plc, 3);
			if (!ret) {
				break;
			}
		}
#endif

		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	case 6: /* CA set, pathLenConstraint given ([0,127] allowed) */
		ret = bufs_differ(buf, ca_true_w_plc, 5);
		if (ret) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Section 4.2.1.9 of RFC 5280 has "Where it appears, the
		 * pathLenConstraint field MUST be greater than or equal
		 * to zero". We check MSB is not set, indicating it is
		 * positive.
		 */
		if (buf[5] & 0x80) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		ctx->ca_true = 1;
		ctx->pathLenConstraint_set = 1;
		break;
	default: /* crap */
		ret = -X509_FILE_LINE_NUM_ERR;
		break;
	}

out:
	return ret;
}

/* 4.2.1.10. Name Constraints */


/*
 * Parse GeneralSubtrees structure.
 *
 *    GeneralSubtree ::= SEQUENCE {
 *         base                    GeneralName,
 *         minimum         [0]     BaseDistance DEFAULT 0,
 *         maximum         [1]     BaseDistance OPTIONAL }
 *
 *    BaseDistance ::= INTEGER (0..MAX)
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_GeneralSubtrees(const u8 *buf, u32 len)
{
	u32 hdr_len = 0, remain = 0, grabbed = 0, data_len = 0;
	int ret, unused = 0;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/* base is a GeneralName */
	ret = parse_GeneralName(buf, remain, &grabbed, &unused);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += grabbed;
	remain -= grabbed;

	/*
	 * Section 4.2.1.10 of RFC5280 has "Within this profile, the minimum
	 * and maximum fields are not used with any name forms, thus, the
	 * minimum MUST be zero, ...
	 *
	 * Note: as the minum defaults to 0 in its definition, the field
	 * must be absent (i.e. cannot be present with a value of 0),
	 * as expected in DER encoding (11.5 of X.690 has: "the encoding of
	 * a set value or sequence value shall not include an encoding for
	 * any component value which is equal to its default value.)"
	 */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &hdr_len, &data_len);
	if (!ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* ... and maximum MUST be absent." */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
			   &hdr_len, &data_len);
	if (!ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Nothing should remain behind */
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}


/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_name_constraints;
  @*/
static int parse_ext_nameConstraints(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len, int critical)
{
	u32 remain = 0, hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 4.2.1.10 of RFC 5280 has "Conforming CAs MUST mark
	 * this extension as critical.
	 */
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &remain);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * 4.2.1.10 has: "Conforming CAs MUST NOT issue certificates
	 * where name constraints is an empty sequence
	 */
	if (!remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check if we have a permittedSubtrees structure */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &hdr_len, &data_len);
	if (!ret) {
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;

		ret = parse_GeneralSubtrees(buf, data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += data_len;
		off += data_len;
		remain -= data_len;
	}

	/* Check if we have an excludedSubtrees structure */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
			   &hdr_len, &data_len);
	if (!ret) {
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;

		ret = parse_GeneralSubtrees(buf, data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += data_len;
		off += data_len;
		remain -= data_len;
	}

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->has_name_constraints = 1;

	ret = 0;

out:
	return ret;
}

/*
 * 4.2.1.11. Policy Constraints
 *
 * id-ce-policyConstraints OBJECT IDENTIFIER ::=  { id-ce 36 }
 *
 *  PolicyConstraints ::= SEQUENCE {
 *       requireExplicitPolicy           [0] SkipCerts OPTIONAL,
 *       inhibitPolicyMapping            [1] SkipCerts OPTIONAL }
 *
 *  SkipCerts ::= INTEGER (0..MAX)
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_policyConstraints(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
				       const u8 *cert, u32 off, u32 len,
				       int critical)
{
	u32 data_len = 0, hdr_len = 0, remain = 0, parsed = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 4.2.1.11 of RFC 5280 has "Conforming CAs MUST mark this
	 * extension as critical".
	 */
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Section 4.2.1.11 of RFC 5280 has "Conforming CAs MUST NOT issue
	 * certificates where policy constraints is an empty sequence".
	 */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	/* Check if we have a requireExplicitPolicy */
	ret = parse_integer(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			    &hdr_len, &data_len);
	if (!ret) {
		/*
		 * As the value is expected to be a very small integer,
		 * content should be encoded on at most 1 byte, i.e.
		 * 'parsed' value should be 3 (w/ 2 bytes header).
		 */
		parsed = hdr_len + data_len;
		if (parsed != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		off += parsed;
		remain -= parsed;
	}

	/* Check if we have an inhibitPolicyMapping */
	ret = parse_integer(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
			    &hdr_len, &data_len);
	if (!ret) {
		/*
		 * As the value is expected to be a very small integer,
		 * content should be encoded on at most 1 byte, i.e.
		 * 'parsed' value should be 3 (w/ 2 bytes header).
		 */
		parsed = hdr_len + data_len;
		if (parsed != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		off += parsed;
		remain -= parsed;
	}

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
       return ret;
}

static const u8 _id_kp_anyEKU[] =       { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x00 };
static const u8 _id_kp_serverAuth[] =   { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x01 };
static const u8 _id_kp_clientAuth[] =   { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x02 };
static const u8 _id_kp_codeSigning[] =  { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x03 };
static const u8 _id_kp_emailProt[] =    { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x04 };
static const u8 _id_kp_timeStamping[] = { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x08 };
static const u8 _id_kp_OCSPSigning[] =  { 0x06, 0x08, 0x2b, 0x06,
					  0x01, 0x05, 0x05, 0x07,
					  0x03, 0x09 };
static const u8 _id_kp_ns_SGC[] = {  0x06, 0x09, 0x60, 0x86, 0x48,
				     0x01, 0x86, 0xF8, 0x42, 0x04,
				     0x01  };
static const u8 _id_kp_ms_SGC[] = {  0x06, 0x0A, 0x2B, 0x06, 0x01,
				     0x04, 0x01, 0x82, 0x37, 0x0A,
				     0x03, 0x03,   };


typedef struct {
	const u8 *oid;
	u8 oid_len;
} _kp_oid;

static const _kp_oid known_kp_oids[] = {
	{ .oid = _id_kp_anyEKU,
	  .oid_len = sizeof(_id_kp_anyEKU),
	},
	{ .oid = _id_kp_serverAuth,
	  .oid_len = sizeof(_id_kp_serverAuth),
	},
	{ .oid = _id_kp_clientAuth,
	  .oid_len = sizeof(_id_kp_clientAuth),
	},
	{ .oid = _id_kp_codeSigning,
	  .oid_len = sizeof(_id_kp_codeSigning),
	},
	{ .oid = _id_kp_emailProt,
	  .oid_len = sizeof(_id_kp_emailProt),
	},
	{ .oid = _id_kp_timeStamping,
	  .oid_len = sizeof(_id_kp_timeStamping),
	},
	{ .oid = _id_kp_OCSPSigning,
	  .oid_len = sizeof(_id_kp_OCSPSigning),
	},
	{ .oid = _id_kp_ns_SGC,
	  .oid_len = sizeof(_id_kp_ns_SGC),
	},
	{ .oid = _id_kp_ms_SGC,
	  .oid_len = sizeof(_id_kp_ms_SGC),
	},
};

const u16 num_known_kp_oids = (sizeof(known_kp_oids) / sizeof(known_kp_oids[0]));

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < num_known_kp_oids && \result == &known_kp_oids[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
static const _kp_oid * find_kp_by_oid(const u8 *buf, u32 len)
{
	const _kp_oid *found = NULL;
	const _kp_oid *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll num_known_kp_oids ;
	  @ loop invariant 0 <= k <= num_known_kp_oids;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (num_known_kp_oids - k);
	  @*/
	for (k = 0; k < num_known_kp_oids; k++) {
		int ret;

		cur = &known_kp_oids[k];

		/*@ assert cur == &known_kp_oids[k];*/
		if (cur->oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->oid, buf, cur->oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*
 * 4.2.1.12. Extended Key Usage
 *
 *    id-ce-extKeyUsage OBJECT IDENTIFIER ::= { id-ce 37 }
 *
 *   ExtKeyUsageSyntax ::= SEQUENCE SIZE (1..MAX) OF KeyPurposeId
 *
 *   KeyPurposeId ::= OBJECT IDENTIFIER
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_eku;
  @*/
static int parse_ext_EKU(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
			 const u8 *cert, u32 off, u32 len,
			 int critical)
{
	u32 remain = 0, data_len = 0, hdr_len = 0, oid_len = 0;
	const u8 *buf = cert + off;
	const _kp_oid *kp = NULL;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret || (data_len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now check each individual KeyPurposeId in the sequence */
	/*@
	  @ loop assigns ret, oid_len, kp, buf, remain, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_OID(buf, remain, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		kp = find_kp_by_oid(buf, oid_len);
		if (kp == NULL) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * RFC5280 sect 4.2.1.12 contains "Conforming CAs SHOULD NOT
		 * mark this extension as critical if the anyExtendedKeyUsage
		 * KeyPurposeId is present." We enforce this expected behavior."
		 */
		if ((kp->oid == _id_kp_anyEKU) && critical) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += oid_len;
		off += oid_len;
		remain -= oid_len;
	}

	ctx->has_eku = 1;

	ret = 0;

out:
	return ret;
}


/*
 * 4.2.1.13. CRL Distribution Points.
 *
 *     CRLDistributionPoints ::= SEQUENCE SIZE (1..MAX) OF DistributionPoint
 *
 * Note that the Freshest CRL extension uses the exact same syntax and
 * convention as CRLDP extension. The only minor difference is that section
 * 4.2.1.13 has that "The extension SHOULD be non-critical" and section
 * 4.2.1.15 has that "The extension MUST be marked as non-critical by
 * conforming CAs". This is the main difference between the function below
 * and parse_crl_ext_FreshestCRL().
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_crldp,
	    ctx->one_crldp_has_all_reasons;
  @*/
static int parse_ext_CRLDP(cert_parsing_ctx *ctx,
			   const u8 *cert, u32 off, u32 len,
			   int ATTRIBUTE_UNUSED critical)
{
	u32 hdr_len = 0, data_len = 0, remain;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->has_crldp = 1;
	ctx->one_crldp_has_all_reasons = 0;

	/* Iterate on DistributionPoint */
	/*@
	  @ loop assigns ret, buf, remain, ctx->one_crldp_has_all_reasons;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		int crldp_has_all_reasons = 0;
		u32 eaten = 0;

		ret = parse_DistributionPoint(buf, remain,
					      &crldp_has_all_reasons, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		if (crldp_has_all_reasons) {
			ctx->one_crldp_has_all_reasons = 1;
		}

		remain -= eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}

/*
 * 4.2.1.14. Inhibit anyPolicy
 *
 * InhibitAnyPolicy ::= SkipCerts
 *
 * SkipCerts ::= INTEGER (0..MAX)
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
#define MAX_INHIBITANYPOLICY 64
static int parse_ext_inhibitAnyPolicy(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
				      const u8 *cert, u32 off, u32 len,
				      int critical)
{
	const u8 *buf = cert + off;
	u32 eaten = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * 4.2.1.14 of RFC5280 has "Conforming CAs MUST mark this
	 * extension as critical".
	 */
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_integer(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
			    &hdr_len, &data_len);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	eaten = hdr_len + data_len;

	/*
	 * We limit SkipCerts values to integers between 0 and
	 * MAX_INHIBITANYPOLICY. This implies an encoding on 3 bytes.
	 */
	if (eaten != 3) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if ((buf[2] & 0x80) || (buf[2] > MAX_INHIBITANYPOLICY)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (eaten != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/* The OID we will support in final implementation */
static const u8 _ext_oid_AIA[] =               { 0x06, 0x08, 0x2b, 0x06, 0x01,
						 0x05, 0x05, 0x07, 0x01, 0x01 };
static const u8 _ext_oid_subjectDirAttr[] =    { 0x06, 0x03, 0x55, 0x1d, 0x09 };
static const u8 _ext_oid_SKI[] =               { 0x06, 0x03, 0x55, 0x1d, 0x0e };
static const u8 _ext_oid_keyUsage[] =          { 0x06, 0x03, 0x55, 0x1d, 0x0f };
static const u8 _ext_oid_SAN[] =               { 0x06, 0x03, 0x55, 0x1d, 0x11 };
static const u8 _ext_oid_IAN[] =               { 0x06, 0x03, 0x55, 0x1d, 0x12 };
static const u8 _ext_oid_basicConstraints[] =  { 0x06, 0x03, 0x55, 0x1d, 0x13 };
static const u8 _ext_oid_nameConstraints[] =   { 0x06, 0x03, 0x55, 0x1d, 0x1e };
static const u8 _ext_oid_CRLDP[] =             { 0x06, 0x03, 0x55, 0x1d, 0x1f };
static const u8 _ext_oid_certPolicies[] =      { 0x06, 0x03, 0x55, 0x1d, 0x20 };
static const u8 _ext_oid_policyMapping[] =     { 0x06, 0x03, 0x55, 0x1d, 0x21 };
static const u8 _ext_oid_AKI[] =               { 0x06, 0x03, 0x55, 0x1d, 0x23 };
static const u8 _ext_oid_policyConstraints[] = { 0x06, 0x03, 0x55, 0x1d, 0x24 };
static const u8 _ext_oid_EKU[] =               { 0x06, 0x03, 0x55, 0x1d, 0x25 };
static const u8 _ext_oid_FreshestCRL[] =       { 0x06, 0x03, 0x55, 0x1d, 0x2e };
static const u8 _ext_oid_inhibitAnyPolicy[] =  { 0x06, 0x03, 0x55, 0x1d, 0x36 };

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_ext_bad_oid(cert_parsing_ctx ATTRIBUTE_UNUSED *ctx,
			     const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len,
			     int ATTRIBUTE_UNUSED critical)
{
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

#ifdef TEMPORARY_LAXIST_HANDLE_COMMON_UNSUPPORTED_EXT_OIDS
/*
 * Some common OID for which we DO NOT CURRENTLY support data parsing but may
 * include for tests only to progress in certificates and improve code coverage
 */
static const u8 _ext_oid_bad_ct1[] = {
	0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07,
	0x01, 0x01
};
static const u8 _ext_oid_bad_ct_poison[] = {
	0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0xd6,
	0x79, 0x02, 0x04, 0x03
};
static const u8 _ext_oid_bad_ct_enabled[] = {
	0x06, 0x0a, 0x2b, 0x06,	 0x01, 0x04, 0x01, 0xd6,
	0x79, 0x02, 0x04, 0x02
};
static const u8 _ext_oid_bad_ns_cert_type[] = {
	0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x86, 0xf8,
	0x42, 0x01, 0x01
};
static const u8 _ext_oid_bad_szOID_ENROLL[] = {
	0x06, 0x09, 0x2b, 0x06,  0x01, 0x04, 0x01, 0x82,
	0x37, 0x14, 0x02
};
static const u8 _ext_oid_bad_smime_cap[] = {
	0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
	0x01, 0x09, 0x0f
};
static const u8 _ext_oid_bad_ns_comment[] = {
	0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x86, 0xf8,
	0x42, 0x01, 0x0d
};
static const u8 _ext_oid_bad_deprecated_AKI[] = {
	0x06, 0x03, 0x55, 0x1d, 0x01
};
static const u8 _ext_oid_bad_szOID_CERT_TEMPLATE[] = {
	0x06, 0x09, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82,
	0x37, 0x15, 0x07
};
static const u8 _ext_oid_bad_pkixFixes[] = {
	0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x97,
	0x55, 0x03, 0x01, 0x05
};
static const u8 _ext_oid_bad_ns_ca_policy_url[] = {
	0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x86, 0xf8,
	0x42, 0x01, 0x08
};
static const u8 _ext_oid_bad_szOID_CERTSRV_CA_VERS[] = {
	0x06, 0x09, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82,
	0x37, 0x15, 0x01
};
static const u8 _ext_oid_bad_szOID_APP_CERT_POL[] = {
	0x06, 0x09, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82,
	0x37, 0x15, 0x0a
};
static const u8 _ext_oid_bad_priv_key_usage_period[] = {
	0x06, 0x03, 0x55, 0x1d, 0x10
};
static const u8 _ext_oid_bad_subject_signing_tool[] = {
	0x06, 0x05, 0x2a, 0x85,	0x03, 0x64, 0x6f
};
static const u8 _ext_oid_bad_issuer_signing_tool[] = {
	0x06, 0x05, 0x2a, 0x85,	0x03, 0x64, 0x70
};
static const u8 _ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH[] = {
	0x06, 0x09, 0x2b, 0x06,	0x01, 0x04, 0x01,
	0x82, 0x37, 0x15, 0x02
};
#endif

typedef struct {
	const u8 *oid;
	u32 oid_len;
	int (*parse_ext_params)(cert_parsing_ctx *ctx,
				const u8 *cert, u32 off, u32 len, int critical);
} _ext_oid;

static const _ext_oid generic_unsupported_ext_oid = {
	.oid = NULL,
	.oid_len = 0,
	.parse_ext_params = parse_ext_bad_oid
};

static const _ext_oid known_ext_oids[] = {
	{ .oid = _ext_oid_AIA,
	  .oid_len = sizeof(_ext_oid_AIA),
	  .parse_ext_params = parse_ext_AIA,
	},
	{ .oid = _ext_oid_AKI,
	  .oid_len = sizeof(_ext_oid_AKI),
	  .parse_ext_params = parse_ext_AKI,
	},
	{ .oid = _ext_oid_SKI,
	  .oid_len = sizeof(_ext_oid_SKI),
	  .parse_ext_params = parse_ext_SKI,
	},
	{ .oid = _ext_oid_keyUsage,
	  .oid_len = sizeof(_ext_oid_keyUsage),
	  .parse_ext_params = parse_ext_keyUsage,
	},
	{ .oid = _ext_oid_certPolicies,
	  .oid_len = sizeof(_ext_oid_certPolicies),
	  .parse_ext_params = parse_ext_certPolicies,
	},
	{ .oid = _ext_oid_policyMapping,
	  .oid_len = sizeof(_ext_oid_policyMapping),
	  .parse_ext_params = parse_ext_policyMapping,
	},
	{ .oid = _ext_oid_SAN,
	  .oid_len = sizeof(_ext_oid_SAN),
	  .parse_ext_params = parse_ext_SAN,
	},
	{ .oid = _ext_oid_IAN,
	  .oid_len = sizeof(_ext_oid_IAN),
	  .parse_ext_params = parse_ext_IAN,
	},
	{ .oid = _ext_oid_subjectDirAttr,
	  .oid_len = sizeof(_ext_oid_subjectDirAttr),
	  .parse_ext_params = parse_ext_subjectDirAttr,
	},
	{ .oid = _ext_oid_basicConstraints,
	  .oid_len = sizeof(_ext_oid_basicConstraints),
	  .parse_ext_params = parse_ext_basicConstraints,
	},
	{ .oid = _ext_oid_nameConstraints,
	  .oid_len = sizeof(_ext_oid_nameConstraints),
	  .parse_ext_params = parse_ext_nameConstraints,
	},
	{ .oid = _ext_oid_policyConstraints,
	  .oid_len = sizeof(_ext_oid_policyConstraints),
	  .parse_ext_params = parse_ext_policyConstraints,
	},
	{ .oid = _ext_oid_EKU,
	  .oid_len = sizeof(_ext_oid_EKU),
	  .parse_ext_params = parse_ext_EKU,
	},
	{ .oid = _ext_oid_CRLDP,
	  .oid_len = sizeof(_ext_oid_CRLDP),
	  .parse_ext_params = parse_ext_CRLDP,
	},
	{ .oid = _ext_oid_inhibitAnyPolicy,
	  .oid_len = sizeof(_ext_oid_inhibitAnyPolicy),
	  .parse_ext_params = parse_ext_inhibitAnyPolicy,
	},
	{ .oid = _ext_oid_FreshestCRL,
	  .oid_len = sizeof(_ext_oid_FreshestCRL),
	  .parse_ext_params = parse_ext_CRLDP,
	},
#ifdef TEMPORARY_LAXIST_HANDLE_COMMON_UNSUPPORTED_EXT_OIDS
	{ .oid = _ext_oid_bad_ct1,
	  .oid_len = sizeof(_ext_oid_bad_ct1),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_ct_poison,
	  .oid_len = sizeof(_ext_oid_bad_ct_poison),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_ct_enabled,
	  .oid_len = sizeof(_ext_oid_bad_ct_enabled),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_ns_cert_type,
	  .oid_len = sizeof(_ext_oid_bad_ns_cert_type),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_szOID_ENROLL,
	  .oid_len = sizeof(_ext_oid_bad_szOID_ENROLL),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_smime_cap,
	  .oid_len = sizeof(_ext_oid_bad_smime_cap),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_ns_comment,
	  .oid_len = sizeof(_ext_oid_bad_ns_comment),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_deprecated_AKI,
	  .oid_len = sizeof(_ext_oid_bad_deprecated_AKI),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_szOID_CERT_TEMPLATE,
	  .oid_len = sizeof(_ext_oid_bad_szOID_CERT_TEMPLATE),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_pkixFixes,
	  .oid_len = sizeof(_ext_oid_bad_pkixFixes),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_ns_ca_policy_url,
	  .oid_len = sizeof(_ext_oid_bad_ns_ca_policy_url),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_szOID_CERTSRV_CA_VERS,
	  .oid_len = sizeof(_ext_oid_bad_szOID_CERTSRV_CA_VERS),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_szOID_APP_CERT_POL,
	  .oid_len = sizeof(_ext_oid_bad_szOID_APP_CERT_POL),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_priv_key_usage_period,
	  .oid_len = sizeof(_ext_oid_bad_priv_key_usage_period),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_subject_signing_tool,
	  .oid_len = sizeof(_ext_oid_bad_subject_signing_tool),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_issuer_signing_tool,
	  .oid_len = sizeof(_ext_oid_bad_issuer_signing_tool),
	  .parse_ext_params = parse_ext_bad_oid,
	},
	{ .oid = _ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH,
	  .oid_len = sizeof(_ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH),
	  .parse_ext_params = parse_ext_bad_oid,
	},
#endif
};

#define NUM_KNOWN_EXT_OIDS (sizeof(known_ext_oids) / sizeof(known_ext_oids[0]))

/*
 * We limit the amount of extensions we accept per certificate. This can be
 * done because each kind of extension is allowed to appear only once in a
 * given certificate. Note that it is logical to allow
 */
#define MAX_EXT_NUM_PER_CERT NUM_KNOWN_EXT_OIDS

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_EXT_OIDS && \result == &known_ext_oids[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
static _ext_oid const * find_ext_by_oid(const u8 *buf, u32 len)
{
	const _ext_oid *found = NULL;
	const _ext_oid *cur = NULL;
	u16 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll NUM_KNOWN_EXT_OIDS ;
	  @ loop invariant 0 <= k <= NUM_KNOWN_EXT_OIDS;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_EXT_OIDS - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_EXT_OIDS; k++) {
		int ret;

		cur = &known_ext_oids[k];

		/*@ assert cur == &known_ext_oids[k];*/
		if (cur->oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->oid, buf, cur->oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*@
  @ requires ext != \null;
  @ requires (parsed_oid_list != NULL) ==> \valid(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1)));
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1)));
  @ requires \separated(ext, parsed_oid_list);
  @
  @ ensures \result <= 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CERT - 1)];
  @*/
static int check_record_ext_unknown(const _ext_oid *ext,
				    const _ext_oid **parsed_oid_list)
{
	u16 pos = 0;
	int ret;

	/*@
	  @ loop invariant pos <= MAX_EXT_NUM_PER_CERT;
	  @ loop assigns ret, pos, parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CERT - 1)];
	  @ loop variant MAX_EXT_NUM_PER_CERT - pos;
	  @*/
	while (pos < MAX_EXT_NUM_PER_CERT) {
		/*
		 * Check if we are at the end of already seen extensions. In
		 * that case, record the extension as a new one.
		 */
		if (parsed_oid_list[pos] == NULL) {
			parsed_oid_list[pos] = ext;
			break;
		}

		if (ext == parsed_oid_list[pos]) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		pos += 1;
	}

	/*
	 * If we went to the end of our array, this means there are too many
	 * extensions in the certificate.
	 */
	if (pos >= MAX_EXT_NUM_PER_CERT) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Parse one extension.
 *
 *  Extension  ::=  SEQUENCE  {
 *       extnID      OBJECT IDENTIFIER,
 *       critical    BOOLEAN DEFAULT FALSE,
 *       extnValue   OCTET STRING
 *                   -- contains the DER encoding of an ASN.1 value
 *                   -- corresponding to the extension type identified
 *                   -- by extnID
 *       }
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (parsed_oid_list != NULL) ==> \valid(parsed_oid_list);
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1)));
  @ requires \separated(ctx, cert+(..),parsed_oid_list,eaten);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 <= *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CERT - 1)], *eaten, *ctx;
  @*/
static int parse_x509_cert_Extension(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len,
				     const _ext_oid **parsed_oid_list,
				     u32 *eaten)
{
	u32 data_len = 0, hdr_len = 0, remain = 0;
	u32 ext_hdr_len = 0, ext_data_len = 0, oid_len = 0;
	u32 saved_ext_len = 0, parsed = 0;
	const u8 *buf = cert + off;
	const _ext_oid *ext = NULL;
	int critical = 0;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1))); */

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &ext_hdr_len, &ext_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1))); */

	/*
	 * Let's parse the OID and then check if we have
	 * an associated handler for that extension.
	 */
	ret = parse_OID(buf, ext_data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1))); */

	ext = find_ext_by_oid(buf, oid_len);
	if (ext == NULL) {
#ifndef TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_EXT_OIDS
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
#else
		ext = &generic_unsupported_ext_oid;
#endif
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1))); */

	/*
	 * There is no efficient way to support check of duplicate OID for
	 * extension we do not known, i.e. if
	 * TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_EXT_OIDS has been enabled
	 * _and_ we end up on an unsupported OID, we just skip duplicate
	 * check, as documented.
	 */
	if (ext != &generic_unsupported_ext_oid) {
		/*
		 * Now that we know the OID is one we support, we verify
		 * this is the first time we handle an instance of this
		 * type. Having multiple instances of a given extension
		 * in a certificate is forbidden by both section 4.2 of
		 * RFC5280 and section 8 of X.509, w/ respectively
		 *
		 * - "A certificate MUST NOT include more than one
		 *    instance of a particular extension."
		 * - "For all certificate extensions, CRL extensions,
		 *    and CRL entry extensions defined in this Directory
		 *    Specification, there shall be no more than one
		 *    instance of each extension type in any certificate,
		 *    CRL, or CRL entry, respectively."
		 *
		 * This is done by recording for each extension we
		 * processed the pointer to its vtable and compare
		 * it with current one.
		 */
		ret = check_record_ext_unknown(ext, parsed_oid_list);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;

	/*
	 * Now that we got the OID, let's check critical
	 * field value if present. It's a boolean
	 * defaulting to FALSE (in which case, it is absent).
	 * We could parse it as an integer but that
	 * would be a lot of work for three simple bytes.
	 */
	ret = parse_boolean(buf, ext_data_len, &parsed);
	if (ret) {
		/*
		 * parse_boolean() returned an error which means this
		 * was either a boolean with invalid content or
		 * something else. If this was indeed a boolean, we can
		 * just leave. Otherwise, this just measn the critical
		 * flag was missing and the default value (false) applies,
		 * in which case, we can continue parsing.
		 */
		if (ext_data_len && (buf[0] == ASN1_TYPE_BOOLEAN)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	} else {
		/*
		 * We now know it's a valid BOOLEAN, *but* in our
		 * case (DER), the DEFAULT FALSE means we cannot
		 * accept an encoded value of FALSE. Note that we
		 * sanity check the value we expect for the length
		 */
		if (parsed != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

#ifndef TEMPORARY_LAXIST_EXTENSION_CRITICAL_FLAG_BOOLEAN_EXPLICIT_FALSE
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
#endif

		/*
		 * We now know the BOOLEAN is present and has
		 * a value of TRUE. Record that.
		 */
		critical = 1;

		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}

	/*
	 * We should now be in front of the octet string
	 * containing the extnValue.
	 */
	ret = parse_id_len(buf, ext_data_len,
			   CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;

	/* Check nothing remains behind the extnValue */
	if (data_len != ext_data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Parse the parameters for that extension */
	/*@ assert ext->parse_ext_params \in {
		  parse_ext_AIA, parse_ext_AKI,
		  parse_ext_SKI, parse_ext_keyUsage,
		  parse_ext_certPolicies, parse_ext_policyMapping,
		  parse_ext_SAN, parse_ext_IAN, parse_ext_subjectDirAttr,
		  parse_ext_basicConstraints, parse_ext_nameConstraints,
		  parse_ext_policyConstraints, parse_ext_EKU,
		  parse_ext_CRLDP, parse_ext_inhibitAnyPolicy,
		  parse_ext_bad_oid }; @*/
	/*@ calls parse_ext_AIA, parse_ext_AKI,
		  parse_ext_SKI, parse_ext_keyUsage,
		  parse_ext_certPolicies, parse_ext_policyMapping,
		  parse_ext_SAN, parse_ext_IAN,
		  parse_ext_subjectDirAttr, parse_ext_basicConstraints,
		  parse_ext_nameConstraints, parse_ext_policyConstraints,
		  parse_ext_EKU, parse_ext_CRLDP,
		  parse_ext_inhibitAnyPolicy, parse_ext_bad_oid ; @*/
	ret = ext->parse_ext_params(ctx, cert, off, ext_data_len, critical);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = saved_ext_len;
	ret = 0;

out:
	return ret;
}



/*
 * Parse X.509 extensions.
 *
 *  TBSCertificate  ::=  SEQUENCE  {
 *
 *       ...
 *
 *       extensions      [3]  EXPLICIT Extensions OPTIONAL
 *  }
 *
 *  Extensions  ::=  SEQUENCE SIZE (1..MAX) OF Extension
 *
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(eaten, ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 <= *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_cert_Extensions(cert_parsing_ctx *ctx,
				      const u8 *cert, u32 off, u32 len,
				      u32 *eaten)
{
	u32 data_len = 0, hdr_len = 0, remain = 0;
	const u8 *buf = cert + off;
	u32 saved_len = 0;
	const _ext_oid *parsed_oid_list[MAX_EXT_NUM_PER_CERT];
	int ret;
	u16 i;

	if ((cert == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Extensions in X.509 v3 certificates is an EXPLICITLY tagged
	 * sequence.
	 */
	ret = parse_explicit_id_len(buf, len, 3,
				    CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				    &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	/*@ assert \valid_read(buf + (0 .. (remain - 1))); */

	saved_len = hdr_len + data_len;
	/*@ assert saved_len <= len; */
	/*@ assert data_len <= saved_len; */

	/* If present, it must contain at least one extension */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Initialize list of already seen extensions */
	/*@ loop unroll MAX_EXT_NUM_PER_CERT;
	  @ loop assigns i, parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CERT - 1)];
	  @ loop invariant (i < MAX_EXT_NUM_PER_CERT) ==> \valid(&parsed_oid_list[i]);
	  @ loop variant (MAX_EXT_NUM_PER_CERT - i);
	  @*/
	for (i = 0; i < MAX_EXT_NUM_PER_CERT; i++) {
		parsed_oid_list[i] = NULL;
	}
	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CERT - 1))); */

	/* Now, let's work on each extension in the sequence */
	/*@
	  @ loop assigns off, ret, buf, remain, parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CERT - 1)], *ctx;
	  @ loop invariant (remain != 0) ==> \valid_read(cert + (off .. (off + remain - 1)));
	  @ loop invariant (remain != 0) ==> off + remain <= MAX_UINT32;
	  @ loop variant remain;
	  @*/
	while (remain) {
		u32 ext_len = 0;

		ret = parse_x509_cert_Extension(ctx, cert, off, remain,
						parsed_oid_list, &ext_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}

	/*
	 * RFC5280 has "If the subject field contains an empty sequence,
	 * then the issuing CA MUST include a subjectAltName extension
	 * that is marked as critical."
	 */
	if (ctx->empty_subject) {
		if (!ctx->has_san) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		if (!ctx->san_critical) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	/*@ assert 1 <= saved_len <= len; */
	*eaten = saved_len;

	ret = 0;

out:
	return ret;
}

/*
 *
 *	TBSCertificate	::=  SEQUENCE  {
 *	version		[0]  EXPLICIT Version DEFAULT v1,
 *	serialNumber	     CertificateSerialNumber,
 *	signature	     AlgorithmIdentifier,
 *	issuer		     Name,
 *	validity	     Validity,
 *	subject		     Name,
 *	subjectPublicKeyInfo SubjectPublicKeyInfo,
 *	issuerUniqueID	[1]  IMPLICIT UniqueIdentifier OPTIONAL,
 *			     -- If present, version MUST be v2 or v3
 *	subjectUniqueID [2]  IMPLICIT UniqueIdentifier OPTIONAL,
 *			     -- If present, version MUST be v2 or v3
 *	extensions	[3]  EXPLICIT Extensions OPTIONAL
 *			     -- If present, version MUST be v3
 *	}
 *
 * On success, the function returns the size of the tbsCertificate
 * structure in 'eaten' parameter. It also provides in 'sig_alg'
 * a pointer to the signature algorithm found in the signature field.
 * This one is provided to be able to check later against the signature
 * algorithm found in the signatureAlgorithm field of the certificate.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(sig_alg, eaten, cert+(..), ctx);
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (sig_alg == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->sig_alg));
  @ ensures (\result == 0) ==> ctx->tbs_sig_alg_start < off + *eaten;
  @
  @ assigns *eaten, *ctx, *sig_alg;
  @*/
static int parse_x509_tbsCertificate(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len,
				     const _sig_alg **sig_alg, u32 *eaten)
{
	u32 tbs_data_len = 0;
	u32 tbs_hdr_len = 0;
	u32 tbs_cert_len = 0;
	u32 remain = 0;
	u32 parsed = 0;
	u32 cur_off = off;
	const u8 *buf = cert + cur_off;
	const u8 *subject_ptr, *issuer_ptr;
	u32 subject_len, issuer_len;
	const _sig_alg *alg = NULL;
	int ret, empty_issuer = 1;

	if ((ctx == NULL) || (cert == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Let's first check we are dealing with a valid sequence containing
	 * all the elements of the certificate.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &tbs_hdr_len, &tbs_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	tbs_cert_len = tbs_hdr_len + tbs_data_len;
	buf += tbs_hdr_len;
	cur_off += tbs_hdr_len;
	remain = tbs_data_len;

	/*
	 * Now, we can start and parse all the elements in the sequence
	 * one by one.
	 */

	/* version */
	ret = parse_x509_cert_Version(cert, cur_off, remain, &ctx->version, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* serialNumber */
	ret = parse_CertSerialNumber(ctx, cert, cur_off, remain,
				     CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
				     &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (ctx->version != 0x02) {
		ret = X509_PARSER_ERROR_VERSION_NOT_3;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* signature */
	ret = parse_x509_tbsCert_sig_AlgorithmIdentifier(ctx, cert, cur_off, remain,
							 &alg, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert \initialized(&(ctx->tbs_sig_alg_start)); */
	/*@ assert ctx->tbs_sig_alg_start == cur_off; */
	/*@ assert ctx->tbs_sig_alg_start < off + tbs_cert_len; */
	/*@ assert \initialized(&(ctx->tbs_sig_alg_len)); */
	/*@ assert \initialized(&(ctx->sig_alg)); */

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* issuer */
	ret = parse_x509_Name(buf, remain, &parsed, &empty_issuer);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	ctx->issuer_start = cur_off;
	ctx->issuer_len = parsed;

	/*
	 * As described in section 4.1.2.4 of RFC 5280, "The issuer field MUST
	 * contain a non-empty distinguished name (DN)".
	 */
	/*@ assert (empty_issuer == 0) || (empty_issuer == 1); */
	if (empty_issuer) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	issuer_ptr = buf;
	issuer_len = parsed;

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* validity */
	ret = parse_x509_Validity(ctx, cert, cur_off, remain, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_cert_len; */

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* subject */
	ret = parse_x509_Name(buf, remain, &parsed, &ctx->empty_subject);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->subject_start = cur_off;
	ctx->subject_len = parsed;

	subject_ptr = buf;
	subject_len = parsed;

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* We can now check if subject and issuer fields are identical */
	ctx->subject_issuer_identical = 0;
	if (subject_len == issuer_len) {
		ctx->subject_issuer_identical = !bufs_differ(subject_ptr,
							     issuer_ptr,
							     issuer_len);
	}

	/* subjectPublicKeyInfo */
	ret = parse_x509_subjectPublicKeyInfo(ctx, cert, cur_off, remain, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_cert_len; */

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/*
	 * At that point, the remainder of the tbsCertificate part
	 * is made of 3 *optional* elements:
	 *
	 *     issuerUniqueID  [1]  IMPLICIT UniqueIdentifier OPTIONAL,
	 *     subjectUniqueID [2]  IMPLICIT UniqueIdentifier OPTIONAL,
	 *     extensions      [3]  EXPLICIT Extensions OPTIONAL
	 *
	 *  w/ UniqueIdentifier  ::=  BIT STRING
	 *     Extensions  ::=  SEQUENCE SIZE (1..MAX) OF Extension
	 *
	 * Section 4.1.2.8 of RFC 5280 explicitly states that "CAs
	 * conforming to this profile MUST NOT generate certificates
	 * with unique identifier" but "Applications conforming to
	 * this profile SHOULD be capable of parsing certificates
	 * that include unique identifiers, but there are no processing
	 * requirements associated with the unique identifiers."
	 *
	 * Additionnally, some tests performed on 9826768 (of 18822321)
	 * certificates that validate in a 2011 TLS campaign, we do not
	 * have any certificate w/ either a subjectUniqueID or
	 * issuerUniqueID.
	 *
	 * For that reason, in order to simplify parsing, we expect NOT
	 * to find either a subject or issuer unique identifier and to
	 * directly find extensions, if any. This is done by checking if
	 * data remain at that point. If that is the case, we perform
	 * a full parsing of the Extensions.
	 */
	if (remain) {
		ret = parse_x509_cert_Extensions(ctx, cert, cur_off, remain,
						 &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_cert_len; */

	if (remain != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC 5280 requires that SKI extension "MUST appear in all conforming
	 * CA certificates, that is, all certificates including the basic
	 * constraints extension (Section 4.2.1.9) where the value of cA is
	 * TRUE"
	 */
#ifndef TEMPORARY_LAXIST_CA_WO_SKI
	if (ctx->ca_true && !ctx->has_ski) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#endif

	/*
	 * RFC 5280 has "If the keyCertSign bit is asserted, then the cA bit in
	 * the basic constraints extension (Section 4.2.1.9) MUST also be
	 * asserted.
	 *
	 * It also has the following regarding basicConstraints extension:
	 * "Conforming CAs MUST include this extension in all CA certificates
	 * that contain public keys used to validate digital signatures on
	 * certificates and MUST mark the extension as critical in such
	 * certificates."
	 *
	 * Note that we do not enforce basicConstraints criticality otherwise
	 * as required by "This extension MAY appear as a critical or
	 * non-critical extension in CA certificates that contain public keys
	 * used exclusively for purposes other than validating digital
	 * signatures on certificates. This extension MAY appear as a critical
	 * or non-critical extension in end entity certificates."
	 */
	if (ctx->keyCertSign_set && (!ctx->ca_true || !ctx->bc_critical)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * If the subject is a CRL issuer (e.g., the key usage extension, as
	 * discussed in Section 4.2.1.3, is present and the value of cRLSign is
	 * TRUE), then the subject field MUST be populated with a non-empty
	 * distinguished name matching the contents of the issuer field (Section
	 * 5.1.2.3) in all CRLs issued by the subject CRL issuer.
	 */
	if (ctx->cRLSign_set && ctx->empty_subject) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC5280 has "CAs MUST NOT include the pathLenConstraint field
	 * unless the cA boolean is asserted and the key usage extension
	 * asserts the keyCertSign bit."
	 */
	if (ctx->pathLenConstraint_set &&
	    (!ctx->ca_true || !ctx->keyCertSign_set)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

       /*
	* RFC5280 has "The name constraints extension, which MUST be used only
	* in a CA certificate, ..."
	*/
	if (ctx->has_name_constraints && !ctx->ca_true) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

       /*
	* RFC5280 has "When a conforming CA includes a cRLDistributionPoints
	* extension in a certificate, it MUST include at least one
	* DistributionPoint that points to a CRL that covers the certificate
	* for all reasons."
	*/
	if (ctx->ca_true && ctx->has_crldp && !ctx->one_crldp_has_all_reasons) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_cert_len; */
	/*@ assert 1 < tbs_cert_len <= len; */
	*eaten = tbs_cert_len;
	/*@ assert ctx->tbs_sig_alg_start < off + *eaten; */
	*sig_alg = alg;

	ret = 0;

out:
	return ret;
}

/*
 * The function is used to parse signatureAlgorithm field found after the
 * tbsCertificate and before the signature. As the field is expected
 * to exactly match the tbsCertificate.signature field which has already
 * been fully parsed and validated, we only verify here that the element
 * to parse is identical to previously parsed tbsCertificate.signature
 * using the pointers in the context.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (0 .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \initialized(&(ctx->tbs_sig_alg_start));
  @ requires \initialized(&(ctx->tbs_sig_alg_len));
  @ requires ctx->tbs_sig_alg_start <= off;
  @ requires \separated(eaten, cert+(..), ctx);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    ctx->sig_alg_start,
	    ctx->sig_alg_len;
  @*/
static int parse_x509_signatureAlgorithm(cert_parsing_ctx *ctx,
					 const u8 *cert, u32 off, u32 len,
					 u32 *eaten)
{
	u32 prev_len;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	prev_len = ctx->tbs_sig_alg_len;
	if (prev_len > len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = bufs_differ(cert + ctx->tbs_sig_alg_start, cert + off, prev_len);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	ctx->sig_alg_start = off;
	ctx->sig_alg_len = prev_len;

	*eaten = prev_len;

	ret = 0;

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (sig_alg != \null) ==> \valid_read(sig_alg) && \valid_function(sig_alg->parse_sig);
  @ requires (\initialized(&ctx->sig_alg));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..), sig_alg, eaten);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (sig_alg == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_signatureValue(cert_parsing_ctx *ctx,
				     const u8 *cert, u32 off, u32 len,
				     const _sig_alg *sig_alg, u32 *eaten)
{
	u32 saved_off = off;
	sig_params *params;
	int ret;

	if ((cert == NULL) || (len == 0) || (sig_alg == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (sig_alg->parse_sig == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	params = &(ctx->sig_alg_params);
	/*@ assert \valid(params); */

	/*@ assert sig_alg->parse_sig \in {
		  parse_sig_ed448,
		  parse_sig_ed25519,
		  parse_sig_ecdsa,
		  parse_sig_sm2,
		  parse_sig_dsa,
		  parse_sig_rsa_pkcs1_v15,
		  parse_sig_rsa_ssa_pss,
		  parse_sig_rsa_9796_2_pad,
		  parse_sig_rsa_belgian,
		  parse_sig_gost94,
		  parse_sig_gost2001,
		  parse_sig_gost2012_512,
		  parse_sig_gost2012_256,
		  parse_sig_bign,
		  parse_sig_monkey }; @*/
	/*@ calls parse_sig_ed448,
		  parse_sig_ed25519,
		  parse_sig_ecdsa,
		  parse_sig_sm2,
		  parse_sig_dsa,
		  parse_sig_rsa_pkcs1_v15,
		  parse_sig_rsa_ssa_pss,
		  parse_sig_rsa_9796_2_pad,
		  parse_sig_rsa_belgian,
		  parse_sig_gost94,
		  parse_sig_gost2001,
		  parse_sig_gost2012_512,
		  parse_sig_gost2012_256,
		  parse_sig_bign,
		  parse_sig_monkey; @*/
	ret = sig_alg->parse_sig(params, cert, off, len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->sig_start = saved_off;
	ctx->sig_len = *eaten;
	ret = 0;

out:
	return ret;
}

/*
 * The goal being to zeroify all the fields of a given structure (not a buffer)
 * using a function, we did not manage to do that using memset() in Typed memory
 * model and at the same time validate assigns clause. I tried and reimplement
 * a specific function but conversion of the structure to a buffer to initialize
 * one by one all the bytes of the structure result in an inability to validate
 * assigns clause for the structure in the function. A good amount of time was
 * spent on annotations strategies w/o success.
 *
 * Until one finds a better solution, a decent workaround has been found with
 * the following function, which internally declares a structure with a static
 * initializer and returns that (zeroized) structure by value. In a caller, the
 * variable to be zeroized can just be overridden with the result of the
 * function. This is not generic but sufficient for the need we have in the
 * parser.
 */
/*@
  @ assigns \nothing;
  @*/
static cert_parsing_ctx get_zeroized_cert_ctx_val(void)
{
	cert_parsing_ctx zeroized_ctx = { 0 };

	return zeroized_ctx;
}

/*@
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (0 .. (len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, cert+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns *ctx;
  @*/
int parse_x509_cert(cert_parsing_ctx *ctx, const u8 *cert, u32 len)
{
	u32 seq_data_len = 0;
	u32 eaten = 0;
	u32 off = 0;
	const _sig_alg *sig_alg = NULL;
	int ret;

	if ((cert == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*ctx = get_zeroized_cert_ctx_val();

	/*
	 * Parse beginning of buffer to verify it's a sequence and get
	 * the length of the data it contains.
	 */
	ret = parse_id_len(cert, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &eaten, &seq_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= eaten;
	off += eaten;
	/*@ assert off + len <= MAX_UINT32; */

	/*
	 * We do expect advertised length to match what now remains in buffer
	 * after the sequence header we just parsed.
	 */
	if (seq_data_len != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Parse first element of the sequence: tbsCertificate */
	ret = parse_x509_tbsCertificate(ctx, cert, off, len, &sig_alg, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(&(ctx->sig_alg)); */
	/*@ assert ctx->tbs_sig_alg_start < off + eaten; */

	ctx->tbs_start = off;
	ctx->tbs_len = eaten;

	len -= eaten;
	off += eaten;

	/*@ assert ctx->tbs_sig_alg_start <= off; */

	/* Parse second element of the sequence: signatureAlgorithm */
	ret = parse_x509_signatureAlgorithm(ctx, cert, off, len, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(&(ctx->sig_alg)); */

	len -= eaten;
	off += eaten;

	/* Parse second element of the sequence: signatureValue */
	ret = parse_x509_signatureValue(ctx, cert, off, len, sig_alg, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check there is nothing left behind */
	if (len != eaten) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}


/* ============== IMPL: x509-common.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

/* removed: #include "x509-common.h" */

#define X509_FILE_NUM 3 /* See x509-utils.h for rationale */

/********************************************************************
 * Hash algs
 ********************************************************************/

#define DECL_HASH_ALG(TTalg, UUname, VVoid, WWoidbuf, XXtype) \
static const u8 _##TTalg##_hash_name[] = UUname;             \
static const u8 _##TTalg##_hash_printable_oid[] = VVoid;     \
static const u8 _##TTalg##_hash_der_oid[] = WWoidbuf;        \
							     \
static const _hash_alg _##TTalg##_hash_alg = {               \
	.alg_name = _##TTalg##_hash_name,                    \
	.alg_printable_oid = _##TTalg##_hash_printable_oid,  \
	.alg_der_oid = _##TTalg##_hash_der_oid,              \
	.alg_der_oid_len = sizeof(_##TTalg##_hash_der_oid),  \
	.hash_id = (XXtype),				     \
}

/* M-x toggle-truncate-lines is your friend here ;-) */
DECL_HASH_ALG(md2, "MD2", "1.2.840.113549.2.2", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x02 }), HASH_ALG_MD2);
DECL_HASH_ALG(md4, "MD4", "1.2.840.113549.2.4", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x04 }), HASH_ALG_MD4);
DECL_HASH_ALG(md5, "MD5", "1.2.840.113549.2.5", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05 }), HASH_ALG_MD5);
DECL_HASH_ALG(mdc2, "MDC2", "1.3.14.3.2.19", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x13 }), HASH_ALG_MDC2);
DECL_HASH_ALG(sha1, "SHA1", "1.3.14.3.2.26", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a }), HASH_ALG_SHA1);
DECL_HASH_ALG(ripemd160, "RIPEMD160", "1.3.36.3.2.1", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x24, 0x03, 0x02, 0x01 }), HASH_ALG_RIPEMD160);
DECL_HASH_ALG(ripemd160_iso, "RIPEMD160", "1.0.10118.3.49", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x31 }), HASH_ALG_RIPEMD160);
DECL_HASH_ALG(ripemd128, "RIPEMD128", "1.3.36.3.2.2", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x24, 0x03, 0x02, 0x02 }), HASH_ALG_RIPEMD128);
DECL_HASH_ALG(ripemd128_iso, "RIPEMD128", "1.0.10118.3.50", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x32 }), HASH_ALG_RIPEMD128);
DECL_HASH_ALG(ripemd256, "RIPEMD256", "1.3.36.3.2.3", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x24, 0x03, 0x02, 0x03 }), HASH_ALG_RIPEMD256);
DECL_HASH_ALG(sha224, "SHA224", "2.16.840.1.101.3.4.2.4", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04 }), HASH_ALG_SHA224);
DECL_HASH_ALG(sha256, "SHA256", "2.16.840.1.101.3.4.2.1", P99_PROTECT({  0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01 }), HASH_ALG_SHA256);
DECL_HASH_ALG(sha384, "SHA384", "2.16.840.1.101.3.4.2.2", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02 }), HASH_ALG_SHA384);
DECL_HASH_ALG(sha512, "SHA512", "2.16.840.1.101.3.4.2.3", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03 }), HASH_ALG_SHA512);
DECL_HASH_ALG(sha512_224, "SHA512_224", "2.16.840.1.101.3.4.2.5", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x05 }), HASH_ALG_SHA512_224);
DECL_HASH_ALG(sha512_256, "SHA512_256", "2.16.840.1.101.3.4.2.6", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03,0x04, 0x02, 0x06 }), HASH_ALG_SHA512_256);
DECL_HASH_ALG(sha3_224, "SHA3_224", "2.16.840.1.101.3.4.2.7", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x07 }), HASH_ALG_SHA3_224);
DECL_HASH_ALG(sha3_256, "SHA3_256", "2.16.840.1.101.3.4.2.8", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x08 }), HASH_ALG_SHA3_256);
DECL_HASH_ALG(sha3_384, "SHA3_384", "2.16.840.1.101.3.4.2.9", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x09 }), HASH_ALG_SHA3_384);
DECL_HASH_ALG(sha3_512, "SHA3_512", "2.16.840.1.101.3.4.2.10", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0a }), HASH_ALG_SHA3_512);
DECL_HASH_ALG(shake128, "SHAKE128", "2.16.840.1.101.3.4.2.11", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0b }), HASH_ALG_SHAKE128);
DECL_HASH_ALG(shake256, "SHAKE256", "2.16.840.1.101.3.4.2.12", P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0c }), HASH_ALG_SHAKE256);
DECL_HASH_ALG(hbelt, "HBELT", "1.2.112.0.2.0.34.101.31.81", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x1F, 0x51 }), HASH_ALG_HBELT);
DECL_HASH_ALG(bash256, "BASH256", "1.2.112.0.2.0.34.101.77.11", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x4D, 0x0B }), HASH_ALG_BASH256);
DECL_HASH_ALG(bash384, "BASH384", "1.2.112.0.2.0.34.101.77.12", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x4D, 0x0C }), HASH_ALG_BASH384);
DECL_HASH_ALG(bash512, "BASH512", "1.2.112.0.2.0.34.101.77.13", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x4D, 0x0D }), HASH_ALG_BASH512);
DECL_HASH_ALG(whirlpool, "WHIRLPOOL", "1.0.10118.3.55", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x37 }), HASH_ALG_WHIRLPOOL);
DECL_HASH_ALG(streebog256, "STREEBOG256", "1.2.643.7.1.1.2.2", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x02, 0x02 }), HASH_ALG_STREEBOG256);
DECL_HASH_ALG(streebog256_bis, "STREEBOG256", "1.0.10118.3.60", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x3c }), HASH_ALG_STREEBOG256);
DECL_HASH_ALG(streebog512, "STREEBOG512", "1.2.643.7.1.1.2.3", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x02, 0x03 }), HASH_ALG_STREEBOG512);
DECL_HASH_ALG(streebog512_bis, "STREEBOG512", "1.0.10118.3.59", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x3b}), HASH_ALG_STREEBOG512);
DECL_HASH_ALG(sm3, "SM3", "1.0.10118.3.65", P99_PROTECT({ 0x06, 0x06, 0x28, 0xCF, 0x06, 0x03, 0x00, 0x41 }), HASH_ALG_SM3);
DECL_HASH_ALG(gostR3411_94, "GOST R 34.11-94", "1.2.643.2.2.9", P99_PROTECT({ 0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x09 }), HASH_ALG_GOSTR3411_94);
DECL_HASH_ALG(gostR3411_94_bis, "GOST R 34.11-94", "1.2.643.2.2.30.1", P99_PROTECT({ 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x1e, 0x01 }), HASH_ALG_GOSTR3411_94); /* unclear see RFC 4357 */

const _hash_alg *known_hashes[] = {
	&_md2_hash_alg,
	&_md4_hash_alg,
	&_md5_hash_alg,
	&_mdc2_hash_alg,
	&_sha1_hash_alg,
	&_ripemd160_hash_alg,
	&_ripemd160_iso_hash_alg,
	&_ripemd128_hash_alg,
	&_ripemd128_iso_hash_alg,
	&_ripemd256_hash_alg,
	&_sha224_hash_alg,
	&_sha256_hash_alg,
	&_sha384_hash_alg,
	&_sha512_hash_alg,
	&_sha512_224_hash_alg,
	&_sha512_256_hash_alg,
	&_sha3_224_hash_alg,
	&_sha3_256_hash_alg,
	&_sha3_384_hash_alg,
	&_sha3_512_hash_alg,
	&_shake128_hash_alg,
	&_shake256_hash_alg,
	&_hbelt_hash_alg,
	&_bash256_hash_alg,
	&_bash384_hash_alg,
	&_bash512_hash_alg,
	&_whirlpool_hash_alg,
	&_gostR3411_94_hash_alg,
	&_gostR3411_94_bis_hash_alg,
	&_streebog256_hash_alg,
	&_streebog256_bis_hash_alg,
	&_streebog512_hash_alg,
	&_streebog512_bis_hash_alg,
	&_sm3_hash_alg,
};

#define NUM_KNOWN_HASHES (sizeof(known_hashes) / sizeof(known_hashes[0]))





/********************************************************************
 * MGF algs (for RSA SSA PSS)
 ********************************************************************/

static const u8 _mgf_alg_mgf1_name[] = "MGF1";
static const u8 _mgf_alg_mgf1_printable_oid[] = "1.2.840.113549.1.1.8";
static const u8 _mgf_alg_mgf1_der_oid[] = { 0x06, 0x09, 0x2a, 0x86,
					    0x48, 0x86, 0xf7, 0x0d,
					    0x01, 0x01, 0x08 };

static const _mgf _mgf1_alg = {
	.alg_name = _mgf_alg_mgf1_name,
	.alg_printable_oid = _mgf_alg_mgf1_printable_oid,
	.alg_der_oid = _mgf_alg_mgf1_der_oid,
	.alg_der_oid_len = sizeof(_mgf_alg_mgf1_der_oid),
	.mgf_id = MGF_ALG_MGF1
};





/********************************************************************
 * Elliptic curves
 ********************************************************************/

#define DECL_CURVE(TTcurve, UUname, VVoid, WWoidbuf, XXtype, YYbitlen)    \
static const u8 _##TTcurve##_curve_name[] = UUname;                       \
static const u8 _##TTcurve##_curve_printable_oid[] = VVoid;               \
static const u8 _##TTcurve##_curve_der_oid[] = WWoidbuf;                  \
									  \
static const _curve _curve_##TTcurve = {                                  \
	.crv_name = _##TTcurve##_curve_name,                              \
	.crv_printable_oid = _##TTcurve##_curve_printable_oid,            \
	.crv_der_oid = _##TTcurve##_curve_der_oid,                        \
	.crv_der_oid_len = sizeof(_##TTcurve##_curve_der_oid),            \
	.crv_order_bit_len = (YYbitlen),				  \
	.crv_id = (XXtype),						  \
}

/* M-x toggle-truncate-lines is your friend here ;-) */
DECL_CURVE(Curve25519, "Curve25519", "1.3.6.1.4.1.11591.15.1", P99_PROTECT({ 0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0xDA, 0x47, 0x0F, 0x01 }), CURVE_WEI25519, 256);
DECL_CURVE(Curve448, "Curve448", "1.3.6.1.4.1.11591.15.2", P99_PROTECT({ 0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0xDA, 0x47, 0x0F, 0x02 }), CURVE_WEI448, 448);
DECL_CURVE(bign_curve256v1, "bign-curve256v1", "1.2.112.0.2.0.34.101.45.3.1", P99_PROTECT({ 0x06, 0x0a, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x03, 0x01 }), CURVE_BIGN256v1, 256);
DECL_CURVE(bign_curve384v1, "bign-curve384v1", "1.2.112.0.2.0.34.101.45.3.2", P99_PROTECT({ 0x06, 0x0a, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x03, 0x02 }), CURVE_BIGN384v1, 384);
DECL_CURVE(bign_curve512v1, "bign-curve512v1", "1.2.112.0.2.0.34.101.45.3.3", P99_PROTECT({ 0x06, 0x0a, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x03, 0x03 }), CURVE_BIGN512v1, 512);
DECL_CURVE(prime192v1, "prime192v1", "1.2.840.10045.3.0.1", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x01 }), CURVE_SECP192R1, 192);
DECL_CURVE(c2pnb163v1, "c2pnb163v1", "1.2.840.10045.3.0.1", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x00, 0x01 }), CURVE_C2PNB163V1, 163);
DECL_CURVE(sect571k1, "sect571k1", "1.3.132.0.38", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x26 }), CURVE_SECT571K1, 571);
DECL_CURVE(sect163k1, "sect163k1", "1.3.132.0.1", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x01 }), CURVE_SECT163K1, 163);
DECL_CURVE(secp192k1, "secp192k1", "1.3.132.0.31", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x1f }), CURVE_SECP192K1, 192);
DECL_CURVE(secp224k1, "secp224k1", "1.3.132.0.32", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x20 }), CURVE_SECP224K1, 224);
DECL_CURVE(secp256k1, "secp256k1", "1.3.132.0.10", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x0a }), CURVE_SECP256K1, 256);
DECL_CURVE(secp224r1, "secp224r1", "1.3.132.0.33", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x21 }), CURVE_SECP224R1, 224);
DECL_CURVE(secp256r1, "secp256r1", "1.2.840.10045.3.1.7", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07 }), CURVE_SECP256R1, 256);
DECL_CURVE(secp384r1, "secp384r1", "1.3.132.0.34", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x22 }), CURVE_SECP384R1, 384);
DECL_CURVE(secp521r1, "secp521r1", "1.3.132.0.35", P99_PROTECT({ 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x23 }), CURVE_SECP521R1, 521);
DECL_CURVE(brainpoolP192R1, "brainpoolP192R1", "1.3.36.3.3.2.8.1.1.3", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x03 }), CURVE_BRAINPOOLP192R1, 192);
DECL_CURVE(brainpoolP224R1, "brainpoolP224R1", "1.3.36.3.3.2.8.1.1.5", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x05 }), CURVE_BRAINPOOLP224R1, 224);
DECL_CURVE(brainpoolP256R1, "brainpoolP256R1", "1.3.36.3.3.2.8.1.1.7", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x07 }), CURVE_BRAINPOOLP256R1, 256);
DECL_CURVE(brainpoolP320R1, "brainpoolP320R1", "1.3.36.3.3.2.8.1.1.9", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x09 }), CURVE_BRAINPOOLP320R1, 320);
DECL_CURVE(brainpoolP384R1, "brainpoolP384R1", "1.3.36.3.3.2.8.1.1.11", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08,0x01, 0x01, 0x0b }), CURVE_BRAINPOOLP384R1, 384);
DECL_CURVE(brainpoolP512R1, "brainpoolP512R1", "1.3.36.3.3.2.8.1.1.13", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0d }), CURVE_BRAINPOOLP512R1, 512);
DECL_CURVE(brainpoolP192T1, "brainpoolP192T1", "1.3.36.3.3.2.8.1.1.4", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x04 }), CURVE_BRAINPOOLP192T1, 192);
DECL_CURVE(brainpoolP224T1, "brainpoolP224T1", "1.3.36.3.3.2.8.1.1.6", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x06 }), CURVE_BRAINPOOLP224T1, 224);
DECL_CURVE(brainpoolP256T1, "brainpoolP256T1", "1.3.36.3.3.2.8.1.1.8", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x08 }), CURVE_BRAINPOOLP256T1, 256);
DECL_CURVE(brainpoolP320T1, "brainpoolP320T1", "1.3.36.3.3.2.8.1.1.10", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0a }), CURVE_BRAINPOOLP320T1, 320);
DECL_CURVE(brainpoolP384T1, "brainpoolP384T1", "1.3.36.3.3.2.8.1.1.12", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0c }), CURVE_BRAINPOOLP384T1, 384);
DECL_CURVE(brainpoolP512T1, "brainpoolP512T1", "1.3.36.3.3.2.8.1.1.14", P99_PROTECT({ 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0e }), CURVE_BRAINPOOLP512T1, 512);
DECL_CURVE(sm2p256v1, "sm2p256v1", "1.2.156.10197.1.301", P99_PROTECT({ 0x06, 0x08, 0x2a, 0x81, 0x1c, 0xcf, 0x55, 0x01, 0x82, 0x2d }), CURVE_SM2P256V1, 256);
DECL_CURVE(frp256v1, "frp256v1", "1.2.250.1.223.101.256.1", P99_PROTECT({ 0x06, 0x0A, 0x2A, 0x81, 0x7A, 0x01, 0x81, 0x5F, 0x65, 0x82, 0x00, 0x01 }), CURVE_FRP256V1, 256);
DECL_CURVE(gost_R3410_2001_CryptoPro_A_ParamSet, "gost_R3410_2001_CryptoPro_A_ParamSet", "1.2.643.2.2.35.1", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x23, 0x01 }), CURVE_GOST_R3410_2001_CRYPTOPRO_A_PARAMSET, 256);
DECL_CURVE(gost_R3410_2001_CryptoPro_B_ParamSet, "gost_R3410_2001_CryptoPro_B_ParamSet", "1.2.643.2.2.35.2", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x23, 0x02 }), CURVE_GOST_R3410_2001_CRYPTOPRO_B_PARAMSET, 256);
DECL_CURVE(gost_R3410_2001_CryptoPro_C_ParamSet, "gost_R3410_2001_CryptoPro_C_ParamSet", "1.2.643.2.2.35.3", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x23, 0x03 }), CURVE_GOST_R3410_2001_CRYPTOPRO_C_PARAMSET, 256);
DECL_CURVE(gost_R3410_2001_CryptoPro_XchA_ParamSet, "gost_R3410_2001_CryptoPro_XchA_ParamSet", "1.2.643.2.2.36.0", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x24, 0x00 }), CURVE_GOST_R3410_2001_CRYPTOPRO_XCHA_PARAMSET, 256);
DECL_CURVE(gost_R3410_2001_CryptoPro_XchB_ParamSet, "gost_R3410_2001_CryptoPro_XchB_ParamSet", "1.2.643.2.2.36.1", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x24, 0x01 }), CURVE_GOST_R3410_2001_CRYPTOPRO_XCHB_PARAMSET, 256);
DECL_CURVE(gost_R3410_2001_TestParamSet, "gost_R3410_2001_TestParamSet", "1.2.643.2.2.35.0", P99_PROTECT({ 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x23, 0x00 }), CURVE_GOST_R3410_2001_TESTPARAMSET, 256);
DECL_CURVE(gost_R3410_2012_256_paramSetA, "gost_R3410_2012_256_paramSetA", "1.2.643.7.1.2.1.1.1", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x01, 0x01 }), CURVE_GOST_R3410_2012_256_PARAMSETA, 257);
DECL_CURVE(gost_R3410_2012_256_paramSetB, "gost_R3410_2012_256_paramSetB", "1.2.643.7.1.2.1.1.2", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x01, 0x02 }), CURVE_GOST_R3410_2012_256_PARAMSETB, 256);
DECL_CURVE(gost_R3410_2012_256_paramSetC, "gost_R3410_2012_256_paramSetC", "1.2.643.7.1.2.1.1.3", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x01, 0x02 }), CURVE_GOST_R3410_2012_256_PARAMSETC, 256);
DECL_CURVE(gost_R3410_2012_256_paramSetD, "gost_R3410_2012_256_paramSetD", "1.2.643.7.1.2.1.1.4", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x01, 0x04 }), CURVE_GOST_R3410_2012_256_PARAMSETD, 256);
DECL_CURVE(gost_R3410_2012_512_paramSetA, "gost_R3410_2012_512_paramSetA", "1.2.643.7.1.2.1.2.1", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x01 }), CURVE_GOST_R3410_2012_512_PARAMSETA, 512);
DECL_CURVE(gost_R3410_2012_512_paramSetB, "gost_R3410_2012_512_paramSetB", "1.2.643.7.1.2.1.2.2", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x02 }), CURVE_GOST_R3410_2012_512_PARAMSETB, 512);
DECL_CURVE(gost_R3410_2012_512_paramSetC, "gost_R3410_2012_512_paramSetC", "1.2.643.7.1.2.1.2.3", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x03 }), CURVE_GOST_R3410_2012_512_PARAMSETC, 512);
DECL_CURVE(gost_R3410_2012_512_paramSetTest, "gost_R3410_2012_512_paramSetTest",  "1.2.643.7.1.2.1.2.0", P99_PROTECT({ 0x06, 0x09, 0x2A, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x00 }), CURVE_GOST_R3410_2012_512_PARAMSETTEST, 511);

const _curve *known_curves[] = {
	&_curve_secp224r1,
	&_curve_secp256r1,
	&_curve_secp384r1,
	&_curve_secp521r1,
	&_curve_prime192v1,
	&_curve_c2pnb163v1,
	&_curve_sect571k1,
	&_curve_sect163k1,
	&_curve_secp192k1,
	&_curve_secp224k1,
	&_curve_secp256k1,

	&_curve_brainpoolP192R1,
	&_curve_brainpoolP224R1,
	&_curve_brainpoolP256R1,
	&_curve_brainpoolP320R1,
	&_curve_brainpoolP384R1,
	&_curve_brainpoolP512R1,
	&_curve_brainpoolP192T1,
	&_curve_brainpoolP224T1,
	&_curve_brainpoolP256T1,
	&_curve_brainpoolP320T1,
	&_curve_brainpoolP384T1,
	&_curve_brainpoolP512T1,

	&_curve_sm2p256v1,

	&_curve_bign_curve256v1,
	&_curve_bign_curve384v1,
	&_curve_bign_curve512v1,

	&_curve_frp256v1,

	&_curve_gost_R3410_2001_CryptoPro_A_ParamSet,
	&_curve_gost_R3410_2001_CryptoPro_B_ParamSet,
	&_curve_gost_R3410_2001_CryptoPro_C_ParamSet,
	&_curve_gost_R3410_2001_CryptoPro_XchA_ParamSet,
	&_curve_gost_R3410_2001_CryptoPro_XchB_ParamSet,
	&_curve_gost_R3410_2001_TestParamSet,
	&_curve_gost_R3410_2012_256_paramSetA,
	&_curve_gost_R3410_2012_256_paramSetB,
	&_curve_gost_R3410_2012_256_paramSetC,
	&_curve_gost_R3410_2012_256_paramSetD,
	&_curve_gost_R3410_2012_512_paramSetA,
	&_curve_gost_R3410_2012_512_paramSetB,
	&_curve_gost_R3410_2012_512_paramSetC,
	&_curve_gost_R3410_2012_512_paramSetTest,

	&_curve_Curve25519,
	&_curve_Curve448,
};

#define NUM_KNOWN_CURVES (sizeof(known_curves) / sizeof(known_curves[0]))





/********************************************************************
 * Signature algs
 ********************************************************************/

#define DECL_SIG_ALG(TTalg, SSsig, HHhash, YYparse_sig, ZZparse_algoid, UUname, VVoid, WWoidbuf) \
static const u8 _##TTalg##_sig_name[] = UUname;             \
static const u8 _##TTalg##_sig_printable_oid[] = VVoid;     \
static const u8 _##TTalg##_sig_der_oid[] = WWoidbuf;        \
							    \
static const _sig_alg _##TTalg##_sig_alg = {                \
	.alg_name = _##TTalg##_sig_name,                    \
	.alg_printable_oid = _##TTalg##_sig_printable_oid,  \
	.alg_der_oid = _##TTalg##_sig_der_oid,              \
	.alg_der_oid_len = sizeof(_##TTalg##_sig_der_oid),  \
	.sig_id = (SSsig),				    \
	.hash_id = (HHhash),				    \
	.parse_sig = (YYparse_sig),			    \
	.parse_algoid_sig_params = (ZZparse_algoid),	    \
}

/* M-x toggle-truncate-lines is your friend here ;-) */

/*
 *-----------------------------------+---------------------------+----------------------+-------------------------+---------------------------------------------+-----------------------------------------------+-----------------------------+----------------------...
 *           struct name             | signature alg             | hash alg if known    | sig parsing func        | sig algs params parsing func                | pretty name for sig alg                       | printable OID               | OID in DER format
 *-----------------------------------+---------------------------+----------------------+-------------------------+---------------------------------------------+-----------------------------------------------+-----------------------------+----------------------...
 */
DECL_SIG_ALG(ecdsa_sha1              , SIG_ALG_ECDSA             , HASH_ALG_SHA1        , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-SHA1"                             , "1.2.840.10045.4.1"         , P99_PROTECT({ 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x01 }));
DECL_SIG_ALG(ecdsa_sha224            , SIG_ALG_ECDSA             , HASH_ALG_SHA224      , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-SHA224"                           , "1.2.840.10045.4.3.1"       , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x01 }));
DECL_SIG_ALG(ecdsa_sha256            , SIG_ALG_ECDSA             , HASH_ALG_SHA256      , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-SHA256"                           , "1.2.840.10045.4.3.2"       , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02 }));
DECL_SIG_ALG(ecdsa_sha384            , SIG_ALG_ECDSA             , HASH_ALG_SHA384      , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-SHA384"                           , "1.2.840.10045.4.3.3"       , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x03 }));
DECL_SIG_ALG(ecdsa_sha512            , SIG_ALG_ECDSA             , HASH_ALG_SHA512      , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-SHA512"                           , "1.2.840.10045.4.3.4"       , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x04 }));
DECL_SIG_ALG(ecdsa_with_sha3_256     , SIG_ALG_ECDSA             , HASH_ALG_SHA3_256    , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "id-ecdsa-with-sha3-256"                      , "2.16.840.1.101.3.4.3.10"   , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0a }));
DECL_SIG_ALG(ecdsa_with_shake128     , SIG_ALG_ECDSA             , HASH_ALG_SHAKE128    , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-shake128"                         , "1.3.6.1.5.5.7.6.32"        , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x20 })); /* RFC 8692 */
DECL_SIG_ALG(ecdsa_with_shake256     , SIG_ALG_ECDSA             , HASH_ALG_SHAKE256    , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with          , "ecdsa-with-shake256"                         , "1.3.6.1.5.5.7.6.32"        , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x20 })); /* RFC 8692 */
DECL_SIG_ALG(ecdsa_with_specified    , SIG_ALG_ECDSA             , HASH_ALG_UNKNOWN     , parse_sig_ecdsa         , parse_algoid_sig_params_ecdsa_with_specified, "ecdsa-with-specified"                        , "1.2.840.10045.4.3"         , P99_PROTECT({ 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03 })); /* draft-ietf-pkix-sha2-dsa-ecdsa-03 */
DECL_SIG_ALG(ed25519                 , SIG_ALG_ED25519           , HASH_ALG_SHA512      , parse_sig_ed25519       , parse_algoid_sig_params_eddsa               , "Ed25519"                                     , "1.3.101.112"               , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x70 }));
DECL_SIG_ALG(ed448                   , SIG_ALG_ED448             , HASH_ALG_SHAKE256    , parse_sig_ed448         , parse_algoid_sig_params_eddsa               , "Ed448"                                       , "1.3.101.113"               , P99_PROTECT({ 0x06, 0x03, 0x2b, 0x65, 0x71 }));
DECL_SIG_ALG(sm2_sm3                 , SIG_ALG_SM2               , HASH_ALG_SM3         , parse_sig_sm2           , parse_algoid_sig_params_sm2                 , "SM2 w/ SM3"                                  , "1.2.156.10197.1.501"       , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x81, 0x1c, 0xcf, 0x55, 0x01, 0x83, 0x75 }));
DECL_SIG_ALG(rsa_mdc2                , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_MDC2        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "mdc2WithRSA"                                 , "2.5.8.3.100"               , P99_PROTECT({ 0x06, 0x04, 0x55, 0x08, 0x03, 0x64 }));
DECL_SIG_ALG(rsa_md2                 , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_MD2         , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "md2WithRSAEncryption"                        , "1.2.840.113549.1.1.2"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x02 }));
DECL_SIG_ALG(rsa_md4                 , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_MD4         , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "md4WithRSAEncryption"                        , "1.2.840.113549.1.1.3"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x03 }));
DECL_SIG_ALG(rsa_md5                 , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_MD5         , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "md5WithRSAEncryption"                        , "1.2.840.113549.1.1.4"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x04 }));
DECL_SIG_ALG(sha1WithRSAEnc          , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha1WithRSAEncryption"                       , "1.2.840.113549.1.1.5"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05 }));
DECL_SIG_ALG(sha1WithRSAEnc_bis      , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha1WithRSAEncryption (bis)"                 , "1.3.14.3.2.29"             , P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1d }));
DECL_SIG_ALG(sha1WithRSAEnc_alt      , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha1WithRSAEncryption_alt"                   , "1.2.836.113549.1.1.5"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x44, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05 })); /* XXX CRAP. typo or SEU ? */
DECL_SIG_ALG(sha1WithRSAEnc_alt2     , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha1WithRSAEncryption_alt2"                  , "1.2.4936.113549.1.1.5"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0xa6, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05 })); /* XXX CRAP. typo or SEU ? */
DECL_SIG_ALG(sha1WithRSAEnc_ter      , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha1WithRSAEncryption (ter)"                 , "1.2.840.113549.0.1.5"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x00, 0x01, 0x05 })); /* CRAP */
DECL_SIG_ALG(rsa_sha1_crap           , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "Another unspecified RSA-SHA1 oid"            , "1.2.856.113549.1.1.5"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x58, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05 })); /* CRAP */
DECL_SIG_ALG(rsa_sha1_crap_bis       , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA1        , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "crappy sha1-with-rsa-signature"              , "1.2.872.113549.1.1.5"      , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x68, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05 })); /* CRAP */
DECL_SIG_ALG(shaWithRSASig_9796_2    , SIG_ALG_RSA_9796_2_PAD    , HASH_ALG_SHA1        , parse_sig_rsa_9796_2_pad, parse_algoid_sig_params_rsa                 , "shaWithRSASignature-9796-2"                  , "1.3.14.3.2.15"             , P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x0f })); /* Oddball using ISO/IEC 9796-2 padding rules */
DECL_SIG_ALG(rsa_sha224              , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA224      , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha224WithRSAEncryption"                     , "1.2.840.113549.1.1.14"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0e }));
DECL_SIG_ALG(rsa_sha256              , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA256      , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha256WithRSAEncryption"                     , "1.2.840.113549.1.1.11"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b }));
DECL_SIG_ALG(rsa_sha384              , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA384      , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha384WithRSAEncryption"                     , "1.2.840.113549.1.1.12"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0c }));
DECL_SIG_ALG(rsa_sha512              , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA512      , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "sha512WithRSAEncryption"                     , "1.2.840.113549.1.1.13"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0d }));
DECL_SIG_ALG(rsa_ripemd160           , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_RIPEMD160   , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "rsaSignatureWithripemd160"                   , "1.3.36.3.3.1.2"            , P99_PROTECT({ 0x06, 0x06, 0x2b, 0x24, 0x03, 0x03, 0x01, 0x02 }));
DECL_SIG_ALG(rsa_ripemd128           , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_RIPEMD128   , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "rsaSignatureWithripemd128"                   , "1.3.36.3.3.1.3"            , P99_PROTECT({ 0x06, 0x06, 0x2b, 0x24, 0x03, 0x03, 0x01, 0x03 }));
DECL_SIG_ALG(rsa_ripemd256           , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_RIPEMD256   , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "rsaSignatureWithripemd256"                   , "1.3.36.3.3.1.3"            , P99_PROTECT({ 0x06, 0x06, 0x2b, 0x24, 0x03, 0x03, 0x01, 0x03 }));
DECL_SIG_ALG(pkcs1_v15_w_sha3_224    , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA3_224    , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "id-rsassa-pkcs1-v1-5-with-sha3-224"          , "2.16.840.1.101.3.4.3.13"   , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0d }));
DECL_SIG_ALG(pkcs1_v15_w_sha3_256    , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA3_256    , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "id-rsassa-pkcs1-v1-5-with-sha3-256"          , "2.16.840.1.101.3.4.3.14"   , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0e }));
DECL_SIG_ALG(pkcs1_v15_w_sha3_384    , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA3_384    , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "id-rsassa-pkcs1-v1-5-with-sha3-384"          , "2.16.840.1.1.1.3.4.3.15"   , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0f,  }));
DECL_SIG_ALG(pkcs1_v15_w_sha3_512    , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_SHA3_512    , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "id-rsassa-pkcs1-v1-5-with-sha3-512"          , "2.16.840.1.1.1.3.4.3.16"   , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x10,  }));
DECL_SIG_ALG(rsassa_pss              , SIG_ALG_RSA_SSA_PSS       , HASH_ALG_UNKNOWN     , parse_sig_rsa_ssa_pss   , parse_algoid_sig_params_rsassa_pss          , "RSASSA-PSS"                                  , "1.2.840.113549.1.1.10"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0a }));
DECL_SIG_ALG(rsassa_pss_shake128     , SIG_ALG_RSA_SSA_PSS       , HASH_ALG_SHAKE128    , parse_sig_rsa_ssa_pss   , parse_algoid_sig_params_none                , "RSASSA-PSS-SHAKE128"                         , "1.3.6.1.5.5.7.6.30"        , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x1e }));  /* RFC 8692 */
DECL_SIG_ALG(rsassa_pss_shake256     , SIG_ALG_RSA_SSA_PSS       , HASH_ALG_SHAKE256    , parse_sig_rsa_ssa_pss   , parse_algoid_sig_params_none                , "RSASSA-PSS-SHAKE256"                         , "1.3.6.1.5.5.7.6.31"        , P99_PROTECT({ 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x06, 0x1f }));  /* RFC 8692 */
DECL_SIG_ALG(belgian_rsa             , SIG_ALG_BELGIAN_RSA       , HASH_ALG_UNKNOWN     , parse_sig_rsa_belgian   , parse_algoid_sig_params_rsa                 , "Undoc. Belgian RSA sig oid"                  , "2.16.56.2.1.4.1.1.3880.1"  , P99_PROTECT({ 0x06, 0x0b, 0x60, 0x38, 0x02, 0x01, 0x04, 0x01, 0x01, 0x82, 0xaf, 0x16, 0x01 })); /* Belgian CRAP */
DECL_SIG_ALG(rsalabs1                , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_UNKNOWN     , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "Unknown RSA Labs OID"                        , "1.2.840.113549.1.1.99"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x63 }));
DECL_SIG_ALG(rsalabs2                , SIG_ALG_RSA_PKCS1_V1_5    , HASH_ALG_UNKNOWN     , parse_sig_rsa_pkcs1_v15 , parse_algoid_sig_params_rsa                 , "Unspecified RSA oid"                         , "1.2.840.113605.1.1.11"     , P99_PROTECT({ 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x45, 0x01, 0x01, 0x0b }));
DECL_SIG_ALG(dsa_sha1                , SIG_ALG_DSA               , HASH_ALG_SHA1        , parse_sig_dsa           , parse_algoid_sig_params_none                , "dsaWithSHA1"                                 , "1.3.14.3.2.27"             , P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1b }));
DECL_SIG_ALG(dsa_sha1_old            , SIG_ALG_DSA               , HASH_ALG_SHA1        , parse_sig_dsa           , parse_algoid_sig_params_none                , "dsaWithSHA1-old"                             , "1.3.14.3.2.12"             , P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x0c }));
DECL_SIG_ALG(dsa_sha1_jdk            , SIG_ALG_DSA               , HASH_ALG_SHA1        , parse_sig_dsa           , parse_algoid_sig_params_none                , "dsaWithSHA1-jdk"                             , "1.3.14.3.2.13"             , P99_PROTECT({ 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x0d })); /* oid-info.com has "Incorrectly used by the JDK 1.1 in place of 1.3.14.3.2.27" */
DECL_SIG_ALG(dsa_sha1_bis            , SIG_ALG_DSA               , HASH_ALG_SHA1        , parse_sig_dsa           , parse_algoid_sig_params_none                , "dsa-with-sha1"                               , "1.2.840.10040.4.3"         , P99_PROTECT({ 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x38, 0x04, 0x03 }));
DECL_SIG_ALG(dsa_with_sha224         , SIG_ALG_DSA               , HASH_ALG_SHA224      , parse_sig_dsa           , parse_algoid_sig_params_none                , "id-dsa-with-sha224"                          , "2.16.840.1.101.3.4.3.1"    , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x01 }));
DECL_SIG_ALG(dsa_with_sha256         , SIG_ALG_DSA               , HASH_ALG_SHA256      , parse_sig_dsa           , parse_algoid_sig_params_none                , "id-dsa-with-sha256"                          , "2.16.840.1.101.3.4.3.2"    , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x02 }));
DECL_SIG_ALG(dsa_with_sha384         , SIG_ALG_DSA               , HASH_ALG_SHA384      , parse_sig_dsa           , parse_algoid_sig_params_none                , "id-dsa-with-sha384"                          , "2.16.840.1.101.3.4.3.3"    , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x03 }));
DECL_SIG_ALG(dsa_with_sha512         , SIG_ALG_DSA               , HASH_ALG_SHA512      , parse_sig_dsa           , parse_algoid_sig_params_none                , "id-dsa-with-sha512"                          , "2.16.840.1.101.3.4.3.4"    , P99_PROTECT({ 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x04 }));
DECL_SIG_ALG(gost_R3411_94_R3410_2001, SIG_ALG_GOSTR3410_2001    , HASH_ALG_GOSTR3411_94, parse_sig_gost2001      , parse_algoid_sig_params_none                , "sig_gostR3411-94-with-gostR3410-2001"        , "1.2.643.2.2.3"             , P99_PROTECT({ 0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x03 }));
DECL_SIG_ALG(gost_R3411_94_R3410_94  , SIG_ALG_GOSTR3410_94      , HASH_ALG_GOSTR3411_94, parse_sig_gost94        , parse_algoid_sig_params_none                , "sig_gostR3411-94-with-gostR3410-94"          , "1.2.643.2.2.4"             , P99_PROTECT({ 0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x04 }));
DECL_SIG_ALG(gost_R3410_2012_256     , SIG_ALG_GOSTR3410_2012_256, HASH_ALG_STREEBOG256 , parse_sig_gost2012_256  , parse_algoid_sig_params_none                , "sig_gost3410-2012-256"                       , "1.2.643.7.1.1.3.2"         , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x03, 0x02 }));
DECL_SIG_ALG(gost_R3410_2012_512     , SIG_ALG_GOSTR3410_2012_512, HASH_ALG_STREEBOG512 , parse_sig_gost2012_512  , parse_algoid_sig_params_none                , "sig_gost3410-2012-512"                       , "1.2.643.7.1.1.3.3"         , P99_PROTECT({ 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x03, 0x03 }));
DECL_SIG_ALG(bign_with_hbelt         , SIG_ALG_BIGN              , HASH_ALG_HBELT       , parse_sig_bign          , parse_algoid_sig_params_none                , "bign (STB 34.101.45-2013) using hbelt hash"  , "1.2.112.0.2.0.34.101.45.12", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x0c }));
DECL_SIG_ALG(bign_with_bash256       , SIG_ALG_BIGN              , HASH_ALG_BASH256     , parse_sig_bign          , parse_algoid_sig_params_none                , "bign (STB 34.101.45-2013) using BASH256"     , "1.2.112.0.2.0.34.101.45.13", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x0d }));
DECL_SIG_ALG(bign_with_bash384       , SIG_ALG_BIGN              , HASH_ALG_BASH384     , parse_sig_bign          , parse_algoid_sig_params_none                , "bign (STB 34.101.45-2013) using BASH384"     , "1.2.112.0.2.0.34.101.45.14", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x0e }));
DECL_SIG_ALG(bign_with_bash512       , SIG_ALG_BIGN              , HASH_ALG_BASH512     , parse_sig_bign          , parse_algoid_sig_params_none                , "bign (STB 34.101.45-2013) using BASH512"     , "1.2.112.0.2.0.34.101.45.15", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x0f }));
DECL_SIG_ALG(bign_with_hspec         , SIG_ALG_BIGN              , HASH_ALG_UNKNOWN     , parse_sig_bign          , parse_algoid_sig_params_bign_with_hspec     , "bign (STB 34.101.45-2013) w/ given hash func", "1.2.112.0.2.0.34.101.45.11", P99_PROTECT({ 0x06, 0x09, 0x2a, 0x70, 0x00, 0x02, 0x00, 0x22, 0x65, 0x2d, 0x0b }));
DECL_SIG_ALG(monkeysphere            , SIG_ALG_MONKEYSPHERE      , HASH_ALG_UNKNOWN     , parse_sig_monkey        , parse_algoid_sig_params_none                , "unknown OID from The Monkeysphere Project"   , "1.3.6.1.4.1.37210.1.1"     , P99_PROTECT({ 0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0xa2, 0x5a, 0x01, 0x01 }));

const _sig_alg *known_sig_algs[] = {
	&_ecdsa_sha1_sig_alg,
	&_ecdsa_sha224_sig_alg,
	&_ecdsa_sha256_sig_alg,
	&_ecdsa_sha384_sig_alg,
	&_ecdsa_sha512_sig_alg,
	&_ecdsa_with_sha3_256_sig_alg,
	&_ecdsa_with_shake128_sig_alg,
	&_ecdsa_with_shake256_sig_alg,

	&_rsassa_pss_sig_alg,
	&_rsassa_pss_shake128_sig_alg,
	&_rsassa_pss_shake256_sig_alg,

	&_rsa_mdc2_sig_alg,
	&_rsa_md2_sig_alg,
	&_rsa_md4_sig_alg,
	&_rsa_md5_sig_alg,
	&_rsa_sha224_sig_alg,
	&_rsa_sha256_sig_alg,
	&_rsa_sha384_sig_alg,
	&_rsa_sha512_sig_alg,
	&_sha1WithRSAEnc_sig_alg,
	&_sha1WithRSAEnc_bis_sig_alg,
	&_sha1WithRSAEnc_alt_sig_alg,
	&_sha1WithRSAEnc_alt2_sig_alg,
	&_sha1WithRSAEnc_ter_sig_alg,
	&_shaWithRSASig_9796_2_sig_alg,
	&_rsa_ripemd160_sig_alg,
	&_rsa_ripemd128_sig_alg,
	&_rsa_ripemd256_sig_alg,
	&_pkcs1_v15_w_sha3_224_sig_alg,
	&_pkcs1_v15_w_sha3_256_sig_alg,
	&_pkcs1_v15_w_sha3_384_sig_alg,
	&_pkcs1_v15_w_sha3_512_sig_alg,

	&_dsa_sha1_sig_alg,
	&_dsa_sha1_old_sig_alg,
	&_dsa_sha1_jdk_sig_alg,
	&_dsa_with_sha224_sig_alg,
	&_dsa_with_sha256_sig_alg,
	&_dsa_with_sha384_sig_alg,
	&_dsa_with_sha512_sig_alg,

	&_ed25519_sig_alg,
	&_ed448_sig_alg,

	&_sm2_sm3_sig_alg,

	&_gost_R3410_2012_256_sig_alg,
	&_gost_R3410_2012_512_sig_alg,
	&_gost_R3411_94_R3410_2001_sig_alg,
	&_gost_R3411_94_R3410_94_sig_alg,

	&_bign_with_hbelt_sig_alg,
	&_bign_with_bash256_sig_alg,
	&_bign_with_bash384_sig_alg,
	&_bign_with_bash512_sig_alg,
	&_bign_with_hspec_sig_alg,

	&_monkeysphere_sig_alg,
	&_belgian_rsa_sig_alg,
	&_rsalabs1_sig_alg,
	&_rsalabs2_sig_alg,

	&_dsa_sha1_bis_sig_alg,
	&_ecdsa_with_specified_sig_alg,
	&_rsa_sha1_crap_sig_alg,
	&_rsa_sha1_crap_bis_sig_alg,
};

const u16 num_known_sig_algs = (sizeof(known_sig_algs) / sizeof(known_sig_algs[0]));





/*
 * Some implementation notes:
 *
 * The implementation is based on X.690 and X.680 (both 07/2002). It is
 * voluntarily limited to parsing a buffer of small size (no more than
 * ASN1_MAX_BUFFER_SIZE bytes long) containing a DER encoded ASN.1
 * structure.
 *
 */


/*
 * Helper for ASN.1 DER identifier field parsing, which extracts the tag number
 * when it is not encoded on a single byte, i.e. when the first encountered byte
 * for the field is 0x1f. The function takes as parameter the buffer following
 * this 0x1f byte and, upon success, extracts the tag value.
 *
 * In our implementation, tag numbers are limited by the return type used for
 * the parameter (32 bit unsigned integers). In practice we allow tag encoded
 * on 4 bytes, i.e. with a final value of 4 * 7 bits, i.e. 28 bits. This is
 * considered largely sufficient in the context of X.509 certificates (which is
 * validated by our tests).
 *
 * Note that the function does verify that extracted tag is indeed >= 0x1f.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (tag_num != NULL) ==> \valid(tag_num);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(tag_num, eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> 1 <= *eaten <= len;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten > 0);
  @
  @ assigns *tag_num, *eaten;
  @*/
static int _extract_complex_tag(const u8 *buf, u32 len, u32 *tag_num, u32 *eaten)
{
	u32 rbytes;
	u32 t = 0;
	int ret;

	if ((len == 0) || (buf == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len > 4) {
		len = 4;
	}

	/*@ loop unroll 4;
	  @ loop invariant 0 <= rbytes <= len;
	  @ loop invariant t <= (((u32)1 << (u32)(7*(rbytes))) - 1);
	  @ loop invariant \forall integer x ; 0 <= x < rbytes ==>
		 ((buf[x] & 0x80) != 0);
	  @ loop assigns rbytes, t;
	  @ loop variant (len - rbytes);
	  @ */
	for (rbytes = 0; rbytes < len; rbytes++) {
		u32 tmp1, tmp2;
		/*@ assert rbytes <= 3; */
		/*@ assert t <= (((u32)1 << (u32)(7*(rbytes))) - 1); */
		/*@ assert t <= (u32)0x1fffff; */
		tmp1 = (t << (u32)7);
		tmp2 = ((u32)buf[rbytes] & (u32)0x7f);
		/*@ assert tmp1 <= (u32)0xfffff80; */
		/*@ assert tmp2 <= (u32)0x7f; */
		t = tmp1 + tmp2;
		/*@ assert t <= (((u32)1 << (u32)(7*(rbytes + 1))) - 1); */
		/*@ assert t <= 0xfffffff; */

		if ((buf[rbytes] & 0x80) == 0) {
			break;
		}
	}

	/* Check if we left the loop w/o finding tag's end */
	if (rbytes == len) {
		/*@ assert ((buf[len - 1] & 0x80) != 0); */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (t < 0x1f) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*tag_num = t;
	*eaten = rbytes + 1;

	ret = 0;

out:
	return ret;
}

/*
 * Parse beginning of current buffer to get the identifier parameters (class,
 * P/C flag and tag number). On success, the amount of bytes eaten to extract
 * those information is returned in 'eaten' parameter, which is guaranteed to
 * be lower or equal than 'len' parameter. On error, a non-zero negative value
 * is returned.
 *
 * Note: tags numbers are limited by the return type used for the parameter
 * (32 bit unsigned integer). In practice, this allows tag encoded on 4 bytes,
 * i.e. 4 x 7 bits, i.e. 28 bits. This is considered largely sufficient in
 * the context of X.509 certificates. An error is returned if a tag number
 * higher than that is found.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==>
	     \valid_read(buf + (0 .. (len - 1)));
  @ requires (cls != NULL) ==> \valid(cls);
  @ requires (prim != NULL) ==> \valid(prim);
  @ requires (tag_num != NULL) ==> \valid(tag_num);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(cls, prim, tag_num, eaten, buf+(..));
  @
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (0 < *eaten <= len);
  @ ensures (\result == 0) ==> (*cls <= 0x3);
  @ ensures (\result == 0) ==> (*prim <= 0x1);
  @
  @ assigns *tag_num, *eaten, *prim, *cls;
  @*/
static int get_identifier(const u8 *buf, u32 len,
			  tag_class *cls, u8 *prim, u32 *tag_num, u32 *eaten)
{
	int ret;
	u32 t;
	u32 rbytes = 0;
	u8 p;
	tag_class c;

	if (buf == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * First byte (if available) will give us the class and P/C, and also
	 * tells us (based on the value of the 6 LSB of the bytes) if the tag
	 * number is stored on additional bytes.
	 */
	if (len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* See 8.1.2.3 */
	c = (buf[0] >> 6) & 0x03; /* Extract class from 2 MSB */
	p = (buf[0] >> 5) & 0x01; /* Extract P/C bit */
	t = buf[0] & 0x1f;        /* Extract tag number from 6 LSB */
	rbytes = 1;

	/*
	 * Check if we know given class (see Table 1 from 8.1.2.2). In practice,
	 * there is no way to end up in default case, because 'c' has at most
	 * its two MSB set (see above).
	 */
	switch (c) {
	case CLASS_UNIVERSAL:
	case CLASS_APPLICATION:
	case CLASS_CONTEXT_SPECIFIC:
	case CLASS_PRIVATE:
		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

	/*
	 * If the tag number (6 LSB of first byte) we already extracted is less
	 * than 0x1f, then it directly represents the tag number (which is <=
	 * 30) and our work is over. Otherwise (t == 0x1f) the real tag number
	 * is encoded on multiple bytes following the first byte. Note that we
	 * limit ourselves to tag encoded on less than 28 bits, i.e. only accept
	 * at most 4 bytes (only 7 LSB of each byte will count because MSB tells
	 * if this is the last).
	 */
	if (t == 0x1f) {
		u32 tag_len = 0;

		/*@
		  @ assert (len >= rbytes) &&
		    \valid_read(buf + (rbytes .. len - 1));
		  @*/
		ret = _extract_complex_tag(buf + rbytes, len - rbytes,
					   &t, &tag_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		rbytes += tag_len;
	}

	/* Export what we extracted to caller */
	*cls = c;
	*prim = p;
	*tag_num = t;
	*eaten = rbytes;

	ret = 0;

out:
	return ret;
}

/*
 * Parse beginning of current buffer to get the length parameter. Input buffer
 * 'buf' has size 'len'. On success, 0 is returned, advertised length is
 * returned in 'adv_len' and the number of bytes used for the encoding of the
 * length is returned in 'eaten'.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (adv_len != NULL) ==> \valid(adv_len);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(adv_len, eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (0 < *eaten <= len);
  @ ensures (\result == 0) ==> (*adv_len <= len);
  @ ensures (\result == 0) ==> ((*adv_len + *eaten) <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *adv_len, *eaten;
  @*/
int get_length(const u8 *buf, u32 len, u32 *adv_len, u32 *eaten)
{
	u32 l, rbytes = 0;
	u32 len_len, b0;
	int ret;

	if (buf == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + 0); */
	b0 = buf[0];

	/* Single byte length (i.e. definitive form, on one byte)? */
	if ((b0 & 0x80) == 0) {
		l = b0 & 0x7f;
		/*@ assert l <= 0x7f ; */

		/*
		 * Adding 1 below to take into account the byte that
		 * encode the length is possible because l does not
		 * have its MSB set, i.e. is less than or equal to
		 * 127.
		 */
		if ((l + 1) > len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* Advertised length looks ok */
		*eaten = 1;
		*adv_len = l;
		/*@ assert (*eaten + *adv_len) <= len ; */
		ret = 0;
		goto out;
	}

	/*
	 * DER requires the definitive form for the length, i.e. that
	 * first byte of the length field is not 0x80. At that point,
	 * we already know that MSB of the byte is 1.
	 */
	if (b0 == 0x80) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We now know that the long form of the length is used. Let's
	 * extract how many bytes are used to encode the length.
	 */
	len_len = b0 & 0x7f;
	/*@ assert len_len <= 0x7f ; */
	rbytes += 1;
	/*@ assert rbytes > 0 ; */

	/*
	 * We first check that given length for the length field is not
	 * more than the size of the buffer (including the first byte
	 * encoding the length of that length). Note that we can do
	 * the addition below because MSB of len_len is 0.
	 */
	if ((len_len + 1) > len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Now that we have length's length, let's now extract its value */
	switch (len_len) {
	case 0: /* Not acceptable */
		/* Length's length cannot be 0 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;

	case 1: /* Length values in [ 128, 255 ] */
		/* assert \valid_read(buf + 1); */
		l = buf[1];
		if (l <= 127) {
			/*
			 * For such a length value, the compact encoding
			 * (definitive form) should have been used.
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert 127 < l ; */
		rbytes += 1;
		break;

	case 2: /* Length values in [ 256, 65535 ] */
		/* assert \valid_read(buf + (1 .. 2)); */
		l = (((u32)buf[1]) << 8) + buf[2];
		if (l <= 0xff) {
			/* Why 2 bytes if most significant is 0? */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert 0xff < l <= 0xffff ; */
		rbytes += 2;
		break;

	case 3: /* Length values in [ 65536, 16777215 ] */
		/* assert \valid_read(buf + (1 .. 3)); */
		l = (((u32)buf[1]) << 16) + (((u32)buf[2]) << 8) + buf[3];
		if (l <= 0xffff) {
			/* Why 3 bytes if most significant is 0? */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert 0xffff < l <= 0xffffff ; */
		rbytes += 3;
		break;

	case 4: /* Length values in [ 16777215, 4294967295 ] */
		/* assert \valid_read(buf + (1 .. 4)); */
		l = (((u32)buf[1]) << 24) + (((u32)buf[2]) << 16) + (((u32)buf[3]) << 8) + buf[4];
		if (l <= 0xffffff) {
			/* Why 4 bytes if most significant is 0? */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert 0xffffff < (u64)l <= 0xffffffff ; */
		rbytes += 4;
		break;

	default: /* Not acceptable */
		/*
		 * Length cannot be encoded on more than fours bytes (we
		 * have an *intentional* internal limitation for
		 * all ASN.1 DER structures set to 2^32 - 1 bytes (all
		 * our lengths are u32)
		 */
		 ret = -X509_FILE_LINE_NUM_ERR;
		 ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		 goto out;
		 break;
	}

	/*@ assert l > 127 ; */
	/*@ assert len >= rbytes ; */
	if ((len - rbytes) < l) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (rbytes + l) <= len ; */
	/*@ assert rbytes > 0 ; */
	*eaten = rbytes;
	*adv_len = l;

	ret = 0;

out:
	return ret;
}

/*
 * All DER-encoded elements are basically TLV structures (or identifier octets,
 * length octets, contents octets). This function parses the T and L elements
 * from given buffer and verifies the advertised length for the value (content
 * octets) does not overflow outside of the buffer. Additionally, the expected
 * class and type for the tag are verified. On success, the size of parsed
 * elements (class, type and length) are returned in 'parsed' and the size of
 * content octets are returned in 'content_len'.
 *
 * Note that the function does not parse the content of the encoded value, i.e.
 * the 'content_len' bytes in the buffer after the 'parsed' (TL header) ones. It
 * only guarantees that they are in the buffer.
 *
 * This function is critical for the security of the module.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (parsed != NULL) ==> \valid(parsed);
  @ requires (content_len != NULL) ==> \valid(content_len);
  @ requires \separated(parsed, content_len, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *parsed <= len);
  @ ensures (\result == 0) ==> (*content_len <= len);
  @ ensures (\result == 0) ==> (1 < (*content_len + *parsed) <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *parsed, *content_len;
  @*/
int parse_id_len(const u8 *buf, u32 len, tag_class exp_class,
			u32 exp_type, u32 *parsed, u32 *content_len)
{
	tag_class c = 0;
	u8 p;
	u32 t = 0;
	u32 cur_parsed = 0;
	u32 grabbed;
	u32 adv_len = 0;
	int ret;

	if (buf == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Get the first part of the encoding, i.e. the identifier */
	ret = get_identifier(buf, len, &c, &p, &t, &cur_parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert cur_parsed > 0; */

	/*
	 * Now, verify we are indeed dealing with an element of
	 * given type ...
	 */
	if (t != exp_type) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* ... and class. */
	if (c != exp_class) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	grabbed = cur_parsed;
	/*@ assert grabbed > 0; */
	len -= cur_parsed;
	buf += cur_parsed;

	/* Get the second part of the encoding, i.e. the length */
	ret = get_length(buf, len, &adv_len, &cur_parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert cur_parsed > 0; */

	grabbed += cur_parsed;
	/*@ assert grabbed > 1; */
	len -= cur_parsed;
	buf += cur_parsed;

	/* Verify advertised length is smaller than remaining buffer length */
	if (adv_len > len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*parsed = grabbed;
	/*@ assert *parsed > 1; */
	*content_len = adv_len;

	ret = 0;

out:
	return ret;
}

/*
 * Here, we have to deal with a wrapper around our a usual ASN.1 TLV.
 * The id of that wrapper has a given type and a context specific
 * class (CLASS_CONTEXT_SPECIFIC), i.e. a T1L1T2L2V where
 * T1 has exp_ext_type and class CLASS_CONTEXT_SPECIFIC
 * L1 provides the length of T2L2V
 * T2 has exp_int_type and exp_int_class
 * L2 provides the length of V
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (parsed != NULL) ==> \valid(parsed);
  @ requires (data_len != NULL) ==> \valid(data_len);
  @ requires \separated(parsed, data_len, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*parsed <= len);
  @ ensures (\result == 0) ==> (*data_len <= len);
  @ ensures (\result == 0) ==> ((*data_len + *parsed) <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *parsed, *data_len;
  @*/
int parse_explicit_id_len(const u8 *buf, u32 len,
				 u32 exp_ext_type,
				 tag_class exp_int_class, u32 exp_int_type,
				 u32 *parsed, u32 *data_len)
{
	u32 hdr_len = 0;
	u32 val_len = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Work on external packaging */
	ret = parse_id_len(buf, len, CLASS_CONTEXT_SPECIFIC,
			   exp_ext_type, &hdr_len, &val_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	len -= hdr_len;
	*parsed = hdr_len;

	/* Work on internal packaging */
	ret = parse_id_len(buf, len, exp_int_class, exp_int_type,
			   &hdr_len, &val_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= hdr_len;
	*parsed += hdr_len;
	if (len < val_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Export the size of data */
	*data_len = val_len;

	ret = 0;

out:
	return ret;
}


/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (arc_val != NULL) ==> \valid(arc_val);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(arc_val, eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten > 0);
  @ ensures ((len > 0) && (buf != \null) && (\result != 0)) ==>
	    \forall integer x ; 0 <= x < \min(len, 4) ==>
	    ((buf[x] & 0x80) != 0);
  @ ensures (len == 0) ==> \result < 0;
  @
  @ assigns *arc_val, *eaten;
  @*/
static int _parse_arc(const u8 *buf, u32 len, u32 *arc_val, u32 *eaten)
{
	u32 rbytes;
	u32 av = 0;
	int ret;

	if ((len == 0) || (buf == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC 5280 has "There is no maximum size for OIDs. This specification
	 * mandates support for OIDs that have arc elements with values that
	 * are less than 2^28, that is, they MUST be between 0 and 268,435,455,
	 * inclusive. This allows each arc element to be represented within a
	 * single 32-bit word." For that reason, we just leave if we end up
	 * encountering more than 4 bytes here.
	 */
	if (len > 4) {
		len = 4;
	}

	/*@ loop unroll 4;
	  @ loop invariant 0 <= rbytes <= len;
	  @ loop invariant av <= (((u32)1 << (u32)(7*(rbytes))) - 1);
	  @ loop invariant \forall integer x ; 0 <= x < rbytes ==>
		 ((buf[x] & 0x80) != 0);
	  @ loop assigns rbytes, av;
	  @ loop variant (len - rbytes);
	  @ */
	for (rbytes = 0; rbytes < len; rbytes++) {
		u32 tmp1, tmp2;

		/*@ assert rbytes <= 3; */
		/*@ assert av <= (((u32)1 << (u32)(7*(rbytes))) - 1); */
		/*@ assert av <= (u32)0x1fffff; */

		tmp1 = (av << (u32)7);
		/*@ assert tmp1 <= (u32)0xfffff80; */
		tmp2 = ((u32)buf[rbytes] & (u32)0x7f);
		/*@ assert tmp2 <= (u32)0x7f; */
		av = tmp1 + tmp2;
		/*@ assert av <= (((u32)1 << (u32)(7*(rbytes + 1))) - 1); */
		/*@ assert av <= 0xfffffff; */

		if ((buf[rbytes] & 0x80) == 0) {
			break;
		}
	}

	if (rbytes >= len) {
		/*@ assert ((buf[len - 1] & 0x80) != 0); */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*arc_val = av;
	*eaten = rbytes + 1;

	ret = 0;

out:
	return ret;
}


static const u8 null_encoded_val[] = { 0x05, 0x00 };

/*
 * Implements a function for parsing ASN1. NULL object. On success, the function
 * returns 0 and set 'parsed' parameters to the amount of bytes parsed (i.e. 2).
 * -1 is returned on error.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (parsed != NULL) ==> \valid(parsed);
  @ requires \separated(parsed, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> *parsed == 2;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *parsed;
  @*/
int parse_null(const u8 *buf, u32 len, u32 *parsed)
{
	int ret;

	if ((len == 0) || (buf == NULL) || (parsed == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len != sizeof(null_encoded_val)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = bufs_differ(buf, null_encoded_val, sizeof(null_encoded_val));
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;
	*parsed = sizeof(null_encoded_val);

out:
	return ret;
}

/*
 * Implements a function for parsing OID as described in section 8.19
 * of X.690. On success, the function returns 0 and set 'parsed'
 * parameters to the amount of bytes on which the OID is encoded
 * (header and content bytes).
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (parsed != NULL) ==> \valid(parsed);
  @ requires \separated(parsed,buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < *parsed <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *parsed;
  @*/
int parse_OID(const u8 *buf, u32 len, u32 *parsed)
{
	u32 data_len = 0;
	u32 hdr_len = 0;
	u32 remain = 0;
	u32 num;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_OID,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= hdr_len;
	buf += hdr_len;
	if (data_len < 1) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert \valid_read(buf + (0 .. (data_len - 1))); */

	remain = data_len;
	num = 0;

	/*@
	  @ loop assigns ret, num, buf, remain;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		u32 arc_val = 0;
		u32 rbytes = 0;

		/*
		 * RFC 5280 has "Implementations MUST be able to handle
		 * OIDs with up to 20 elements (inclusive)".
		 */
		if (num > 20) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		ret = _parse_arc(buf, remain, &arc_val, &rbytes);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*@ assert rbytes <= remain ; */

		num += 1;

		buf += rbytes;
		remain -= rbytes;
	}

	/*
	 * Let's check the OID had at least the first initial
	 * subidentifier (the one derived from the two first
	 * components) as described in section 8.19 of X.690.
	 */
	if (num < 1) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*parsed = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 * Implements a function for parsing integers as described in section 8.3
 * of X.690. As integers may be used in a context specific way, we allow
 * passing the expected class and type values which are to be found. The
 * parameter pos_or_zero allows specifying if only non-negative integer
 * (postive or zero) should be accepted. When set, the function returns
 * a non zero value when the parsed integer is negative. The functions
 * is not expected to be used directly but only as a helper in the
 * two function that follows it.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (hdr_len != NULL) ==> \valid(hdr_len);
  @ requires (data_len != NULL) ==> \valid(data_len);
  @ requires \separated(hdr_len, data_len, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> ((u64)2 < ((u64)*hdr_len + (u64)*data_len) <= (u64)len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *hdr_len, *data_len;
  @*/
static int _parse_integer(const u8 *buf, u32 len,
			  tag_class exp_class, u32 exp_type,
			  u32 *hdr_len, u32 *data_len,
			  int pos_or_zero)
{
	int ret;

	if ((buf == NULL) || (len == 0) || (hdr_len == NULL) || (data_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*hdr_len = 0;
	*data_len = 0;
	ret = parse_id_len(buf, len, exp_class, exp_type, hdr_len, data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += *hdr_len;

	/*
	 * Regarding integer encoding, 8.3.1 of X.690 has "The contents octets
	 * shall consist of one or more octets".
	 */
	if (*data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * On integer encoding, 8.3.2 of x.690 has "If the contents octets of
	 * an integer value encoding consist of more than one octet, then the
	 * bits of the first octet and bit 8 of the second octet:
	 *
	 *  a) shall not all be ones; and
	 *  b) shall not all be zero.
	 */
	if (*data_len > 1) {
		if ((buf[0] == 0) && ((buf[1] & 0x80) == 0)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		if ((buf[0] == 0xff) && (buf[1] & 0x80)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	if (pos_or_zero && (buf[0]) & 0x80) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Common version. Allow positive and negative integers.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (hdr_len != NULL) ==> \valid(hdr_len);
  @ requires (data_len != NULL) ==> \valid(data_len);
  @ requires \separated(hdr_len, data_len, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < ((u64)*hdr_len + (u64)*data_len) <= (u64)len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *hdr_len, *data_len;
  @*/
int parse_integer(const u8 *buf, u32 len,
				tag_class exp_class, u32 exp_type,
				u32 *hdr_len, u32 *data_len)
{
	return _parse_integer(buf, len, exp_class, exp_type,
			      hdr_len, data_len, 0);
}

/*
 * Same as previous but additionally enforce the value at hand is not negative,
 * i.e. positive or 0. This is for instance useful for integer encoding value
 * that are reduced modulo another value (e.g. ECDSA signature components).
 * The function returns 0 when the integer is valid and positive.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (hdr_len != NULL) ==> \valid(hdr_len);
  @ requires (data_len != NULL) ==> \valid(data_len);
  @ requires \separated(hdr_len, data_len, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < ((u64)*hdr_len + (u64)*data_len) <= (u64)len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *hdr_len, *data_len;
  @*/
int parse_non_negative_integer(const u8 *buf, u32 len,
					     tag_class exp_class, u32 exp_type,
					     u32 *hdr_len, u32 *data_len)
{
	return _parse_integer(buf, len, exp_class, exp_type,
			      hdr_len, data_len, 1);
}

/*
 * Implements a function for parsing booleans as described in section 8.2
 * of X.690. When encoded in DER, a boolean is a 3 bytes elements which
 * can take a value of:
 *
 *  FALSE : { 0x01, 0x01, 0x00 }
 *  TRUE  : { 0x01, 0x01, 0xff }
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_boolean(const u8 *buf, u32 len, u32 *eaten)
{
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 3) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if ((buf[0] != ASN1_TYPE_BOOLEAN) || (buf[1] != 0x01)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	switch (buf[2]) {
	case 0x00: /* FALSE */
	case 0xff: /* TRUE  */
		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

	*eaten = 3;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires 0x30 <= d <= 0x39;
  @ requires 0x30 <= u <= 0x39;
  @
  @ ensures 0 <= \result <= 99;
  @
  @ assigns \nothing;
  @*/
static u8 compute_decimal(u8 d, u8 u)
{
	return (d - 0x30) * 10 + (u - 0x30);
}

/* Validate UTCTime (including the constraints from RFC 5280) */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (year != NULL) ==> \valid(year);
  @ requires (month != NULL) ==> \valid(month);
  @ requires (day != NULL) ==> \valid(day);
  @ requires (hour != NULL) ==> \valid(hour);
  @ requires (min != NULL) ==> \valid(min);
  @ requires (sec != NULL) ==> \valid(sec);
  @ requires \separated(eaten, year, month, day, hour, min, sec, buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten == 15);
  @ ensures (len < 15) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> 1950 <= *year <= 2049;
  @ ensures (\result == 0) ==> *month <= 12;
  @ ensures (\result == 0) ==> *day <= 31;
  @ ensures (\result == 0) ==> *hour <= 23;
  @ ensures (\result == 0) ==> *min <= 59;
  @ ensures (\result == 0) ==> *sec <= 59;
  @
  @ assigns *eaten, *year, *month, *day, *hour, *min, *sec;
  @*/
static int parse_UTCTime(const u8 *buf, u32 len, u32 *eaten,
			 u16 *year, u8 *month, u8 *day,
			 u8 *hour, u8 *min, u8 *sec)
{
	u16 yyyy;
	u8 mo, dd, hh, mm, ss;
	const u8 c_zero = '0';
	u8 time_type;
	u8 time_len;
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 i;

	if (buf == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As described in section 4.1.2.5.1 of RFC 5280, we do
	 * expect the following encoding: YYMMDDHHMMSSZ, i.e.
	 * a length of at least 15 bytes for the buffer, i.e.
	 * an advertised length of 13 bytes for the string
	 * it contains.
	 */
	if (len < 15) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	time_type = buf[0];
	if (time_type != ASN1_TYPE_UTCTime) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	time_len = buf[1];
	if (time_len != 13) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	/*
	 * Check all first 12 characters are decimal digits and
	 * last one is character 'Z'
	 */
	/*@
	  @ loop invariant \valid_read(buf + i);
	  @ loop invariant \forall integer x ; 0 <= x < i ==>
		 0x30 <= buf[x] <= 0x39;
	  @ loop assigns i;
	  @ loop variant (12 - i);
	  @ */
	for (i = 0; i < 12; i++) {
		if (c_zero > buf[i]) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		if ((buf[i] - c_zero) > 9) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert 0 <= buf[i] - c_zero <= 9; */
	}
	if (buf[12] != 'Z') {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert c_zero == 0x30; */
	/*@ assert \forall integer x ; 0 <= x < 12 ==> 0x30 <= buf[x] <= 0x39; */
	yyyy = compute_decimal(buf[0], buf[1]);
	/*@ assert yyyy <= 99; */
	if (yyyy >= 50) {
		yyyy += 1900;
		/*@ assert 1950 <= yyyy <= 1999; */
	} else {
		yyyy += 2000;
		/*@ assert 2000 <= yyyy <= 2049; */
	}
	/*@ assert 1950 <= yyyy <= 2049; */

	mo = compute_decimal(buf[ 2], buf[ 3]);
	dd = compute_decimal(buf[ 4], buf[ 5]);
	hh = compute_decimal(buf[ 6], buf[ 7]);
	mm = compute_decimal(buf[ 8], buf[ 9]);
	ss = compute_decimal(buf[10], buf[11]);

	/*
	 * Check values are valid.
	 *
	 * n.b.: for dates in validity period, RFC 5280 requires the use of
	 * UTCTime for dates through the year 2049. Dates in 2050 or later
	 * MUST be encoded as GeneralizedTime. But this is not true for
	 * use of GeneralizeTime used in other fields in X.509 structures:
	 * for instance, the Invalidity date in CRL is expected to encode
	 * dates in GeneralizedTime for all values of time. For that reason
	 * the check for the year (<= 2049 or >= 2050) is done at a higher
	 * level, i.e. see verify_correct_time_use().
	 *
	 */
	/*    month         day          hour         min          sec     */
	if ((mo > 12) || (dd > 31) || (hh > 23) || (mm > 59) || (ss > 59)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Export what we extracted */
	*year  = yyyy;
	*month = mo;
	*day   = dd;
	*hour  = hh;
	*min   = mm;
	*sec   = ss;

	ret = 0;

out:
	if (!ret) {
		*eaten = 15;
	}

	return ret;
}

/*@
  @ requires 0x30 <= d1 <= 0x39;
  @ requires 0x30 <= d2 <= 0x39;
  @ requires 0x30 <= d3 <= 0x39;
  @ requires 0x30 <= d4 <= 0x39;
  @
  @ ensures 0 <= \result <= 9999;
  @
  @ assigns \nothing;
  @*/
static u16 compute_year(u8 d1, u8 d2, u8 d3, u8 d4)
{
	return ((u16)d1 - (u16)0x30) * 1000 +
	       ((u16)d2 - (u16)0x30) * 100 +
	       ((u16)d3 - (u16)0x30) * 10 +
	       ((u16)d4 - (u16)0x30);
}

/*
 * Validate generalizedTime (including the constraints from RFC 5280). Note that
 * the code is very similar to the one above developed for UTCTime. It
 * could be possible to merge the functions into a unique helper
 * but this would impact readibility.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (year != NULL) ==> \valid(year);
  @ requires (month != NULL) ==> \valid(month);
  @ requires (day != NULL) ==> \valid(day);
  @ requires (hour != NULL) ==> \valid(hour);
  @ requires (min != NULL) ==> \valid(min);
  @ requires (sec != NULL) ==> \valid(sec);
  @ requires \separated(eaten, year, month, day, hour, min, sec, buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten == 17);
  @ ensures (len < 17) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> 0 <= *year <= 9999;
  @ ensures (\result == 0) ==> *month <= 12;
  @ ensures (\result == 0) ==> *day <= 31;
  @ ensures (\result == 0) ==> *hour <= 23;
  @ ensures (\result == 0) ==> *min <= 59;
  @ ensures (\result == 0) ==> *sec <= 59;
  @
  @ assigns *eaten, *year, *month, *day, *hour, *min, *sec;
  @*/
int parse_generalizedTime(const u8 *buf, u32 len, u32 *eaten,
				 u16 *year, u8 *month, u8 *day,
				 u8 *hour, u8 *min, u8 *sec)
{
	u16 yyyy;
	u8 mo, dd, hh, mm, ss;
	const u8 c_zero = '0';
	u8 time_type;
	u8 time_len;
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 i;

	if (buf == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As described in section 4.1.2.5.2 of RFC 5280, we do
	 * expect the following encoding: YYYYMMDDHHMMSSZ, i.e.
	 * a length of at least 17 bytes for the buffer, i.e.
	 * an advertised length of 15 bytes for the string
	 * it contains.
	 */
	if (len < 17) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	time_type = buf[0];
	if (time_type != ASN1_TYPE_GeneralizedTime) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	time_len = buf[1];
	if (time_len != 15) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	/*
	 * Check all first 14 characters are decimal digits and
	 * last one is character 'Z'
	 */
	/*@
	  @ loop invariant \valid_read(buf + i);
	  @ loop invariant \forall integer x ; 0 <= x < i ==>
		 0x30 <= buf[x] <= 0x39;
	  @ loop assigns i;
	  @ loop variant (14 - i);
	  @ */
	for (i = 0; i < 14; i++) {
		if (c_zero > buf[i]) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		if ((buf[i] - c_zero) > 9) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}
	if (buf[14] != 'Z') {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert c_zero == 0x30; */
	/*@ assert \forall integer x ; 0 <= x < 12 ==> 0x30 <= buf[x] <= 0x39; */
	yyyy = compute_year(buf[0], buf[1], buf[2], buf[3]);
	/*@ assert 0 <= yyyy <= 9999 ; */
	mo = compute_decimal(buf[ 4], buf[ 5]);
	dd = compute_decimal(buf[ 6], buf[ 7]);
	hh = compute_decimal(buf[ 8], buf[ 9]);
	mm = compute_decimal(buf[10], buf[11]);
	ss = compute_decimal(buf[12], buf[13]);

	/*
	 * Check values are valid.
	 *
	 * n.b.: for dates in validity period, RFC 5280 requires the use of
	 * UTCTime for dates through the year 2049. Dates in 2050 or later
	 * MUST be encoded as GeneralizedTime. But this is not true for
	 * use of GeneralizeTime used in other fields in X.509 structures:
	 * for instance, the Invalidity date in CRL is expected to encode
	 * dates in GeneralizedTime for all values of time. For that reason
	 * the check for the year (<= 2049 or >= 2050) is done at a higher
	 * level, i.e. see _verify_correct_time_use().
	 */
	/*    month         day          hour         min          sec     */
	if ((mo > 12) || (dd > 31) || (hh > 23) || (mm > 59) || (ss > 59)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert mo <= 12; */
	/*@ assert dd <= 31; */
	/*@ assert hh <= 23; */
	/*@ assert mm <= 59; */
	/*@ assert ss <= 59; */

	/* Export what we extracted */
	*year  = yyyy;
	*month = mo;
	*day   = dd;
	*hour  = hh;
	*min   = mm;
	*sec   = ss;

	ret = 0;

out:
	if (!ret) {
		*eaten = 17;
	}

	return ret;
}


/*
 * This function verify given buffer contains a valid UTF-8 string
 * -1 is returned on error, 0 on success.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_utf8_string(const u8 *buf, u32 len)
{
	int ret;
	u8 b0;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop invariant \valid_read(buf + (0 .. (len - 1)));
	  @ loop assigns b0, len, buf, ret;
	  @ loop variant len;
	  @ */
	while (len) {
		b0 = buf[0];

		/*
		 * CP encoded on a single byte, coding from 1 to 7 bits,
		 * U+000 to U+07FF.
		 */

		if (b0 <= 0x7f) {                   /* 0x00 to 0x7f */
			len -= 1;
			buf += 1;
			continue;
		}

		/*
		 * CP encoded on 2 bytes, coding from 8 to 11 bits,
		 * U+0080 to U+07FF
		 */

		if ((b0 >= 0xc2) && (b0 <= 0xdf)) { /* 0xc2 to 0xdf */
			if (len < 2) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			/*@ assert \valid_read(buf + (0 .. 1)); */
			if ((buf[1] & 0xc0) != 0x80) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			len -= 2;
			buf += 2;
			continue;
		}

		/*
		 * CP encoded on 3 bytes, coding 12 to 16 bits,
		 * U+0800 to U+FFFF.
		 */

		if ((b0 >= 0xe0) && (b0 <= 0xef)) { /* 0xe0 to 0xef */
			if (len < 3) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			/*@ assert \valid_read(buf + (0 .. 2)); */
			if (((buf[1] & 0xc0) != 0x80) ||
			    ((buf[2] & 0xc0) != 0x80)) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			/*
			 * 1rst byte is 0xe0 => 2nd byte in [0xa0, 0xbf]
			 * 1rst byte is 0xed => 2nd byte in [0x80, 0x9f]
			 */
			if ((b0 == 0xe0) &&
			    ((buf[1] < 0xa0) || (buf[1] > 0xbf))) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			} else if ((b0 == 0xed) &&
				   ((buf[1] < 0x80) || (buf[1] > 0x9f))) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			len -= 3;
			buf += 3;
			continue;
		}

		/*
		 * CP encoded on 4 bytes, coding 17 to 21 bits,
		 * U+10000 to U+10FFFF.
		 */

		if ((b0 >= 0xf0) && (b0 <= 0xf4)) { /* 0xf0 to 0xf4 */
			if (len < 4) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			/*
			 * 1rst byte is 0xe0 => 2nd byte in [0xa0, 0xbf]
			 * 1rst byte is 0xed => 2nd byte in [0x80, 0x9f]
			 */
			/*@ assert \valid_read(buf + (0 .. 3)); */
			if ((b0 == 0xf0) &&
			    ((buf[1] < 0x90) || (buf[1] > 0xbf))) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			} else if ((b0 == 0xf4) &&
				   ((buf[1] < 0x80) || (buf[1] > 0x8f))) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			if (((buf[1] & 0xc0) != 0x80) ||
			    ((buf[2] & 0xc0) != 0x80) ||
			    ((buf[3] & 0xc0) != 0x80)) {
				ret = -X509_FILE_LINE_NUM_ERR;
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			len -= 4;
			buf += 4;
			continue;
		}

		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Verify given buffer contains only printable characters. -1 is
 * returned on error, 0 on success.
 *
 * RFC 5280 has: "The character string type PrintableString supports a
 * very basic Latin character set: the lowercase letters 'a' through 'z',
 * uppercase letters 'A' through 'Z', the digits '0' through '9',
 * eleven special characters ' = ( ) + , - . / : ? and space."
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_printable_string(const u8 *buf, u32 len)
{
	int ret;
	u32 rbytes;
	u8 c;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop invariant 0 <= rbytes <= len;
	  @ loop assigns rbytes, c, ret;
	  @ loop variant (len - rbytes);
	  @ */
	for (rbytes = 0; rbytes < len; rbytes++) {
		c = buf[rbytes];

		if ((c >= 'a' && c <= 'z') ||
		    (c >= 'A' && c <= 'Z') ||
		    (c >= '0' && c <= '9')) {
			continue;
		}

		switch (c) {
		case 39: /* ' */
		case '=':
		case '(':
		case ')':
		case '+':
		case ',':
		case '-':
		case '.':
		case '/':
		case ':':
		case '?':
		case ' ':
			continue;
		default:
			break;
		}

		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Verify given buffer contains only numeric characters. -1 is
 * returned on error, 0 on success.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_numeric_string(const u8 *buf, u32 len)
{
	int ret;
	u32 rbytes;
	u8 c;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop invariant 0 <= rbytes <= len;
	  @ loop assigns rbytes, c, ret;
	  @ loop variant (len - rbytes);
	  @ */
	for (rbytes = 0; rbytes < len; rbytes++) {
		c = buf[rbytes];

		if ((c < '0') || (c > '9')) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	ret = 0;

out:
	return ret;
}

/*
 * VisibleString == ISO646String == ASCII
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_visible_string(const u8 *buf, u32 len)
{
	int ret;
	u32 rbytes = 0;
	u8 c;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop assigns rbytes, c, ret;
	  @ loop variant (len - rbytes);
	  @ */
	while (rbytes < len) {
		c = buf[rbytes];

		if ((c >= 'a' && c <= 'z') ||
		    (c >= 'A' && c <= 'Z') ||
		    (c >= '0' && c <= '9')) {
			rbytes += 1;
			continue;
		}

		switch (c) {
		case 39: /* ' */
		case '=':
		case '(':
		case ')':
		case '+':
		case ',':
		case '-':
		case '.':
		case '/':
		case ':':
		case '?':
		case ' ':
		case '!':
		case '"':
		case '#':
		case '$':
		case '%':
		case '&':
		case '*':
		case ';':
		case '<':
		case '>':
		case '[':
		case '\\':
		case ']':
		case '^':
		case '_':
		case '`':
		case '{':
		case '|':
		case '}':
		case '~':
			rbytes += 1;
			continue;
		default:
			break;
		}

		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

#ifdef TEMPORARY_LAXIST_DIRECTORY_STRING
/*
 * Teletex string is not supposed to be supported and there is no good
 * defintiion of allowed charset. At the moment, we perform the check
 * using visible string charset. XXX we should revisit that later
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_teletex_string(const u8 *buf, u32 len)
{
	return check_visible_string(buf, len);
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_universal_string(const u8 ATTRIBUTE_UNUSED *buf,
				  u32 ATTRIBUTE_UNUSED len)
{
	return -X509_FILE_LINE_NUM_ERR;
}
#endif

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_bmp_string(const u8 ATTRIBUTE_UNUSED *buf,
			    u32 ATTRIBUTE_UNUSED len)
{
	/* Support is OPTIONAL */
	return -X509_FILE_LINE_NUM_ERR;
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int check_ia5_string(const u8 *buf, u32 len)
{
	int ret;
	u32 i;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop invariant \forall integer x ; 0 <= x < i ==>
		 ((buf[x] <= 0x7f));
	  @ loop assigns i;
	  @ loop variant (len - i);
	  @ */
	for (i = 0; i < len; i++) {
		if (buf[i] > 0x7f) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	ret = 0;

out:
	return ret;
}


/*
 * Parse GeneralName (used in SAN and AIA extensions)
 *
 *  GeneralName ::= CHOICE {
 *       otherName                 [0]  AnotherName,
 *       rfc822Name                [1]  IA5String,
 *       dNSName                   [2]  IA5String,
 *       x400Address               [3]  ORAddress,
 *       directoryName             [4]  Name,
 *       ediPartyName              [5]  EDIPartyName,
 *       uniformResourceIdentifier [6]  IA5String,
 *       iPAddress                 [7]  OCTET STRING,
 *       registeredID              [8]  OBJECT IDENTIFIER }
 *
 *  OtherName ::= SEQUENCE {
 *       type-id    OBJECT IDENTIFIER,
 *       value      [0] EXPLICIT ANY DEFINED BY type-id }
 *
 *  EDIPartyName ::= SEQUENCE {
 *       nameAssigner            [0]     DirectoryString OPTIONAL,
 *       partyName               [1]     DirectoryString }
 *
 */

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (empty != NULL) ==> \valid(empty);
  @ requires \separated(eaten, empty, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (\result == 0) ==> (0 <= *empty <= 1);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten, *empty;
  @*/
int parse_GeneralName(const u8 *buf, u32 len, u32 *eaten, int *empty)
{
	u32 remain = 0, name_len = 0, name_hdr_len = 0, grabbed = 0;
	u8 name_type;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/*
	 * We expect the id for current name to be encoded on
	 * a single byte, i.e. we expect its MSB to be set.
	 */
	name_type = buf[0];
	if (!(name_type & 0x80)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	switch (name_type) {
	case NAME_TYPE_rfc822Name: /* 0x81 - rfc822Name - IA5String */
	case NAME_TYPE_dNSName:    /* 0x82 - dNSName - IA5String */
	case NAME_TYPE_URI:        /* 0x86 - uniformResourceIdentifier - IA5String */
		buf += 1;
		remain -= 1;

		ret = get_length(buf, remain, &name_len, &grabbed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		buf += grabbed;
		remain -= grabbed;

		if (name_len > remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		ret = check_ia5_string(buf, name_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* Now, do some more specific checks */
		switch (name_type) {
		case NAME_TYPE_rfc822Name: /* rfc822Name - IA5String */

			/* FIXME! We should do more parsing on that one */

			break;
		case NAME_TYPE_dNSName: /* dNSName - IA5String */

			/* FIXME! We should do more parsing on that one */
			/*
			 * From section 4.2.1.6 of RFC5280:
			 * The name MUST be in the "preferred name syntax",
			 * as specified by Section 3.5 of [RFC1034] and as
			 * modified by Section 2.1 of [RFC1123].
			 */
			break;
		case NAME_TYPE_URI: /* uniformResourceIdentifier - IA5String */

			/* FIXME! We should do more parsing on that one */

			break;
		default:
			break;
		}

		remain -= name_len;
		buf += name_len;
		*eaten = name_len + grabbed + 1;
		*empty = !name_len;
		/*@ assert *eaten > 1; */
		break;

	case NAME_TYPE_iPAddress: /* 0x87 - iPaddress - OCTET STRING */
		buf += 1;
		remain -= 1;

		ret = get_length(buf, remain, &name_len, &grabbed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		buf += grabbed;
		remain -= grabbed;

		if (name_len > remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* FIXME! Check size is 4, resp. 16, for IPv4, resp. IPv6. */

		remain -= name_len;
		buf += name_len;
		*eaten = name_len + grabbed + 1;
		*empty = !name_len;
		/*@ assert *eaten > 1; */
		break;

	case NAME_TYPE_otherName: /* 0xa0 - otherName - OtherName */
		/* FIXME! unsupported */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;

	case NAME_TYPE_x400Address: /* 0xa3 - x400Address - ORAddress */
		/* FIXME! unsupported */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;

	case NAME_TYPE_directoryName: /* 0xa4 - directoryName - Name */
		ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 4,
				   &name_hdr_len, &name_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += name_hdr_len;
		remain = name_len;

		ret = parse_x509_Name(buf, remain, &grabbed, empty);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += grabbed;
		remain -= grabbed;

		if (remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		*eaten = name_hdr_len + name_len;
		/*@ assert *eaten > 1; */
		break;

	case NAME_TYPE_ediPartyName: /* 0xa5 - ediPartyName - EDIPartyName */
		/* FIXME! unsupported */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;

	case NAME_TYPE_registeredID: /* 0x88 - registeredID - OBJECT IDENTIFIER */
		/* FIXME! unsupported */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;

	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

	/*@ assert *eaten > 1; */
	ret = 0;

out:
	return ret;
}


/* GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_GeneralNames(const u8 *buf, u32 len, tag_class exp_class,
		       u32 exp_type, u32 *eaten)
{
	u32 remain, parsed = 0, hdr_len = 0, data_len = 0;
	int ret, unused = 0;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, exp_class, exp_type,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/*@
	  @ loop assigns ret, buf, remain, parsed, unused;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_GeneralName(buf, remain, &parsed, &unused);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= parsed;
		buf += parsed;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}


/*
 * used for SerialNumber (in tbsCertificate or AKI). As the underlying integer
 * might be used with context specific class and types, those two elements are
 * passed to the function and verified to match in given encoding.
 *
 *     CertificateSerialNumber  ::=  INTEGER
 *
 */
#define MAX_SERIAL_NUM_LEN 22 /* w/ header */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_SerialNumber(const u8 *cert, u32 off, u32 len,
		       tag_class exp_class, u32 exp_type,
		       u32 *eaten)
{
	const u8 *buf = cert + off;
	u32 parsed = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((cert == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Verify the integer is DER-encoded as it should */
	ret = parse_integer(buf, len, exp_class, exp_type,
			    &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	parsed = hdr_len + data_len;
	/*@ assert parsed > 2; */

	*eaten = parsed;
	/*@ assert *eaten > 2; */

	/*
	 * We now have the guarantee the integer has the following format:
	 * [2 bytes for t/c and len][data_len bytes for encoded value]
	 */

	/*
	 * Serial is expected not to be 0. Because we are guaranteed with the
	 * check above to deal with a DER encoded integer, 0 would be encoded
	 * on exactly 3 bytes (2 for header and 1 for the value), the last one
	 * being 0.
	 */
	if ((data_len == 1) && (buf[2] == 0)) {
#ifndef TEMPORARY_LAXIST_SERIAL_NULL
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
#else
		ret = 0;
		goto out;
#endif
	}

	/*
	 * serialNumber value is expected to be at most 20 bytes long, which
	 * makes 22 bytes for the whole structure (if we include the associated
	 * two bytes header (a length of 20 is encoded on a single byte of
	 * header following the type/class byte.
	 */
	if (parsed > MAX_SERIAL_NUM_LEN) {
#ifndef TEMPORARY_LAXIST_SERIAL_LENGTH
		ret = -X509_FILE_LINE_NUM_ERR;
	       ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	       goto out;
#else
	       ret = 0;
#endif
	}

	/* ... and be positive */
	if (buf[2] & 0x80) {
#ifndef TEMPORARY_LAXIST_SERIAL_NEGATIVE
		/*
		 * RFC has a MUST (4.1.2.2) for serial integer to
		 * be positive.
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
#else
		/* here, we let it happen */
		ret = 0;
#endif
	}

	ret = 0;

out:
	return ret;
}

/*
 * RFC 5280 has "CAs conforming to this profile MUST always encode certificate
 * validity dates through the year 2049 as UTCTime; certificate validity dates
 * in 2050 or later MUST be encoded as GeneralizedTime."
 *
 * This function performs that simple check. It returns 0 on success, a non
 * zero value on error.
 */
/*@ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
int verify_correct_time_use(u8 time_type, u16 yyyy)
{
	int ret;

	switch (time_type) {
	case ASN1_TYPE_UTCTime:
		ret = (yyyy <= 2049) ? 0 : -X509_FILE_LINE_NUM_ERR;
		break;
	case ASN1_TYPE_GeneralizedTime:
		ret = (yyyy >= 2050) ? 0 : -X509_FILE_LINE_NUM_ERR;
		break;
	default:
		ret = -1;
		break;
	}

	return ret;

}

/*
 * Time ::= CHOICE {
 *    utcTime        UTCTime,
 *    generalTime    GeneralizedTime }
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (t_type != NULL) ==> \valid(t_type);
  @ requires (year != NULL) ==> \valid(year);
  @ requires (month != NULL) ==> \valid(month);
  @ requires (day != NULL) ==> \valid(day);
  @ requires (hour != NULL) ==> \valid(hour);
  @ requires (min != NULL) ==> \valid(min);
  @ requires (sec != NULL) ==> \valid(sec);
  @ requires \separated(t_type,eaten,year,month,day,hour,min,sec,buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> 0 <= *year <= 9999;
  @ ensures (\result == 0) ==> *month <= 12;
  @ ensures (\result == 0) ==> *day <= 31;
  @ ensures (\result == 0) ==> *hour <= 23;
  @ ensures (\result == 0) ==> *min <= 59;
  @ ensures (\result == 0) ==> *sec <= 59;
  @
  @ assigns *t_type, *eaten, *year, *month, *day, *hour, *min, *sec;
  @*/
int parse_Time(const u8 *buf, u32 len, u8 *t_type, u32 *eaten,
	       u16 *year, u8 *month, u8 *day,
	       u8 *hour, u8 *min, u8 *sec)
{
	u8 time_type;
	int ret = -X509_FILE_LINE_NUM_ERR;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	time_type = buf[0];

	switch (time_type) {
	case ASN1_TYPE_UTCTime:
		ret = parse_UTCTime(buf, len, eaten, year, month,
				    day, hour, min, sec);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case ASN1_TYPE_GeneralizedTime:
		ret = parse_generalizedTime(buf, len, eaten, year, month,
					    day, hour, min, sec);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		break;
	}

	*t_type = time_type;

out:
	if (ret) {
		*eaten = 0;
	}
	return ret;
}

/* Specification version for serial number field from AKI */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (2 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_AKICertSerialNumber(const u8 *cert, u32 off, u32 len,
			      tag_class exp_class, u32 exp_type,
			      u32 *eaten)
{
	int ret;

	ret = parse_SerialNumber(cert, off, len, exp_class, exp_type, eaten);
	if (ret) {
	       ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	       goto out;
	}

out:
	return ret;
}

/*
 * X.509 certificates includes 2 definitions of named bit list which
 * both define 9 flags: KeyUsage and ReasonFlags. For that reason, most
 * of the decoding logic for the instances of this types (keyUsage
 * extension, CRLDP and FreshestCRL) can be done in a single location.
 *
 * Note that the function enforces that at least one bit is set in the
 * nit named bit list, as explicitly required at least for Key Usage.
 * This is in sync with what is given in Appendix B of RFC 5280:
 * "When DER encoding a named bit list, trailing zeros MUST be omitted.
 * That is, the encoded value ends with the last named bit that is set
 * to one."
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (val != NULL) ==> \valid(val);
  @ requires \separated(val, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *val;
  @*/
int parse_nine_bit_named_bit_list(const u8 *buf, u32 len, u16 *val)
{
	u8 k, non_signif;
	u16 tmp;
	int ret;

	/*
	 * Initial content octet is required. It provides the number of
	 * non-significative bits at the end of the last bytes carrying
	 * the bitstring value.
	 */
	if ((buf == NULL) || (len == 0) || (val == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* ... and it must be in the range [0,7]. */
	if (buf[0] & 0xf8) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We encode 9 bits of information in a named bit list bitstring.
	 * With the initial octet of the content octets (encoding the number
	 * of unused bits in the final octet), the whole content octets
	 * should be made of 1, 2 or 3 bytes.
	 */
	switch (len) {
	case 1: /* 1 byte giving number of unused bits - no following bytes */
		if (buf[0] != 0) {
			/*
			 * The number of non-significative bits is non-zero
			 * but no bits are following. This is invalid.
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		} else {
			/*
			 * Last paragraph of section 4.2.1.3 of RFC 5280 has
			 * "When the keyUsage extension appears in a
			 * certificate, at least one of the bits
			 * MUST be set to 1.". Regarding ReasonFlags, this
			 * is not explictly stated but would not make sense
			 * either.
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		break;

	case 2: /* 1 byte giving number of unused bits - 1 following byte */
		/*
		 * Last paragraph of section 4.2.1.3 of RFC 5280 has
		 * "When the keyUsage extension appears in a
		 * certificate, at least one of the bits
		 * MUST be set to 1". Regarding ReasonFlags, this would
		 * not make sense either to have an empty list.
		 */
		if (buf[1] == 0) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Now that we are sure at least one bit is set, we can see
		 * which one is the last one set to verify it matches what
		 * the byte giving the number of unused bits tells us. When
		 * we are dealing w/ non-significative bits, they should be
		 * set to 0 in the byte (11.2.1 of X.690).
		 * Additionally, keyUsage type is a NamedBitList-based
		 * bitstring, and for that reason X.680 clause 11.2.2 requires
		 * "the bitstring shall have all trailing 0 bits removed
		 * before it is encoded". This is also the conclusion of
		 * http://www.ietf.org/mail-archive/web/pkix/current/
		 * msg10424.html. This is also explicitly stated in Appendix B
		 * of RFC5280: "When DER encoding a named bit list, trailing
		 * zeros MUST be omitted.  That is, the encoded value ends with
		 * the last named bit that is set to one."
		 */

		non_signif = 0;

		/*@
		  @ loop assigns k, non_signif;
		  @ loop variant (8 - k);
		  @*/
		for (k = 0; k < 8; k++) {
			if ((buf[1] >> k) & 0x1) {
				non_signif = k;
				break;
			}
		}

		if (buf[0] != non_signif) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* Revert bits to provide a usable value to caller */
		tmp = 0;
		/*@
		  @ loop unroll 8;
		  @ loop invariant 0 <= k <= 8;
		  @ loop assigns k, tmp;
		  @ loop variant (8 - k);
		  @*/
		for (k = 0; k < 8; k++) {
			const u8 mask[8] = {1, 2, 4, 8, 16, 32, 64, 128 };
			tmp |= (buf[1] & mask[k]) ? mask[7-k] : 0;
		}
		*val = tmp;

		break;

	case 3: /* 1 byte for unused bits - 2 following bytes */
		/*
		 * keyUsage and ReasonFlags support at most 9 bits. When the
		 * named bit list bitstring is made of 1 byte giving unused
		 * bits and 2 following bytes, this means the 9th bit (i.e.
		 * bit 8, decipherOnly) is asserted.
		 * Because of that, the value of the byte giving the number
		 * of unused bits is necessarily set to 7.
		 */
		if ((buf[0] != 7) || (buf[2] != 0x80)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Revert bits to provide a usable value to caller,
		 * working on first byte's bits and then on single
		 * MSB from second byte.
		 */
		tmp = 0;
		/*@
		  @ loop unroll 8;
		  @ loop invariant 0 <= k <= 8;
		  @ loop assigns k, tmp;
		  @ loop variant (8 - k);
		  @*/
		for (k = 0; k < 8; k++) {
			const u8 mask[8] = {1, 2, 4, 8, 16, 32, 64, 128 };
			tmp |= (buf[1] & mask[k]) ? mask[7-k] : 0;
		}
		tmp |= (buf[2] & 0x80) ? 0x0100 : 0x0000;
		*val = tmp;
		break;

	default: /* too many bytes for encoding 9 poor bits */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 *  ReasonFlags ::= BIT STRING {
 *       unused                  (0),
 *       keyCompromise           (1),
 *       cACompromise            (2),
 *       affiliationChanged      (3),
 *       superseded              (4),
 *       cessationOfOperation    (5),
 *       certificateHold         (6),
 *       privilegeWithdrawn      (7),
 *       aACompromise            (8) }
 *
 * Note that the occurences of reasons in the specs are all context specific
 * with different type values: 0x01 in CRLDP and 0x03 in IDP. For that reason
 * the parsing function below takes an exp_type value.
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_crldp_reasons(const u8 *buf, u32 len, u32 exp_type, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0;
	u16 val = 0;
	int ret;

	if ((buf == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_CONTEXT_SPECIFIC, exp_type,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	len -= hdr_len;

	ret = parse_nine_bit_named_bit_list(buf, data_len, &val);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}



/*
 *     DistributionPoint ::= SEQUENCE {
 *          distributionPoint       [0]     DistributionPointName OPTIONAL,
 *          reasons                 [1]     ReasonFlags OPTIONAL,
 *          cRLIssuer               [2]     GeneralNames OPTIONAL }
 *
 *     DistributionPointName ::= CHOICE {
 *          fullName                [0]     GeneralNames,
 *          nameRelativeToCRLIssuer [1]     RelativeDistinguishedName }
 *
 *     ReasonFlags ::= BIT STRING {
 *          unused                  (0),
 *          keyCompromise           (1),
 *          cACompromise            (2),
 *          affiliationChanged      (3),
 *          superseded              (4),
 *          cessationOfOperation    (5),
 *          certificateHold         (6),
 *          privilegeWithdrawn      (7),
 *          aACompromise            (8) }
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (crldp_has_all_reasons != NULL) ==> \valid(crldp_has_all_reasons);
  @ requires \separated(eaten, buf+(..), crldp_has_all_reasons);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (0 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *crldp_has_all_reasons;
  @*/
int parse_DistributionPoint(const u8 *buf, u32 len,
			    int *crldp_has_all_reasons, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, remain = 0, total_len = 0;
	int dp_or_issuer_present = 0;
	u32 parsed = 0;
	int ret, has_all_reasons = 0;

	if ((buf == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* DistributionPoint is a sequence */
	ret = parse_id_len(buf, len,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		goto out;
	}

	total_len = hdr_len + data_len;
	/*@ assert total_len > 0; */
	remain = data_len;
	buf += hdr_len;

	/*
	 * Check if we have a (optional) distributionPoint field
	 * (of type DistributionPointName)
	 */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &hdr_len, &data_len);
	if (!ret) {
		u32 dpn_remain = 0, dpn_eaten = 0;
		u8 dpn_type;

		buf += hdr_len;
		remain -= hdr_len;
		dpn_remain = data_len;

		if (data_len == 0) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		dpn_type = buf[0];

		/*
		 * distributionPoint field of type DistributionPointName
		 * can be either a fullName or a nameRelativeToCRLIssuer.
		 */
		switch (dpn_type) {
		case 0xa0: /* fullName (i.e. a GeneralNames) */
			ret = parse_GeneralNames(buf, dpn_remain,
						 CLASS_CONTEXT_SPECIFIC, 0,
						 &dpn_eaten);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			dpn_remain -= dpn_eaten;
			buf += dpn_eaten;
			break;

		case 0xa1: /* nameRelativeToCRLIssuer (RDN) */
			/*
			 * This form of distributionPoint is never used
			 * in practice in real X.509 certs, so not
			 * supported here. Note that RFC5280 has the
			 * following: "Conforming CAs SHOULD NOT use
			 * nameRelativeToCRLIssuer to specify distribution
			 * point names."
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
			break;

		default:
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
			break;
		}

		if (dpn_remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		/* Record the fact we found a DP */
		dp_or_issuer_present |= 1;

		remain -= data_len;
	}

	/* Check if we have a (optional) ReasonFlags */
	ret = parse_crldp_reasons(buf, remain, 0x01, &parsed);
	if (!ret) {
		buf += parsed;
		remain -= parsed;
	} else {
		/*
		 * RFC 5280 has "If the DistributionPoint omits the reasons
		 * field, the CRL MUST include revocation information for all
		 * reasons", i.e. no reasonFlags means all reasons.
		 */
		has_all_reasons = 1;
	}

	/* Check if we have a (optional) cRLIssuer (GeneralNames) */
	ret = parse_GeneralNames(buf, remain, CLASS_CONTEXT_SPECIFIC, 2,
				 &parsed);
	if (!ret) {
		/* Record the fact we found a cRLIssuer */
		dp_or_issuer_present |= 1;

		buf += parsed;
		remain -= parsed;
	}

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC580 has (about DP and cRLIssuer): "While each of these fields is
	 * optional, a DistributionPoint MUST NOT consist of only the reasons
	 * field; either distributionPoint or cRLIssuer MUST be present."
	 */
	if (!dp_or_issuer_present) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = total_len;
	*crldp_has_all_reasons = has_all_reasons;
	/*@ assert *eaten > 0; */

	ret = 0;

out:
	return ret;
}


/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_AccessDescription(const u8 *buf, u32 len, u32 *eaten)
{
	const u8 id_ad_caIssuers_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05,
					   0x05, 0x07, 0x30, 0x01 };
	const u8 id_ad_ocsp_oid[] = { 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05,
				      0x05, 0x07, 0x30, 0x02 };
	u32 remain, hdr_len = 0, data_len = 0, oid_len = 0;
	u32 al_len = 0, saved_ad_len = 0;
	int ret, found, unused;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	saved_ad_len = hdr_len + data_len;
	/*@ assert saved_ad_len <= len ; */
	remain -= hdr_len;
	/*@ assert remain >= data_len ; */
	buf += hdr_len;

	/* accessMethod is an OID */
	ret = parse_OID(buf, data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We only support the two OID that are reference in the RFC,
	 * i.e. id-ad-caIssuers and id-ad-ocsp.
	 */
	found = 0;

	if (oid_len == sizeof(id_ad_caIssuers_oid)) {
		found = !bufs_differ(buf, id_ad_caIssuers_oid, oid_len);
	}

	if ((!found) && (oid_len == sizeof(id_ad_ocsp_oid))) {
		found = !bufs_differ(buf, id_ad_ocsp_oid, oid_len);
	}

	if (!found) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += oid_len;
	remain -= oid_len;
	data_len -= oid_len;
	/*@ assert remain >= data_len ; */

	/* accessLocation is a GeneralName */
	ret = parse_GeneralName(buf, data_len, &al_len, &unused);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * FIXME! I guess we could do some specific parsing on the
	 * content of the generalName based on what is described
	 * in section 4.2.2.1 of RFC 5280.
	 */

	buf += al_len;
	/*@ assert remain >= data_len >= al_len; */
	remain -= al_len;
	data_len -= al_len;

	if (data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = saved_ad_len;
	/*@ assert *eaten <= len ; */
	ret = 0;

out:
	return ret;
}

/*
 * 4.2.2.1 - Certificate Authority Information Access (Certificate extension)
 * 5.2.7 - Authority Information Access (CRL extension)
 *
 *    id-pe-authorityInfoAccess OBJECT IDENTIFIER ::= { id-pe 1 }
 *
 *    AuthorityInfoAccessSyntax  ::=
 *            SEQUENCE SIZE (1..MAX) OF AccessDescription
 *
 *    AccessDescription  ::=  SEQUENCE {
 *            accessMethod          OBJECT IDENTIFIER,
 *            accessLocation        GeneralName  }
 *
 *    id-ad OBJECT IDENTIFIER ::= { id-pkix 48 }
 *
 *    id-ad-caIssuers OBJECT IDENTIFIER ::= { id-ad 2 }
 *
 *    id-ad-ocsp OBJECT IDENTIFIER ::= { id-ad 1 }
 *
 *  GeneralName ::= CHOICE {
 *       otherName                 [0]  AnotherName,
 *       rfc822Name                [1]  IA5String,
 *       dNSName                   [2]  IA5String,
 *       x400Address               [3]  ORAddress,
 *       directoryName             [4]  Name,
 *       ediPartyName              [5]  EDIPartyName,
 *       uniformResourceIdentifier [6]  IA5String,
 *       iPAddress                 [7]  OCTET STRING,
 *       registeredID              [8]  OBJECT IDENTIFIER }
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (critical != 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
int parse_AIA(const u8 *cert, u32 off, u32 len, int critical)
{
	u32 hdr_len = 0, data_len = 0, remain;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * 4.2.2.1 of RFC5280 has "Conforming CAs MUST mark this
	 * extension as non-critical"
	 */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain -= hdr_len;

	/* We do expect sequence to exactly match the length */
	if (remain != data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Empty AIA extensions are not authorized (AIA is a non empty sequence
	 * of AccessDescription structures.
	 */
	if (!remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Iterate on AccessDescription structures: each is
	 * a sequence containing an accessMethod (an OID)
	 * and an accessLocation (a GeneralName).
	 */
	/*@
	  @ loop assigns ret, buf, remain;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		u32 parsed = 0;

		ret = parse_AccessDescription(buf, remain, &parsed);
		if (ret) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= parsed;
		buf += parsed;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \forall integer k; 0 <= k < num_known_sig_algs ==> \valid_read(known_sig_algs[k]);
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < num_known_sig_algs && \result == known_sig_algs[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \result;
  @*/
const _sig_alg * find_sig_alg_by_oid(const u8 *buf, u32 len)
{
	const _sig_alg *found = NULL;
	const _sig_alg *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@
	  @ loop unroll num_known_sig_algs;
	  @ loop invariant 0 <= k <= num_known_sig_algs;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (num_known_sig_algs - k);
	  @*/
	for (k = 0; k < num_known_sig_algs; k++) {
		int ret;

		cur = known_sig_algs[k];

		/*@ assert cur == known_sig_algs[k]; */
		if (cur->alg_der_oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->alg_der_oid, buf, cur->alg_der_oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}


/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && \result == known_hashes[i] && \valid_read(\result);
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \result;
  @*/
const _hash_alg * find_hash_by_oid(const u8 *buf, u32 len)
{
	const _hash_alg *found = NULL;
	const _hash_alg *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll NUM_KNOWN_HASHES ;
	  @ loop invariant 0 <= k <= NUM_KNOWN_HASHES;
	  @ loop invariant found == NULL;
	  @ loop invariant (cur != NULL) ==> \valid_read(cur);
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_HASHES - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_HASHES; k++) {
		int ret;

		cur = known_hashes[k];
		/*@ assert \valid_read(cur); */

		/*@ assert cur == known_hashes[k];*/
		if (cur->alg_der_oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->alg_der_oid, buf, cur->alg_der_oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_CURVES && \result == known_curves[i] && \valid_read(known_curves[i]);
  @ ensures (\result != NULL) ==> \result->crv_order_bit_len <= 571;
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \result;
  @*/
const _curve * find_curve_by_oid(const u8 *buf, u32 len)
{
	const _curve *found = NULL;
	const _curve *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ assert \forall integer k; 0 <= k < NUM_KNOWN_CURVES ==> \valid_read(known_curves[k]); */
	/*@ assert \forall integer k; 0 <= k < NUM_KNOWN_CURVES ==> known_curves[k]->crv_order_bit_len <= 571; */

	/*@
	  @ loop unroll NUM_KNOWN_CURVES;
	  @ loop invariant 0 <= k <= NUM_KNOWN_CURVES;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_CURVES - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_CURVES; k++) {
		int ret;

		cur = known_curves[k];

		/*@ assert cur == known_curves[k];*/
		if (cur->crv_der_oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->crv_der_oid, buf, cur->crv_der_oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}
	/*@ assert (found != NULL) ==> found->crv_order_bit_len <= 571; */

out:
	return found;
}


static const u8 _dn_oid_cn[] =        { 0x06, 0x03, 0x55, 0x04, 0x03 };
static const u8 _dn_oid_surname[] =   { 0x06, 0x03, 0x55, 0x04, 0x04 };
static const u8 _dn_oid_serial[] =    { 0x06, 0x03, 0x55, 0x04, 0x05 };
static const u8 _dn_oid_country[] =   { 0x06, 0x03, 0x55, 0x04, 0x06 };
static const u8 _dn_oid_locality[] =  { 0x06, 0x03, 0x55, 0x04, 0x07 };
static const u8 _dn_oid_state[] =     { 0x06, 0x03, 0x55, 0x04, 0x08 };
static const u8 _dn_oid_org[] =       { 0x06, 0x03, 0x55, 0x04, 0x0a };
static const u8 _dn_oid_org_unit[] =  { 0x06, 0x03, 0x55, 0x04, 0x0b };
static const u8 _dn_oid_title[] =     { 0x06, 0x03, 0x55, 0x04, 0x0c };
static const u8 _dn_oid_name[] =      { 0x06, 0x03, 0x55, 0x04, 0x29 };
static const u8 _dn_oid_emailaddress[] = { 0x06, 0x09, 0x2a, 0x86, 0x48,
					   0x86, 0xf7, 0x0d, 0x01, 0x09,
					   0x01  };
static const u8 _dn_oid_given_name[] = { 0x06, 0x03, 0x55, 0x04, 0x2a };
static const u8 _dn_oid_initials[] =  { 0x06, 0x03, 0x55, 0x04, 0x2b };
static const u8 _dn_oid_gen_qual[] =  { 0x06, 0x03, 0x55, 0x04, 0x2c };
static const u8 _dn_oid_dn_qual[] =   { 0x06, 0x03, 0x55, 0x04, 0x2e };
static const u8 _dn_oid_pseudo[] =    { 0x06, 0x03, 0x55, 0x04, 0x41 };
static const u8 _dn_oid_dc[] =        { 0x06, 0x0a, 0x09, 0x92, 0x26,
					0x89, 0x93, 0xf2, 0x2c, 0x64,
					0x01, 0x19 };
static const u8 _dn_oid_ogrn[] =      { 0x06, 0x05, 0x2a, 0x85, 0x03,
					0x64, 0x01 };
static const u8 _dn_oid_snils[] =     { 0x06, 0x05, 0x2a, 0x85, 0x03,
					0x64, 0x03 };
static const u8 _dn_oid_ogrnip[] =    { 0x06, 0x05, 0x2a, 0x85, 0x03,
					0x64, 0x05 };
static const u8 _dn_oid_inn[] =       { 0x06, 0x08, 0x2a, 0x85, 0x03,
					0x03, 0x81, 0x03, 0x01, 0x01 };
static const u8 _dn_oid_street_address[] = { 0x06, 0x03, 0x55, 0x04, 0x09 };

/*
 * Most RDN values are encoded using the directory string type
 * defined below. This function performs the required check to
 * verify the given string is a valid DirectoryString. The
 * function returns 0 on success and -1 on error.
 *
 * This is an error if the length does not match that of the
 * string, if buffer is too short or too long for the encoded
 * string.
 *
 *  DirectoryString ::= CHOICE {
 *        teletexString           TeletexString (SIZE (1..MAX)),
 *        printableString         PrintableString (SIZE (1..MAX)),
 *        universalString         UniversalString (SIZE (1..MAX)),
 *        utf8String              UTF8String (SIZE (1..MAX)),
 *        bmpString               BMPString (SIZE (1..MAX)) }
 *
 *
 * 'len' is the size of given buffer, including string type
 * and string length. 'lb' and 'ub' are respectively lower and
 * upper bounds for the effective string.
 *
 * Note that RFC 5280 has the following: "upper bounds on string types,
 * such as TeletexString, are measured in characters.  Excepting
 * PrintableString or IA5String, a significantly greater number of
 * octets will be required to hold  such a value.  As a minimum, 16
 * octets, or twice the specified  upper bound, whichever is the larger,
 * should be allowed for TeletexString.  For UTF8String or UniversalString
 * at least four times the upper bound should be allowed.
 *
 */
#define STR_TYPE_UTF8_STRING      12
#define STR_TYPE_NUMERIC_STRING   18
#define STR_TYPE_PRINTABLE_STRING 19
#define STR_TYPE_TELETEX_STRING   20
#define STR_TYPE_IA5_STRING       22
#define STR_TYPE_VISIBLE_STRING   26
#define STR_TYPE_UNIVERSAL_STRING 28
#define STR_TYPE_BMP_STRING       30
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures \result == 0 ==> ((len >= 2) && (lb <= len - 2 <= ub));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_directory_string(const u8 *buf, u32 len, u32 lb, u32 ub)
{
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];

	len -= 2;
	if (buf[1] != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	switch (str_type) {
	case STR_TYPE_PRINTABLE_STRING:
		ret = check_printable_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case STR_TYPE_UTF8_STRING:
		ret = check_utf8_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
#ifdef TEMPORARY_LAXIST_DIRECTORY_STRING
		/*
		 * Section 4.1.2.4 of RFC 5280 has "CAs conforming to this
		 * profile MUST use either the PrintableString or UTF8String
		 * encoding of DirectoryString, with two exceptions
		 */
	case STR_TYPE_TELETEX_STRING:
		ret = check_teletex_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case STR_TYPE_UNIVERSAL_STRING:
		ret = check_universal_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case STR_TYPE_BMP_STRING:
		ret = check_bmp_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case STR_TYPE_IA5_STRING:
		ret = check_ia5_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	case STR_TYPE_NUMERIC_STRING:
		ret = check_numeric_string(buf, len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
#endif
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		break;
	}

	if (ret) {
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Some RDN values are specifically encoded as PrintableString and the usual
 * directoryString. The function verifies that. It returns -1 on error, 0
 * on success.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures \result == 0 ==> ((len >= 2) && (lb <= len - 2 <= ub));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_printable_string(const u8 *buf, u32 len, u32 lb, u32 ub)
{
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];
	if (str_type != STR_TYPE_PRINTABLE_STRING) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= 2;
	if (buf[1] != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = check_printable_string(buf, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * Some RDN values are specifically encoded as NumericString. The function
 * verifies that. It returns -1 on error, 0 on success.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures \result == 0 ==> ((len >= 2) && (lb <= len - 2 <= ub));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_numeric_string(const u8 *buf, u32 len, u32 lb, u32 ub)
{
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];
	if (str_type != STR_TYPE_NUMERIC_STRING) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= 2;
	if (buf[1] != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = check_numeric_string(buf, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * As pointed by RFC 4519 and described by RFC 4517, IA5String
 * is defined in ABNF form as *(%x00-7F).
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures \result == 0 ==> ((len >= 2) && (lb <= len - 2 <= ub));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
int parse_ia5_string(const u8 *buf, u32 len, u32 lb, u32 ub)
{
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];
	if (str_type != STR_TYPE_IA5_STRING) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= 2;
	if (buf[1] != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = check_ia5_string(buf, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

#ifdef TEMPORARY_LAXIST_EMAILADDRESS_WITH_UTF8_ENCODING
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures \result == 0 ==> ((len >= 2) && (lb <= len - 2 <= ub));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_utf8_string(const u8 *buf, u32 len, u32 lb, u32 ub)
{
	int ret = -X509_FILE_LINE_NUM_ERR;
	u8 str_type;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 2) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];
	if (str_type != STR_TYPE_IA5_STRING) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= 2;
	if (buf[1] != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 2;

	if ((len < lb) || (len > ub)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = check_utf8_string(buf, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}
#endif

/*
 *
 * -- Naming attributes of type X520CommonName:
 * --   X520CommonName ::= DirectoryName (SIZE (1..ub-common-name))
 * --
 * -- Expanded to avoid parameterized type:
 * X520CommonName ::= CHOICE {
 *       teletexString     TeletexString   (SIZE (1..ub-common-name)),
 *       printableString   PrintableString (SIZE (1..ub-common-name)),
 *       universalString   UniversalString (SIZE (1..ub-common-name)),
 *       utf8String        UTF8String      (SIZE (1..ub-common-name)),
 *       bmpString         BMPString       (SIZE (1..ub-common-name)) }
 */
#ifdef TEMPORARY_LAXIST_RDN_UPPER_BOUND
#define UB_COMMON_NAME 192
#else
#define UB_COMMON_NAME 64
#endif
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_cn(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_COMMON_NAME);
}

#define UB_NAME 32768
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_x520name(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_NAME);
}

#define UB_EMAILADDRESS 255
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_emailaddress(const u8 *buf, u32 len)
{
	int ret;

	/* RFC 5280 has:
	 *
	 * "Legacy implementations exist where an electronic mail address is
	 * embedded in the subject distinguished name as an emailAddress
	 * attribute [RFC2985].  The attribute value for emailAddress is of
	 * type IA5String to permit inclusion of the character '@', which is
	 * not part of the PrintableString character set.  emailAddress
	 * attribute values are not case-sensitive (e.g.,
	 * "subscriber@example.com" is the same as "SUBSCRIBER@EXAMPLE.COM").
	 *
	 * Conforming implementations generating new certificates with
	 * electronic mail addresses MUST use the rfc822Name in the subject
	 * alternative name extension (Section 4.2.1.6) to describe such
	 * identities.  Simultaneous inclusion of the emailAddress attribute
	 * in the subject distinguished name to support legacy implementations
	 * is deprecated but permitted."
	 */
	ret = parse_ia5_string(buf, len, 1, UB_EMAILADDRESS);

	/*
	 * As a side note, tests performed on our set indicates some
	 * implementations currently use UTF8 encoding for emailAddress.
	 * Hence the quirks below to support the (invalid) certificates
	 * generated by those implementations.
	 */
#ifdef TEMPORARY_LAXIST_EMAILADDRESS_WITH_UTF8_ENCODING
	if (ret) {
		ret = parse_utf8_string(buf, len, 1, UB_EMAILADDRESS);
	}
#endif

	return ret;
}

#define UB_SERIAL_NUMBER 64
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_serial(const u8 *buf, u32 len)
{
	int ret;

	ret = parse_printable_string(buf, len, 1, UB_SERIAL_NUMBER);
	if (ret) {
#ifdef TEMPORARY_LAXIST_SERIAL_RDN_AS_IA5STRING
		ret = parse_ia5_string(buf, len, 1, UB_SERIAL_NUMBER);
#endif
	}

	return ret;
}

#define UB_COUNTRY 2
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_country(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, UB_COUNTRY, UB_COUNTRY);
}

#define UB_LOCALITY_NAME 128
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_locality(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_LOCALITY_NAME);
}

#define UB_STATE_NAME 128
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_state(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_STATE_NAME);
}

#ifdef TEMPORARY_LAXIST_RDN_UPPER_BOUND
#define UB_ORGANIZATION_NAME 64
#else
#define UB_ORGANIZATION_NAME 128
#endif
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_org(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_ORGANIZATION_NAME);
}

#ifdef TEMPORARY_LAXIST_RDN_UPPER_BOUND
#define UB_ORGANIZATION_UNIT_NAME 128
#else
#define UB_ORGANIZATION_UNIT_NAME 64
#endif
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_org_unit(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_ORGANIZATION_UNIT_NAME);
}

#define UB_TITLE_NAME 64
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_title(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_TITLE_NAME);
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_dn_qual(const u8 *buf, u32 len)
{
	/*
	 * There is no specific limit on that one, so giving the maximum
	 * buffer size we support will do the job.
	 */
	return parse_printable_string(buf, len, 1, ASN1_MAX_BUFFER_SIZE);
}

/*@
  @ assigns \nothing;
  @*/
static inline int _is_digit(u8 c)
{
	return ((c >= '0') && (c <= '9'));
}

/*@
  @ assigns \nothing;
  @*/
static inline int _is_alpha(u8 c)
{
	return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
}

/*
 * As point in RFC 5280 and defined in section 2.4 of RFC 4519,
 * 'dc' (domainComponent) is a string attribute type holding a
 * DNS label. It is encoded as an IA5String. The ABNF for the
 * label is the following:
 *
 * label = (ALPHA / DIGIT) [*61(ALPHA / DIGIT / HYPHEN) (ALPHA / DIGIT)]
 * ALPHA   = %x41-5A / %x61-7A     ; "A"-"Z" / "a"-"z"
 * DIGIT   = %x30-39               ; "0"-"9"
 * HYPHEN  = %x2D                  ; hyphen ("-")
 *
 * To simplify things, we first verify this is a valid IA5string and then
 * verify the additional restrictions given above for the label.
 *
 * Note that RFC 2181 (Clarifications on DNS) has the following: "The DNS
 * itself places only one restriction on the particular labels that can
 * be used to identify resource records.  That one restriction relates
 * to the length of the label and the full name...". In the case of dc
 * attributes, limitations are imposed by the use of IA5String for
 * encoding and by the ABNF above.
 */
#define UB_DC 63
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_dc(const u8 *buf, u32 len)
{
	int ret;
	u32 i;
	u8 c;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* We expect an IA5String */
	ret = parse_ia5_string(buf, len, 1, UB_DC);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += 2;
	len -= 2;

	/* Restriction on first byte */
	c = buf[0];
	ret = _is_alpha(c) || _is_digit(c);
	if (!ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += 1;
	len -= 1;

	if (!len) { /* over ? */
		ret = 0;
		goto out;
	}

	/* Restriction on last byte if any */
	c = buf[len - 1];
	ret = _is_alpha(c) || _is_digit(c);
	if (!ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += 1;
	len -= 1;

	/* Restriction on middle bytes */
	/*@
	  @ loop invariant 0 <= i <= len;
	  @ loop assigns c, ret, i;
	  @ loop variant (len - i);
	  @ */
	for (i = 0; i < len; i++) {
		c = buf[i];
		ret = _is_digit(c) || _is_alpha(c) || (c == '-');
		if (!ret) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	}

	ret = 0;

out:
	return ret;
}

#define UB_PSEUDONYM 128
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_pseudo(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_PSEUDONYM);
}


/* From section 5.1 of draft-deremin-rfc4491-bis-01 */

/* OGRN is the main state registration number of juridical entities */
#define UB_OGRN 13
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_ogrn(const u8 *buf, u32 len)
{
	return parse_numeric_string(buf, len, 1, UB_OGRN);
}

/* SNILS is the individual insurance account number */
#define UB_SNILS 11
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_snils(const u8 *buf, u32 len)
{
	return parse_numeric_string(buf, len, 1, UB_SNILS);
}

/*
 * OGRNIP is the main state registration number of individual
 * enterpreneurs
 */
#define UB_OGRNIP 15
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_ogrnip(const u8 *buf, u32 len)
{
	return parse_numeric_string(buf, len, 1, UB_OGRNIP);
}

/* INN is the individual taxpayer number (ITN). */
#define UB_INN 12
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_inn(const u8 *buf, u32 len)
{
	return parse_numeric_string(buf, len, 1, UB_INN);
}

/* street address. */
#define UB_STREET_ADDRESS 64 /* XXX FIXME Don't know what the limit is */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_street_address(const u8 *buf, u32 len)
{
	return parse_directory_string(buf, len, 1, UB_STREET_ADDRESS);
}

typedef struct {
	const u8 *oid;
	u8 oid_len;
	int (*parse_rdn_val)(const u8 *buf, u32 len);
} _name_oid;

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
static int parse_rdn_val_bad_oid(const u8 *buf, u32 len)
{
	(void) buf;
	(void) len;
	return 0;
}

static const _name_oid generic_unsupported_rdn_oid = {
	.oid = NULL,
	.oid_len = 0,
	.parse_rdn_val = parse_rdn_val_bad_oid
};

static const _name_oid known_dn_oids[] = {
	{ .oid = _dn_oid_cn,
	  .oid_len = sizeof(_dn_oid_cn),
	  .parse_rdn_val = parse_rdn_val_cn
	},
	{ .oid = _dn_oid_surname,
	  .oid_len = sizeof(_dn_oid_surname),
	  .parse_rdn_val = parse_rdn_val_x520name
	},
	{ .oid = _dn_oid_serial,
	  .oid_len = sizeof(_dn_oid_serial),
	  .parse_rdn_val = parse_rdn_val_serial
	},
	{ .oid = _dn_oid_country,
	  .oid_len = sizeof(_dn_oid_country),
	  .parse_rdn_val = parse_rdn_val_country
	},
	{ .oid = _dn_oid_locality,
	  .oid_len = sizeof(_dn_oid_locality),
	  .parse_rdn_val = parse_rdn_val_locality
	},
	{ .oid = _dn_oid_state,
	  .oid_len = sizeof(_dn_oid_state),
	  .parse_rdn_val = parse_rdn_val_state
	},
	{ .oid = _dn_oid_org,
	  .oid_len = sizeof(_dn_oid_org),
	  .parse_rdn_val = parse_rdn_val_org
	},
	{ .oid = _dn_oid_org_unit,
	  .oid_len = sizeof(_dn_oid_org_unit),
	  .parse_rdn_val = parse_rdn_val_org_unit
	},
	{ .oid = _dn_oid_title,
	  .oid_len = sizeof(_dn_oid_title),
	  .parse_rdn_val = parse_rdn_val_title
	},
	{ .oid = _dn_oid_name,
	  .oid_len = sizeof(_dn_oid_name),
	  .parse_rdn_val = parse_rdn_val_x520name
	},
	{ .oid = _dn_oid_emailaddress,
	  .oid_len = sizeof(_dn_oid_emailaddress),
	  .parse_rdn_val = parse_rdn_val_emailaddress
	},
	{ .oid = _dn_oid_given_name,
	  .oid_len = sizeof(_dn_oid_given_name),
	  .parse_rdn_val = parse_rdn_val_x520name
	},
	{ .oid = _dn_oid_initials,
	  .oid_len = sizeof(_dn_oid_initials),
	  .parse_rdn_val = parse_rdn_val_x520name
	},
	{ .oid = _dn_oid_gen_qual,
	  .oid_len = sizeof(_dn_oid_gen_qual),
	  .parse_rdn_val = parse_rdn_val_x520name
	},
	{ .oid = _dn_oid_dn_qual,
	  .oid_len = sizeof(_dn_oid_dn_qual),
	  .parse_rdn_val = parse_rdn_val_dn_qual
	},
	{ .oid = _dn_oid_pseudo,
	  .oid_len = sizeof(_dn_oid_pseudo),
	  .parse_rdn_val = parse_rdn_val_pseudo
	},
	{ .oid = _dn_oid_dc,
	  .oid_len = sizeof(_dn_oid_dc),
	  .parse_rdn_val = parse_rdn_val_dc
	},
	{ .oid = _dn_oid_ogrn,
	  .oid_len = sizeof(_dn_oid_ogrn),
	  .parse_rdn_val = parse_rdn_val_ogrn
	},
	{ .oid = _dn_oid_snils,
	  .oid_len = sizeof(_dn_oid_snils),
	  .parse_rdn_val = parse_rdn_val_snils
	},
	{ .oid = _dn_oid_ogrnip,
	  .oid_len = sizeof(_dn_oid_ogrnip),
	  .parse_rdn_val = parse_rdn_val_ogrnip
	},
	{ .oid = _dn_oid_inn,
	  .oid_len = sizeof(_dn_oid_inn),
	  .parse_rdn_val = parse_rdn_val_inn
	},
	{ .oid = _dn_oid_street_address,
	  .oid_len = sizeof(_dn_oid_street_address),
	  .parse_rdn_val = parse_rdn_val_street_address
	},
};

#define NUM_KNOWN_DN_OIDS (sizeof(known_dn_oids) / sizeof(known_dn_oids[0]))

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \forall integer i ; 0 <= i < NUM_KNOWN_DN_OIDS ==> \valid_read(&known_dn_oids[i]);
  @ requires \forall integer i ; 0 <= i < NUM_KNOWN_DN_OIDS ==> \valid_function((known_dn_oids[i]).parse_rdn_val);
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_DN_OIDS && \result == &known_dn_oids[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \result;
  @*/
static const _name_oid * find_dn_by_oid(const u8 *buf, u32 len)
{
	const _name_oid *found = NULL;
	const _name_oid *cur = NULL;
	u8 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll NUM_KNOWN_DN_OIDS ;
	  @ loop invariant 0 <= k <= NUM_KNOWN_DN_OIDS;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_DN_OIDS - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_DN_OIDS; k++) {
		int ret;

		cur = &known_dn_oids[k];

		/*@ assert cur == &known_dn_oids[k];*/
		/*@ assert \valid_function(cur->parse_rdn_val); */
		if (cur->oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->oid, buf, cur->oid_len);
		if (ret) {
			found = cur;
			/*@ assert \valid_function(found->parse_rdn_val); */
			break;
		}
	}
	/*@ assert (found != NULL) ==> \valid_function(found->parse_rdn_val); */

out:
	/*@ assert (found != NULL) ==> \valid_function(found->parse_rdn_val); */
	return found;
}


/*
 *  AttributeTypeAndValue ::= SEQUENCE {
 *    type     AttributeType,
 *    value    AttributeValue }
 *
 *  AttributeType ::= OBJECT IDENTIFIER
 *
 *  AttributeValue ::= ANY -- DEFINED BY AttributeType
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(buf+(..), eaten);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_AttributeTypeAndValue(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 oid_len = 0;
	u32 parsed;
	const _name_oid *cur = NULL;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* ... of SEQUENCEs ... */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	parsed = hdr_len + data_len;
	/*@ assert parsed <= len ; */

	buf += hdr_len;
	len -= hdr_len;

	/* ... Containing an OID ... */
	ret = parse_OID(buf, data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	cur = find_dn_by_oid(buf, oid_len);
	if (cur == NULL) {
#ifndef TEMPORARY_LAXIST_HANDLE_ALL_REMAINING_RDN_OIDS
		/*
		 * OID not found => over. The trick below is a nop
		 * to let generic_unsupported_rdn_oid and
		 * parse_rdn_val_bad_oid() it contains be available
		 * and defined for Frama-C in all cases for the assert
		 * and calls just below.
		 */
		(void)generic_unsupported_rdn_oid;
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
#else
		cur = &generic_unsupported_rdn_oid;
#endif
	}

	data_len -= oid_len;
	buf += oid_len;

	/*
	 * Let's now check the value associated w/ and
	 * following the OID has a valid format.
	 */
	/*@ assert cur->parse_rdn_val \in {
		  parse_rdn_val_cn, parse_rdn_val_x520name,
		  parse_rdn_val_serial, parse_rdn_val_country,
		  parse_rdn_val_locality, parse_rdn_val_state,
		  parse_rdn_val_org, parse_rdn_val_org_unit,
		  parse_rdn_val_title,  parse_rdn_val_dn_qual,
		  parse_rdn_val_pseudo, parse_rdn_val_dc,
		  parse_rdn_val_ogrn, parse_rdn_val_snils,
		  parse_rdn_val_ogrnip, parse_rdn_val_inn,
		  parse_rdn_val_street_address,
		  parse_rdn_val_emailaddress,
		  parse_rdn_val_bad_oid };
	  @*/
	/*@ calls parse_rdn_val_cn, parse_rdn_val_x520name,
		  parse_rdn_val_serial, parse_rdn_val_country,
		  parse_rdn_val_locality, parse_rdn_val_state,
		  parse_rdn_val_org, parse_rdn_val_org_unit,
		  parse_rdn_val_title, parse_rdn_val_dn_qual,
		  parse_rdn_val_pseudo, parse_rdn_val_dc,
		  parse_rdn_val_ogrn, parse_rdn_val_snils,
		  parse_rdn_val_ogrnip, parse_rdn_val_inn,
		  parse_rdn_val_street_address,
		  parse_rdn_val_emailaddress,
		  parse_rdn_val_bad_oid;
	  @*/
	ret = cur->parse_rdn_val(buf, data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = parsed;
	/*@ assert *eaten <= \at(len,Pre) ; */

	ret = 0;

out:
	return ret;
}


/*
 *  RelativeDistinguishedName ::=
 *    SET SIZE (1..MAX) OF AttributeTypeAndValue
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten,buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_RelativeDistinguishedName(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 rdn_remain, saved_rdn_len;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Each RDN is a SET ... */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SET,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	saved_rdn_len = hdr_len + data_len;
	buf += hdr_len;
	rdn_remain = data_len;

	/*@
	  @ loop assigns ret, buf, rdn_remain;
	  @ loop invariant \valid_read(buf + (0 .. (rdn_remain - 1)));
	  @ loop variant rdn_remain;
	  @ */
	while (rdn_remain) {
		u32 parsed = 0;

		ret = parse_AttributeTypeAndValue(buf, rdn_remain, &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * FIXME! we should check the amount of AttributeTypeAndValue
		 * elements is between 1 and MAX
		 */

		rdn_remain -= parsed;
		buf += parsed;
	}

	*eaten = saved_rdn_len;

	ret = 0;

out:
	return ret;
}

/*
 * Used for Issuer and subject
 *
 *  Name ::= CHOICE { -- only one possibility for now --
 *    rdnSequence  RDNSequence }
 *
 *  RDNSequence ::= SEQUENCE OF RelativeDistinguishedName
 *
 *  RelativeDistinguishedName ::=
 *    SET SIZE (1..MAX) OF AttributeTypeAndValue
 *
 *  AttributeTypeAndValue ::= SEQUENCE {
 *    type     AttributeType,
 *    value    AttributeValue }
 *
 *  AttributeType ::= OBJECT IDENTIFIER
 *
 *  AttributeValue ::= ANY -- DEFINED BY AttributeType
 *
 * Here is what section 4.1.2.4 of RFC 5280 has:
 *
 * As noted above, distinguished names are composed of attributes.  This
 * specification does not restrict the set of attribute types that may
 * appear in names.  However, conforming implementations MUST be
 * prepared to receive certificates with issuer names containing the set
 * of attribute types defined below.  This specification RECOMMENDS
 * support for additional attribute types.
 *
 * Standard sets of attributes have been defined in the X.500 series of
 * specifications [X.520].  Implementations of this specification MUST
 * be prepared to receive the following standard attribute types in
 * issuer and subject (Section 4.1.2.6) names:
 *
 *    * country,
 *    * organization,
 *    * organizational unit,
 *    * distinguished name qualifier,
 *    * state or province name,
 *    * common name (e.g., "Susan Housley"), and
 *    * serial number.
 *
 * In addition, implementations of this specification SHOULD be prepared
 * to receive the following standard attribute types in issuer and
 * subject names:
 *
 *    * locality,
 *    * title,
 *    * surname,
 *    * given name,
 *    * initials,
 *    * pseudonym, and
 *    * generation qualifier (e.g., "Jr.", "3rd", or "IV").
 *
 * The syntax and associated object identifiers (OIDs) for these
 * attribute types are provided in the ASN.1 modules in Appendix A.
 *
 * In addition, implementations of this specification MUST be prepared
 * to receive the domainComponent attribute, as defined in [RFC4519].
 * The Domain Name System (DNS) provides a hierarchical resource
 * labeling system.  This attribute provides a convenient mechanism for
 * organizations that wish to use DNs that parallel their DNS names.
 * This is not a replacement for the dNSName component of the
 * alternative name extensions.  Implementations are not required to
 * convert such names into DNS names.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (empty != NULL) ==> \valid(empty);
  @ requires \separated(eaten, empty, buf+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (\result == 0) ==> ((*empty == 0) || (*empty == 1));
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten, *empty;
  @*/
int parse_x509_Name(const u8 *buf, u32 len, u32 *eaten, int *empty)
{
	u32 name_hdr_len = 0;
	u32 name_data_len = 0;
	u32 remain = 0;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &name_hdr_len, &name_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += name_hdr_len;
	remain = name_data_len;

	/*@
	  @ loop assigns ret, buf, remain;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		u32 parsed = 0;

		ret = parse_RelativeDistinguishedName(buf, remain, &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		remain -= parsed;
	}

	*eaten = name_hdr_len + name_data_len;
	*empty = !name_data_len;
	/*@ assert (*empty == 0) ||  (*empty == 1); */

	ret = 0;

out:
	return ret;
}


/*
 *     DisplayText ::= CHOICE {
 *          ia5String        IA5String      (SIZE (1..200)),
 *          visibleString    VisibleString  (SIZE (1..200)),
 *          bmpString        BMPString      (SIZE (1..200)),
 *          utf8String       UTF8String     (SIZE (1..200)) }
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, buf+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
int parse_DisplayText(const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0;
	u8 str_type;
	int ret = -1;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	str_type = buf[0];

	switch (str_type) {
	case STR_TYPE_UTF8_STRING:    /* UTF8String */
	case STR_TYPE_IA5_STRING:     /* IA5String */
	case STR_TYPE_VISIBLE_STRING: /* VisibileString */
	case STR_TYPE_BMP_STRING:     /* BMPString */
		ret = parse_id_len(buf, len, CLASS_UNIVERSAL, str_type,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += hdr_len;

		switch (str_type) {
		case STR_TYPE_UTF8_STRING:
			ret = check_utf8_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}
			break;
		case STR_TYPE_IA5_STRING:
			ret = check_ia5_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}
			break;
		case STR_TYPE_VISIBLE_STRING:
			ret = check_visible_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}
			break;
		case STR_TYPE_BMP_STRING:
			ret = check_bmp_string(buf, data_len);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}
			break;
		default:
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
			break;
		}

		*eaten = hdr_len + data_len;

		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

out:
	return ret;
}


/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->monkeysphere.sig_raw_off,
	    params->monkeysphere.sig_raw_len;
  @*/
int parse_sig_monkey(sig_params *params,
			    const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 remain, hdr_len = 0, data_len = 0;
	const u8 *buf = cert  + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * In practice, the sig contains an ASCII string: 'use OpenPGP' in the
	 * certificate we have. XXX Revisit later.
	 */
	params->monkeysphere.sig_raw_off = off;
	params->monkeysphere.sig_raw_len = remain;

	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*
 * All version of GOST signature algorithms (GOST R34.10-94, -2001 and -2012)
 * do encode their signature using a bitstring. This is what this helper
 * implements.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (r_start_off != NULL) ==> \valid(r_start_off);
  @ requires (r_len != NULL) ==> \valid(r_len);
  @ requires (s_start_off != NULL) ==> \valid(s_start_off);
  @ requires (s_len != NULL) ==> \valid(s_len);
  @ requires \separated(eaten, buf+(..),r_start_off,r_len,s_start_off,s_len);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> *r_start_off <= len;
  @ ensures (\result == 0) ==> *s_start_off <= len;
  @ ensures (\result == 0) ==> (u64)*r_start_off + (u64)*r_len <= len;
  @ ensures (\result == 0) ==> (u64)*s_start_off + (u64)*s_len <= len;
  @
  @ assigns *eaten, *r_start_off, *r_len, *s_start_off, *s_len;
  @*/
static int sig_gost_extract_r_s(const u8 *buf, u32 len,
				u32 *r_start_off, u32 *r_len,
				u32 *s_start_off, u32 *s_len,
				u32 *eaten)
{
	u32 remain, hdr_len = 0, data_len = 0, off = 0;
	int ret;

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	/*
	 * For an unknown reason, the order of signature components is
	 * reversed, i.e. s comes before r.
	 */
	*s_start_off = off;
	*s_len = remain / 2;
	*r_start_off = off + *s_len;
	*r_len = *s_len;
	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/* Handle GOST R34.10-94 signature parsing */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (params != NULL) ==> \valid(params);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->gost_r3410_94.r_raw_off,
	    params->gost_r3410_94.r_raw_len,
	    params->gost_r3410_94.s_raw_off,
	    params->gost_r3410_94.s_raw_len;
  @*/
int parse_sig_gost94(sig_params *params,
		     const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len = 0, s_start_off = 0, s_len = 0;
	const u8 *buf =  cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_gost_extract_r_s(buf, len, &r_start_off, &r_len,
				   &s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->gost_r3410_94.r_raw_off = off + r_start_off;
	params->gost_r3410_94.r_raw_len = r_len;
	params->gost_r3410_94.s_raw_off = off + s_start_off;
	params->gost_r3410_94.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}

/* Handle GOST R34.10-2001 signature parsing */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (params != NULL) ==> \valid(params);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->gost_r3410_2001.r_raw_off,
	    params->gost_r3410_2001.r_raw_len,
	    params->gost_r3410_2001.s_raw_off,
	    params->gost_r3410_2001.s_raw_len;
  @*/
int parse_sig_gost2001(sig_params *params,
		       const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len = 0, s_start_off = 0, s_len = 0;
	const u8 *buf =  cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_gost_extract_r_s(buf, len, &r_start_off, &r_len,
				   &s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->gost_r3410_2001.r_raw_off = off + r_start_off;
	params->gost_r3410_2001.r_raw_len = r_len;
	params->gost_r3410_2001.s_raw_off = off + s_start_off;
	params->gost_r3410_2001.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}

/* Handle bign (Belarus Signature standard TB 34.101.45-2013) signature parsing */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->bign.sig_raw_off,
	    params->bign.sig_raw_len;
  @*/
int parse_sig_bign(sig_params *params,
		   const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 remain, hdr_len = 0, data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	remain = data_len - 1;

	params->bign.sig_raw_off = off;
	params->bign.sig_raw_len = remain;
	*eaten = hdr_len + data_len;
	ret = 0;

out:
	return ret;
}

/* Handle GOST R34.10-2012 signature parsing */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (params != NULL) ==> \valid(params);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->gost_r3410_2012_256.r_raw_off,
	    params->gost_r3410_2012_256.r_raw_len,
	    params->gost_r3410_2012_256.s_raw_off,
	    params->gost_r3410_2012_256.s_raw_len;
  @*/
int parse_sig_gost2012_256(sig_params *params,
			   const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len = 0, s_start_off = 0, s_len = 0;
	const u8 *buf =  cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_gost_extract_r_s(buf, len, &r_start_off, &r_len,
				   &s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->gost_r3410_2012_256.r_raw_off = off + r_start_off;
	params->gost_r3410_2012_256.r_raw_len = r_len;
	params->gost_r3410_2012_256.s_raw_off = off + s_start_off;
	params->gost_r3410_2012_256.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->gost_r3410_2012_512.r_raw_off,
	    params->gost_r3410_2012_512.r_raw_len,
	    params->gost_r3410_2012_512.s_raw_off,
	    params->gost_r3410_2012_512.s_raw_len;
  @*/
int parse_sig_gost2012_512(sig_params *params,
			   const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len = 0, s_start_off = 0, s_len = 0;
	const u8 *buf =  cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_gost_extract_r_s(buf, len, &r_start_off, &r_len,
				   &s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->gost_r3410_2012_512.r_raw_off = off + r_start_off;
	params->gost_r3410_2012_512.r_raw_len = r_len;
	params->gost_r3410_2012_512.s_raw_off = off + s_start_off;
	params->gost_r3410_2012_512.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (bs_data_start_off != NULL) ==> \valid(bs_data_start_off);
  @ requires (bs_data_len != NULL) ==> \valid(bs_data_len);
  @ requires \separated(eaten, buf+(..), bs_data_start_off, bs_data_len);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> ((*bs_data_start_off + *bs_data_len) <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (bs_data_start_off == \null) ==> \result < 0;
  @ ensures (bs_data_len == \null) ==> \result < 0;
  @
  @ assigns *eaten, *bs_data_start_off, *bs_data_len;
  @*/
int parse_sig_rsa_helper(const u8 *buf, u32 len,
			 u32 *bs_data_start_off, u32 *bs_data_len,
			 u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0;
	int ret;

	if ((buf == NULL) || (len == 0) || (eaten == NULL) ||
	    (bs_data_start_off == NULL) || (bs_data_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert hdr_len + data_len == len; */

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	*bs_data_start_off = hdr_len + 1;
	*bs_data_len = data_len - 1;
	*eaten = hdr_len + data_len;
	ret = 0;
	/*@ assert (*bs_data_start_off + *bs_data_len) <= len; */

out:
	return ret;
}

/*
 * RFC 8410 defines Agorithm Identifiers for Ed25519 and Ed448
 *
 * The same algorithm identifiers are used for signatures as are used
 * for public keys.  When used to identify signature algorithms, the
 * parameters MUST be absent.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (r_start_off != NULL) ==> \valid(r_start_off);
  @ requires (r_len != NULL) ==> \valid(r_len);
  @ requires (s_start_off != NULL) ==> \valid(s_start_off);
  @ requires (s_len != NULL) ==> \valid(s_len);
  @ requires \separated(eaten, cert+(..), r_start_off, r_len, s_start_off, s_len);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (((u64)*r_start_off + (u64)*r_len) <= (u64)off + (u64)len);
  @ ensures (\result == 0) ==> (((u64)*s_start_off + (u64)*s_len) <= (u64)off + (u64)len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (r_start_off == \null) ==> \result < 0;
  @ ensures (r_len == \null) ==> \result < 0;
  @ ensures (s_start_off == \null) ==> \result < 0;
  @ ensures (s_len == \null) ==> \result < 0;
  @
  @ assigns *eaten, *r_start_off, *r_len, *s_start_off, *s_len;
  @*/
int parse_sig_eddsa(const u8 *cert, u32 off, u32 len, u32 exp_sig_len,
		    u32 *r_start_off, u32 *r_len, u32 *s_start_off, u32 *s_len,
		    u32 *eaten)
{
	u32 comp_len, sig_len = 0, hdr_len = 0, data_len = 0, remain = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((cert == NULL) || (len == 0) || (eaten == NULL) ||
	    (r_start_off == NULL) || (r_len == NULL) ||
	    (s_start_off == NULL) || (s_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert hdr_len + data_len == len; */

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	sig_len = data_len - 1;
	if (sig_len != exp_sig_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	comp_len = sig_len / 2;

	if (sig_len != (comp_len * 2)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert sig_len == 2 * comp_len; */

	*r_start_off = off + hdr_len + 1;
	*r_len = comp_len;
	/*@ assert *r_len == comp_len; */
	/*@ assert (u64)*r_start_off + (u64)*r_len <= (u64)off + (u64)len; */

	*s_start_off = off + hdr_len + 1 + comp_len;
	*s_len = comp_len;
	/*@ assert *s_len == comp_len; */
	/*@ assert ((u64)*s_start_off + (u64)*s_len) <= (u64)off + (u64)len; */

	/*
	 * Check there is nothing remaining in the bitstring
	 * after the two integers
	 */
	if (remain != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = hdr_len + data_len;
	ret = 0;
	/*@ assert ((u64)*r_start_off + (u64)*r_len) <= (u64)off + (u64)len; */
	/*@ assert ((u64)*s_start_off + (u64)*s_len) <= (u64)off + (u64)len; */

out:
	return ret;
}

#define ED448_SIG_LEN 114
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->ed448.r_raw_off,
	    params->ed448.r_raw_len,
	    params->ed448.s_raw_off,
	    params->ed448.s_raw_len;
  @*/
int parse_sig_ed448(sig_params *params,
		    const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_sig_eddsa(cert, off, len, ED448_SIG_LEN,
			      &params->ed448.r_raw_off,
			      &params->ed448.r_raw_len,
			      &params->ed448.s_raw_off,
			      &params->ed448.s_raw_len,
			      eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

#define ED25519_SIG_LEN 64
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->ed25519.r_raw_off,
	    params->ed25519.r_raw_len,
	    params->ed25519.s_raw_off,
	    params->ed25519.s_raw_len;
  @*/
int parse_sig_ed25519(sig_params *params,
		      const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_sig_eddsa(cert, off, len, ED25519_SIG_LEN,
			      &params->ed25519.r_raw_off,
			      &params->ed25519.r_raw_len,
			      &params->ed25519.s_raw_off,
			      &params->ed25519.s_raw_len,
			      eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*
 * DSA-based signatures often share the same layout for (r,s), i.e. a bitstring
 * whose content will be parsed as a sequence of two positive integers. This
 * helper does that job by providing positions of integer *values* and length
 * along with the size of the whole structure (bitstring). Note that the
 * function cannot and does not make hypothesis on the length of r and s. This
 * checks, if needed, are left to the caller.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (r_start_off != NULL) ==> \valid(r_start_off);
  @ requires (r_len != NULL) ==> \valid(r_len);
  @ requires (s_start_off != NULL) ==> \valid(s_start_off);
  @ requires (s_len != NULL) ==> \valid(s_len);
  @ requires \separated(eaten, buf+(..), r_start_off, r_len, s_start_off, s_len);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> ((*r_start_off + *r_len) <= len);
  @ ensures (\result == 0) ==> ((*s_start_off + *s_len) <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (r_start_off == \null) ==> \result < 0;
  @ ensures (r_len == \null) ==> \result < 0;
  @ ensures (s_start_off == \null) ==> \result < 0;
  @ ensures (s_len == \null) ==> \result < 0;
  @
  @ assigns *eaten, *r_start_off, *r_len, *s_start_off, *s_len;
  @*/
int sig_dsa_based_extract_r_s(const u8 *buf, u32 len,
			      u32 *r_start_off, u32 *r_len,
			      u32 *s_start_off, u32 *s_len,
			      u32 *eaten)
{
	u32 bs_hdr_len = 0, bs_data_len = 0, sig_len = 0, hdr_len = 0;
	u32 data_len = 0, remain = 0, saved_sig_len = 0;
	u32 integer_len = 0;
	u32 off;
	int ret;

	if ((buf == NULL) || (len == 0) || (eaten == NULL) ||
	    (r_start_off == NULL) || (r_len == NULL) ||
	    (s_start_off == NULL) || (s_len == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING,
			   &bs_hdr_len, &bs_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	saved_sig_len = bs_hdr_len + bs_data_len;
	/*@ assert saved_sig_len <= len; */
	buf += bs_hdr_len;
	off = bs_hdr_len;
	/*@ assert off + bs_data_len <= len; */

	/*
	 * We expect the bitstring data to contain at least the initial
	 * octet encoding the number of unused bits in the final
	 * subsequent octet of the bistring.
	 * */
	if (bs_data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * The signature field is always a bitstring whose content
	 * may then be interpreted depending on the signature
	 * algorithm. At the moment, we only support ECDSA signature
	 * mechanisms. In that case, the content of the bitstring
	 * is parsed as defined in RFC5480, i.e. as a sequence of
	 * two integers:
	 *
	 * ECDSA-Sig-Value ::= SEQUENCE {
	 *   r  INTEGER,
	 *   s  INTEGER
	 * }
	 *
	 * As we only structural checks here, we do not verify
	 * the values stored in the integer are valid r and s
	 * values for the specific alg/curve.
	 */

	/*
	 * We expect the initial octet to encode a value of 0
	 * indicating that there are no unused bits in the final
	 * subsequent octet of the bitstring.
	 */
	if (buf[0] != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	buf += 1;
	off += 1;
	sig_len = bs_data_len - 1;
	/*@ assert off + bs_data_len - 1 <= len; */
	/*@ assert off + sig_len <= len; */

	/*
	 * Now that we know we are indeed dealing w/ a DSA sig mechanism,
	 * let's check we have a sequence of two integers.
	 */
	ret = parse_id_len(buf, sig_len,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Nothing must remain in the bitstring after the sequence. */
	if (sig_len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	/*@ assert (off + remain) <= len; */

	/*
	 * Now, we should find the first non negative integer, r.
	 */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	integer_len = hdr_len + data_len;
	/*@ assert integer_len <= remain; */
	/*@ assert (off + remain) <= len; */
	/*@ assert (off + integer_len) <= len; */
	remain -= integer_len;
	buf += integer_len;
	*r_start_off = off + hdr_len;
	/*@ assert *r_start_off == off + hdr_len; */
	*r_len = data_len;
	/*@ assert *r_len == data_len; */
	/*@ assert *r_start_off == off + hdr_len; */
	/*@ assert (*r_start_off + *r_len) == (off + integer_len); */
	/*@ assert *r_start_off + *r_len <= len; */
	off += hdr_len + data_len;
	/*@ assert (off + remain) <= len; */

	/* An then, the second one, s */
	ret = parse_non_negative_integer(buf, remain, CLASS_UNIVERSAL,
					 ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	integer_len = hdr_len + data_len;
	/*@ assert integer_len <= remain; */
	/*@ assert (off + remain) <= len; */
	/*@ assert (off + integer_len) <= len; */
	remain -= integer_len;
	buf += hdr_len + data_len;
	*s_start_off = off + hdr_len;
	/*@ assert *s_start_off == off + hdr_len; */
	*s_len = data_len;
	/*@ assert *s_len == data_len; */
	/*@ assert *s_start_off == off + hdr_len; */
	/*@ assert (*s_start_off + *s_len) == (off + integer_len); */
	/*@ assert *s_start_off + *s_len <= len; */
	off += integer_len;

	/*
	 * Check there is nothing remaining in the bitstring
	 * after the two integers
	 */
	if (remain != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert saved_sig_len <= len; */
	/*@ assert *r_start_off + *r_len <= len; */
	/*@ assert *s_start_off + *s_len <= len; */
	*eaten = saved_sig_len;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (params != NULL) ==> \valid(params);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->ecdsa.r_raw_off,
	    params->ecdsa.r_raw_len,
	    params->ecdsa.s_raw_off,
	    params->ecdsa.s_raw_len;
  @*/
int parse_sig_ecdsa(sig_params *params,
		    const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len= 0, s_start_off = 0, s_len = 0;
	const u8 *buf =  cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_dsa_based_extract_r_s(buf, len, &r_start_off, &r_len,
					&s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->ecdsa.r_raw_off = off + r_start_off;
	params->ecdsa.r_raw_len = r_len;
	params->ecdsa.s_raw_off = off + s_start_off;
	params->ecdsa.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}

/*
 * SM2 signature and ECDSA signature have exactly the same structre, i.e. a
 * bitstring which encapsulates a sequence of 2 integers (r and s).
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->sm2.r_raw_off,
	    params->sm2.r_raw_len,
	    params->sm2.s_raw_off,
	    params->sm2.s_raw_len;
  @*/
int parse_sig_sm2(sig_params *params,
		  const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len = 0, s_start_off = 0, s_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_dsa_based_extract_r_s(buf, len, &r_start_off, &r_len,
					&s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->sm2.r_raw_off = off + r_start_off;
	params->sm2.r_raw_len = r_len;
	params->sm2.s_raw_off = off + s_start_off;
	params->sm2.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}


/*
 * Handle parsing of RSA PKCS#1 v1.5 signature structure i.e. the opaque
 * content of the bitstring.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->rsa_pkcs1_v1_5.sig_raw_off,
	    params->rsa_pkcs1_v1_5.sig_raw_len;
  @*/
int parse_sig_rsa_pkcs1_v15(sig_params *params,
			    const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 bs_data_start_off = 0, bs_data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_sig_rsa_helper(buf, len,
				   &bs_data_start_off, &bs_data_len,
				   eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	/*
	 * We can now record start and length of data carried in the signature
	 * bitstring, i.e. usually the raw big endian encoded integer value (no
	 * ASN.1 integer encoding) corresponding to the signature.
	 */
	params->rsa_pkcs1_v1_5.sig_raw_off = off + bs_data_start_off;
	params->rsa_pkcs1_v1_5.sig_raw_len = bs_data_len;

out:
	return ret;
}

/*
 * Handle parsing of RSASSA PSS signature structure i.e. the opaque
 * content of the bitstring.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->rsa_ssa_pss.sig_raw_off,
	    params->rsa_ssa_pss.sig_raw_len;
  @*/
int parse_sig_rsa_ssa_pss(sig_params *params,
			  const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 bs_data_start_off = 0, bs_data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_sig_rsa_helper(buf, len,
				   &bs_data_start_off, &bs_data_len,
				   eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	/*
	 * We can now record start and length of data carried in the signature
	 * bitstring, i.e. usually the raw big endian encoded integer value (no
	 * ASN.1 integer encoding) corresponding to the signature.
	 */
	params->rsa_ssa_pss.sig_raw_off = off + bs_data_start_off;
	params->rsa_ssa_pss.sig_raw_len = bs_data_len;

out:
	return ret;
}

/*
 * Handle parsing of 9796 pad 2 signature structure i.e. the opaque
 * content of the bitstring.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->rsa_9796_2_pad.sig_raw_off,
	    params->rsa_9796_2_pad.sig_raw_len;
  @*/
int parse_sig_rsa_9796_2_pad(sig_params *params,
			     const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 bs_data_start_off = 0, bs_data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_sig_rsa_helper(buf, len,
				   &bs_data_start_off, &bs_data_len,
				   eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	/*
	 * We can now record start and length of data carried in the signature
	 * bitstring, i.e. usually the raw big endian encoded integer value (no
	 * ASN.1 integer encoding) corresponding to the signature.
	 */
	params->rsa_9796_2_pad.sig_raw_off = off + bs_data_start_off;
	params->rsa_9796_2_pad.sig_raw_len = bs_data_len;

out:
	return ret;
}

/*
 * Handle parsing of belgian rsa signature structure i.e. the opaque
 * content of the bitstring.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->belgian_rsa.sig_raw_off,
	    params->belgian_rsa.sig_raw_len;
  @*/
int parse_sig_rsa_belgian(sig_params *params,
			  const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 bs_data_start_off = 0, bs_data_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_sig_rsa_helper(buf, len,
				   &bs_data_start_off, &bs_data_len,
				   eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	/*
	 * We can now record start and length of data carried in the signature
	 * bitstring, i.e. usually the raw big endian encoded integer value (no
	 * ASN.1 integer encoding) corresponding to the signature.
	 */
	params->belgian_rsa.sig_raw_off = off + bs_data_start_off;
	params->belgian_rsa.sig_raw_len = bs_data_len;

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (params != NULL) ==> \valid(params);
  @ requires \separated(eaten, cert+(..), params);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    params->dsa.r_raw_off,
	    params->dsa.r_raw_len,
	    params->dsa.s_raw_off,
	    params->dsa.s_raw_len;
  @*/
int parse_sig_dsa(sig_params *params,
		  const u8 *cert, u32 off, u32 len, u32 *eaten)
{
	u32 r_start_off = 0, r_len=0, s_start_off = 0, s_len = 0;
	const u8 *buf = cert + off;
	int ret;

	if ((params == NULL) || (cert == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = sig_dsa_based_extract_r_s(buf, len, &r_start_off, &r_len,
					&s_start_off, &s_len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert *eaten <= len; */

	params->dsa.r_raw_off = off + r_start_off;
	params->dsa.r_raw_len = r_len;
	params->dsa.s_raw_off = off + s_start_off;
	params->dsa.s_raw_len = s_len;
	ret = 0;

out:
	return ret;
}


/*
 * HashAlgorithm is a sequence containing an OID and parameters which are
 * always NULL for all the hash functions defined in RFC 8017. For that
 * reason, the function only returns a pointer to a known hash alg(from
 * known_hashes) on success (i.e. parameters is useless). The function
 * returns a negative value on error. On success, the length of
 * HashAlgorithm structure is returned.
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (hash_alg != NULL) ==> \valid(hash_alg);
  @ requires \separated(eaten, buf+(..), hash_alg);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && *hash_alg == known_hashes[i] && \valid_read(*hash_alg);
  @
  @ assigns *eaten, *hash_alg;
  @*/
static int parse_HashAlgorithm(const u8 *buf, u32 len, _hash_alg const **hash_alg,
			       u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0, oid_len = 0, remain = 0;
	int ret;

	if ((buf == NULL) || (hash_alg == NULL) || (eaten == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* We expect a sequence ...  */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/* ... starting with a hash OID ... */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*hash_alg = find_hash_by_oid(buf, oid_len);
	if (*hash_alg == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && *hash_alg == known_hashes[i]; */

	buf += oid_len;
	remain -= oid_len;

	/* ... followed by a NULL ... */
	if ((remain != 2) || (buf[0] != 0x05) || (buf[1] != 0x00)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = hdr_len + data_len;
	ret = 0;

out:
	return ret;
}


/*
 * The function parses the parameters associated with id-RSASSA-PSS
 * OID (1.2.840.113549.1.1.10) found in signature AlgorithmIdentifier
 * The expected structure of the parameters is:
 *
 * RSASSA-PSS-params ::= SEQUENCE {
 *      hashAlgorithm      [0] HashAlgorithm      DEFAULT sha1,
 *      maskGenAlgorithm   [1] MaskGenAlgorithm   DEFAULT mgf1SHA1,
 *      saltLength         [2] INTEGER            DEFAULT 20,
 *      trailerField       [3] TrailerField       DEFAULT trailerFieldBC
 *  }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(params, hash_alg, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(hash_alg) &&
	    \initialized(&params->rsa_ssa_pss.mgf_alg) &&
	    \initialized(&params->rsa_ssa_pss.mgf_hash_alg) &&
	    \initialized(&params->rsa_ssa_pss.salt_len) &&
	    \initialized(&params->rsa_ssa_pss.trailer_field);
  @
  @ assigns *hash_alg,
	    params->rsa_ssa_pss.mgf_alg,
	    params->rsa_ssa_pss.mgf_hash_alg,
	    params->rsa_ssa_pss.salt_len,
	    params->rsa_ssa_pss.trailer_field;
  @*/
int parse_algoid_sig_params_rsassa_pss(sig_params *params, hash_alg_id *hash_alg,
					      const u8 *cert, u32 off, u32 len)
{
	u32 remain, hdr_len = 0, data_len = 0, oid_len = 0;
	u32 int_hdr_len = 0, int_data_len = 0;
	u32 attr_hdr_len = 0, attr_data_len = 0, eaten = 0;
	u8 salt_len = 0;
	const u8 *buf = cert + off;
	_hash_alg const *hash = NULL;
	_mgf const *mgf = NULL;
	_hash_alg const *mgf_hash = NULL;
	u8 trailer_field = 0;
	int ret;

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	/*****************************************************************
	 * hashAlgorithm      [0] HashAlgorithm      DEFAULT sha1,
	 *****************************************************************/
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &attr_hdr_len, &attr_data_len);
	if (ret) {
		/*
		 * hashAlgorithm is missing, which means hash algorithm
		 * to use is the default, i.e. sha1.
		 */
		hash = &_sha1_hash_alg;
	} else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;

		ret = parse_HashAlgorithm(buf, attr_data_len, &hash, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*@ assert \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && hash == known_hashes[i]; */

		/* Verify we have no trailing data */
		if (eaten != attr_data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += eaten;
		remain -= eaten;
	}

	/* Record the hash algorithm we just learnt */
	*hash_alg = hash->hash_id;
	/*@ assert \initialized(hash_alg); */

	/*****************************************************************
	 * maskGenAlgorithm   [1] MaskGenAlgorithm   DEFAULT mgf1SHA1,   *
	 *****************************************************************/
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
			   &attr_hdr_len, &attr_data_len);
	if (ret) {
		/*
		 * maskGenAlgorithm is missing, which means MGF
		 * to use is the default, i.e. MGF1 (the only one
		 * defined).
		 */
		mgf = &_mgf1_alg;
		mgf_hash = &_sha1_hash_alg;
	} else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;

		/* We expect a sequence ...  */
		ret = parse_id_len(buf, remain, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				   &hdr_len, &data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Length of the sequence should match data length of attribute,
		 * i.e. we do not accept trailing data
		 */
		if (attr_data_len != (hdr_len + data_len)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += hdr_len;
		remain -= hdr_len;

		/* ... starting with a MGF OID ... */
		ret = parse_OID(buf, data_len, &oid_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* Check this is indeed the only OID we support (MGF1 oid). */
		if ((oid_len != _mgf1_alg.alg_der_oid_len) ||
		    bufs_differ(buf, _mgf1_alg.alg_der_oid, oid_len)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		mgf = &_mgf1_alg;

		buf += oid_len;
		remain -= oid_len;
		data_len -= oid_len;

		/*
		 * ... followed by a HashAlgorithm (a sequence containing a hash
		 * OID and a NULL for associated parameters) ...
		 */
		ret = parse_HashAlgorithm(buf, data_len, &mgf_hash, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*@ assert \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && mgf_hash == known_hashes[i]; */

		buf += eaten;
		remain -= eaten;
		data_len -= eaten;

		/* Verify we have no trailing data */
		if (data_len != 0) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

	}

	/* Record the MGF and associated MGF hash alg */
	params->rsa_ssa_pss.mgf_alg = mgf->mgf_id;
	params->rsa_ssa_pss.mgf_hash_alg = mgf_hash->hash_id;
	/*@ assert \initialized(&params->rsa_ssa_pss.mgf_alg); */
	/*@ assert \initialized(&params->rsa_ssa_pss.mgf_hash_alg); */

	/*****************************************************************
	 * saltLength         [2] INTEGER            DEFAULT 20,         *
	 *****************************************************************/
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 2,
			   &attr_hdr_len, &attr_data_len);
	if (ret) {
		/*
		 * saltLength is missing, which means the default should
		 * be used.
		 */
		salt_len = 20;
	} else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;

		ret = parse_non_negative_integer(buf, attr_data_len, CLASS_UNIVERSAL,
				ASN1_TYPE_INTEGER, &int_hdr_len,
				&int_data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* We expect no trailing data */
		eaten = int_hdr_len + int_data_len;
		if (eaten != attr_data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * The spec has no limit on salt length value. As it does not
		 * make sense for salt length to be more than 127 bytes, we
		 * limit integer value to 127, i.e. we expect its value to be
		 * encoded on one byte.
		 */
		if (int_data_len != 1) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		salt_len = buf[2];

		buf += eaten;
		remain -= eaten;
	}

	/* Record the salt_len */
	params->rsa_ssa_pss.salt_len = salt_len;
	/*@ assert \initialized(&params->rsa_ssa_pss.salt_len); */

	/*****************************************************************
	 * trailerField       [3] TrailerField    DEFAULT trailerFieldBC *
	 *****************************************************************/
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 3,
			   &attr_hdr_len, &attr_data_len);
	if (ret) {
		/*
		 * trailerField is missing, which means the default (0xbc)
		 * should be used.
		 */
		trailer_field = 1; /* indicating 0xbc trailerfield */
	} else {
		/*
		 * The spec only support value 1 for trailerField, which is
		 * the default. Here, the certificate contains an explicit
		 * integer value, which either 1 (same as the default, so
		 * DER makes that invalid) or different (which we do not
		 * support. In both cases, this is invalid. We just go a
		 * bit furtuer in parsing to report a more specific error.
		 */
		buf += attr_hdr_len;
		remain -= attr_hdr_len;

		ret = parse_integer(buf, attr_data_len, CLASS_UNIVERSAL,
				    ASN1_TYPE_INTEGER,
				    &int_hdr_len, &int_data_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* We expect no trailing data */
		eaten = int_hdr_len + int_data_len;
		if (eaten != attr_data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		if (eaten != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		if (buf[2] == 1) {
			/*
			 * This is the default trailer field. DER prevents
			 * explicit setting of the default value
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/*
		 * Another value than 1 is also invalid because RFC5280
		 * does not support it
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Record trailer field */
	params->rsa_ssa_pss.trailer_field = trailer_field;
	/*@ assert \initialized(&params->rsa_ssa_pss.trailer_field); */

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(hash_alg) &&
	    \initialized(&params->rsa_ssa_pss.mgf_alg) &&
	    \initialized(&params->rsa_ssa_pss.mgf_hash_alg) &&
	    \initialized(&params->rsa_ssa_pss.salt_len) &&
	    \initialized(&params->rsa_ssa_pss.trailer_field); */

	ret = 0;

out:
	return ret;
}



/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(params, hash_alg, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_sig_params_ecdsa_with(sig_params *params, hash_alg_id *hash_alg,
					      const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	/*
	 * Based on the OID, specific parameters may follow. As we currently
	 * only support ECDSA-based signature algorithms and RFC5758 specifies
	 * those come w/o any additional parameters, we expect data_len to
	 * exactly match oid_len.
	 *
	 * Section 3.2 of RFC 5758 reads:
	 *
	 *   When the ecdsa-with-SHA224, ecdsa-with-SHA256, ecdsa-with-SHA384,
	 *   or ecdsa-with-SHA512 algorithm identifier appears in the algorithm
	 *   field as an AlgorithmIdentifier, the encoding MUST omit the
	 *   parameters field.  That is, the AlgorithmIdentifier SHALL be a
	 *   SEQUENCE of one component, the OID ecdsa-with-SHA224,
	 *   ecdsa-with-SHA256, ecdsa-with-SHA384, or ecdsa-with-SHA512.
	 *
	 * Section 3 of RFC 8692 defining ECDSA with SHAKE128/256 has the same
	 * requirement.
	 */

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	} else {
		ret = 0;
	}

out:
	return ret;
}


/*
 * When Alg OID is 1.2.840.10045.4.3 (ecdsa-with-SHA2), the parameters contains
 * the OID of the hash function to be used with ECDSA for the signature.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(params, hash_alg, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @
  @ assigns *hash_alg;
  @*/
int parse_algoid_sig_params_ecdsa_with_specified(sig_params *params, hash_alg_id *hash_alg,
							const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	const _hash_alg *hash = NULL;
	u32 parsed = 0;
	int ret;

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	ret = parse_HashAlgorithm(buf, len, &hash, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \exists integer i ; 0 <= i < NUM_KNOWN_HASHES && hash == known_hashes[i]; */

	if (parsed != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Record the hash algorithm we just learnt */
	*hash_alg = hash->hash_id;

out:
	return ret;
}


/*
 * When Alg OID is bign-with-hspec (1.2.112.0.2.0.34.101.45.11), we expect the
 * the parameters to directly contain the OID of the hash function. Nothing else
 * would be valid.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(params, hash_alg, cert+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @
  @ assigns *hash_alg;
  @*/
int parse_algoid_sig_params_bign_with_hspec(sig_params *params, hash_alg_id *hash_alg,
						   const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	const _hash_alg *hash;
	u32 oid_len = 0;
	u32 remain;
	int ret;

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/* Let's see if we have on OID here ... */
	ret = parse_OID(buf, remain, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* ... for a known hash function */
	hash = find_hash_by_oid(buf, oid_len);
	if (hash == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += oid_len;
	remain -= oid_len;

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Record the hash algorithm we just learnt */
	*hash_alg = hash->hash_id;
	ret = 0;

out:
	return ret;
}

/*
 * When parsing SM2 signature algorithm identifier, we may either find no
 * params or ASN.1 NULL object (SM2 is always used with SM3 hash algorithm).
 * We support those 2 cases we found in real world certs.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(cert+(..), params, hash_alg);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_sig_params_sm2(sig_params *params, hash_alg_id *hash_alg,
				       const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 parsed = 0;
	int ret;

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	switch (len) {
	case 0:
		ret = 0;
		break;
	case 2:
		ret = parse_null(buf, len, &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		break;
	}

out:
	return ret;
}

/*
 * RFC 8410 has: "For all of the OIDs, the parameters MUST be absent."
 * This is what the function enforces. This applies to both signature
 * parameters and SPKI parameters. We have a specific helper for each
 * case.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(cert+(..), params, hash_alg);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @ ensures (len != 0) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_sig_params_eddsa(sig_params *params, hash_alg_id *hash_alg,
					 const u8 *cert, u32 ATTRIBUTE_UNUSED off, u32 len)
{
	int ret;

	if ((params == NULL) || (hash_alg == NULL) || (cert == NULL) || (len != 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(cert+(..), params, hash_alg);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_sig_params_none(sig_params *params, hash_alg_id *hash_alg,
					const u8 *cert, u32 off, u32 len)
{
	int ret;

	if ((params == NULL) || (hash_alg == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_none(cert, off, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

/*
 * Handles expected lack of optinal parameters associated with sig and pubkey
 * OID. The function also support the case where lack of parames has been
 * implemented by some software by a adding a NULL instead of nothing.
 * We define specific sig and pubky function from that one below.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (cert == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_params_none(const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 parsed = 0;
	int ret;

	if (cert == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (len > 0) ==> \valid_read(buf + (0 .. len - 1)); */

	switch (len) {
	case 0: /* Nice ! */
		ret = 0;
		break;
	case 2: /* Null ? */
		ret = parse_null(buf, len, &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		}
		break;
	default: /* Crap ! */
		ret = -1;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		break;
	}

out:
	return ret;
}

/*
 * Parser for parameters associated with the OID for RSA public key
 * (rsaEncryption, GIP-CPS, ) and common PKCS#1 v1.5 signature OID
 * (sha*WithRSAEncryption, md5WithRSAEncryption).
 * Those all expect a NULL parameter. From this function,
 * we define specific instances for signature and pubkey parameters
 * below.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires (len > 0) ==> \valid_read(cert + (off .. (off + len - 1)));
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_params_rsa(const u8 *cert, u32 off, u32 len)
{
	const u8 *buf = cert + off;
	u32 parsed = 0;
	int ret;

	if (cert == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (len > 0) ==> \valid_read(buf + (0 .. len - 1)); */

#ifdef TEMPORARY_LAXIST_RSA_PUBKEY_AND_SIG_NO_PARAMS_INSTEAD_OF_NULL
	/*
	 * We expect a null but will allow empty params
	 * in that case.
	 */
	if (len == 0) {
		ret = 0;
		goto out;
	}
#endif
	/*
	 * Section 3.2 of RFC 3370 explicitly states that
	 * "When the rsaEncryption, sha1WithRSAEncryption, or
	 * md5WithRSAEncryption signature value algorithm
	 * identifiers are used, the AlgorithmIdentifier parameters
	 * field MUST be NULL.", i.e. contain { 0x05, 0x00 }
	 *
	 */
	ret = parse_null(buf, len, &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires ((params != \null)) ==> \valid(params);
  @ requires ((hash_alg != \null)) ==> \valid(hash_alg);
  @ requires \separated(cert+(..), params, hash_alg);
  @
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (params == \null) ==> \result < 0;
  @ ensures (hash_alg == \null) ==> \result < 0;
  @ ensures \result < 0 || \result == 0;
  @
  @ assigns \nothing;
  @*/
int parse_algoid_sig_params_rsa(sig_params *params, hash_alg_id *hash_alg,
				       const u8 *cert, u32 off, u32 len)
{
	int ret;

	if ((cert == NULL) || (params == NULL) || (hash_alg == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_algoid_params_rsa(cert, off, len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}


/*@
  @ requires na_year <= 9999;
  @ requires na_month <= 12;
  @ requires na_day <= 31;
  @ requires na_hour <= 23;
  @ requires na_min <= 59;
  @ requires na_sec <= 59;
  @
  @ ensures \result < (1ULL << 55);
  @
  @ assigns \nothing;
  @*/
u64 time_components_to_comparable_u64(u16 na_year, u8 na_month, u8 na_day,
				      u8 na_hour, u8 na_min, u8 na_sec)
{
	u64 res, tmp;

	res = ((u64)na_sec);                 /* start with seconds */
	/*@ assert res < (1ULL << 6); */

	tmp = ((u64)na_min) * (1ULL << 8);            /* add shifted minutes */
	/*@ assert tmp < (1ULL << 14); */
	res += tmp;
	/*@ assert res < (1ULL << 15); */

	tmp = (((u64)na_hour) * (1ULL << 16));        /* add shifted hours */
	/*@ assert tmp < (1ULL << 21); */
	res += tmp;
	/*@ assert res < (1ULL << 22); */

	tmp = ((u64)na_day) * (1ULL << 24);           /* add shifted days */
	/*@ assert tmp < (1ULL << 29); */
	res += tmp;
	/*@ assert res < (1ULL << 30); */

	tmp = ((u64)na_month) * (1ULL << 32);         /* add shifted days */
	/*@ assert tmp < (1ULL << 36); */
	res += tmp;
	/*@ assert res < (1ULL << 37); */

	tmp = ((u64)na_year) * (1ULL << 40);         /* add shifted years */
	/*@ assert tmp < (1ULL << 54); */
	res += tmp;
	/*@ assert res < (1ULL << 55); */

	return res;
}




/* ============== IMPL: x509-crl-parser.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

/* removed: #include "x509-crl-parser.h" */

#define X509_FILE_NUM 5 /* See x509-utils.h for rationale */

/*
 * Reminder on CRL format:
 *
 *    CertificateList  ::=  SEQUENCE  {
 *        tbsCertList          TBSCertList,
 *        signatureAlgorithm   AlgorithmIdentifier,
 *        signatureValue       BIT STRING  }
 *
 *   TBSCertList  ::=  SEQUENCE  {
 *        version                 Version OPTIONAL,
 *                                     -- if present, MUST be v2
 *        signature               AlgorithmIdentifier,
 *        issuer                  Name,
 *        thisUpdate              Time,
 *        nextUpdate              Time OPTIONAL,
 *        revokedCertificates     SEQUENCE OF SEQUENCE  {
 *             userCertificate         CertificateSerialNumber,
 *             revocationDate          Time,
 *             crlEntryExtensions      Extensions OPTIONAL
 *                                      -- if present, version MUST be v2
 *                                  }  OPTIONAL,
 *        crlExtensions           [0]  EXPLICIT Extensions OPTIONAL
 *                                      -- if present, version MUST be v2
 *                                  }
 *
 * Structure of the module, by order of appearance :
 *
 *  - Code for parsing CRL Entry Extensions
 *  - Code for parsing revokedCertificates
 *  - Code for parsing CRL Extensions
 *  - Code for parsing tbsCertList
 *  - Code for parsing signatureAlgorithm
 *  - Code for parsing signatureValue
 *  - parse_x509_crl()
 */


/*****************************************************************************
 * Code for parsing CRL Entry extensions
 *****************************************************************************/

typedef enum {
	crl_reason_unspecified          = 0x00,
	crl_reason_keyCompromise        = 0x01,
	crl_reason_cACompromise         = 0x02,
	crl_reason_affiliationChanged   = 0x03,
	crl_reason_superseded           = 0x04,
	crl_reason_cessationOfOperation = 0x05,
	crl_reason_certificateHold      = 0x06,
	/* value 7 is not used */
	crl_reason_removeFromCRL        = 0x08,
	crl_reason_privilegeWithdrawn   = 0X09,
	crl_reason_aACompromise         = 0x0a
} crl_reason;

static const u8 _crl_entry_ext_oid_ReasonCode[] =      { 0x06, 0x03, 0x55, 0x1d, 0x15 };
static const u8 _crl_entry_ext_oid_InvalidityDate[] =  { 0x06, 0x03, 0x55, 0x1d, 0x18 };
static const u8 _crl_entry_ext_oid_CertIssuer[] =      { 0x06, 0x03, 0x55, 0x1d, 0x1d };

/*
 * Section 5.3.1.  Reason Code
 *
 *    -- reasonCode ::= { CRLReason }
 *
 *    CRLReason ::= ENUMERATED {
 *         unspecified             (0),
 *         keyCompromise           (1),
 *         cACompromise            (2),
 *         affiliationChanged      (3),
 *         superseded              (4),
 *         cessationOfOperation    (5),
 *         certificateHold         (6),
 *              -- value 7 is not used
 *         removeFromCRL           (8),
 *         privilegeWithdrawn      (9),
 *         aACompromise           (10) }
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(crl+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_entry_ext_ReasonCode(crl_parsing_ctx *ctx,
					  const u8 *crl, u32 off, u32 len,
					  int critical)
{
	const u8 *buf = crl + off;
	crl_reason reasonCode;
	int ret = -1;

	if ((ctx == NULL) || (crl == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (critical) {
		/*
		 * Section 5.3.1 of RFC 5280 has "The reasonCode is a
		 * non-critical CRL entry extension"
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * The reasonCode enumerated should be encoded on exactly 3 bytes
	 * First one has enumerated type, followed by a length value of 1
	 * followed by the actual reason code.
	 */
	if ((len != 3) || (buf[0] != ASN1_TYPE_ENUMERATED) || (buf[1] != 0x01)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	reasonCode = buf[2];
	switch (reasonCode) {
	case crl_reason_unspecified:
	case crl_reason_keyCompromise:
	case crl_reason_cACompromise:
	case crl_reason_affiliationChanged:
	case crl_reason_superseded:
	case crl_reason_cessationOfOperation:
	case crl_reason_certificateHold:
	case crl_reason_removeFromCRL:
	case crl_reason_privilegeWithdrawn:
	case crl_reason_aACompromise:
		ret = 0;
		break;

	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

/*
 * Section 5.3.2 of RFC 5280:
 *
 * InvalidityDate ::=  GeneralizedTime
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(crl+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_entry_ext_InvalidityDate(crl_parsing_ctx *ctx,
					  const u8 *crl, u32 off, u32 len,
					  int critical)
{
	u8 month = 0, day = 0, hour = 0, min = 0, sec = 0;
	const u8 *buf = crl + off;
	u32 eaten;
	u16 year;
	int ret = -1;

	if ((ctx == NULL) || (crl == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (critical) {
		/*
		 * Section 5.3.1 of RFC 5280 has "The invalidity date is a
		 * non-critical CRL entry extension"
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_generalizedTime(buf, len, &eaten,
				    &year, &month, &day, &hour, &min, &sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}


/*
 * Section 5.3.3 of RFC 5280:
 *
 * CertificateIssuer ::=     GeneralNames
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(crl+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_entry_ext_CertIssuer(crl_parsing_ctx *ctx,
					  const u8 *crl, u32 off, u32 len,
					  int critical)
{
	const u8 *buf = crl + off;
	u32 eaten;
	int ret;

	if ((ctx == NULL) || (crl == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (!critical) {
		/*
		 * Section 5.3.3 of RFC 5280 has "CRL issuers MUST mark this
		 * extension as critical since an implementation that ignored
		 * this extension could not correctly attribute CRL entries
		 * to certificates.  This specification RECOMMENDS that
		 * implementations recognize this extension."
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_GeneralNames(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				 &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

typedef struct {
	const u8 *oid;
	u8 oid_len;
	int (*parse_crl_entry_ext_params)(crl_parsing_ctx *ctx,
					  const u8 *crl, u32 off, u32 len, int critical);
} _crl_entry_ext_oid;

static const _crl_entry_ext_oid known_crl_entry_ext_oids[] = {
	{ .oid = _crl_entry_ext_oid_ReasonCode, /* Reason Code */
	  .oid_len = sizeof(_crl_entry_ext_oid_ReasonCode),
	  .parse_crl_entry_ext_params = parse_crl_entry_ext_ReasonCode,
	},
	{ .oid = _crl_entry_ext_oid_InvalidityDate, /* Invalidity Date */
	  .oid_len = sizeof(_crl_entry_ext_oid_InvalidityDate),
	  .parse_crl_entry_ext_params = parse_crl_entry_ext_InvalidityDate,
	},
	{ .oid = _crl_entry_ext_oid_CertIssuer, /* Certificate Issuer */
	  .oid_len = sizeof(_crl_entry_ext_oid_CertIssuer),
	  .parse_crl_entry_ext_params = parse_crl_entry_ext_CertIssuer,
	},
};

#define NUM_KNOWN_CRL_ENTRY_EXT_OIDS (sizeof(known_crl_entry_ext_oids) /     \
				      sizeof(known_crl_entry_ext_oids[0]))
#define MAX_EXT_NUM_PER_CRL_ENTRY NUM_KNOWN_CRL_ENTRY_EXT_OIDS

/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_CRL_ENTRY_EXT_OIDS && \result == &known_crl_entry_ext_oids[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
static _crl_entry_ext_oid const * find_crl_entry_ext_by_oid(const u8 *buf, u32 len)
{
	const _crl_entry_ext_oid *found = NULL;
	const _crl_entry_ext_oid *cur = NULL;
	u16 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll NUM_KNOWN_CRL_ENTRY_EXT_OIDS ;
	  @ loop invariant 0 <= k <= NUM_KNOWN_CRL_ENTRY_EXT_OIDS;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_CRL_ENTRY_EXT_OIDS - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_CRL_ENTRY_EXT_OIDS; k++) {
		int ret;

		cur = &known_crl_entry_ext_oids[k];

		/*@ assert cur == &known_crl_entry_ext_oids[k];*/
		if (cur->oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->oid, buf, cur->oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*@
  @ requires ext != \null;
  @ requires \valid(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)));
  @ requires \valid_read(ext);
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)));
  @ requires \separated(ext, parsed_oid_list);
  @
  @ ensures \result <= 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)];
  @*/
static int check_record_crl_entry_ext_unknown(const _crl_entry_ext_oid *ext,
					      const _crl_entry_ext_oid **parsed_oid_list)
{
	u16 pos = 0;
	int ret;

	/*@
	  @ loop invariant pos <= MAX_EXT_NUM_PER_CRL_ENTRY;
	  @ loop assigns ret, pos, parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)];
	  @ loop variant MAX_EXT_NUM_PER_CRL_ENTRY - pos;
	  @*/
	while (pos < MAX_EXT_NUM_PER_CRL_ENTRY) {
		/*
		 * Check if we are at the end of already seen extensions. In
		 * that case, record the extension as a new one.
		 */
		if (parsed_oid_list[pos] == NULL) {
			parsed_oid_list[pos] = ext;
			break;
		}

		if (ext == parsed_oid_list[pos]) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		pos += 1;
	}

	/*
	 * If we went to the end of our array, this means there are too many
	 * extensions in the certificate.
	 */
	if (pos >= MAX_EXT_NUM_PER_CRL_ENTRY) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (parsed_oid_list != NULL) ==> \valid(parsed_oid_list);
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)));
  @ requires \separated(ctx, crl+(..), parsed_oid_list, eaten);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 <= *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)], *eaten, *ctx;
  @*/
static int parse_x509_crl_entry_Extension(crl_parsing_ctx *ctx,
					  const u8 *crl, u32 off, u32 len,
					  const _crl_entry_ext_oid **parsed_oid_list,
					  u32 *eaten)
{
	u32 ext_hdr_len = 0, ext_data_len = 0;
	u32 hdr_len = 0, data_len = 0;
	u32 saved_ext_len = 0, oid_len = 0;
	u32 remain, parsed = 0;
	const u8 *buf = crl + off;
	const _crl_entry_ext_oid *ext = NULL;
	int critical = 0;
	int ret;

	(void)parsed_oid_list;

	if ((crl == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1))); */

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &ext_hdr_len, &ext_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1))); */

	/*
	 * Let's parse the OID and then check if we have
	 * an associated handler for that extension.
	 */
	ret = parse_OID(buf, ext_data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1))); */

	ext = find_crl_entry_ext_by_oid(buf, oid_len);
	if (ext == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1))); */

	/*
	 * Now that we know the OID is one we support, we verify
	 * this is the first time we handle an instance of this
	 * type. Having multiple instances of a given extension
	 * in a certificate is forbidden by both section 4.2 of
	 * RFC5280 and section 8 of X.509, w/ respectively
	 *
	 * - "A certificate MUST NOT include more than one
	 *    instance of a particular extension."
	 * - "For all certificate extensions, CRL extensions,
	 *    and CRL entry extensions defined in this Directory
	 *    Specification, there shall be no more than one
	 *    instance of each extension type in any certificate,
	 *    CRL, or CRL entry, respectively."
	 *
	 * This is done by recording for each extension we
	 * processed the pointer to its vtable and compare
	 * it with current one.
	 */
	ret = check_record_crl_entry_ext_unknown(ext, parsed_oid_list);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;

	/*
	 * Now that we got the OID, let's check critical
	 * field value if present. It's a boolean
	 * defaulting to FALSE (in which case, it is absent).
	 * We could parse it as an integer but that
	 * would be a lot of work for three simple bytes.
	 */
	ret = parse_boolean(buf, ext_data_len, &parsed);
	if (ret) {
		/*
		 * parse_boolean() returned an error which means this
		 * was either a boolean with invalid content or
		 * something else. If this was indeed a boolean, we can
		 * just leave. Otherwise, this just measn the critical
		 * flag was missing and the default value (false) applies,
		 * in which case, we can continue parsing.
		 */
		if (ext_data_len && (buf[0] == ASN1_TYPE_BOOLEAN)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	} else {
		/*
		 * We now know it's a valid BOOLEAN, *but* in our
		 * case (DER), the DEFAULT FALSE means we cannot
		 * accept an encoded value of FALSE. Note that we
		 * sanity check the value we expect for the length
		 */
		if (parsed != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

#ifndef TEMPORARY_LAXIST_EXTENSION_CRITICAL_FLAG_BOOLEAN_EXPLICIT_FALSE
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
#endif

		/*
		 * We now know the BOOLEAN is present and has
		 * a value of TRUE. Record that.
		 */
		critical = 1;

		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}

	/*
	 * We should now be in front of the octet string
	 * containing the extnValue.
	 */
	ret = parse_id_len(buf, ext_data_len,
			   CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;

	/* Check nothing remains behind the extnValue */
	if (data_len != ext_data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Parse the parameters for that extension */
	/*@ assert ext->parse_crl_entry_ext_params \in {
	  parse_crl_entry_ext_ReasonCode,
	  parse_crl_entry_ext_InvalidityDate,
	  parse_crl_entry_ext_CertIssuer }; @*/
	/*@ calls parse_crl_entry_ext_ReasonCode,
	  parse_crl_entry_ext_InvalidityDate,
	  parse_crl_entry_ext_CertIssuer ; @*/
	ret = ext->parse_crl_entry_ext_params(ctx, crl, off, ext_data_len, critical);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = saved_ext_len;
	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(eaten, ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 <= *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_crl_entry_Extensions(crl_parsing_ctx *ctx,
					   const u8 *crl, u32 off, u32 len,
					   u32 *eaten)
{
	u32 data_len = 0, hdr_len = 0, remain = 0;
	const u8 *buf = crl + off;
	u32 saved_len = 0;
	const _crl_entry_ext_oid *parsed_crl_entry_ext_oid_list[MAX_EXT_NUM_PER_CRL_ENTRY];

	int ret;
	u16 i;

	if ((crl == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Extensions in X.509 CRL is an EXPLICITLY tagged sequence.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	/*@ assert \valid_read(buf + (0 .. (remain - 1))); */

	saved_len = hdr_len + data_len;
	/*@ assert saved_len <= len; */
	/*@ assert data_len <= saved_len; */

#ifndef TEMPORARY_LAXIST_ALLOW_CRL_ENTRY_EXT_WITH_EMPTY_SEQ
	/* If present, it must contain at least one extension */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#endif

	/* Initialize list of already seen extensions */
	/*@
	  @ loop unroll MAX_EXT_NUM_PER_CRL_ENTRY;
	  @ loop assigns i, parsed_crl_entry_ext_oid_list[0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)];
	  @ loop invariant (i < MAX_EXT_NUM_PER_CRL_ENTRY) ==> \valid(&parsed_crl_entry_ext_oid_list[i]);
	  @ loop variant (MAX_EXT_NUM_PER_CRL_ENTRY - i);
	  @*/
	for (i = 0; i < MAX_EXT_NUM_PER_CRL_ENTRY; i++) {
		parsed_crl_entry_ext_oid_list[i] = NULL;
	}
	/*@ assert \initialized(parsed_crl_entry_ext_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1))); */

	/* Now, let's work on each extension in the sequence */
	/*@
	  @ loop assigns off, ret, buf, remain, parsed_crl_entry_ext_oid_list[0 .. (MAX_EXT_NUM_PER_CRL_ENTRY - 1)], *ctx;
	  @ loop invariant (remain != 0) ==> \valid_read(crl + (off .. (off + remain - 1)));
	  @ loop invariant (remain != 0) ==> ((u64)off + (u64)remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @*/
	while (remain) {
		u32 ext_len = 0;

		ret = parse_x509_crl_entry_Extension(ctx, crl, off, remain,
						     parsed_crl_entry_ext_oid_list,
						     &ext_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}

	/*@ assert 1 <= saved_len <= len; */
	*eaten = saved_len;

	ret = 0;

out:
	return ret;
}



/*****************************************************************************
 * Code for parsing revokedCertificates
 *****************************************************************************/

/*
 * We call revokedCertificate the component found in revokedCertificates
 * sequence. revokedCertificate is itself a sequence:
 *
 *         revokedCertificates     SEQUENCE OF SEQUENCE  {
 *            userCertificate         CertificateSerialNumber,
 *            revocationDate          Time,
 *            crlEntryExtensions      Extensions OPTIONAL
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(eaten, crl+(..), ctx);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_crl_revokedCertificate(crl_parsing_ctx *ctx,
					     const u8 *crl, u32 off, u32 len,
					     u32 *eaten)
{
	const u8 *buf = crl + off;
	u32 rev_len = 0;
	u16 rev_year = 0;
	u8 rev_month = 0, rev_day = 0, rev_hour = 0, rev_min = 0, rev_sec = 0;
	u8 t_type = 0;
	u32 hdr_len = 0, data_len = 0;
	u32 parsed = 0;
	u32 remain = len;
	u32 saved_rev_len = 0;
	int ret;

	if ((ctx == NULL) || (crl == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
	saved_rev_len = hdr_len + data_len;

	/* userCertificate, i.e. revoked certificate serial number */
	ret = parse_SerialNumber(crl, off, remain, CLASS_UNIVERSAL,
				 ASN1_TYPE_INTEGER, &parsed);
	if (ret) {
	       ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	       goto out;
	}

	buf += parsed;
	remain -= parsed;
	off += parsed;

	/* revocationDate */
	ret = parse_Time(buf, remain, &t_type, &rev_len, &rev_year, &rev_month,
			 &rev_day, &rev_hour, &rev_min, &rev_sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check valid time type was used for year value */
	ret = verify_correct_time_use(t_type, rev_year);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += rev_len;
	remain -= rev_len;
	off += rev_len;

	/* crlEntryExtensions is only acceptable for v2 crl */
	if ((ctx->version != 0x01) && remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (remain) {
		ret = parse_x509_crl_entry_Extensions(ctx, crl, off, remain,
						      &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= parsed;
		off += parsed;
		buf += parsed;
	}

	*eaten = saved_rev_len;
	ret = 0;

out:
	return ret;
}


/*****************************************************************************
 * Code for parsing CRL Extensions
 *****************************************************************************/


/*
 * Authority Key Identifier extension for CRL. Defined in secion 5.2.1 of
 * RFC 5280. Like certificate's AKI extension, the CRL extension structure
 * is defined in section 4.2.1.1 of RFC 5280:
 *
 *   id-ce-authorityKeyIdentifier OBJECT IDENTIFIER ::=  { id-ce 35 }
 *
 *   AuthorityKeyIdentifier ::= SEQUENCE {
 *      keyIdentifier             [0] KeyIdentifier           OPTIONAL,
 *      authorityCertIssuer       [1] GeneralNames            OPTIONAL,
 *      authorityCertSerialNumber [2] CertificateSerialNumber OPTIONAL  }
 *      -- authorityCertIssuer and authorityCertSerialNumber MUST both
 *      -- be present or both be absent
 *
 *   KeyIdentifier ::= OCTET STRING
 *
 * The function below has code duplication with parse_ext_AKI but they
 * do not feed the same parsing context (CRL vs cert parsing context).
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->aki_has_keyIdentifier,
	    ctx->aki_keyIdentifier_start,
	    ctx->aki_keyIdentifier_len,
	    ctx->aki_has_generalNames_and_serial,
	    ctx->aki_generalNames_start,
	    ctx->aki_generalNames_len,
	    ctx->aki_serial_start,
	    ctx->aki_serial_len,
	    ctx->has_aki;
  @*/
static int parse_crl_ext_AKI(crl_parsing_ctx *ctx,
			     const u8 *crl, u32 off, u32 len,
			     int critical)
{
	u32 hdr_len = 0, data_len = 0;
	const u8 *buf = crl + off;
	u32 key_id_hdr_len = 0, key_id_data_len = 0, key_id_data_off = 0;
	u32 gen_names_off = 0, gen_names_len = 0;
	u32 cert_serial_off = 0, cert_serial_len = 0;
	u32 remain;
	u32 parsed = 0;
	int ret, has_keyIdentifier = 0, has_gen_names_and_serial = 0;

	if ((crl == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 4.2.1.1. of RFC 5280 has "Conforming CAs MUST mark this
	 * extension as non-critical"
	 */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are indeed dealing w/ a sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * First, the KeyIdentifier if present (KeyIdentifier ::= OCTET STRING)
	 */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &key_id_hdr_len, &key_id_data_len);
	if (!ret) {
		/* An empty KeyIdentifier does not make any sense. Drop it! */
		if (!key_id_data_len) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		key_id_data_off = off + key_id_hdr_len;
		buf += key_id_hdr_len + key_id_data_len;
		off += key_id_hdr_len + key_id_data_len;
		remain -= key_id_hdr_len + key_id_data_len;
		has_keyIdentifier = 1;
	}


	/*
	 * See if a (GeneralNames, CertificateSerialNumber) couple follows.
	 * GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName. We do
	 * not accept one w/o the other.
	 */
	ret = parse_GeneralNames(buf, remain, CLASS_CONTEXT_SPECIFIC, 1,
				 &parsed);
	if (!ret) {
		gen_names_off = off;
		gen_names_len = parsed;

		buf += parsed;
		off += parsed;
		remain -= parsed;
		/* CertificateSerialNumber ::= INTEGER */
		ret = parse_AKICertSerialNumber(crl, off, remain,
						CLASS_CONTEXT_SPECIFIC, 2,
						&cert_serial_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
		/*@ assert cert_serial_len > 2; */

		has_gen_names_and_serial = 1;
		cert_serial_off = off;

		buf += cert_serial_len;
		off += cert_serial_len;
		remain -= cert_serial_len;
	}

	/* Nothing should remain behind */
	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Only populate context when we know everything is ok. */
	ctx->aki_has_keyIdentifier = has_keyIdentifier;
	/*@ assert \initialized(&ctx->aki_has_keyIdentifier); */
	if (ctx->aki_has_keyIdentifier) {
		ctx->aki_keyIdentifier_start = key_id_data_off;
		/*@ assert \initialized(&ctx->aki_keyIdentifier_start); */
		ctx->aki_keyIdentifier_len = key_id_data_len;
		/*@ assert \initialized(&ctx->aki_keyIdentifier_len); */
	}
	ctx->aki_has_generalNames_and_serial = has_gen_names_and_serial;
	/*@ assert \initialized(&ctx->aki_has_generalNames_and_serial); */
	if (ctx->aki_has_generalNames_and_serial) {
		ctx->aki_generalNames_start = gen_names_off;
		/*@ assert \initialized(&ctx->aki_generalNames_start); */
		ctx->aki_generalNames_len = gen_names_len;
		/*@ assert \initialized(&ctx->aki_generalNames_len); */
		ctx->aki_serial_start = cert_serial_off + 2;  /* 2 bytes long hdr for a valid SN */
		/*@ assert \initialized(&ctx->aki_serial_start); */
		ctx->aki_serial_len = cert_serial_len - 2;
		/*@ assert \initialized(&ctx->aki_serial_len); */
	}
	ctx->has_aki = 1;
	/*@ assert \initialized(&ctx->has_aki); */
	ret = 0;

out:
	return ret;
}

/*
 * section 5.2.2. of RFC 5280. The extension is basically the same as X.509
 * Certificate Issuer Alternative Name, as defined in section 4.2.1.7 of
 * RFC 5280. Code is duplicated here compared to the parsing function
 * defined in cert module, expecting different exports strategy to occur in
 * the future for both modules.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_IAN(crl_parsing_ctx *ctx,
			     const u8 *crl, u32 off, u32 len,
			     int ATTRIBUTE_UNUSED critical)
{
	u32 data_len = 0, hdr_len = 0, remain = 0, eaten = 0;
	const u8 *buf = crl + off;
	int ret, unused = 0;

	if ((crl == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Section 4.2.1.7 of RFC 5280 has "Where present, conforming CAs
	 * SHOULD mark this extension as non-critical."
	 *
	 * FIXME! add a check?
	 */

	/* Let's first check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * As specified in section 4.2.1.6. of RFC 5280, "if the subjectAltName
	 * extension is present, the sequence MUST contain at least one entry.
	 * Unlike the subject field, conforming CAs MUST NOT issue certificates
	 * with subjectAltNames containing empty GeneralName fields.
	 *
	 * The first check is done here.
	 *
	 * FIXME! second check remains to be done. Possibly in adding an
	 * additional out parameter to parse_GeneralName(), to tell if an empty
	 * one is empty.
	 */
	if (!data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@
	  @ loop assigns ret, buf, remain, eaten, unused, off;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop invariant (off + remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_GeneralName(buf, remain, &eaten, &unused);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= eaten;
		off += eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}


/*
 * CRL Number CRL extension as defined in section 5.2.3 of RFC 5280
 *
 *  CRLNumber ::= INTEGER (0..MAX)
 *
 */
#define MAX_CRL_EXT_NUM_LEN 22 /* w/ header */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_crlnumber, ctx->crlnumber_start, ctx->crlnumber_len;
  @*/
static int parse_crl_ext_CRLnum(crl_parsing_ctx *ctx,
				const u8 *crl, u32 off, u32 len,
				int critical)
{
	const u8 *buf = crl + off;
	u32 parsed = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((crl == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* RFC has: "The CRL number is a non-critical CRL extension" */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/* Verify the integer is DER-encoded as it should */
	ret = parse_integer(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
			    &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	parsed = hdr_len + data_len;
	/*@ assert parsed > 2; */

	/*
	 * We now have the guarantee the integer has the following format:
	 * [2 bytes for t/c and len][data_len bytes for encoded value]
	 */

	/*
	 * serialNumber value is expected to be at most 20 bytes long, which
	 * makes 22 bytes for the whole structure (if we include the associated
	 * two bytes header (a length of 20 is encoded on a single byte of
	 * header following the type/class byte.
	 */
	if (parsed > MAX_CRL_EXT_NUM_LEN) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* CRL number with a negative integer does not make sense */
	if (buf[2] & 0x80) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Export presence info, start and end of crl_num to context */
	ctx->has_crlnumber = 1;
	ctx->crlnumber_start = off + hdr_len;
	ctx->crlnumber_len = data_len;

	ret = 0;

out:
	return ret;
}

/*
 * section 5.2.4 of RFC 5280;
 *
 *   BaseCRLNumber ::= CRLNumber
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_DeltaCRL_indicator(crl_parsing_ctx *ctx,
					    const u8 *crl, u32 off, u32 len,
					    int critical)
{
	const u8 *buf = crl + off;
	u32 parsed = 0, hdr_len = 0, data_len = 0;
	int ret;

	if ((crl == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* RFC has: "The delta CRL indicator is a critical CRL extension" and
	 * "When a conforming CRL issuer generates a delta CRL, the delta CRL
	 * MUST include a critical delta CRL indicator extension."
	 */
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Verify the integer is DER-encoded as it should */
	ret = parse_integer(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
			    &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	parsed = hdr_len + data_len;
	/*@ assert parsed > 2; */

	/*
	 * We now have the guarantee the integer has the following format:
	 * [2 bytes for t/c and len][data_len bytes for encoded value]
	 */

	/*
	 * serialNumber value is expected to be at most 20 bytes long, which
	 * makes 22 bytes for the whole structure (if we include the associated
	 * two bytes header (a length of 20 is encoded on a single byte of
	 * header following the type/class byte.
	 */
	if (parsed > MAX_CRL_EXT_NUM_LEN) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* CRL number with a negative integer does not make sense */
	if (buf[2] & 0x80) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * XXX FIXME export start and end of crl_num to context and
	 * mark the fact that CRL has deltaindicator extension
	 */
	(void)ctx;

out:
	return ret;
}

/*
 * Some structure (e.g. CRL IDP extension) have context specific booleans.
 * We define a specific function for that purpose but avoid using
 * parse_id_len() becasue the expected encoding is simple enough:
 *
 *  FALSE : { 0x80 | tag, 0x01, 0x00 }
 *  TRUE  : { 0x80 | tag, 0x01, 0xff }
 *
 *  with tag on 6 bits
 *
 */
/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \separated(eaten, buf+(..));
  @ requires \valid(eaten);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten == 3);
  @ ensures (\result == 0) ==> (len >= 3);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @
  @ assigns *eaten;
  @*/
static int parse_context_specific_boolean(const u8 *buf, u32 len, u8 tag, u32 *eaten)
{
	u8 c, p, t;
	int ret;

	if ((buf == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (len < 3) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	c = (buf[0] >> 6) & 0x03; /* Extract class from 2 MSB */
	p = (buf[0] >> 5) & 0x01; /* Extract P/C bit */
	t = buf[0] & 0x1f;        /* Extract tag number from 6 LSB */

	if ((c != CLASS_CONTEXT_SPECIFIC) || (p != 0) || (t != tag)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (buf[1] != 0x01) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	switch (buf[2]) {
	case 0x00: /* FALSE */
	case 0xff: /* TRUE  */
		break;
	default:
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
		break;
	}

	*eaten = 3;

	ret = 0;

out:
	return ret;
}

/* Section 5.2.5 of RFC 5280 has:
 *
 *    IssuingDistributionPoint ::= SEQUENCE {
 *         distributionPoint          [0] DistributionPointName OPTIONAL,
 *         onlyContainsUserCerts      [1] BOOLEAN DEFAULT FALSE,
 *         onlyContainsCACerts        [2] BOOLEAN DEFAULT FALSE,
 *         onlySomeReasons            [3] ReasonFlags OPTIONAL,
 *         indirectCRL                [4] BOOLEAN DEFAULT FALSE,
 *         onlyContainsAttributeCerts [5] BOOLEAN DEFAULT FALSE }
 *
 *         -- at most one of onlyContainsUserCerts, onlyContainsCACerts,
 *         -- and onlyContainsAttributeCerts may be set to TRUE.
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_IDP(crl_parsing_ctx *ctx,
			     const u8 *crl, u32 off, u32 len,
			     int critical)
{
	u32 remain, hdr_len = 0, data_len = 0, eaten = 0;
	int has_dp = 0;
	int onlyContainsUserCerts;
	int onlyContainsCACerts;
	int indirectCRL;
	int onlyContainsAttributeCerts;
	const u8 *buf = crl + off;
	u8 dpn_type = 0;
	int ret;

	if ((crl == NULL) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC has "The issuing distribution point is a critical CRL extension"
	 */
#ifndef TEMPORARY_LAXIST_ALLOW_IDP_CRL_EXT_WITHOUT_CRITICAL_BIT_SET
	if (!critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#else
	(void)critical;
#endif

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= hdr_len;
	buf += hdr_len;

	/*
	 * RFC 5280 section 5.2.5 has: "Conforming CRLs issuers MUST NOT issue
	 * CRLs where the DER encoding of the issuing distribution point
	 * extension is an empty sequence.  That is, if onlyContainsUserCerts,
	 * onlyContainsCACerts, indirectCRL, and onlyContainsAttributeCerts
	 * are all FALSE, then either the distributionPoint field or the
	 * onlySomeReasons field MUST be present.
	 */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* We should have nothing behind */
	if (remain != data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Check if we have a (optional) distributionPoint field
	 * (of type DistributionPointName)
	 */
	ret = parse_id_len(buf, remain, CLASS_CONTEXT_SPECIFIC, 0,
			   &hdr_len, &data_len);
	if (!ret) {
		u32 dpn_remain = 0, dpn_eaten= 0;

		buf += hdr_len;
		remain -= hdr_len;
		dpn_remain = data_len;

		if (data_len == 0) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		dpn_type = buf[0];

		/*
		 * distributionPoint field of type DistributionPointName
		 * can be either a fullName or a nameRelativeToCRLIssuer.
		 */
		switch (dpn_type) {
		case 0xa0: /* fullName (i.e. a GeneralNames) */
			ret = parse_GeneralNames(buf, dpn_remain,
						 CLASS_CONTEXT_SPECIFIC, 0,
						 &dpn_eaten);
			if (ret) {
				ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
				goto out;
			}

			dpn_remain -= dpn_eaten;
			buf += dpn_eaten;
			break;

		case 0xa1: /* nameRelativeToCRLIssuer (RDN) */
			/*
			 * This form of distributionPoint is never used
			 * in practice in real X.509 certs, so not
			 * supported here. Note that RFC5280 has the
			 * following: "Conforming CAs SHOULD NOT use
			 * nameRelativeToCRLIssuer to specify distribution
			 * point names."
			 */
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
			break;

		default:
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
			break;
		}

		if (dpn_remain) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		/* Record the fact we found a DP */
		has_dp = 1;

		remain -= data_len;
	}

	/* All boolean in the structure default to FALSE */
	onlyContainsUserCerts = 0;
	onlyContainsCACerts = 0;
	indirectCRL = 0;
	onlyContainsAttributeCerts = 0;

	/*
	 * See if onlyContainsUserCerts is asserted
	 * onlyContainsUserCerts      [1] BOOLEAN DEFAULT FALSE,
	 */
	ret = parse_context_specific_boolean(buf, remain, 0x01, &eaten);
	if (!ret) {
		/*
		 * We got a boolean with valid tag. Because it defaults
		 * to FALSE, boolean should not be there at all if it
		 * has value false.
		 */
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		onlyContainsUserCerts = 1;

		remain -= eaten;
		buf += eaten;
	}

	/*
	 * See if onlyContainsUserCerts is asserted
	 * onlyContainsCACerts        [2] BOOLEAN DEFAULT FALSE,
	 */
	ret = parse_context_specific_boolean(buf, remain, 0x02, &eaten);
	if (!ret) {
		/*
		 * We got a boolean with valid tag. Because it defaults
		 * to FALSE, boolean should not be there at all if it
		 * has value false.
		 */
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		onlyContainsCACerts = 1;

		remain -= eaten;
		buf += eaten;
	}

	/*
	 * See if we have onlySomeReasons
	 * onlySomeReasons	      [3] ReasonFlags OPTIONAL,
	 */
	ret = parse_crldp_reasons(buf, remain, 0x03, &eaten);
	if (!ret) {
		buf += eaten;
		remain -= eaten;
	}

	/*
	 * See if onlyContainsUserCerts is asserted
	 * indirectCRL                [4] BOOLEAN DEFAULT FALSE,
	 */
	ret = parse_context_specific_boolean(buf, remain, 0x04, &eaten);
	if (!ret) {
		/*
		 * We got a boolean with valid tag. Because it defaults
		 * to FALSE, boolean should not be there at all if it
		 * has value false.
		 */
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		indirectCRL = 1;

		remain -= eaten;
		buf += eaten;
	}

	/*
	 * See if onlyContainsUserCerts is asserted
	 * onlyContainsAttributeCerts [5] BOOLEAN DEFAULT FALSE }
	 */
	ret = parse_context_specific_boolean(buf, remain, 0x05, &eaten);
	if (!ret) {
		/*
		 * We got a boolean with valid tag. Because it defaults
		 * to FALSE, boolean should not be there at all if it
		 * has value false.
		 */
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		onlyContainsAttributeCerts = 1;

		remain -= eaten;
		buf += eaten;
	}

	/*
	 * RFC 5280 section 5.2.5 has: "at most one of onlyContainsUserCerts,
	 * onlyContainsCACerts, and onlyContainsAttributeCerts may be set
	 * to TRUE.
	 */
	if ((onlyContainsUserCerts + onlyContainsCACerts + onlyContainsAttributeCerts) > 1) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

	/* XXX FIXME: export that to CRL context */
	(void)dpn_type;
	(void)has_dp;
	(void)indirectCRL;

out:
	return ret;
}

/*
 * CRLDP extension is expected in certificates but not in CRL. We implement
 * it to catch errors.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_CRLDP(crl_parsing_ctx *ctx,
			       const u8 *crl, u32 off, u32 len,
			       int critical)
{
	int ret;

	(void)ctx;
	(void)crl;
	(void)off;
	(void)len;
	(void)critical;

	ret = -X509_FILE_LINE_NUM_ERR;
	ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
	return ret;
}


/*
 * This non-critical ExpiredCertsOnCRL CRL extension is defined by ITU:
 *
 * ExpiredCertsOnCRL ::= GeneralizedTime
 *
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_expCertsOnCRL(crl_parsing_ctx *ctx,
				       const u8 *crl, u32 off, u32 len,
				       int critical)
{
	u8 month = 0, day = 0, hour = 0, min = 0, sec = 0;
	const u8 *buf = crl + off;
	u32 eaten;
	u16 year;
	int ret = -1;


	if ((crl == NULL) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_generalizedTime(buf, len, &eaten,
				    &year, &month, &day, &hour, &min, &sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}


/*
 * 5.2.6.  Freshest CRL (a.k.a. Delta CRL Distribution Point)
 *
 *     CRLDistributionPoints ::= SEQUENCE SIZE (1..MAX) OF DistributionPoint
 *
 * Note that the Freshest CRL extension uses the exact same syntax and
 * convention as CRLDP extension. The only minor difference is that section
 * 4.2.1.13 has that "The extension SHOULD be non-critical" and section
 * 4.2.1.15 has that "The extension MUST be marked as non-critical by
 * conforming CAs". This is the main difference between the function below
 * and parse_crl_ext_FreshestCRL().
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns ctx->has_crldp,
	    ctx->one_crldp_has_all_reasons;
  @*/
static int parse_crl_ext_FreshestCRL(crl_parsing_ctx *ctx,
				     const u8 *crl, u32 off, u32 len,
				     int critical)
{
	u32 hdr_len = 0, data_len = 0, remain;
	const u8 *buf = crl + off;
	int ret;

	if ((crl == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * RFC has: "Conforming CRL issuers MUST mark this extension as
	 * non-critical".
	 */
	if (critical) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	remain = data_len;

	if (len != (hdr_len + data_len)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->has_crldp = 1;
	ctx->one_crldp_has_all_reasons = 0;

	/* Iterate on DistributionPoint */
	/*@
	  @ loop assigns ret, buf, remain, ctx->one_crldp_has_all_reasons;
	  @ loop invariant \valid_read(buf + (0 .. (remain - 1)));
	  @ loop variant remain;
	  @ */
	while (remain) {
		int crldp_has_all_reasons = 0;
		u32 eaten = 0;

		ret = parse_DistributionPoint(buf, remain,
					      &crldp_has_all_reasons, &eaten);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		if (crldp_has_all_reasons) {
			ctx->one_crldp_has_all_reasons = 1;
		}

		remain -= eaten;
		buf += eaten;
	}

	ret = 0;

out:
	return ret;
}

/* Section 5.2.7 of RFC RFC 5280 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (critical != 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_AIA(crl_parsing_ctx ATTRIBUTE_UNUSED *ctx,
			     const u8 *crl, u32 off, u32 len, int critical)
{
	int ret;

	if (ctx == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_AIA(crl, off, len, critical);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

/* This Microsoft CRL extension contains a Time entry */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_szOID_CRL_NEXT_PUBLISH(crl_parsing_ctx *ctx,
				     const u8 *crl, u32 off, u32 len,
				     int ATTRIBUTE_UNUSED critical)
{
	u8 t_month = 0, t_day = 0, t_hour = 0, t_min = 0, t_sec = 0;
	const u8 *buf = crl + off;
	u32 t_len = 0;
	u16 t_year = 0;
	u8 t_type = 0;
	int ret = -1;

	if ((ctx == NULL) || (crl == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_Time(buf, len, &t_type, &t_len, &t_year, &t_month,
			 &t_day, &t_hour, &t_min, &t_sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (t_len != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

out:
	return ret;
}

/*
 * https://learn.microsoft.com/en-us/windows/win32/seccrypto/certification-authority-renewal
 *
 * We expect an integer value containing a DWORD
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_szOID_CERTSRV_CA_VERSION(crl_parsing_ctx *ctx,
						  const u8 *crl, u32 off, u32 len,
						  int ATTRIBUTE_UNUSED critical)
{
	u32 parsed = 0, hdr_len = 0, data_len = 0;
	const u8 *buf = crl + off;
	int ret = -1;

	if ((ctx == NULL) || (crl == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = parse_non_negative_integer(buf, len,
					 CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
					 &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* We expected the integer to fit in a DWORD */
	if (data_len > 4) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	parsed = hdr_len + data_len;
	if (parsed != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	(void)critical;

out:
	return ret;
}

/*
 * XXX FIXME, move to header and disable. When defined the extension is
 * accepted without being parsed and validated. If not defined, an error
 * is returned when the extenion is encoutered.
 */
#define TEMPORARY_LAXIST_CRL_ALLOW_UNPARSED_MS_CRL_SELF_CDP

/*
 * XXX FIXME we currently do not have the definition for this MS extension. It
 * looks a bit more complicated than just a sequence of DP as in a common
 * CRLDP extension.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ assigns \nothing;
  @*/
static int parse_crl_ext_szOID_CRL_SELF_CDP(crl_parsing_ctx *ctx,
				     const u8 *crl, u32 off, u32 len,
				     int critical)
{
	(void)ctx;
	(void)crl;
	(void)off;
	(void)len;
	(void)critical;

#ifdef  TEMPORARY_LAXIST_CRL_ALLOW_UNPARSED_MS_CRL_SELF_CDP
	return 0;
#else
	return -1;
#endif
}


/* From RFC 5280 */

static const u8 _crl_ext_oid_AKI[] =           { 0x06, 0x03, 0x55, 0x1d, 0x23 };
static const u8 _crl_ext_oid_IAN[] =           { 0x06, 0x03, 0x55, 0x1d, 0x12 };
static const u8 _crl_ext_oid_CRLnum[] =        { 0x06, 0x03, 0x55, 0x1d, 0x14 };
static const u8 _crl_ext_oid_DeltaCRL[] =      { 0x06, 0x03, 0x55, 0x1d, 0x1b };
static const u8 _crl_ext_oid_IDP[] =           { 0x06, 0x03, 0x55, 0x1d, 0x1c };
static const u8 _crl_ext_oid_CRLDP[] =         { 0x06, 0x03, 0x55, 0x1d, 0x1f };
static const u8 _crl_ext_oid_FreshestCRL[] =   { 0x06, 0x03, 0x55, 0x1d, 0x2e };
static const u8 _crl_ext_oid_AIA[] =           { 0x06, 0x08, 0x2b, 0x06, 0x01,
						 0x05, 0x05, 0x07, 0x01, 0x01 };

/* From ITU */
static const u8 _crl_ext_oid_expCertsOnCRL[] = { 0x06, 0x03, 0x55, 0x1d, 0x3c };

/* From Microsoft */
static const u8 _crl_ext_oid_szOID_CRL_NEXT_PUBLISH[] = {
	0x06, 0x09, 0x2b, 0x06,	0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x04 };
static const u8 _crl_ext_oid_szOID_CERTSRV_CA_VERSION[] = {
	0x06, 0x09, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x01 };
static const u8 _crl_ext_oid_szOID_CRL_SELF_CDP[] = {
	0x06, 0x09, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x0e };



typedef struct {
	const u8 *oid;
	u8 oid_len;
	int (*parse_crl_ext_params)(crl_parsing_ctx *ctx,
				    const u8 *crl, u32 off, u32 len, int critical);
} _crl_ext_oid;

static const _crl_ext_oid known_crl_ext_oids[] = {
	{ .oid = _crl_ext_oid_AKI, /* Authority Key Identifier */
	  .oid_len = sizeof(_crl_ext_oid_AKI),
	  .parse_crl_ext_params = parse_crl_ext_AKI,
	},
	{ .oid = _crl_ext_oid_IAN, /* Issuer Alternative Name */
	  .oid_len = sizeof(_crl_ext_oid_IAN),
	  .parse_crl_ext_params = parse_crl_ext_IAN,
	},
	{ .oid = _crl_ext_oid_CRLnum, /* CRL Number */
	  .oid_len = sizeof(_crl_ext_oid_CRLnum),
	  .parse_crl_ext_params = parse_crl_ext_CRLnum,
	},
	{ .oid = _crl_ext_oid_DeltaCRL, /* Delta CRL Indicator */
	  .oid_len = sizeof(_crl_ext_oid_DeltaCRL),
	  .parse_crl_ext_params = parse_crl_ext_DeltaCRL_indicator,
	},
	{ .oid = _crl_ext_oid_IDP, /* Issuing Distribution Point */
	  .oid_len = sizeof(_crl_ext_oid_IDP),
	  .parse_crl_ext_params = parse_crl_ext_IDP,
	},
	{ .oid = _crl_ext_oid_CRLDP, /* CRLDP - not expected!!!! */
	  .oid_len = sizeof(_crl_ext_oid_CRLDP),
	  .parse_crl_ext_params = parse_crl_ext_CRLDP,
	},
	{ .oid = _crl_ext_oid_FreshestCRL, /* FreshestCRL */
	  .oid_len = sizeof(_crl_ext_oid_FreshestCRL),
	  .parse_crl_ext_params = parse_crl_ext_FreshestCRL,
	},
	{ .oid = _crl_ext_oid_AIA, /* Authority Information Access */
	  .oid_len = sizeof(_crl_ext_oid_AIA),
	  .parse_crl_ext_params = parse_crl_ext_AIA,
	},
	{ .oid = _crl_ext_oid_expCertsOnCRL, /* expired Certs On CRL */
	  .oid_len = sizeof(_crl_ext_oid_expCertsOnCRL),
	  .parse_crl_ext_params = parse_crl_ext_expCertsOnCRL,
	},
	{ .oid = _crl_ext_oid_szOID_CRL_NEXT_PUBLISH, /* szOID_CRL_NEXT_PUBLISH */
	  .oid_len = sizeof(_crl_ext_oid_szOID_CRL_NEXT_PUBLISH),
	  .parse_crl_ext_params = parse_crl_ext_szOID_CRL_NEXT_PUBLISH,
	},
	{ .oid = _crl_ext_oid_szOID_CERTSRV_CA_VERSION, /* szOID_CERTSRV_CA_VERSION */
	  .oid_len = sizeof(_crl_ext_oid_szOID_CERTSRV_CA_VERSION),
	  .parse_crl_ext_params = parse_crl_ext_szOID_CERTSRV_CA_VERSION,
	},
	{ .oid = _crl_ext_oid_szOID_CRL_SELF_CDP, /* szOID_CRL_SELF_CDP */
	  .oid_len = sizeof(_crl_ext_oid_szOID_CRL_SELF_CDP),
	  .parse_crl_ext_params = parse_crl_ext_szOID_CRL_SELF_CDP,
	},
};

#define NUM_KNOWN_CRL_EXT_OIDS (sizeof(known_crl_ext_oids) /       \
				sizeof(known_crl_ext_oids[0]))
#define MAX_EXT_NUM_PER_CRL NUM_KNOWN_CRL_EXT_OIDS


/*@
  @ requires ((len > 0) && (buf != NULL)) ==> \valid_read(buf + (0 .. (len - 1)));
  @
  @ ensures (\result != NULL) ==> \exists integer i ; 0 <= i < NUM_KNOWN_CRL_EXT_OIDS && \result == &known_crl_ext_oids[i];
  @ ensures (len == 0) ==> \result == NULL;
  @ ensures (buf == NULL) ==> \result == NULL;
  @
  @ assigns \nothing;
  @*/
static _crl_ext_oid const * find_crl_ext_by_oid(const u8 *buf, u32 len)
{
	const _crl_ext_oid *found = NULL;
	const _crl_ext_oid *cur = NULL;
	u16 k;

	if ((buf == NULL) || (len == 0)) {
		goto out;
	}

	/*@ loop unroll NUM_KNOWN_CRL_EXT_OIDS ;
	  @ loop invariant 0 <= k <= NUM_KNOWN_CRL_EXT_OIDS;
	  @ loop invariant found == NULL;
	  @ loop assigns cur, found, k;
	  @ loop variant (NUM_KNOWN_CRL_EXT_OIDS - k);
	  @*/
	for (k = 0; k < NUM_KNOWN_CRL_EXT_OIDS; k++) {
		int ret;

		cur = &known_crl_ext_oids[k];

		/*@ assert cur == &known_crl_ext_oids[k];*/
		if (cur->oid_len != len) {
			continue;
		}

		/*@ assert \valid_read(buf + (0 .. (len - 1))); @*/
		ret = !bufs_differ(cur->oid, buf, cur->oid_len);
		if (ret) {
			found = cur;
			break;
		}
	}

out:
	return found;
}

/*@
  @ requires ext != \null;
  @ requires \valid(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1)));
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1)));
  @ requires \separated(ext, parsed_oid_list);
  @
  @ ensures \result <= 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL - 1)];
  @*/
static int check_record_crl_ext_unknown(const _crl_ext_oid *ext,
					const _crl_ext_oid **parsed_oid_list)
{
	u16 pos = 0;
	int ret;

	/*@
	  @ loop invariant pos <= MAX_EXT_NUM_PER_CRL;
	  @ loop assigns ret, pos, parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL - 1)];
	  @ loop variant MAX_EXT_NUM_PER_CRL - pos;
	  @*/
	while (pos < MAX_EXT_NUM_PER_CRL) {
		/*
		 * Check if we are at the end of already seen extensions. In
		 * that case, record the extension as a new one. This also
		 * means this is the first time we see this extension.
		 */
		if (parsed_oid_list[pos] == NULL) {
			parsed_oid_list[pos] = ext;
			break;
		}

		/* Check if already seen */
		if (ext == parsed_oid_list[pos]) {
			ret = -X509_FILE_LINE_NUM_ERR;
			goto out;
		}

		pos += 1;
	}

	/*
	 * If we went to the end of our array, this means there are too many
	 * extensions in the certificate.
	 */
	if (pos >= MAX_EXT_NUM_PER_CRL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}


/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (parsed_oid_list != NULL) ==> \valid(parsed_oid_list);
  @ requires \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1)));
  @ requires \separated(ctx, crl+(..),parsed_oid_list,eaten);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns parsed_oid_list[0 .. (MAX_EXT_NUM_PER_CRL - 1)], *eaten, *ctx;
  @*/
static int parse_x509_crl_Extension(crl_parsing_ctx *ctx,
				    const u8 *crl, u32 off, u32 len,
				    const _crl_ext_oid **parsed_oid_list,
				    u32 *eaten)
{
	u32 ext_hdr_len = 0, ext_data_len = 0;
	u32 hdr_len = 0, data_len = 0;
	u32 saved_ext_len = 0, oid_len = 0;
	u32 remain, parsed = 0;
	const u8 *buf = crl + off;
	const _crl_ext_oid *ext = NULL;
	int critical = 0;
	int ret;

	if ((crl == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL) ||
	    (parsed_oid_list == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1))); */

	remain = len;

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, remain,
			   CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &ext_hdr_len, &ext_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1))); */

	/*
	 * Let's parse the OID and then check if we have
	 * an associated handler for that extension.
	 */
	ret = parse_OID(buf, ext_data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1))); */

	ext = find_crl_ext_by_oid(buf, oid_len);
	if (ext == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(parsed_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1))); */

	/*
	 * Now that we know the OID is one we support, we verify
	 * this is the first time we handle an instance of this
	 * type. Having multiple instances of a given extension
	 * in a certificate is forbidden by both section 4.2 of
	 * RFC5280 and section 8 of X.509, w/ respectively
	 *
	 * - "A certificate MUST NOT include more than one
	 *    instance of a particular extension."
	 * - "For all certificate extensions, CRL extensions,
	 *    and CRL entry extensions defined in this Directory
	 *    Specification, there shall be no more than one
	 *    instance of each extension type in any certificate,
	 *    CRL, or CRL entry, respectively."
	 *
	 * This is done by recording for each extension we
	 * processed the pointer to its vtable and compare
	 * it with current one.
	 */
	ret = check_record_crl_ext_unknown(ext, parsed_oid_list);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;

	/*
	 * Now that we got the OID, let's check critical
	 * field value if present. It's a boolean
	 * defaulting to FALSE (in which case, it is absent).
	 * We could parse it as an integer but that
	 * would be a lot of work for three simple bytes.
	 */
	ret = parse_boolean(buf, ext_data_len, &parsed);
	if (ret) {
		/*
		 * parse_boolean() returned an error which means this
		 * was either a boolean with invalid content or
		 * something else. If this was indeed a boolean, we can
		 * just leave. Otherwise, this just measn the critical
		 * flag was missing and the default value (false) applies,
		 * in which case, we can continue parsing.
		 */
		if (ext_data_len && (buf[0] == ASN1_TYPE_BOOLEAN)) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
	} else {
		/*
		 * We now know it's a valid BOOLEAN, *but* in our
		 * case (DER), the DEFAULT FALSE means we cannot
		 * accept an encoded value of FALSE. Note that we
		 * sanity check the value we expect for the length
		 */
		if (parsed != 3) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

#ifndef TEMPORARY_LAXIST_EXTENSION_CRITICAL_FLAG_BOOLEAN_EXPLICIT_FALSE
		if (buf[2] == 0x00) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}
#endif

		/*
		 * We now know the BOOLEAN is present and has
		 * a value of TRUE. Record that.
		 */
		critical = (buf[2] == 0) ? 0 : 1;

		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}

	/*
	 * We should now be in front of the octet string
	 * containing the extnValue.
	 */
	ret = parse_id_len(buf, ext_data_len,
			   CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;

	/* Check nothing remains behind the extnValue */
	if (data_len != ext_data_len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ((u64)off + (u64)ext_data_len) <= MAX_UINT32; */
	/*@ assert \valid_read(crl + (off .. (off + ext_data_len - 1))); */

	/* Parse the parameters for that extension */
	/*@ assert ext->parse_crl_ext_params \in {
	  parse_crl_ext_AKI,
	  parse_crl_ext_IAN,
	  parse_crl_ext_CRLnum,
	  parse_crl_ext_DeltaCRL_indicator,
	  parse_crl_ext_IDP,
	  parse_crl_ext_CRLDP,
	  parse_crl_ext_FreshestCRL,
	  parse_crl_ext_AIA,
	  parse_crl_ext_expCertsOnCRL,
	  parse_crl_ext_szOID_CRL_NEXT_PUBLISH,
	  parse_crl_ext_szOID_CERTSRV_CA_VERSION,
	  parse_crl_ext_szOID_CRL_SELF_CDP }; @*/
	/*@ calls parse_crl_ext_AKI,
	  parse_crl_ext_IAN,
	  parse_crl_ext_CRLnum,
	  parse_crl_ext_DeltaCRL_indicator,
	  parse_crl_ext_IDP,
	  parse_crl_ext_CRLDP,
	  parse_crl_ext_FreshestCRL,
	  parse_crl_ext_AIA,
	  parse_crl_ext_expCertsOnCRL,
	  parse_crl_ext_szOID_CRL_NEXT_PUBLISH,
	  parse_crl_ext_szOID_CERTSRV_CA_VERSION,
	  parse_crl_ext_szOID_CRL_SELF_CDP ; @*/
	ret = ext->parse_crl_ext_params(ctx, crl, off, ext_data_len, critical);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*eaten = saved_ext_len;
	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(eaten, ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (1 <= *eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_crl_Extensions(crl_parsing_ctx *ctx,
				     const u8 *crl, u32 off, u32 len,
				     u32 *eaten)
{
	u32 data_len = 0, hdr_len = 0, remain = 0;
	const u8 *buf = crl + off;
	u32 saved_len = 0;
	const _crl_ext_oid *parsed_crl_ext_oid_list[MAX_EXT_NUM_PER_CRL];

	int ret;
	u16 i;

	if ((crl == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Extensions in X.509 CRL is an EXPLICITLY tagged sequence.
	 */
	ret = parse_explicit_id_len(buf, len, 0,
				    CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
				    &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	/*@ assert \valid_read(buf + (0 .. (remain - 1))); */

	saved_len = hdr_len + data_len;
	/*@ assert saved_len <= len; */
	/*@ assert data_len <= saved_len; */

	/*
	 * If present, it must contain at least one extension, as specified in
	 * section 5.1.2.7 of RFC 5280: "If present, this field is a sequence
	 * of one or more CRL extensions.
	 */
	if (data_len == 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Initialize list of already seen extensions */
	/*@ loop unroll MAX_EXT_NUM_PER_CRL;
	  @ loop assigns i, parsed_crl_ext_oid_list[0 .. (MAX_EXT_NUM_PER_CRL - 1)];
	  @ loop invariant (i < MAX_EXT_NUM_PER_CRL) ==> \valid(&parsed_crl_ext_oid_list[i]);
	  @ loop variant (MAX_EXT_NUM_PER_CRL - i);
	  @*/
	for (i = 0; i < MAX_EXT_NUM_PER_CRL; i++) {
		parsed_crl_ext_oid_list[i] = NULL;
	}
	/*@ assert \initialized(parsed_crl_ext_oid_list + (0 .. (MAX_EXT_NUM_PER_CRL - 1))); */

	/* Now, let's work on each extension in the sequence */
	/*@
	  @ loop assigns off, ret, buf, remain, parsed_crl_ext_oid_list[0 .. (MAX_EXT_NUM_PER_CRL - 1)], *ctx;
	  @ loop invariant (remain != 0) ==> \valid_read(crl + (off .. (off + remain - 1)));
	  @ loop invariant (remain != 0) ==> ((u64)off + (u64)remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @*/
	while (remain) {
		u32 ext_len = 0;

		ret = parse_x509_crl_Extension(ctx, crl, off, remain,
					       parsed_crl_ext_oid_list,
					       &ext_len);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}

	/*
	 * RFC 5280 has "When CRLs are issued, the CRLs MUST be version 2 CRLs,
	 * include the date by which the next CRL will be issued in the
	 * nextUpdate field (Section 5.1.2.5), include the CRL number extension
	 * (Section 5.2.3), and include the authority key identifier extension
	 * (Section 5.2.1)." If the CRL misses the AKI and CRL number extensions
	 * then it is invalid ;-)
	 */
#ifndef TEMPORARY_LAXIST_ALLOW_MISSING_AKI_OR_CRLNUM
	if (!(ctx->has_crlnumber && ctx->has_aki)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#endif

	/*@ assert 1 <= saved_len <= len; */
	*eaten = saved_len;

	ret = 0;

out:
	return ret;
}


/*****************************************************************************
 * Code for parsing tbsCertList
 *****************************************************************************/

/*
 * Unlike Certificate Version field which is EXPLICIT, CRL one is not. It is
 * a simple integer.
 *
 *       version                 Version OPTIONAL,
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (cert != \null)) ==> \valid_read(cert + (off .. (off + len - 1)));
  @ requires \valid(eaten);
  @ requires \valid(version);
  @ requires \separated(eaten, cert+(..), version);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (cert == \null) ==> \result < 0;
  @ ensures (version == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \initialized(version);
  @
  @ assigns *eaten, *version;
  @*/
static int parse_x509_crl_Version(const u8 *cert, u32 off, u32 len, u8 *version, u32 *eaten)
{
	const u8 *buf = cert + off;
	u32 data_len = 0;
	u32 hdr_len = 0;
	int ret;

	if ((cert == NULL) || (version == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */
	ret = parse_integer(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_INTEGER,
			    &hdr_len, &data_len);
	if (ret) {
		ret = X509_PARSER_ERROR_VERSION_ABSENT;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += hdr_len;

	/* Integer value is less than 3: data_len must be 1. */
	if (data_len != 1) {
		ret = X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*version = buf[0];
	*eaten = hdr_len + data_len;

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires (alg != NULL) ==> \valid(alg);
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires \separated(crl+(..),alg,ctx,eaten);
  @
  @ ensures \result < 0 || \result == 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> \valid_read(*alg);
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_oid_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_oid_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->sig_alg));
  @ ensures (\result == 0) ==> \initialized(&(ctx->hash_alg));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_len));
  @ ensures (\result == 0) ==> ctx->tbs_sig_alg_start == off;
  @ ensures (\result == 0) ==> \exists integer x; 0 <= x < num_known_sig_algs && *alg == known_sig_algs[x];
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @
  @ assigns *alg, *eaten, *ctx;
  @*/
static int parse_x509_tbsCertList_sig_AlgorithmIdentifier(crl_parsing_ctx *ctx,
							  const u8 *crl, u32 off, u32 len,
							  const _sig_alg **alg,
							  u32 *eaten)
{
	const _sig_alg *talg = NULL;
	const u8 *buf = crl + off;
	u32 saved_off = off;
	u32 parsed = 0;
	u32 hdr_len = 0;
	u32 data_len = 0;
	u32 param_len;
	u32 oid_len = 0;
	int ret;

	if ((ctx == NULL) || (crl == NULL) || (len == 0)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check we are dealing with a valid sequence */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;}

	parsed = hdr_len + data_len;
	/*@ assert (1 < parsed <= len); */

	buf += hdr_len;
	off += hdr_len;

	/* The first thing we should find in the sequence is an OID */
	ret = parse_OID(buf, data_len, &oid_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Let's now see if that OID is one associated w/ an alg we support */
	talg = find_sig_alg_by_oid(buf, oid_len);
	if (talg == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(talg); */
	/*@ assert \exists integer i ; 0 <= i < num_known_sig_algs && talg == known_sig_algs[i]; */
	/*
	 * Record position of spki alg oid. Also keep track of
	 * the signature mechanism alg identifier and possibly
	 * the associated hash identifier if it was explicit in
	 * the mechanism. Otherwise, we will temporarily inherit
	 * from HASH_ALG_UNKNOWN and possibly get the hash during
	 * the parsing of alg oid sig parameters just below.
	 */
	ctx->tbs_sig_alg_oid_start = off;
	ctx->tbs_sig_alg_oid_len = oid_len;
	ctx->sig_alg = talg->sig_id;
	ctx->hash_alg = talg->hash_id;

	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;

	/*@ assert talg->parse_algoid_sig_params \in {
		  parse_algoid_sig_params_ecdsa_with,
		  parse_algoid_sig_params_ecdsa_with_specified,
		  parse_algoid_sig_params_sm2,
		  parse_algoid_sig_params_eddsa,
		  parse_algoid_sig_params_rsa,
		  parse_algoid_sig_params_rsassa_pss,
		  parse_algoid_sig_params_none,
		  parse_algoid_sig_params_bign_with_hspec }; @*/
	/*@ calls parse_algoid_sig_params_ecdsa_with,
		  parse_algoid_sig_params_ecdsa_with_specified,
		  parse_algoid_sig_params_sm2,
		  parse_algoid_sig_params_eddsa,
		  parse_algoid_sig_params_rsa,
		  parse_algoid_sig_params_rsassa_pss,
		  parse_algoid_sig_params_none,
		  parse_algoid_sig_params_bign_with_hspec; @*/
	ret = talg->parse_algoid_sig_params(&ctx->sig_alg_params,
					    &ctx->hash_alg, crl, off, param_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \exists integer i ; 0 <= i < num_known_sig_algs && talg == known_sig_algs[i]; */
	*alg = talg;
	*eaten = parsed;
	ctx->tbs_sig_alg_start = saved_off;
	ctx->tbs_sig_alg_len = parsed;

	ret = 0;

out:
	return ret;
}

/*
 * Revoked Certificates as defined in section 5.1.2.6 of RFC 5280.
 * The field is optional, i.e. "When there are no revoked certificates,
 * the revoked certificates list MUST be absent.".
 *
 * If one wonders how the disambiguation between absent revokedCertificates
 * and absent extension is performed: the first is a usual sequence, the
 * second is explicitly tagged.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(eaten);
  @ requires \valid(ctx);
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_crl_revokedCertificates(crl_parsing_ctx *ctx,
					      const u8 *crl, u32 off, u32 len,
					      u32 *eaten)
{
	u32 remain = 0, hdr_len = 0, data_len = 0, parsed = 0, cur_off = 0;
	const u8 *buf = crl + off;
	int ret;

	if ((ctx == NULL) || (crl == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */
	/*@ assert \valid_read(crl + (off .. (off + len - 1))); */

	/*
	 * Let's first check we are dealing with a valid sequence. Note that
	 * the field is OPTIONAL, i.e. it must be absent if sequence is empty:
	 * "When there are no revoked certificates, the revoked certificates
	 * list MUST be absent". For that reason, not finding a sequence here
	 * is ok.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ctx->has_revoked_certs = 0;
		*eaten = 0;
		ret = 0;
		goto out;
	}

	remain = data_len;
	cur_off = off + hdr_len;

	/*@ assert ((u64)cur_off + (u64)remain) <= MAX_UINT32; */
	/*@ assert \valid_read(crl + (cur_off .. (cur_off + remain - 1))); */


#ifndef TEMPORARY_LAXIST_ALLOW_REVOKED_CERTS_LIST_EMPTY
	if (data_len == 0) {
		/*
		 * We do not accept sequences with no data, because
		 * the whole sequence is optional, i.e. it must be
		 * absent when empty.
		 */
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
#endif

	/*@
	  @ loop assigns ret, parsed, *ctx, remain, cur_off;
	  @ loop invariant (remain != 0) ==>
		 \valid_read(crl + (cur_off .. (cur_off + remain - 1)));
	  @ loop invariant (remain != 0) ==>
		 ((u64)cur_off + (u64)remain) <= MAX_UINT32;
	  @ loop variant remain;
	  @ */
	while (remain) {
		ret = parse_x509_crl_revokedCertificate(ctx, crl, cur_off,
							remain,
							&parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= parsed;
		cur_off += parsed;
	}

	ctx->has_revoked_certs = data_len != 0;
	*eaten = data_len + hdr_len;
	ret = 0;

out:
	return ret;
}

/*
 *
 *    TBSCertList  ::=  SEQUENCE  {
 *         version                 Version OPTIONAL,
 *                                      -- if present, MUST be v2
 *         signature               AlgorithmIdentifier,
 *         issuer                  Name,
 *         thisUpdate              Time,
 *         nextUpdate              Time OPTIONAL,
 *         revokedCertificates     SEQUENCE OF SEQUENCE  {
 *              userCertificate         CertificateSerialNumber,
 *              revocationDate          Time,
 *              crlEntryExtensions      Extensions OPTIONAL
 *                                       -- if present, version MUST be v2
 *                                   }  OPTIONAL,
 *         crlExtensions           [0]  EXPLICIT Extensions OPTIONAL
 *                                       -- if present, version MUST be v2
 *                                   }
 *
 * On success, the function returns the size of the TBSCertList
 * structure in 'eaten' parameter. It also provides in 'sig_alg'
 * a pointer to the signature algorithm found in the signature field.
 * This one is provided to be able to check later against the signature
 * algorithm found in the signatureAlgorithm field of the certificate.
 */
/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires \valid(eaten);
  @ requires \valid(ctx);
  @ requires \separated(sig_alg, eaten, crl+(..), ctx);
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (sig_alg == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (1 < *eaten <= len);
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_start));
  @ ensures (\result == 0) ==> \initialized(&(ctx->tbs_sig_alg_len));
  @ ensures (\result == 0) ==> \initialized(&(ctx->sig_alg));
  @ ensures (\result == 0) ==> ctx->tbs_sig_alg_start < off + *eaten;
  @
  @ assigns *eaten, *ctx, *sig_alg;
  @*/
static int parse_x509_TBSCertList(crl_parsing_ctx *ctx,
			     const u8 *crl, u32 off, u32 len,
			     const _sig_alg **sig_alg, u32 *eaten)
{
	u32 tbs_data_len = 0;
	u32 tbs_hdr_len = 0;
	u32 tbs_crl_len = 0;
	u32 remain = 0;
	u32 parsed = 0;
	u32 cur_off = off;
	const u8 *buf = crl + cur_off;
	const _sig_alg *alg = NULL;
	int ret, empty_issuer = 1;
	u32 tu_len = 0, nu_len = 0;
	u16 nu_year = 0, tu_year = 0;
	u8 nu_month = 0, nu_day = 0, nu_hour = 0, nu_min = 0, nu_sec = 0;
	u8 tu_month = 0, tu_day = 0, tu_hour = 0, tu_min = 0, tu_sec = 0;
	u64 thisUpdate, nextUpdate;
	u8 t_type = 0;

	if ((ctx == NULL) || (crl == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \valid_read(buf + (0 .. len - 1)); */

	/*
	 * Let's first check we are dealing with a valid sequence containing
	 * all the elements of the TBSCertList.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &tbs_hdr_len, &tbs_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	tbs_crl_len = tbs_hdr_len + tbs_data_len;
	buf += tbs_hdr_len;
	cur_off += tbs_hdr_len;
	remain = tbs_data_len;

	/*@ assert \valid_read(buf + (0 .. remain - 1)); */

	/*
	 * Now, we can start and parse all the elements in the sequence
	 * one by one.
	 */

	/*
	 * version: field is optional in which case it is 0x00, i.e. v1.
	 * if present it must be 0x01, i.e. v2. Unlike Version in
	 * certificate, the version field in the CRL is not 'explicit'.
	 * A simle integer is expected.
	 */
	ret = parse_x509_crl_Version(crl, cur_off, remain,
				     &ctx->version, &parsed);
	if (ret) {
		/* missing, use v1 */
		ctx->version = 0x00;
	} else {
		/* present, check this is v2 */
		if (ctx->version != 0x01) {
			ret = -X509_FILE_LINE_NUM_ERR;
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}

	/* signature */
	ret = parse_x509_tbsCertList_sig_AlgorithmIdentifier(ctx, crl, cur_off,
							     remain, &alg,
							     &parsed);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	/*@ assert \initialized(&(ctx->tbs_sig_alg_start)); */
	/*@ assert ctx->tbs_sig_alg_start == cur_off; */
	/*@ assert ctx->tbs_sig_alg_start < off + tbs_crl_len; */
	/*@ assert \initialized(&(ctx->tbs_sig_alg_len)); */
	/*@ assert \initialized(&(ctx->sig_alg)); */

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* issuer */
	ret = parse_x509_Name(buf, remain, &parsed, &empty_issuer);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	ctx->issuer_start = cur_off;
	ctx->issuer_len = parsed;

	/*
	 * As described in section 5.1.2.3 of RFC 5280, "The issuer field MUST
	 * contain a non-empty X.500 distinguished name (DN)".
	 */
	/*@ assert (empty_issuer == 0) || (empty_issuer == 1); */
	if (empty_issuer) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	buf += parsed;
	cur_off += parsed;
	remain -= parsed;

	/* thisUpdate */
	ret = parse_Time(buf, remain, &t_type, &tu_len, &tu_year, &tu_month,
			 &tu_day, &tu_hour, &tu_min, &tu_sec);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check valid time type was used for year value */
	ret = verify_correct_time_use(t_type, tu_year);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	remain -= tu_len;
	cur_off += tu_len;
	buf += tu_len;

	/*
	 * To export time to context we do not bother converting to unix
	 * but encode all the components of thisUpdate on an u64 in the
	 * following way. Same for nextUpdate. This makes resulting
	 * thisUpdate and nextUpdate values comparable.
	 */

	/*@ assert tu_year <= 9999; */
	/*@ assert tu_month <= 12; */
	/*@ assert tu_day <= 31; */
	/*@ assert tu_hour <= 23; */
	/*@ assert tu_min <= 59; */
	/*@ assert tu_sec <= 59; */
	thisUpdate = time_components_to_comparable_u64(tu_year, tu_month, tu_day,
							tu_hour, tu_min, tu_sec);

	/* nextUpdate */
	ret = parse_Time(buf, remain, &t_type, &nu_len, &nu_year, &nu_month,
			 &nu_day, &nu_hour, &nu_min, &nu_sec);
	if (!ret) {
		/* Check valid time type was used for year value */
		ret = verify_correct_time_use(t_type, nu_year);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		remain -= nu_len;
		cur_off += nu_len;
		buf += nu_len;
	} else {
#ifdef TEMPORARY_LAXIST_ALLOW_MISSING_CRL_NEXT_UPDATE
		/*
		 * In that case, we will define the nextUpdate components to
		 * the largest admisssible values, i.e. encode the "infinity",
		 * i.e. 23h59:59 31/12/9999
		 */
		nu_year = 9999;
		nu_month = 12;
		nu_day = 31;
		nu_hour = 23;
		nu_min = 59;
		nu_sec = 59;
#else
		/*
		 * nextUpdate field is optional in ASN.1 definition but RFC5280
		 * explicitly states both at end of section 5. introduction and
		 * in section 5.1.2 that conforming CRL issuers are required /
		 * MUST include nextUpdate field.
		 */
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
#endif
	}

	/*@ assert nu_year <= 9999; */
	/*@ assert nu_month <= 12; */
	/*@ assert nu_day <= 31; */
	/*@ assert nu_hour <= 23; */
	/*@ assert nu_min <= 59; */
	/*@ assert nu_sec <= 59; */
	nextUpdate = time_components_to_comparable_u64(nu_year, nu_month, nu_day,
						       nu_hour, nu_min, nu_sec);

	if (thisUpdate >= nextUpdate) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert (remain > 0) ==> \valid_read(crl + (cur_off .. (cur_off + remain - 1))); */
	/*
	 * We should now be in front of revokedCertificates sequence, if any.
	 * Note that the field is OPTIONAL, i.e. it must be absent
	 * if sequence is empty: "When there are no revoked certificates,
	 * the revoked certificates list MUST be absent".
	 */
	if (remain) {
		ret = parse_x509_crl_revokedCertificates(ctx, crl, cur_off, remain,
							 &parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		cur_off += parsed;
		remain -= parsed;
		/*@ assert (remain > 0) ==> \valid_read(crl + (cur_off .. (cur_off + remain - 1))); */
	}

	/* crlExtensions is only acceptable for v2 crl */
	if ((ctx->version != 0x01) && remain) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ((u64)cur_off + (u64)remain) <= MAX_UINT32; */
	/*@ assert (remain > 0) ==> \valid_read(crl + (cur_off .. (cur_off + remain - 1))); */

	/* Parse CRL extensions, if any */
	if (remain) {
		ret = parse_x509_crl_Extensions(ctx, crl, cur_off, remain,
						&parsed);
		if (ret) {
			ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
			goto out;
		}

		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_crl_len; */

	if (remain != 0) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert ctx->tbs_sig_alg_start < off + tbs_crl_len; */
	/*@ assert 1 < tbs_crl_len <= len; */
	*eaten = tbs_crl_len;
	/*@ assert ctx->tbs_sig_alg_start < off + *eaten; */
	*sig_alg = alg;

	ret = 0;

out:
	return ret;
}


/*****************************************************************************
 * Code for parsing signatureAlgorithm
 *****************************************************************************/

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (0 .. (off + len - 1)));
  @ requires (eaten != NULL) ==> \valid(eaten);
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \initialized(&(ctx->tbs_sig_alg_start));
  @ requires \initialized(&(ctx->tbs_sig_alg_len));
  @ requires ctx->tbs_sig_alg_start <= off;
  @ requires \separated(eaten, crl+(..), ctx);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten,
	    ctx->sig_alg_start,
	    ctx->sig_alg_len;
  @*/
static int parse_x509_crl_signatureAlgorithm(crl_parsing_ctx *ctx,
					    const u8 *crl, u32 off, u32 len,
					    u32 *eaten)
{
	u32 prev_len;
	int ret;

	if ((crl == NULL) || (len == 0) || (ctx == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	prev_len = ctx->tbs_sig_alg_len;
	if (prev_len > len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Section 5.1.1.2 of RFC 5280 has the following regarding
	 * signatureAlgorithm field: "This field MUST contain the same algorithm
	 * identifier as the signature field in the sequence tbsCertList
	 * (Section 5.1.2.2). This is what we verify here.
	 */
	ret = bufs_differ(crl + ctx->tbs_sig_alg_start, crl + off, prev_len);
	if (ret) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}
	ctx->sig_alg_start = off;
	ctx->sig_alg_len = prev_len;

	*eaten = prev_len;

	ret = 0;

out:
	return ret;
}


/*****************************************************************************
 * Code for parsing signatureValue
 *****************************************************************************/

/*@
  @ requires ((u64)off + (u64)len) <= MAX_UINT32;
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (off .. (off + len - 1)));
  @ requires (sig_alg != \null) ==> \valid_read(sig_alg) && \valid_function(sig_alg->parse_sig);
  @ requires (\initialized(&ctx->sig_alg));
  @ requires \valid(eaten);
  @ requires \valid(ctx);
  @ requires \separated(ctx, crl+(..), sig_alg, eaten);
  @
  @ ensures \result <= 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @ ensures (ctx == \null) ==> \result < 0;
  @ ensures (sig_alg == \null) ==> \result < 0;
  @ ensures (eaten == \null) ==> \result < 0;
  @
  @ assigns *eaten, *ctx;
  @*/
static int parse_x509_crl_signatureValue(crl_parsing_ctx *ctx,
					const u8 *crl, u32 off, u32 len,
					const _sig_alg *sig_alg, u32 *eaten)
{
	u32 saved_off = off;
	int ret;

	if ((ctx == NULL) || (crl == NULL) || (len == 0) ||
	    (sig_alg == NULL) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	if (sig_alg->parse_sig == NULL) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert sig_alg->parse_sig \in {
		  parse_sig_ed448,
		  parse_sig_ed25519,
		  parse_sig_ecdsa,
		  parse_sig_sm2,
		  parse_sig_dsa,
		  parse_sig_rsa_pkcs1_v15,
		  parse_sig_rsa_ssa_pss,
		  parse_sig_rsa_9796_2_pad,
		  parse_sig_rsa_belgian,
		  parse_sig_gost94,
		  parse_sig_gost2001,
		  parse_sig_gost2012_512,
		  parse_sig_gost2012_256,
		  parse_sig_bign,
		  parse_sig_monkey }; @*/
	/*@ calls parse_sig_ed448,
		  parse_sig_ed25519,
		  parse_sig_ecdsa,
		  parse_sig_sm2,
		  parse_sig_dsa,
		  parse_sig_rsa_pkcs1_v15,
		  parse_sig_rsa_ssa_pss,
		  parse_sig_rsa_9796_2_pad,
		  parse_sig_rsa_belgian,
		  parse_sig_gost94,
		  parse_sig_gost2001,
		  parse_sig_gost2012_512,
		  parse_sig_gost2012_256,
		  parse_sig_bign,
		  parse_sig_monkey; @*/
	ret = sig_alg->parse_sig(&(ctx->sig_alg_params), crl, off, len, eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ctx->sig_start = saved_off;
	ctx->sig_len = *eaten;
	ret = 0;

out:
	return ret;
}


/*****************************************************************************
 * parse_x509_crl()
 *****************************************************************************/

/*@
  @ assigns \nothing;
  @*/
static crl_parsing_ctx get_zeroized_crl_ctx_val(void)
{
	crl_parsing_ctx zeroized_ctx = { 0 };

	return zeroized_ctx;
}

/*
 * RFC 5280, section 5.1 has the following X509 v2 CRL syntax:
 *
 *    CertificateList  ::=  SEQUENCE  {
 *        tbsCertList          TBSCertList,
 *        signatureAlgorithm   AlgorithmIdentifier,
 *        signatureValue       BIT STRING  }
 *
 * DER encoding is used.
 */
/*@
  @ requires ((len > 0) && (crl != \null)) ==> \valid_read(crl + (0 .. (len - 1)));
  @ requires (ctx != NULL) ==> \valid(ctx);
  @ requires \separated(ctx, crl+(..));
  @
  @ ensures \result <= 0;
  @ ensures (len == 0) ==> \result < 0;
  @ ensures (ctx == 0) ==> \result < 0;
  @ ensures (crl == \null) ==> \result < 0;
  @
  @ assigns *ctx;
  @*/
int parse_x509_crl(crl_parsing_ctx *ctx, const u8 *crl, u32 len)
{
	u32 seq_data_len = 0;
	u32 eaten = 0;
	u32 off = 0;
	const _sig_alg *sig_alg = NULL;
	int ret;

	if ((crl == NULL) || (len == 0) || (ctx == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	*ctx = get_zeroized_crl_ctx_val();

	/*
	 * Parse beginning of buffer to verify it's a sequence and get
	 * the length of the data it contains.
	 */
	ret = parse_id_len(crl, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &eaten, &seq_data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	len -= eaten;
	off += eaten;
	/*@ assert ((u64)off + (u64)len) <= MAX_UINT32; */

	/*
	 * We do expect advertised length to match what now remains in buffer
	 * after the sequence header we just parsed.
	 */
	if (seq_data_len != len) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Parse first element of the sequence: tbsCertList */
	ret = parse_x509_TBSCertList(ctx, crl, off, len, &sig_alg, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * XXX as specified in 5.2.1, Conforming CRL issuers MUST use the key
	 * identifier method. This should be enforced here. The thing is that
	 * CRL AKI is an extension and will not be present in old CRL that do
	 * not supported extensions. See what should be done on that aspect.
	 */

	/*@ assert \initialized(&(ctx->sig_alg)); */
	/*@ assert ctx->tbs_sig_alg_start < off + eaten; */

	ctx->tbs_start = off;
	ctx->tbs_len = eaten;

	len -= eaten;
	off += eaten;

	/*@ assert ctx->tbs_sig_alg_start <= off; */

	/* Parse second element of the sequence: signatureAlgorithm */
	ret = parse_x509_crl_signatureAlgorithm(ctx, crl, off, len, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*@ assert \initialized(&(ctx->sig_alg)); */

	len -= eaten;
	off += eaten;

	/* Parse second element of the sequence: signatureValue */
	ret = parse_x509_crl_signatureValue(ctx, crl, off, len, sig_alg, &eaten);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Check there is nothing left behind */
	if (len != eaten) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}




/* ============== IMPL: x509-parser.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

/* removed: #include "x509-parser.h" */
/* removed: #include "x509-common.h" */

#define X509_FILE_NUM 4 /* See x509-utils.h for rationale */

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \valid(eaten);
  @ requires \separated(eaten, buf+(..), ctx);
  @
  @ ensures \result <= 1;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten > 0);
  @
  @ assigns *eaten, *ctx;
 */
int parse_x509_cert_relaxed(cert_parsing_ctx *ctx, const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0;
	int ret;

	if ((ctx == NULL) || (buf == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Parse beginning of buffer to verify it's a sequence and get
	 * the length of the data it contains.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ret = 1;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* Certificate has that exact length */
	*eaten = hdr_len + data_len;

	/* Parse it */
	ret = parse_x509_cert(ctx, buf, hdr_len + data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

/*@
  @ requires ((len > 0) && (buf != \null)) ==> \valid_read(buf + (0 .. (len - 1)));
  @ requires \valid(eaten);
  @ requires \separated(eaten, buf+(..), ctx);
  @
  @ ensures \result <= 1;
  @ ensures (eaten == \null) ==> \result < 0;
  @ ensures (buf == \null) ==> \result < 0;
  @ ensures (\result == 0) ==> (*eaten <= len);
  @ ensures (\result == 0) ==> (*eaten > 0);
  @
  @ assigns *eaten, *ctx;
 */
int parse_x509_crl_relaxed(crl_parsing_ctx *ctx, const u8 *buf, u32 len, u32 *eaten)
{
	u32 hdr_len = 0, data_len = 0;
	int ret;

	if ((ctx == NULL) || (buf == NULL) || (len == 0) || (eaten == NULL)) {
		ret = -X509_FILE_LINE_NUM_ERR;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/*
	 * Parse beginning of buffer to verify it's a sequence and get
	 * the length of the data it contains.
	 */
	ret = parse_id_len(buf, len, CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE,
			   &hdr_len, &data_len);
	if (ret) {
		ret = 1;
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	/* CRL has that exact length */
	*eaten = hdr_len + data_len;

	/* Parse it */
	ret = parse_x509_crl(ctx, buf, hdr_len + data_len);
	if (ret) {
		ERROR_TRACE_APPEND(X509_FILE_LINE_NUM_ERR);
		goto out;
	}

	ret = 0;

out:
	return ret;
}

#if defined(__FRAMAC__)

/* This dummy main allows testing */

/* removed: #include "__fc_builtin.h" */

/*
 * In order to fully tests functions, we need to be able to get
 * access to a random length random content buffer. The function
 * takes as input a *valid* buffer of length len and returns
 * either
 *
 *  - a NULL pointer with a random length using out_len parameter
 *  - a pointer to input buffer and a random length in interval
 *    [0,len-1]. in that case, output buffer content is randomized
 *    using Frama_C_make_unknown() on out_len bytes.
 *
 */
/*@
  @ requires \valid(buf + (0 .. (len - 1)));
  @ requires \valid(out_len);
  @ requires \valid(out_buf);
  @ requires \separated(out_buf, out_len, buf + (..), &Frama_C_entropy_source);
  @
  @ ensures (*out_buf == \null) ==> 0 <= *out_len <= len;
  @ ensures (*out_buf != \null) ==> (*out_buf == buf) && (0 <= *out_len <= len) && \valid(buf + (0 .. (*out_len - 1)));
  @
  @*/
static void rand_buf_or_null(unsigned char *buf,
			     unsigned int len,
			     unsigned char **out_buf,
			     unsigned int *out_len)
{
	unsigned int toggle;

	toggle = Frama_C_unsigned_int_interval(0, 1);
	/*@ assert \valid(buf + (0 .. (len - 1))); */
	Frama_C_make_unknown(buf, len);

	*out_len = Frama_C_unsigned_int_interval(0, len);
	*out_buf = toggle ? NULL : buf;
}

int main(int argc, char *argv[]) {
	u8 main_buf[ASN1_MAX_BUFFER_SIZE];
	u32 main_buf_len = sizeof(main_buf);
	u8 *buf;
	u32 len;
	int ret = 0;

	{ /* find_sig_alg_by_oid(() */
		const _sig_alg *alg;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		alg = find_sig_alg_by_oid(buf, len);
		ret |= (alg == NULL) ? 0 : 1;
	}

	{ /* find_hash_by_oid(() */
		const _hash_alg *alg;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		alg = find_hash_by_oid(buf, len);
		ret |= (alg == NULL) ? 0 : 1;
	}

	{ /* find_curve_by_oid(() */
		const _curve *crv;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		crv = find_curve_by_oid(buf, len);
		ret |= (crv == NULL) ? 0 : 1;
	}

	{ /* get_length() */
		u32 adv_len, eaten;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&adv_len, sizeof(adv_len));
		Frama_C_make_unknown(&eaten, sizeof(eaten));

		ret |= get_length(buf, len, &adv_len, &eaten);
	}

	{ /* parse_id_len() */
		tag_class exp_class;
		u32 exp_type, parsed, content_len;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&parsed, sizeof(parsed));
		Frama_C_make_unknown(&content_len, sizeof(content_len));

		ret |= parse_id_len(buf, len,
				    exp_class, exp_type,
				    &parsed, &content_len);
	}

	{ /* parse_explicit_id_len() */
		tag_class exp_int_class;
		u32 exp_ext_type, exp_int_type, parsed, data_len;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&exp_int_class, sizeof(exp_int_class));
		Frama_C_make_unknown(&exp_int_type, sizeof(exp_int_type));
		Frama_C_make_unknown(&exp_ext_type, sizeof(exp_ext_type));
		Frama_C_make_unknown(&parsed, sizeof(parsed));
		Frama_C_make_unknown(&data_len, sizeof(data_len));

		ret |= parse_explicit_id_len(buf, len,
					     exp_ext_type, exp_int_class, exp_int_type,
					     &parsed, &data_len);
	}

	{ /* parse_null() */
		u32 parsed;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&parsed, sizeof(parsed));

		ret |= parse_null(buf, len, &parsed);
	}

	{ /* parse_OID() */
		u32 parsed;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&parsed, sizeof(parsed));

		ret |= parse_OID(buf, len, &parsed);
	}

	{ /* parse_integer() */
		tag_class exp_class;
		u32 exp_type, hdr_len, data_len;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&hdr_len, sizeof(hdr_len));
		Frama_C_make_unknown(&data_len, sizeof(data_len));

		ret |= parse_integer(buf, len,
				     exp_class, exp_type,
				     &hdr_len, &data_len);
	}

	{ /* parse_non_negative_integer() */
		tag_class exp_class;
		u32 exp_type, hdr_len, data_len;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&hdr_len, sizeof(hdr_len));
		Frama_C_make_unknown(&data_len, sizeof(data_len));

		ret |= parse_non_negative_integer(buf, len,
						  exp_class, exp_type,
						  &hdr_len, &data_len);
	}

	{ /* parse_boolean() */
		u32 parsed;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&parsed, sizeof(parsed));

		ret |= parse_boolean(buf, len, &parsed);
	}

	{ /* parse_generalizedTime() */
		u32 eaten;
		u16 year;
		u8 month, day, hour, min, sec;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		Frama_C_make_unknown(&year, sizeof(year));
		Frama_C_make_unknown(&month, sizeof(month));
		Frama_C_make_unknown(&day, sizeof(day));
		Frama_C_make_unknown(&hour, sizeof(hour));
		Frama_C_make_unknown(&min, sizeof(min));
		Frama_C_make_unknown(&sec, sizeof(sec));

		ret |= parse_generalizedTime(buf, len, &eaten,
					     &year, &month, &day,
					     &hour, &min, &sec);
	}

	{ /* parse_GeneralName() */
		u32 eaten;
		int empty;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		Frama_C_make_unknown(&empty, sizeof(empty));

		ret |= parse_GeneralName(buf, len, &eaten, &empty);
	}

	{ /* parse_SerialNumber() */
		tag_class exp_class;
		u32 exp_type, eaten, off;

		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		/*@ assert (buf != \null) ==> \valid(buf + (0 .. (len - 1))); */
		off = Frama_C_unsigned_int_interval(0, len);
		/*@ assert off <= len; */
		len -= off;
		/*@ assert (buf != \null) ==> \valid(buf + (off .. (off + len - 1))); */

		ret |= parse_SerialNumber(buf, off, len,
					  exp_class, exp_type,
					  &eaten);
	}

	{ /* verify_correct_time_use() */
		u8 time_type;
		u16 yyyy;

		Frama_C_make_unknown(&time_type, sizeof(time_type));
		Frama_C_make_unknown(&yyyy, sizeof(yyyy));

		ret |= verify_correct_time_use(time_type, yyyy);
	}

	{ /* parse_Time() */
		u32 eaten;
		u16 year;
		u8 t_type, month, day, hour, min, sec;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		Frama_C_make_unknown(&year, sizeof(year));
		Frama_C_make_unknown(&t_type, sizeof(t_type));
		Frama_C_make_unknown(&month, sizeof(month));
		Frama_C_make_unknown(&day, sizeof(day));
		Frama_C_make_unknown(&hour, sizeof(hour));
		Frama_C_make_unknown(&min, sizeof(min));
		Frama_C_make_unknown(&sec, sizeof(sec));

		ret |= parse_Time(buf, len, &t_type, &eaten,
				  &year, &month, &day, &hour, &min, &sec);
	}

	{ /* parse_AKICertSerialNumber() */
		tag_class exp_class;
		u32 exp_type, eaten, off;

		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		/*@ assert (buf != \null) ==> \valid(buf + (0 .. (len - 1))); */
		off = Frama_C_unsigned_int_interval(0, len);
		/*@ assert off <= len; */
		len -= off;
		/*@ assert (buf != \null) ==> \valid(buf + (off .. (off + len - 1))); */

		ret |= parse_AKICertSerialNumber(buf, off, len,
						 exp_class, exp_type,
						 &eaten);
	}

	{ /* parse_crldp_reasons() */
		u32 exp_type, eaten;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&eaten, sizeof(eaten));

		ret |= parse_crldp_reasons(buf, len, exp_type, &eaten);
	}

	{ /* parse_DistributionPoint() */
		int crldp_has_all_reasons;
		u32 eaten;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&crldp_has_all_reasons, sizeof(crldp_has_all_reasons));
		Frama_C_make_unknown(&eaten, sizeof(eaten));

		ret |= parse_DistributionPoint(buf, len, &crldp_has_all_reasons, &eaten);
	}

	{ /* parse_AIA() */
		int critical;
		u32 off;

		Frama_C_make_unknown(&critical, sizeof(critical));
		Frama_C_make_unknown(&off, sizeof(off));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		/*@ assert (buf != \null) ==> \valid(buf + (0 .. (len - 1))); */
		off = Frama_C_unsigned_int_interval(0, len);
		/*@ assert off <= len; */
		len -= off;
		/*@ assert (buf != \null) ==> \valid(buf + (off .. (off + len - 1))); */

		ret |= parse_AIA(buf, off, len, critical);
	}

	{ /* parse_ia5_string() */
		u32 lb, ub;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&lb, sizeof(lb));
		Frama_C_make_unknown(&ub, sizeof(ub));

		ret |= parse_ia5_string(buf, len, lb, ub);
	}

	{ /* parse_x509_Name() */
		u32 eaten;
		int empty;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		Frama_C_make_unknown(&empty, sizeof(empty));

		ret |= parse_x509_Name(buf, len, &eaten, &empty);
	}

	{ /* parse_DisplayText() */
		u32 eaten;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));

		ret |= parse_DisplayText(buf, len, &eaten);
	}

	{ /* parse_nine_bit_named_bit_list() */
		u16 val;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&val, sizeof(val));

		ret |= parse_nine_bit_named_bit_list(buf, len, &val);
	}

	{ /* parse_GeneralName() */
		u32 eaten;
		int empty;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		Frama_C_make_unknown(&empty, sizeof(empty));

		ret |= parse_GeneralName(buf, len, &eaten, &empty);
	}

	{ /* parse_GeneralNames() */
		tag_class exp_class;
		u32 exp_type, eaten;

		Frama_C_make_unknown(&exp_class, sizeof(exp_class));
		Frama_C_make_unknown(&exp_type, sizeof(exp_type));
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);

		ret |= parse_GeneralNames(buf, len, exp_class, exp_type,
					  &eaten);
	}

	{ /* time_components_to_comparable_u64() */
		u64 val;
		u16 na_year;
		u8 na_month, na_day, na_hour, na_min, na_sec;

		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		na_year = Frama_C_unsigned_int_interval(0, 9999);
		na_month = Frama_C_unsigned_int_interval(0, 12);
		na_day = Frama_C_unsigned_int_interval(0, 31);
		na_hour = Frama_C_unsigned_int_interval(0, 23);
		na_min = Frama_C_unsigned_int_interval(0, 59);
		na_sec = Frama_C_unsigned_int_interval(0, 59);

		val = time_components_to_comparable_u64(na_year, na_month, na_day, na_hour, na_min, na_sec);
		ret |= val != 0;
	}

	{ /* parse_x509_cert() */
		cert_parsing_ctx cert_ctx;

		Frama_C_make_unknown(&cert_ctx, sizeof(cert_ctx));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		ret |= parse_x509_cert(&cert_ctx, buf, len);
	}

	{ /* parse_x509_crl */
		crl_parsing_ctx crl_ctx;

		Frama_C_make_unknown(&crl_ctx, sizeof(crl_ctx));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		ret |= parse_x509_crl(&crl_ctx, buf, len);
	}

	{ /* parse_x509_cert_relaxed() */
		cert_parsing_ctx cert_ctx;
		u32 eaten;

		Frama_C_make_unknown(&cert_ctx, sizeof(cert_ctx));
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		ret |= parse_x509_cert_relaxed(&cert_ctx, buf, len, &eaten);
	}

	{ /* parse_x509_crl_relaxed() */
		crl_parsing_ctx crl_ctx;
		u32 eaten;

		Frama_C_make_unknown(&crl_ctx, sizeof(crl_ctx));
		Frama_C_make_unknown(&eaten, sizeof(eaten));
		rand_buf_or_null(main_buf, main_buf_len, &buf, &len);
		ret |= parse_x509_crl_relaxed(&crl_ctx, buf, len, &eaten);
	}

	return ret;
}

#elif defined(__IKOS__)

#include <ikos/analyzer/intrinsic.h>

int main(int argc, char *argv[]) {
	u8 buf[ASN1_MAX_BUFFER_SIZE];
	cert_parsing_ctx cert_ctx;
	crl_parsing_ctx crl_ctx;
	u32 len;
	int ret = 0;

	__ikos_abstract_mem(buf, ASN1_MAX_BUFFER_SIZE);

	len = __ikos_nondet_uint();
	__ikos_assume(len <= ASN1_MAX_BUFFER_SIZE);

	ret  = parse_x509_cert(&cert_ctx, buf, len);
	ret |= parse_x509_crl(&crl_ctx, buf, len);

	return ret;
}

#endif


/* ============== IMPL: x509-utils.c ============== */
/*
 *  Copyright (C) 2022 - This file is part of x509-parser project
 *
 *  Author:
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *
 *  This software is licensed under a dual GPLv2/BSD license. See
 *  LICENSE file at the root folder of the project.
 */

/* removed: #include "x509-utils.h" */

#define X509_FILE_NUM 0 /* See x509-utils.h for rationale */

/*@
  @ predicate bmatch(u8 *b1, u8 *b2, u32 n) =
  @   \forall integer i; 0 <= i < n ==> b1[i] == b2[i];
  @
  @ predicate bdiffer(u8 *b1, u8 *b2, u32 n) =
  @   ! bmatch(b1, b2, n);
  @*/
/*@
  @
  @ requires \valid_read(b1 + (0 .. n-1));
  @ requires \valid_read(b2 + (0 .. n-1));
  @
  @ assigns \nothing;
  @*/
int bufs_differ(const u8 *b1, const u8 *b2, u32 n)
{
	int ret = 0;
	u32 i = 0;

	/*@
	  @ loop invariant 0 <= i <= n;
	  @ loop invariant bmatch(b1, b2, i);
	  @ loop assigns i;
	  @ loop variant n - i;
	  @*/
	for (i = 0; i < n; i++) {
		if(b1[i] != b2[i]) {
			ret = 1;
			break;
		}
	}

	return ret;
}

