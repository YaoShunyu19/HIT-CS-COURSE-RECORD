#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "md5.h"
#include "ip_ipsec.h"

#define MD5_DIGEST_SIZE (128/BITS_PER_BYTE)
#define SHA1_DIGEST_SIZE (160 / BITS_PER_BYTE)
#define MAX_DIGEST_LEN (160 / BITS_PER_BYTE)
#define DES_CBC_BLOCK_SIZE	(64 / BITS_PER_BYTE)
#define EN0 0
#define DE1 1

#define GROUPNUM 2
#define ENCRYNUM 2
#define HASHNUM	2

#define HMAC_BUFSIZE         64

union hash_ctx 
{
	IKE_MD5_CTX ctx_md5;
/*	SHA1_CTX ctx_sha1;*/
};

struct hash_desc 
{
    unsigned short hash;
	unsigned int hash_digest_len; /* length of digest, in byte */
    void (*hash_init)(IKE_MD5_CTX *);	/* initialize context */
    void (*hash_update)(IKE_MD5_CTX *, const unsigned char *input, unsigned int len);   /* add input to hash */
    void (*hash_final)(unsigned char *output, IKE_MD5_CTX *);   /* finalize hash */
};

/* unification of cryptographic encoding/decoding algorithms
 * The IV is taken from and returned to st->st_new_iv.
 * This allows the old IV to be retained.
 * Use update_iv to commit to the new IV (for example, once a packet has
 * been validated).
 */

#define MAX_OAKLEY_KEY_LEN  (3 * DES_CBC_BLOCK_SIZE)



/* HMAC package
 * Note that hmac_ctx can be (and is) copied since there are
 * no persistent pointers into it.
 */

struct hmac_ctx {
    struct hash_desc *h;	/* underlying hash function */
    unsigned int hmac_digest_len;	/* copy of h->hash_digest_len */
    IKE_MD5_CTX hash_ctx;	/* ctx for hash function */
    unsigned char buf1[HMAC_BUFSIZE], buf2[HMAC_BUFSIZE];
    };

extern void hmac_init(struct hmac_ctx *ctx, struct hash_desc *h, unsigned char *key, unsigned int key_len);
extern void hmac_update(struct hmac_ctx *ctx, unsigned char *data, unsigned int data_len);
extern void hmac_final(unsigned char *output, struct hmac_ctx *ctx);


#endif
